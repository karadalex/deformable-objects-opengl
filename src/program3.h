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
#include <glm/gtx/string_cast.hpp>   // glm::to_string

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>

#include "CubeCustom.h"
#include "staircase.h"

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
CubeCustom *cube;
Staircase *staircase;


void createContext() {
    cube = new CubeCustom(vec3(0,0,0), 10);
    cube->createContext();

    staircase = new Staircase(10, vec3(0,0,0));
    staircase->createContext();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void free() {
    delete cube;
    delete staircase;

    glfwTerminate();
}


void mainLoop() {
    do {
        // Depth test  | GL_DEPTH_BUFFER_BIT
        // Clear the screen (color and depth)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube->draw();
        staircase->draw();

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

    // Gray background color
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    // Task 12: Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Log
    logGLParameters();
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