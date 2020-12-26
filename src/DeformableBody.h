#ifndef DEFORMABLE_BODY_H
#define DEFORMABLE_BODY_H

#include "RigidBody.h"
#include "Particle.h"
#include <common/model.h>
#include <vector>
#include <string>
#include <iostream>


class DeformableBody : public Drawable {
public:
    DeformableBody(std::string path, vec3 pos, vec3 vel, vec3 omega, float mass);
    void update(float t = 0, float dt = 0);
    ~DeformableBody();

    std::vector<Particle*> particleSystem;
};

#endif