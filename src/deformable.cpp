// Include C++ headers
#include <iostream>
#include <string>

// Include GLEW (always include first)
#include <GL/glew.h>

// Include GLFW to handle window and keyboard events
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>   // glm::to_string

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>

using namespace std;
using namespace glm;

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
GLuint MVPLocation;
GLuint cubeVAO, cubeVBO, cubeColorsVBO;

void createContext() {
    // Create and compile our GLSL program from the shaders
    shaderProgram = loadShaders("deformable.vert", "deformable.frag");

    // Get a pointer location to model matrix in the vertex shader
    MVPLocation = glGetUniformLocation(shaderProgram, "MVP");

    // Define triangle VAO
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    // Define vertex VBO
    static vector<GLfloat> cubeVertices = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    // TODO: Create cube vertices dynamically (Each face will have N^2 triangles)

    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size()*sizeof(GLfloat), static_cast<void*>(cubeVertices.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // color VBO
    static vector<GLfloat> cubeColors = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };
    glGenBuffers(1, &cubeColorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeColorsVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeColors.size()*sizeof(GLfloat), static_cast<void*>(cubeColors.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // enable point size
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void free() {
    // Free allocated buffers
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeColorsVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteProgram(shaderProgram);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

void mainLoop() {
    // Task 4: transformation
    GLfloat matrix[] = {
        1.0f, 0.0f, 0.0f, 0.0f, // first column
        0.0f, 1.0f, 0.0f, 0.0f, // second column
        0.0f, 0.0f, 1.0f, 0.0f, // third column
        0.0f, 0.0f, 0.0f, 1.0f	// fourth column
    };

    // Task 6: triangle scale
    mat4 triangleScaling = glm::scale(mat4(), vec3(0.5, 0.5, 0.5));
    // cout << glm::to_string(triangleScaling) << endl;

    // Task 7: triangle rotate
    mat4 triangleRotation = glm::rotate(mat4(), 3.14f / 4, vec3(0, 0, 1));
    // cout << glm::to_string(triangleRotation) << endl;

    // Task 8: triangle translate
    mat4 triangleTranslation = glm::translate(mat4(), vec3(0.0, 1.5, 1.0));
    cout << glm::to_string(triangleTranslation) << endl;

    // Task 10: cube model matrix
    mat4 cubeScaling = glm::scale(mat4(), vec3(1.0, 1.0, 1.0));
    mat4 cubeRotation = glm::rotate(mat4(), 3.14f / 4, vec3(0, 1, 0));
    cubeRotation = cubeRotation * glm::rotate(mat4(), 3.14f / 4, vec3(0, 0, 1));
    mat4 cubeTranslation = glm::translate(mat4(), vec3(1.0, 0.0, 0.0));

    // Task 13: projection
    // Projection matrix: 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    mat4 projection = perspective(radians(45.f), 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera: -x:x, -y:y, -z:z
    // mat4 Projection = ortho(

    // Task 14: view
    // Camera view matrix
    // mat4 view = lookAt(vec3(0, 0, 10), vec3(0, 0, 0), vec3(0, 1, 0));

    GLfloat previousTime = glfwGetTime();

    do {
        // Task 12: depth test  | GL_DEPTH_BUFFER_BIT
        // Clear the screen (color and depth)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // MVP matrix
        // compute model, view projection operator
        // mat4 triangleModel = triangleTranslation * triangleRotation * triangleScaling; // Normal triangle
        GLfloat dt = glfwGetTime() - previousTime;
        previousTime = glfwGetTime();
         cubeRotation = cubeRotation * glm::rotate(mat4(), dt, vec3(0.5, 0.5, 0.8)); // Rotate with speed = 3cycles/sec
        // GLfloat scaleFactor = sin(glfwGetTime()) + 1; // Add a constant offset so that sin is positive
        cubeScaling = glm::scale(mat4(), vec3(1.0, 1.0, 1.0));
        // Rotate camera
        mat4 view = lookAt(
            vec3(sin(glfwGetTime()), 0, 10),
            vec3(0, 0, 0),
            vec3(0, 1, 0)
        );

        // draw cube
        glBindVertexArray(cubeVAO);

        // Task 11: cube MVP
        mat4 cubeModel = cubeTranslation * cubeRotation * cubeScaling;
        mat4 cubeMVP = projection * view * cubeModel;
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &cubeMVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

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
