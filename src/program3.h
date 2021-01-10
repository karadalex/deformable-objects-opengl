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
#include <common/texture.h>

#include "Collision.h"
#include "Plane.h"
#include "DeformableModel.h"

namespace program3 {

using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "Experiment 3 - Configure deformable objec"

// Global variables
GLFWwindow* window;
Camera* camera;
GLuint shaderProgram;
GLuint projectionMatrixLocation, viewMatrixLocation, modelMatrixLocation;
string selectedModelFile;

// Texture variables
string selectedTextureDir;
GLuint useTexture;
GLuint diffuseColorSampler, ambientColorSampler, normalSampler;
GLuint diffuseTexture, ambientTexture, normalTexture;

float stiffness, damping, mass;

// Scene objects
Plane* plane;
Cube* cube;
Staircase* staircase;
DeformableModel* deformableModel;

// Standard acceleration due to gravity
#define g 9.80665f

// Simulation toggles
bool showModelVertices = false;
bool pausePhysics = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        showModelVertices = !showModelVertices;
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        pausePhysics = !pausePhysics;
    }
}

void createContext() {
    shaderProgram = loadShaders("shaders/deformable.vert", "shaders/deformable.frag");

    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");

    // load diffuse, ambient and normal texture maps
    string diffuseImg = selectedTextureDir + "/basecolor.png";
    diffuseTexture = loadSOIL(diffuseImg.c_str());
    string ambientOcclusionImg = selectedTextureDir + "/ambientOcclusion.png";
    ambientTexture = loadSOIL(ambientOcclusionImg.c_str());
    string normalImg = selectedTextureDir + "/normal.png";
    normalTexture = loadSOIL(normalImg.c_str());

    // get a pointer to the texture samplers (diffuseColorSampler, ambientColorSampler, normalSampler)
    useTexture = glGetUniformLocation(shaderProgram, "useTexture");
    diffuseColorSampler = glGetUniformLocation(shaderProgram, "diffuseColorSampler");
    ambientColorSampler = glGetUniformLocation(shaderProgram, "ambientColorSampler");
    normalSampler = glGetUniformLocation(shaderProgram, "normalSampler");

    plane = new Plane(8);
    float length = 0.1;
    vec3 pos = vec3(0, 5, 0);
    vec3 vel = vec3(0, -1, 0);
    deformableModel = new DeformableModel(selectedModelFile, pos, vel, vec3(0, 0, 0), length, mass, stiffness, damping);
}

void free() {
    delete plane;
    delete deformableModel;
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

        handlePlaneModelCollision(*plane, *deformableModel);

        if (!pausePhysics) {
            deformableModel->update(t, dt);   
        }
        
        // bind textures and transmit diffuse and ambient maps to the GPU
        glUniform1i(useTexture, 1);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
        glUniform1i(diffuseColorSampler, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ambientTexture);
        glUniform1i(ambientColorSampler, 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalTexture);
        glUniform1i(normalSampler, 2);

        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &deformableModel->modelMatrix[0][0]);
        deformableModel->draw(showModelVertices);

        glUniform1i(useTexture, 0);

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

    // Simulation toggles
    glfwSetKeyCallback(window, key_callback);

    // Create camera
    camera = new Camera(window);
}

int main(int argc, char* argv[]) {
    try {
        selectedModelFile = "models/" + selectObject();
        getElasticityParameters(stiffness, damping);
        mass = getMass();
        selectedTextureDir = "textures/" + selectModelTexture();
        
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