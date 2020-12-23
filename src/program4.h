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

namespace program4 {

using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "Experiment 4 - Deformable cube - User interaction"

// Global variables
GLFWwindow* window;
Camera* camera;
GLuint shaderProgram;
GLuint projectionMatrixLocation, viewMatrixLocation, modelMatrixLocation;

// Scene objects
Plane* plane;
Cube* cube;
Staircase* staircase;

// Standard acceleration due to gravity
#define g 9.80665f

void createContext() {
    shaderProgram = loadShaders("deformable.vert", "deformable.frag");

    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");

    plane = new Plane(8);
    cube = new Cube(vec3(4, 5, 4), vec3(0, -1, 0), vec3(0, 0, 0), 2, 10);
    staircase = new Staircase(1);
}

void free() {
    delete plane;
    delete staircase;
    delete cube;
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void mainLoop() {
    float t = glfwGetTime();
    vec3 lightPos = vec3(10, 10, 10);
    camera->position = glm::vec3(0, 8, 30);
    float maxEnergy = 0;

    do {
        // calculate dt
        float currentTime = glfwGetTime();
        float dt = currentTime - t;

        // Change dt to 0.001f and observe the total energy, then change
        // the numerical integration method to Runge - Kutta 4th order (in RigidBody.cpp)

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // camera
        camera->update();
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
        
        handlePlaneCubeCollision(*plane, *cube);

        // Task 2c: model the force due to gravity
        cube->forcing = [&](float t, const vector<float>& y)->vector<float> {
            vector<float> f(6, 0.0f);
            f[1] = -cube->m * g;
            return f;
        };
        cube->update(t, dt);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &cube->modelMatrix[0][0]);
        cube->draw();


        // Calculate the total energy and comment on the previous
        // float KE = cube->calcKinecticEnergy();
		// float PE = cube->m * g * cube->x.y;
        // float T = KE + PE;
        // if (T > maxEnergy) {
        //     cout << "Total Energy: " << T << endl;
        //     maxEnergy = T;
        // }

        t += dt;

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);
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

    // Create camera
    camera = new Camera(window);
}

int main(int argc, char* argv[]) {
    try {
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