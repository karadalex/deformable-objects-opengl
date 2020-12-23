#include "DeformableBody.h"

using namespace glm;
using namespace std;

// Load object using Drawble Class and then for each vertex create
// create and assign a Particle
DeformableBody::DeformableBody(std::string path) : Drawable(path) {
    for (int i = 0; i < vertices.size(); i++) {
        vec3 vertex = vertices.at(i);
        vec3 vel = vec3(0, 0, 0);
        Particle* particle = new Particle(vertex, vel, 0.1);
        particleSystem.push_back(particle);
        cout << "Particle " << i << " " << particle << endl;
    }
    cout << "Initialized Particle System with " << particleSystem.size() << " particles" << endl;
}


void DeformableBody::update(float t, float dt) {
    for (int i = 0; i < particleSystem.size(); i++) {
        // Update particle's physics state
        particleSystem.at(i)->update(t, dt);
        // Get new position of particle and save it in the vertex
        vertices.at(i) = particleSystem.at(i)->x;
    }
    
}


DeformableBody::~DeformableBody() {}