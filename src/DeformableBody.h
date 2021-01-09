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

    /**
     * @brief Get the particle that is associated with a specific vertex
     * from indexed vertices
     * @param index : THis is the index pointing to indexedVertices array inherited from the Drawable class
     * @return Particle instance pointer
     */
    Particle* getVertexParticle(int index, int &prtPos);

    ~DeformableBody();

    std::vector<Particle*> particleSystem;
    std::vector<int> verticesToParticlesMap;
    int particlesNum;
    
    float k = 0.5; // stiffness
    float b = 0.01; // damping
};

#endif
