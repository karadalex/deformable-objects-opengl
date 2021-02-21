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
    for (int i = 0; i < indices.size()-2; i += 3) {
        int ind1, ind2, ind3;
        ind1 = indices.at(i);
        ind2 = indices.at(i+1);
        ind3 = indices.at(i+2);
        int p1Ind, p2Ind, p3Ind;
        Particle *p1, *p2, *p3;
        p1 = getVertexParticle(ind1, p1Ind);
        p2 = getVertexParticle(ind2, p2Ind);
        p3 = getVertexParticle(ind3, p3Ind);

        particleSystem.at(p1Ind)->addNeighbor(particleSystem.at(p2Ind), STRUCT_NEIGHBOR);
        particleSystem.at(p1Ind)->addNeighbor(particleSystem.at(p3Ind), STRUCT_NEIGHBOR);

        particleSystem.at(p2Ind)->addNeighbor(particleSystem.at(p1Ind), STRUCT_NEIGHBOR);
        particleSystem.at(p2Ind)->addNeighbor(particleSystem.at(p3Ind), STRUCT_NEIGHBOR);

        particleSystem.at(p3Ind)->addNeighbor(particleSystem.at(p1Ind), STRUCT_NEIGHBOR);
        particleSystem.at(p3Ind)->addNeighbor(particleSystem.at(p2Ind), STRUCT_NEIGHBOR);
    }

    // Find the 2 extreme vertices for each x,y,z directions and connect them with a structural
    // constraint
    Particle *prtMinX, *prtMaxX, *prtMinY, *prtMaxY, *prtMinZ, *prtMaxZ;
    int pIndMinX, pIndMaxX, pIndMinY, pIndMaxY, pIndMinZ, pIndMaxZ;
    pIndMinX = pIndMaxX = pIndMinY = pIndMaxY = pIndMinZ = pIndMaxZ = 0;
    prtMinX = prtMaxX = prtMinY = prtMaxY = prtMinZ = prtMaxZ = particleSystem.at(0);
    for (int i = 0; i < particlesNum; i++) {
        Particle *particle = particleSystem.at(i);

        if (particle->x.x < prtMinX->x.x) {
            pIndMinX = i;
            prtMinX = particle;
        }
        if (particle->x.y < prtMinX->x.y) {
            pIndMinY = i;
            prtMinY = particle;
        }
        if (particle->x.z < prtMinX->x.z) {
            pIndMinZ = i;
            prtMinZ = particle;
        }

        if (particle->x.x > prtMaxX->x.x) {
            pIndMaxX = i;
            prtMaxX = particle;
        }
        if (particle->x.y > prtMaxX->x.y) {
            pIndMaxY = i;
            prtMaxY = particle;
        }
        if (particle->x.z > prtMaxX->x.z) {
            pIndMaxZ = i;
            prtMaxZ = particle;        
        }
        
    }
    particleSystem.at(pIndMinX)->addNeighbor(particleSystem.at(pIndMaxX), STRUCT_NEIGHBOR);
    particleSystem.at(pIndMinY)->addNeighbor(particleSystem.at(pIndMaxY), STRUCT_NEIGHBOR);
    particleSystem.at(pIndMinZ)->addNeighbor(particleSystem.at(pIndMaxZ), STRUCT_NEIGHBOR);
    cout << "pIndMinX = " << pIndMinX << ", pIndMinY = " << pIndMinY << ", pIndMinZ = " << pIndMinZ << endl;
    cout << "pIndMaxX = " << pIndMaxX << ", pIndMaxY = " << pIndMaxY << ", pIndMaxZ = " << pIndMaxZ << endl;

    // Uncomment the following for testing purposes
    // Test that particle 1 has neighbors, and that changes in one neighbor are seen from the others
    // int ind1 = 1;
    // int ind2 = 0;
    int ind1 = pIndMinX;
    int ind2 = pIndMaxX;
    // float p0p1 = particleSystem.at(ind1)->structDistances.at(ind2);
    // cout << "ind1 = " << ind1 << ", ind2 = " << ind2 << endl;
    // cout << "Rest length of P(ind1) from P(ind2) = " << p0p1 << endl;
    // vec3 p0p1_cur = particleSystem.at(ind1)->x - particleSystem.at(ind1)->structNeighbors.at(ind2)->x;
    // float p0p1_cur_sq = p0p1_cur.x*p0p1_cur.x + p0p1_cur.y*p0p1_cur.y + p0p1_cur.z*p0p1_cur.z;
    // cout << "Current distance of P(ind1) from P(ind2) = " << p0p1_cur_sq << endl;
    // particleSystem.at(ind2)->x = vec3(0, 0, 0);
    // cout << "Rest length after change of P(ind1) from P(ind2) = " << p0p1 << endl;
    // p0p1_cur = particleSystem.at(ind1)->x - particleSystem.at(ind1)->structNeighbors.at(ind2)->x;
    // p0p1_cur_sq = p0p1_cur.x*p0p1_cur.x + p0p1_cur.y*p0p1_cur.y + p0p1_cur.z*p0p1_cur.z;
    // cout << "Current distance of P(ind1) from P(ind2) = " << p0p1_cur_sq << endl;
    // cout << "Particle ind1 neighbors:" << endl;
    for (int i = 0; i < particleSystem.at(ind1)->structNeighbors.size(); i++) {
        cout << "Particle " << i << " " << particleSystem.at(ind1)->structNeighbors.at(i) << endl;
    }

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
        
        // vec3 damping = -b*prt1->v; 
        // vec3 elastic = -k*vec3(0, prt1->x.y, 0);
        vec3 damping = vec3(0,0,0);
        vec3 elastic = vec3(0,0,0);
        for (int j = 0; j < prt1->structNeighbors.size(); j++) {
            Particle* prt2 = prt1->structNeighbors.at(j);
            vec3 p1p2 = prt1->x - prt2->x;
            float p1p2d = sqrt(p1p2.x*p1p2.x + p1p2.y*p1p2.y + p1p2.z*p1p2.z);
            vec3 unit_vec = p1p2;
            elastic += -k * (p1p2d - prt1->structDistances.at(j)) * unit_vec;
            
            vec3 vel = (dot(prt1->v, unit_vec) - dot(prt2->v, unit_vec)) * unit_vec;
            damping += -b*vel;
        }

        vec3 force = gravity + elastic + damping;
        // if (i == 0) {
        //     printVec3(gravity, "gravity");
        //     printVec3(elastic, "elastic");
        //     printVec3(damping, "damping");
        // }
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
