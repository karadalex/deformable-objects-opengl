#include "DeformableBody.h"

using namespace glm;
using namespace std;

// Load object using Drawble Class and then for each vertex create
// create and assign a Particle
DeformableBody::DeformableBody(std::string path) : Drawable(path) {
    for (int i = 0; i < vertices.size(); i++) {
        vec3 vertex = vertices.at(i);
        vec3 vel = vec3(0, 0, 0);
        particleSystem.push_back(new Particle(vertex, vel, 0.1));
    }
    cout << "Initialized Particle System with " << particleSystem.size() << " particles" << endl;
}


DeformableBody::~DeformableBody() {}