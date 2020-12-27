#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>

using namespace glm;

Cube::Cube(vec3 pos, vec3 vel, vec3 omega, float length, float mass) : RigidBody() {
    cube = new DeformableBody("models/cube.obj", pos, vel, omega, mass);

    l = length;
    m = mass;
    x = pos;
    v = vel;
    P = m * v;
    w = omega;

    if (l == 0) throw std::logic_error("Cube: length != 0");
    mat3 I = mat3(
        1.0f / 6 * mass * l * l, 0, 0,
        0, 1.0f / 6 * mass * l * l, 0,
        0, 0, 1.0f / 6 * mass * l * l);

    L = I * w;
    I_inv = inverse(I);
}

Cube::~Cube() {
    delete cube;
}

void Cube::draw(bool showCubeVertices) {
    if (showCubeVertices) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);    
    }
    
    cube->bind();
    cube->draw();

    if (showCubeVertices) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }
}

void Cube::update(float t, float dt) {
    cube->update(t, dt);

    // numerical integration
    // advanceState(t, dt);

    // compute model matrix
//     mat4 scale = glm::scale(mat4(), vec3(l, l, l));
//     mat4 tranlation = translate(mat4(), vec3(x.x, x.y, x.z));
// #ifdef USE_QUATERNIONS
//     mat4 rotation = mat4_cast(q);
// #else
//     mat4 rotation = mat4(R);
// #endif
//     modelMatrix = tranlation * rotation * scale;
}


// OLD CODE
// void CubeCustom::createContext()
// {

//     // Get a pointer location to model matrix in the vertex shader
//     MVPLocation = glGetUniformLocation(shaderProgram, "MVP");

//     // Define cube VAO
//     glGenVertexArrays(1, &cubeVAO);
//     glBindVertexArray(cubeVAO);

//     // Define cube vertices. Three consecutive floats give a 3D vertex; Three
//     // consecutive vertices give a triangle.
//     GLfloat range[2] = {-1.0f, 1.0f};
//     GLfloat step = 2.0f/density;
//     GLfloat x, y, z;
//     for (int r = 0; r < 2; r++)
//     {
//         x = range[r];
//         for (int j = 0; j < density; j++)
//         {
//             for (int k = 0; k < density; k++)
//             {
//                 // 1st triangle
//                 y = range[0] + j*step;
//                 z = range[0] + k*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 z = range[0] + (k+1)*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 y = range[0] + (j+1)*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);

//                 // 2nd triangle
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 z = range[0] + k*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 y = range[0] + j*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//             }
//         }
//     }
//     for (int r = 0; r < 2; r++)
//     {
//         y = range[r];
//         for (int i = 0; i < density; i++)
//         {
//             for (int k = 0; k < density; k++)
//             {
//                 // 1st triangle
//                 x = range[0] + i*step;
//                 z = range[0] + k*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 z = range[0] + (k+1)*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 x = range[0] + (i+1)*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);

//                 // 2nd triangle
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 z = range[0] + k*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 x = range[0] + i*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//             }
//         }
//     }
//     for (int r = 0; r < 2; r++)
//     {
//         z = range[r];
//         for (int i = 0; i < density; i++)
//         {
//             for (int j = 0; j < density; j++)
//             {
//                 // 1st triangle
//                 x = range[0] + i*step;
//                 y = range[0] + j*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 y = range[0] + (j+1)*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 x = range[0] + (i+1)*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);

//                 // 2nd triangle
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 y = range[0] + j*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//                 x = range[0] + i*step;
//                 cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
//             }
//         }
//     }

//     glGenBuffers(1, &cubeVerticesVBO);
//     glBindBuffer(GL_ARRAY_BUFFER, cubeVerticesVBO);
//     glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(GLfloat), static_cast<void *>(cubeVertices.data()), GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//     glEnableVertexAttribArray(0);

//     // color VBO
//     vector<GLfloat> cubeColors;
//     vector<GLfloat> faceColors = {
//         0.583f, 0.771f, 0.014f,
//         0.609f, 0.115f, 0.436f,
//         0.327f, 0.483f, 0.844f,

//         0.583f, 0.771f, 0.014f,
//         0.609f, 0.115f, 0.436f,
//         0.327f, 0.483f, 0.844f
//     };
//     for (int r = 0; r < 2; r++)
//     {
//         x = range[r];
//         for (int j = 0; j < density; j++)
//         {
//             for (int k = 0; k < density; k++)
//             {
//                 cubeColors.insert(cubeColors.end(), faceColors.begin(), faceColors.end());
//             }
//         }
//     }
//     faceColors = {
//         0.597f, 0.770f, 0.761f,
//         0.559f, 0.436f, 0.730f,
//         0.359f, 0.583f, 0.152f,

//         0.483f, 0.596f, 0.789f,
//         0.559f, 0.861f, 0.639f,
//         0.195f, 0.548f, 0.859f
//     };
//     for (int r = 0; r < 2; r++)
//     {
//         y = range[r];
//         for (int i = 0; i < density; i++)
//         {
//             for (int k = 0; k < density; k++)
//             {
//                 cubeColors.insert(cubeColors.end(), faceColors.begin(), faceColors.end());
//             }
//         }
//     }
//     faceColors = {
//         0.014f, 0.184f, 0.576f,
//         0.771f, 0.328f, 0.970f,
//         0.406f, 0.615f, 0.116f,

//         0.676f, 0.977f, 0.133f,
//         0.971f, 0.572f, 0.833f,
//         0.140f, 0.616f, 0.489f
//     };
//     for (int r = 0; r < 2; r++)
//     {
//         z = range[r];
//         for (int i = 0; i < density; i++)
//         {
//             for (int j = 0; j < density; j++)
//             {
//                 cubeColors.insert(cubeColors.end(), faceColors.begin(), faceColors.end());
//             }
//         }
//     }

//     glGenBuffers(1, &cubeColorsVBO);
//     glBindBuffer(GL_ARRAY_BUFFER, cubeColorsVBO);
//     glBufferData(GL_ARRAY_BUFFER, cubeColors.size() * sizeof(GLfloat), static_cast<void *>(cubeColors.data()), GL_STATIC_DRAW);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//     glEnableVertexAttribArray(1);

//     cout << "cubevertices " << cubeVertices.size() << endl;
//     cout << "cubecolors " << cubeColors.size() << endl;
// }