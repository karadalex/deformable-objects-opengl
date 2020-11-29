#include "staircase.h"

Staircase::Staircase(int stairs, vec3 position, int density) : stairs(stairs), position(position), density(density)
{
    previousTime = glfwGetTime();
    // Create and compile our GLSL program from the shaders
    shaderProgram = loadShaders("deformable.vert",
                                "deformable.frag");
}

void Staircase::createContext()
{

    // Get a pointer location to model matrix in the vertex shader
    MVPLocation = glGetUniformLocation(shaderProgram, "MVP");

    // Define staircase VAO
    glGenVertexArrays(1, &staircaseVAO);
    glBindVertexArray(staircaseVAO);

    // Define staircase vertices. Three consecutive floats give a 3D vertex; Three
    // consecutive vertices give a triangle.
    GLfloat width = 2;
    GLfloat xydim = 0.5;
    GLfloat x, y, z;
    x = y = z = 0.0f;
    for (int n = 0; n < stairs; n++) {
        x = n*xydim;
        y = n*xydim;

        staircaseVertices.push_back(x); staircaseVertices.push_back(y); staircaseVertices.push_back(z);
        staircaseVertices.push_back(x); staircaseVertices.push_back(y); staircaseVertices.push_back(z+width);
        staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z);

        staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z);
        staircaseVertices.push_back(x); staircaseVertices.push_back(y); staircaseVertices.push_back(z+width);
        staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z+width);

        staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z);
        staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z+width);
        staircaseVertices.push_back(x+xydim); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z);

        staircaseVertices.push_back(x+xydim); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z);
        staircaseVertices.push_back(x+xydim); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z+width);
        staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z+width);
    }
    

    glGenBuffers(1, &staircaseVerticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, staircaseVerticesVBO);
    glBufferData(GL_ARRAY_BUFFER, staircaseVertices.size() * sizeof(GLfloat), static_cast<void *>(staircaseVertices.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // color VBO
    vector<GLfloat> staircaseColors;
    vector<GLfloat> faceColors;
    for (int i = 0; i < 18; i++)
      faceColors.push_back(0.1f);
    for (int i = 0; i < 18; i++)
      faceColors.push_back(0.2f);
    for (int n = 0; n < stairs; n++) {
        staircaseColors.insert(staircaseColors.end(), faceColors.begin(), faceColors.end());
    }

    glGenBuffers(1, &staircaseColorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, staircaseColorsVBO);
    glBufferData(GL_ARRAY_BUFFER, staircaseColors.size() * sizeof(GLfloat), static_cast<void *>(staircaseColors.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
}

void Staircase::draw()
{
    glUseProgram(shaderProgram);

    // Staircase model matrix
    mat4 staircaseScaling = glm::scale(mat4(), vec3(1.0, 1.0, 1.0));
    mat4 staircaseRotation = mat4();
    mat4 staircaseTranslation = glm::translate(mat4(), position);

    // Projection matrix: 45deg Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    mat4 projection = perspective(radians(45.f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Rotate camera
    mat4 view = lookAt(
        vec3(0, 0, 10),
        vec3(0, 0, 0),
        vec3(0, 1, 0));

    // Draw staircase
    glBindVertexArray(staircaseVAO);

    // Define staircase MVP
    mat4 staircaseModel = staircaseTranslation * staircaseRotation * staircaseScaling;
    mat4 staircaseMVP = projection * view * staircaseModel;
    glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &staircaseMVP[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, staircaseVertices.size());

    dt = glfwGetTime() - previousTime;
}

Staircase::~Staircase()
{
    glDeleteBuffers(1, &staircaseVerticesVBO);
    glDeleteBuffers(1, &staircaseColorsVBO);
    glDeleteVertexArrays(1, &staircaseVAO);

    glDeleteProgram(shaderProgram);
}
