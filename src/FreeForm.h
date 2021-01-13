#ifndef FREE_FORM_H
#define FREE_FORM_H

#include "Grid3D.h"
#include "Particle.h"
#include <common/model.h>
#include <vector>
#include <string>
#include <iostream>
#include <common/util.h>

using namespace std;
using namespace glm;

// Standard acceleration due to gravity
#define g 9.80665f

class FreeForm {
public:
    GLuint VAO, VBO, EBO;
    vector<vec3> vertices;
    vector<uvec4> indices;

    std::vector<Particle*> particleSystem; 
    int particlesNum;   
    float k0 = 0.5; // stiffness
    float b = 0.01; // damping

    FreeForm(vec3 position, vec3 vel, vec3 omega, float mass, float stiffness, float damping);

    void createContext();

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
