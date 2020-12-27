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
    /**
     * @brief DeformableBody
     * @param path
     * @param pos
     * @param vel
     * @param omega
     * @param mass
     * @param stiffness
     * @param damping
     */
    DeformableBody(std::string path, vec3 pos, vec3 vel, vec3 omega, float mass, float stiffness, float damping);

    /**
     * @brief update
     * @param t
     * @param dt
     */
    void update(float t = 0, float dt = 0);
    ~DeformableBody();

    std::vector<Particle*> particleSystem;
    
    float k = 0.5; // stiffness
    float b = 0.01; // damping
};

#endif
