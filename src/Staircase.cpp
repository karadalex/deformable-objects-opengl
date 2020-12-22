#include "Staircase.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>

using namespace glm;

Staircase::Staircase(float s) {
    size = s;
    staircase = new Drawable("models/staircase.obj");
}

Staircase::~Staircase() {
    delete staircase;
}

void Staircase::draw(unsigned int drawable) {
    staircase->bind();
    staircase->draw();
}

void Staircase::update(float t, float dt) {
    mat4 translate = mat4();
    mat4 scale = glm::scale(mat4(), vec3(size, size, size));
    modelMatrix = translate * scale;
}

// OLD CODE
// void Staircase::createContext()
// {

//     // Get a pointer location to model matrix in the vertex shader
//     MVPLocation = glGetUniformLocation(shaderProgram, "MVP");

//     // Define staircase VAO
//     glGenVertexArrays(1, &staircaseVAO);
//     glBindVertexArray(staircaseVAO);

//     // Define staircase vertices. Three consecutive floats give a 3D vertex; Three
//     // consecutive vertices give a triangle.
//     GLfloat width = 2;
//     GLfloat xydim = 0.5;
//     GLfloat x, y, z;
//     x = y = z = 0.0f;
//     for (int n = 0; n < stairs; n++) {
//         x = n*xydim;
//         y = n*xydim;

//         staircaseVertices.push_back(x); staircaseVertices.push_back(y); staircaseVertices.push_back(z);
//         staircaseVertices.push_back(x); staircaseVertices.push_back(y); staircaseVertices.push_back(z+width);
//         staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z);

//         staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z);
//         staircaseVertices.push_back(x); staircaseVertices.push_back(y); staircaseVertices.push_back(z+width);
//         staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z+width);

//         staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z);
//         staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z+width);
//         staircaseVertices.push_back(x+xydim); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z);

//         staircaseVertices.push_back(x+xydim); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z);
//         staircaseVertices.push_back(x+xydim); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z+width);
//         staircaseVertices.push_back(x); staircaseVertices.push_back(y+xydim); staircaseVertices.push_back(z+width);
//     }
    

//     glGenBuffers(1, &staircaseVerticesVBO);
//     glBindBuffer(GL_ARRAY_BUFFER, staircaseVerticesVBO);
//     glBufferData(GL_ARRAY_BUFFER, staircaseVertices.size() * sizeof(GLfloat), static_cast<void *>(staircaseVertices.data()), GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//     glEnableVertexAttribArray(0);

//     // color VBO
//     vector<GLfloat> staircaseColors;
//     vector<GLfloat> faceColors;
//     for (int i = 0; i < 18; i++)
//       faceColors.push_back(0.1f);
//     for (int i = 0; i < 18; i++)
//       faceColors.push_back(0.2f);
//     for (int n = 0; n < stairs; n++) {
//         staircaseColors.insert(staircaseColors.end(), faceColors.begin(), faceColors.end());
//     }

//     glGenBuffers(1, &staircaseColorsVBO);
//     glBindBuffer(GL_ARRAY_BUFFER, staircaseColorsVBO);
//     glBufferData(GL_ARRAY_BUFFER, staircaseColors.size() * sizeof(GLfloat), static_cast<void *>(staircaseColors.data()), GL_STATIC_DRAW);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//     glEnableVertexAttribArray(1);
// }