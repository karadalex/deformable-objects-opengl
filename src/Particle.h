#ifndef PARTICLE_H
#define PARTICLE_H

#include "RigidBody.h"


class Particle : public RigidBody {
public:
    glm::mat4 modelMatrix;

    Particle(glm::vec3 pos, glm::vec3 vel, float mass);
    void update(float t = 0, float dt = 0);
};

#endif