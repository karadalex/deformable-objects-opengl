// Include C++ headers
#include <iostream>
#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>
#include <common/camera.h>

#include "Collision.h"
#include "Plane.h"
#include "Cube.h"
#include "Staircase.h"
#include "FreeForm.h"

namespace program5 {

using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "Experiment 5 - Free-form deformation of objectst"

// Global variables
GLFWwindow* window;
Camera* camera;
GLuint shaderProgram;
GLuint projectionMatrixLocation, viewMatrixLocation, modelMatrixLocation;
string selectedModelFile;

float stiffness, damping;
vec3 position = vec3(0, 10, 0);
vec3 velocity = vec3(0, -2, 0);
vec3 omega = vec3(0, 0, 0);
float mass = 10;

// Scene objects
Plane* plane;
Staircase* staircase;
FreeForm* freeForm;

// Standard acceleration due to gravity
#define g 9.80665f

// Simulation toggles
bool showModelVertices = false;
bool pausePhysics = false;
bool rightMouseClicked = false;
bool leftMouseClicked = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        showModelVertices = !showModelVertices;
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        pausePhysics = !pausePhysics;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        // Reset cube to original mode and start dragging it with mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        freeForm = new FreeForm(selectedModelFile, position, velocity, omega, mass, stiffness, damping);
        rightMouseClicked = true;
        pausePhysics = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        // Stop dragging with the mouse on releasing the right mouse button
        rightMouseClicked = false;
        pausePhysics = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Reset cube to original mode and start dragging it with mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        leftMouseClicked = true;
        pausePhysics = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        // Stop dragging with the mouse on releasing the right mouse button
        leftMouseClicked = false;
        pausePhysics = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void createContext() {
    shaderProgram = loadShaders("shaders/deformable.vert", "shaders/deformable.frag");

    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");

    plane = new Plane(8);
    staircase = new Staircase(2);
    freeForm = new FreeForm(selectedModelFile, position, velocity, omega, mass, stiffness, damping);
}

void free() {
    delete plane;
    delete staircase;
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void mainLoop() {
    float t = glfwGetTime();
    vec3 lightPos = vec3(10, 10, 10);
    camera->position = glm::vec3(0, 8, 30);
    float maxEnergy = 0;

    time_point start_time, stop_time;
    long millis_duration = 0;
    const int num_of_timing_frames = 500;
    int timing_frame_count = 0;

    do {
        start_time = steady_clock::now();

        // calculate dt
        float currentTime = glfwGetTime();
        float dt = currentTime - t;

        // Change dt to 0.001f and observe the total energy, then change
        // the numerical integration method to Runge - Kutta 4th order (in RigidBody.cpp)

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // camera
        if (!leftMouseClicked && !rightMouseClicked) {
            camera->update();
        } else {
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            vec3 displacement = vec3(-float(width / 2 - xPos)/100, float(height / 2 - yPos)/100, 0);

            if (rightMouseClicked) {
                freeForm->translateAllVertices(displacement);
            }
            else {
                freeForm->transformWithControlPoint(0, displacement);
            }
            glfwSetCursorPos(window, width / 2, height / 2);
        }
        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

        plane->update(t, dt);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &plane->modelMatrix[0][0]);
        plane->draw();

        staircase->update(t, dt);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &staircase->modelMatrix[0][0]);
        staircase->draw();

        if (!pausePhysics) {   
            freeForm->update(t, dt);
        }
        freeForm->draw(modelMatrixLocation);

        // Collision checks
        handlePlaneFreeFormCollision(*plane, *freeForm);
        handleStaircaseFreeFormCollision(*staircase, *freeForm);

        t += dt;

        glfwSwapBuffers(window);
        glfwPollEvents();

        stop_time = steady_clock::now();
        if (timing_frame_count < num_of_timing_frames) {
            millis_duration += std::chrono::duration_cast<millis>(stop_time - start_time).count();
            timing_frame_count++;

        } else {
             cout << "millis_duration is calculated " << endl;
        }

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    cout << "average_millis_duration = " << (float)millis_duration/(float)num_of_timing_frames << "ms" << endl;
}

void initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
                            " If you have an Intel GPU, they are not 3.3 compatible." +
                            "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Normal use of mouse
    // To hide the mouse and enable unlimited movement change
    // GLFW_CURSOR_NORMAL to GLFW_CURSOR_DISABLED
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

    // Gray background color
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CW);
    // glFrontFace(GL_CCW);

    // enable point size when drawing points
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Log
    logGLParameters();

    // Simulation toggles
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Create camera
    camera = new Camera(window);
}

int main(int argc, char* argv[]) {
    try {
        selectedModelFile = "models/" + selectObject();
        getElasticityParameters(stiffness, damping);
        // selectedModelFile = "models/suzanne.obj";
        // stiffness = 0.5f;
        // damping = 0.1f;

        initialize();
        createContext();
        mainLoop();
        free();
    } catch (exception& ex) {
        cout << ex.what() << endl;
        getchar();
        free();
        return -1;
    }

    return 0;
}

}