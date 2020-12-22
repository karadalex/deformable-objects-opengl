#ifndef CUBE_CUSTOM_H
#define CUBE_CUSTOM_H

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

class CubeCustom
{
public:
    GLuint cubeVAO;
    GLuint cubeVerticesVBO, cubeColorsVBO;
    GLuint MVPLocation;
    GLuint shaderProgram;
    GLfloat previousTime, dt;
    vec3 position;
    int density;

    vector<GLfloat> cubeVertices;

    CubeCustom(vec3 position = vec3(0, 0, 0), int density = 1);

    void createContext();
    void draw();

    ~CubeCustom();
};

#endif // CUBE_CUSTOM_H
