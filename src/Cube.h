#ifndef CUBE_H
#define CUBE_H

#include "RigidBody.h"
#include "DeformableBody.h"

class DeformableBody;

class Cube : public RigidBody {
public:
    DeformableBody* cube;
    float l;
    glm::mat4 modelMatrix;

    Cube(glm::vec3 pos, glm::vec3 vel, glm::vec3 omega, float length, float mass);
    ~Cube();

    void draw(bool showCubeVertices = false);
    void update(float t = 0, float dt = 0);
};

#endif