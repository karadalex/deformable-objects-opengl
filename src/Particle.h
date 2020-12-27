#ifndef PARTICLE_H
#define PARTICLE_H

#include "RigidBody.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <vector>

using namespace glm;
using namespace std;

class Particle : public RigidBody {
public:
    
    mat4 modelMatrix;
    vec3 force;
    // other properties that are inherited from RigidBody:
    // position, velocity, mass

    // Neighbor particles responsible for elastic, shear and bending forces
    vector<Particle*> structNeighbors, shearNeighbors, bendNeighbors;

    Particle(vec3 pos, vec3 vel, float mass);
    void update(float t = 0, float dt = 0);

    /**
     * Overload << operator to print glm::vec3 particle's properties
     */
    friend ostream& operator<<(ostream& output, const Particle* p) {
        output << "Position: (" << p->x.x << "," << p->x.y << "," << p->x.z << "), Velocity: (" << p->v.x << "," << p->v.y << "," << p->v.z << ")";
        return output;
    }
};

#endif