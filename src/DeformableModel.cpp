#include "DeformableModel.h"


DeformableModel::DeformableModel(string modelFile, vec3 pos, vec3 vel, vec3 omega, float length, float mass, float stiffness, float damping) : RigidBody() {
    model = new DeformableBody(modelFile, pos, vel, omega, mass, stiffness, damping);

    l = length;
    m = mass;
    x = pos;
    v = vel;
    P = m * v;
    w = omega;
}

DeformableModel::~DeformableModel() {
    delete model;
}

void DeformableModel::draw(bool showModelVertices) {
    if (showModelVertices) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);    
    }

    model->bind();
    model->draw();

    if (showModelVertices) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }
}

void DeformableModel::update(float t, float dt) {
    model->update(t, dt);

    // numerical integration
    // advanceState(t, dt);

    // compute model matrix
    // mat4 scale = glm::scale(mat4(), vec3(l, l, l));
    // mat4 tranlation = translate(mat4(), vec3(x.x, x.y, x.z));
    // modelMatrix = tranlation * scale;
}
