#include "Particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>

using namespace glm;

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