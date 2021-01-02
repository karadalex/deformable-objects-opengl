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
    vec3 p1p2 = x - neighbor->x;
    float p1p2d = sqrt(p1p2.x*p1p2.x + p1p2.y*p1p2.y + p1p2.z*p1p2.z);

    switch (type) {
    case STRUCT_NEIGHBOR:
        structNeighbors.push_back(neighbor);
        structDistances.push_back(p1p2d);
        break;
    case SHEAR_NEIGHBOR:
        shearNeighbors.push_back(neighbor);
        shearDistances.push_back(p1p2d);
        break;
    case BEND_NEIGHBOR:
        bendNeighbors.push_back(neighbor);
        bendDistances.push_back(p1p2d);
        break;
    default:
        break;
    }
}
