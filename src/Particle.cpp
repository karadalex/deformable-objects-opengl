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
    if (x.x != neighbor->x.x &&
        x.y != neighbor->x.y &&
        x.z != neighbor->x.z) 
    {    
        vec3 p1p2 = x - neighbor->x;
        float p1p2d = sqrt(p1p2.x*p1p2.x + p1p2.y*p1p2.y + p1p2.z*p1p2.z);
        
        bool canAddNeighbor = true;
        switch (type) {
        case STRUCT_NEIGHBOR:
            for (int i = 0; i < structNeighbors.size(); i++) {
                Particle* structNeighbor = structNeighbors.at(i);
                if (structNeighbor->x.x == neighbor->x.x &&
                    structNeighbor->x.y == neighbor->x.y &&
                    structNeighbor->x.z == neighbor->x.z) {
                    canAddNeighbor = false;
                    break;
                }
            }
            if (canAddNeighbor) {
                structNeighbors.push_back(neighbor);
                structDistances.push_back(p1p2d);
            }
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
}
