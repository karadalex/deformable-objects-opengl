#ifndef SPHERE_H
#define SPHERE_H

#include "RigidBody.h"
#include "Particle.h"
#include <common/model.h>
#include <vector>
#include <string>
#include <iostream>

class DeformableBody : public Drawable {
public:
    DeformableBody(std::string path);
    ~DeformableBody();

    std::vector<Particle*> particleSystem;
};

#endif