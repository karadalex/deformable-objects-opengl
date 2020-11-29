#include "plane.h"

Plane::Plane()
{
    previousTime = glfwGetTime();
    // Create and compile our GLSL program from the shaders
    shaderProgram = loadShaders("deformable.vert",
                                "deformable.frag");
}

void Plane::createContext()
{

    // Get a pointer location to model matrix in the vertex shader
    MVPLocation = glGetUniformLocation(shaderProgram, "MVP");

    // Define plane VAO
    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);

    planeVertices = {
        -5.0f, 0.0f, -5.0f,
        -5.0f, 0.0f, 5.0f,
        5.0f, 0.0f, -5.0f,

        5.0f, 0.0f, -5.0f,
        -5.0f, 0.0f, 5.0f,
        5.0f, 0.0f, 5.0f,
    };

    glGenBuffers(1, &planeVerticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVerticesVBO);
    glBufferData(GL_ARRAY_BUFFER, planeVertices.size() * sizeof(GLfloat), static_cast<void *>(planeVertices.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // color VBO
    vector<GLfloat> planeColors;
    for (int i = 0; i < 18; i++)
      planeColors.push_back(0.2f);
    

    glGenBuffers(1, &planeColorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeColorsVBO);
    glBufferData(GL_ARRAY_BUFFER, planeColors.size() * sizeof(GLfloat), static_cast<void *>(planeColors.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
}

void Plane::draw()
{
    glUseProgram(shaderProgram);

    // Plane model matrix
    mat4 planeScaling = glm::scale(mat4(), vec3(1.0, 1.0, 1.0));
    mat4 planeRotation = mat4();
    mat4 planeTranslation = mat4();

    // Projection matrix: 45deg Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    mat4 projection = perspective(radians(45.f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Rotate camera
    mat4 view = lookAt(
        vec3(0, 0, 10),
        vec3(0, 0, 0),
        vec3(0, 1, 0));

    // Draw plane
    glBindVertexArray(planeVAO);

    // Define plane MVP
    mat4 planeModel = planeTranslation * planeRotation * planeScaling;
    mat4 planeMVP = projection * view * planeModel;
    glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &planeMVP[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, planeVertices.size());

    dt = glfwGetTime() - previousTime;
}

Plane::~Plane()
{
    glDeleteBuffers(1, &planeVerticesVBO);
    glDeleteBuffers(1, &planeColorsVBO);
    glDeleteVertexArrays(1, &planeVAO);

    glDeleteProgram(shaderProgram);
}
