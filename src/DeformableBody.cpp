#include "DeformableBody.h"
#include <random>

using namespace glm;
using namespace std;

// Standard acceleration due to gravity
#define g 9.80665f

// Load object using Drawble Class and then for each vertex create
// create and assign a Particle
DeformableBody::DeformableBody(std::string path, vec3 pos, vec3 vel, vec3 omega, float mass) : Drawable(path) {
    if (path.substr(path.size() - 3, 3) == "obj") {
        loadOBJWithTiny(path.c_str(), vertices, uvs, normals, VEC_UINT_DEFAUTL_VALUE);
    } else if (path.substr(path.size() - 3, 3) == "vtp") {
        loadVTP(path.c_str(), vertices, uvs, normals, VEC_UINT_DEFAUTL_VALUE);
    } else {
        throw runtime_error("File format not supported: " + path);
    }

    createContext();

    float particleMass = mass / indexedVertices.size();

    for (int i = 0; i < indexedVertices.size(); i++) {
        vec3 vertex = indexedVertices.at(i) + pos;
        Particle* particle = new Particle(vertex, vel, particleMass);
        particleSystem.push_back(particle);
        cout << "Particle " << i << " " << particle << endl;
    }
    cout << "Initialized Particle System with " << particleSystem.size() << " particles" << endl;
}


void DeformableBody::update(float t, float dt) {
    for (int i = 3; i < particleSystem.size(); i++) {
        // TODO: Calculate total force for particle:
        // Forces: gravity, spring-damp, shear, bending, collision
        vec3 force = vec3(
            0,
            -particleSystem.at(i)->m * g,
            0
        );
        particleSystem.at(i)->forcing = [&](float t, const vector<float>& y)->vector<float> {
            vector<float> f(6, 0.0f);
            f[0] = force.x;
            f[1] = force.y;
            f[2] = force.z;
            return f;
        };
        // Update particle's physics state
        particleSystem.at(i)->update(t, dt);
        // Get new position of particle and save it in the vertex
        indexedVertices.at(i) = particleSystem.at(i)->x;
    }
    // cout << particleSystem.at(0) << endl;

    // Update Vertices with new positions
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, indexedVertices.size() * sizeof(vec3), &indexedVertices[0]);
}


DeformableBody::~DeformableBody() {}