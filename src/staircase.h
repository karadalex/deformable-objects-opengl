#ifndef STAIRCASE_H
#define STAIRCASE_H

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

class Staircase
{
public:
    GLuint staircaseVAO;
    GLuint staircaseVerticesVBO, staircaseColorsVBO;
    GLuint MVPLocation;
    GLuint shaderProgram;
    GLfloat previousTime, dt;
    vec3 position;
    int density;
    int stairs;

    vector<GLfloat> staircaseVertices;

    Staircase(int stairs, vec3 position = vec3(0, 0, 0), int density = 1);

    void createContext();
    void draw();

    ~Staircase();
};

#endif // STAIRCASE_H
