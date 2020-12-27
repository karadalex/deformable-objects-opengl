#ifndef PARTICLE_H
#define PARTICLE_H

#include "RigidBody.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <vector>

using namespace glm;
using namespace std;

// Neighbor types constants
#define STRUCT_NEIGHBOR 0
#define SHEAR_NEIGHBOR 1
#define BEND_NEIGHBOR 2


class Particle : public RigidBody {
public:
    
    mat4 modelMatrix;
    vec3 force;
    // other properties that are inherited from RigidBody:
    // position, velocity, mass

    // Neighbor particles responsible for elastic, shear and bending forces
    vector<Particle*> structNeighbors, shearNeighbors, bendNeighbors;
    // Initial distances from neighbors (rest lengths)
    // these lists have a 1-1 correspondace with the particle lists above
    vector<vec3> structDistances, shearDistances, bendDistances;
    // Unit vectors connecting the neighbors
    vector<vec3> structUnitVectors, shearUnitVectors, bendUnitVectors;

    /**
     * @brief Particle
     * @param pos
     * @param vel
     * @param mass
     */
    Particle(vec3 pos, vec3 vel, float mass);

    /**
     * @brief update
     * @param t
     * @param dt
     */
    void update(float t = 0, float dt = 0);

    /**
     * @brief addNeighbor
     * @param neighbor
     * @param type
     */
    void addNeighbor(Particle* neighbor, int type);

    /**
     * @brief Overload << operator to print glm::vec3 particle's properties
     * @param output
     * @param p
     * @return
     */
    friend ostream& operator<<(ostream& output, const Particle* p) {
        output << "Position: (" << p->x.x << "," << p->x.y << "," << p->x.z << "), Velocity: (" << p->v.x << "," << p->v.y << "," << p->v.z << ")";
        return output;
    }
};

#endif
