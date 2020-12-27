#include "Particle.h"


Particle::Particle(vec3 pos, vec3 vel, float mass) : RigidBody() {
    m = mass;
    x = pos;
    v = vel;
    P = m * v;
}


void Particle::update(float t, float dt) {
    //integration
    advanceState(t, dt);

    // compute model matrix
    modelMatrix = translate(mat4(), vec3(x.x, x.y, x.z));
}


void Particle::addNeighbor(Particle* neighbor, int type) {
    switch (type) {
    case STRUCT_NEIGHBOR:
        structNeighbors.push_back(neighbor);
        structDistances.push_back(x - neighbor->x);
        break;
    case SHEAR_NEIGHBOR:
        shearNeighbors.push_back(neighbor);
        shearDistances.push_back(x - neighbor->x);
        break;
    case BEND_NEIGHBOR:
        bendNeighbors.push_back(neighbor);
        bendDistances.push_back(x - neighbor->x);
        break;
    default:
        break;
    }
}
