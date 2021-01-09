#include "DeformableBody.h"
#include <random>

using namespace glm;
using namespace std;

// Standard acceleration due to gravity
#define g 9.80665f

// Load object using Drawble Class and then for each vertex create
// create and assign a Particle
DeformableBody::DeformableBody(std::string path, vec3 pos, vec3 vel, vec3 omega, float mass, float stiffness, float damping) : Drawable(path) {
    k = stiffness;
    b = damping;
    
    if (path.substr(path.size() - 3, 3) == "obj") {
        loadOBJWithTiny(path.c_str(), vertices, uvs, normals, VEC_UINT_DEFAUTL_VALUE);
    } else if (path.substr(path.size() - 3, 3) == "vtp") {
        loadVTP(path.c_str(), vertices, uvs, normals, VEC_UINT_DEFAUTL_VALUE);
    } else {
        throw runtime_error("File format not supported: " + path);
    }

    createContext();

    // Create array of unique, non-repeated vertices and map them to the indexedVertices
    vector<vec3> uniqueVertices;
    for (int i = 0; i < indexedVertices.size(); i++) {
        vec3 v1 = indexedVertices.at(i);
        bool canAddParticle = true;
        int j;
        for (j = 0; j < uniqueVertices.size(); j++) {
            vec3 v2 = uniqueVertices.at(j);
            if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z) {
                canAddParticle = false;
                break;
            }
        }
        if (canAddParticle) {
            uniqueVertices.push_back(v1);
        }
        verticesToParticlesMap.push_back(j);
    }

    // Number of unique particles
    particlesNum = uniqueVertices.size();
    float particleMass = mass / particlesNum;

    for (int i = 0; i < particlesNum; i++) {
        vec3 vertex = uniqueVertices.at(i) + pos;
        Particle* particle = new Particle(vertex, vel, particleMass);
        particleSystem.push_back(particle);
        cout << "Particle " << i << " " << particle << endl;
    }
    cout << "Initialized Particle System with " << particlesNum << " particles" << endl;
    cout << "Total number of indexed vertices: " << indexedVertices.size() << endl;

    // Setup structural neighbors
    // Add connections with previous and next vertices from the indexedVertices array
    for (int i = 0; i < indexedVertices.size()-3; i += 3) {
        int p1Ind, p2Ind, p3Ind;
        Particle *p1, *p2, *p3;
        p1 = getVertexParticle(i, p1Ind);
        p2 = getVertexParticle(i+1, p2Ind);
        p3 = getVertexParticle(i+2, p3Ind);

        particleSystem.at(p1Ind)->addNeighbor(particleSystem.at(p2Ind), STRUCT_NEIGHBOR);
        particleSystem.at(p1Ind)->addNeighbor(particleSystem.at(p3Ind), STRUCT_NEIGHBOR);

        particleSystem.at(p2Ind)->addNeighbor(particleSystem.at(p1Ind), STRUCT_NEIGHBOR);
        particleSystem.at(p2Ind)->addNeighbor(particleSystem.at(p3Ind), STRUCT_NEIGHBOR);

        particleSystem.at(p3Ind)->addNeighbor(particleSystem.at(p1Ind), STRUCT_NEIGHBOR);
        particleSystem.at(p3Ind)->addNeighbor(particleSystem.at(p2Ind), STRUCT_NEIGHBOR);
    }

    // Uncomment the following for testing purposes
    // Test that particle 1 has neighbors, and that changes in one neighbor are seen from the others
    // vec3 p0p1 = particleSystem.at(1)->structDistances.at(0);
    // float p0p1_sq = p0p1.x*p0p1.x + p0p1.y*p0p1.y + p0p1.z*p0p1.z;
    // cout << "Rest length of P1 from P0 = " << p0p1_sq << endl;
    // vec3 p0p1_cur = particleSystem.at(1)->x - particleSystem.at(1)->structNeighbors.at(0)->x;
    // float p0p1_cur_sq = p0p1_cur.x*p0p1_cur.x + p0p1_cur.y*p0p1_cur.y + p0p1_cur.z*p0p1_cur.z;
    // cout << "Current distance of P1 from P0 = " << p0p1_cur_sq << endl;
    // particleSystem.at(0)->x = vec3(0, 0, 0);
    // cout << "Rest length after change of P1 from P0 = " << p0p1_sq << endl;
    // p0p1_cur = particleSystem.at(1)->x - particleSystem.at(1)->structNeighbors.at(0)->x;
    // p0p1_cur_sq = p0p1_cur.x*p0p1_cur.x + p0p1_cur.y*p0p1_cur.y + p0p1_cur.z*p0p1_cur.z;
    // cout << "Current distance of P1 from P0 = " << p0p1_cur_sq << endl;
    // cout << "Particle 1 neighbors:" << endl;
    // for (int i = 0; i < particleSystem.at(1)->structNeighbors.size(); i++) {
    //     cout << particleSystem.at(1)->structNeighbors.at(i) << endl;
    // }

    // TODO: Setup shear neighbors

    // TODO: Setup bending neighbors
}


void DeformableBody::update(float t, float dt) {
    for (int i = 0; i < particlesNum; i++) {
        Particle* prt1 = particleSystem.at(i);
        // TODO: Calculate total force for particle:
        // Forces: gravity, spring-damp, shear, bending, collision
        vec3 gravity = vec3(
            0,
            -prt1->m * g,
            0
        );
        
        vec3 damping = -b*prt1->v; 
        vec3 elastic = -k*vec3(0, prt1->x.y, 0);
        // vec3 damping = vec3(0,0,0);
        // vec3 elastic = vec3(0,0,0);
        // for (int j = 0; j < prt1->structNeighbors.size(); j++) {
        //     Particle* prt2 = prt1->structNeighbors.at(j);
        //     vec3 p1p2 = prt1->x - prt2->x;
        //     float p1p2d = sqrt(p1p2.x*p1p2.x + p1p2.y*p1p2.y + p1p2.z*p1p2.z);
        //     if (p1p2d >= 0.0000001f) {
        //         vec3 unit_vec = p1p2 / p1p2d;
        //         elastic += -k * (p1p2d - prt1->structDistances.at(j)) * unit_vec;
                
        //         vec3 vel = (dot(prt2->v, unit_vec) - dot(prt1->v, unit_vec)) * unit_vec;
        //         damping += -b*vel;
        //     }
        // }

        vec3 force = gravity + elastic + damping;
        prt1->forcing = [&](float t, const vector<float>& y)->vector<float> {
            vector<float> f(6, 0.0f);
            f[0] = force.x;
            f[1] = force.y;
            f[2] = force.z;
            return f;
        };
        // Update particle's physics state
        prt1->update(t, dt);
    }

    // Get new position of particle and save it in the vertex
    for (int i = 0; i < indexedVertices.size(); i++) {
        int prtInd;
        Particle* particle = getVertexParticle(i, prtInd);
        indexedVertices.at(i) = particle->x;
    }
    

    // Update Vertices with new positions
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, indexedVertices.size() * sizeof(vec3), &indexedVertices[0]);
}


Particle* DeformableBody::getVertexParticle(int index, int &particleIndex) {
    particleIndex = verticesToParticlesMap.at(index);
    // cout << "vertex " << index << " maps to particle " << particleIndex << endl;
    Particle* mappedParticle = particleSystem.at(particleIndex);
    return mappedParticle;
}


DeformableBody::~DeformableBody() {}
