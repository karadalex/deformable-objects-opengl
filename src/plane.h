#ifndef PLANE_H
#define PLANE_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp> 
#include <common/shader.h>

using namespace std;
using namespace glm;

class Plane
{
public:
    GLuint planeVAO;
    GLuint planeVerticesVBO, planeColorsVBO;
    GLuint MVPLocation;
    GLuint shaderProgram;
    GLfloat previousTime, dt;

    vector<GLfloat> planeVertices;

    Plane();

    void createContext();
    void draw();

    ~Plane();
};

#endif // PLANE_H
