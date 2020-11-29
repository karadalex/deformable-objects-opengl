#ifndef CUBE_H
#define CUBE_H

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

class Cube
{
public:
    GLuint cubeVAO;
    GLuint cubeVerticiesVBO, cubeColorsVBO;
    GLuint MVPLocation;
    GLuint shaderProgram;
    GLfloat previousTime, dt;

    Cube();

    void createContext();
    void draw();

    ~Cube();
};

#endif // CUBE_H
