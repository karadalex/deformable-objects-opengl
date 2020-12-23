#include "DeformableBody.h"
#include <random>

using namespace glm;
using namespace std;

// Standard acceleration due to gravity
#define g 9.80665f

// Load object using Drawble Class and then for each vertex create
// create and assign a Particle
DeformableBody::DeformableBody(std::string path) : Drawable(path) {
    if (path.substr(path.size() - 3, 3) == "obj") {
        loadOBJWithTiny(path.c_str(), vertices, uvs, normals, VEC_UINT_DEFAUTL_VALUE);
    } else if (path.substr(path.size() - 3, 3) == "vtp") {
        loadVTP(path.c_str(), vertices, uvs, normals, VEC_UINT_DEFAUTL_VALUE);
    } else {
        throw runtime_error("File format not supported: " + path);
    }

    createContext();

    for (int i = 0; i < indexedVertices.size(); i++) {
        vec3 vertex = indexedVertices.at(i);
        vec3 vel = vec3(0, 0, 0);
        Particle* particle = new Particle(vertex, vel, 0.1);
        particleSystem.push_back(particle);
        cout << "Particle " << i << " " << particle << endl;
    }
    cout << "Initialized Particle System with " << particleSystem.size() << " particles" << endl;
}


void DeformableBody::update(float t, float dt) {
    for (int i = 0; i < particleSystem.size(); i++) {
        particleSystem.at(i)->forcing = [&](float t, const vector<float>& y)->vector<float> {
            vector<float> f(6, 0.0f);
            f[1] = particleSystem.at(i)->m * g;
            return f;
        };
        // Update particle's physics state
        particleSystem.at(i)->update(t, dt);
        // Get new position of particle and save it in the vertex
        indexedVertices.at(i) = particleSystem.at(i)->x;
    }

    // Update Vertices with new positions
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, indexedVertices.size() * sizeof(vec3), &indexedVertices[0]);
}


DeformableBody::~DeformableBody() {}