// Include C++ headers
#include <iostream>
#include <string>

// Include GLEW (always include first)
#include <GL/glew.h>

// Include GLFW to handle window and keyboard events
#include <glfw3.h>

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>

using namespace std;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "Deformable"

// Global variables
GLFWwindow* window;
GLuint shaderProgram;
GLuint triangleVAO;
GLuint verticesVBO, colorsVBO;

void createContext() {
    // Create and compile our GLSL program from the shaders
    shaderProgram = loadShaders("simple.vertexshader", "simple.fragmentshader");

    // task define VAO

    // task define vertex VBO


    // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // enable point size
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void free() {
    // Free allocated buffers
    glDeleteBuffers(1, &verticesVBO);
    glDeleteBuffers(1, &colorsVBO);
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteProgram(shaderProgram);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

void mainLoop() {
    do {
        // Clear the screen.
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw

        // task bind shader

        // task bind VAO

        // task draw triangle

        // task draw points

        // Swap buffers
        glfwSwapBuffers(window);

        // Events
        glfwPollEvents();
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
}

void initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);  // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
                            " If you have an Intel GPU, they are not 3.3 compatible." +
                            "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    // Set Handler function for resizing the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Set background color (gray) [r, g, b, a]
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    // Log
    logGLParameters();
}

int main(void) {
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


// glfw: whenever the window size changed this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}