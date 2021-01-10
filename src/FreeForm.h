#ifndef FREE_FORM_H
#define FREE_FORM_H

#include "Particle.h"
#include <common/model.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace glm;


class FreeForm {
public:
    GLuint VAO, VBO, EBO;
    vector<vec3> vertices;
    vector<uvec4> indices;

    FreeForm();

    void createContext();

    void bind();

    /* Bind VAO before calling draw */
    void draw(int mode = GL_TRIANGLES);

    /**
     * @brief update
     * @param t
     * @param dt
     */
    void update(float t = 0, float dt = 0);

    ~FreeForm();
};

#endif
