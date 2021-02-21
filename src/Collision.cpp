#include "Collision.h"


using namespace glm;
using namespace std;


void handlePlaneSphereCollision(Plane& plane, Sphere& sphere) {
    vec3 n;
    if (checkForPlaneSphereCollision(sphere.x, sphere.r, plane.size, n)) {
        // Define the velocity of the sphere after the collision
		sphere.v = sphere.v -2.0f * dot(sphere.v, n) * n;
		sphere.P = sphere.v * sphere.m;
    }
}


bool checkForPlaneSphereCollision(vec3& pos, const float& r, const float& size, vec3& n) {
    bool insideXZ = true; // TODO: Check if sphere is inside XZ plane
    if (pos.y - r <= 0 && insideXZ) {
        //correction
        float dis = -(pos.y - r);
        pos = pos + vec3(0, dis, 0);

        n = vec3(0, -1, 0);
    } else {
        return false;
    }
    return true;
}


void handlePlaneCubeCollision(Plane& plane, Cube& cube) {
    for (int i = 0; i < cube.particleSystem.size(); i++) {
        vec3 n;
        if (checkForPlaneParticleCollision(cube.particleSystem.at(i)->x, n)) {
            // Define the velocity of the cube after the collision
            cube.particleSystem.at(i)->v = (cube.particleSystem.at(i)->v - 2.0f * dot(cube.particleSystem.at(i)->v, n) * n) * (1.0f/(float)cube.particlesNum);
            cube.particleSystem.at(i)->P = cube.particleSystem.at(i)->v * cube.particleSystem.at(i)->m * (1.0f/(float)cube.particlesNum);
        }
    }  
}


void handlePlaneModelCollision(Plane& plane, DeformableModel& model) {
    for (int i = 0; i < model.model->particleSystem.size(); i++) {
        vec3 n;
        if (checkForPlaneParticleCollision(model.model->particleSystem.at(i)->x, n)) {
            // Define the velocity of the model after the collision
            model.model->particleSystem.at(i)->v = model.model->particleSystem.at(i)->v - 2.0f * dot(model.model->particleSystem.at(i)->v, n) * n;
            model.model->particleSystem.at(i)->P = model.model->particleSystem.at(i)->v * model.model->particleSystem.at(i)->m;
        }
    }
}


void handlePlaneFreeFormCollision(Plane& plane, FreeForm& model) {
    for (int i = 0; i < model.particleSystem.size(); i++) {
        vec3 n;
        if (checkForPlaneParticleCollision(model.particleSystem.at(i)->x, n)) {
            // Define the velocity of the model after the collision
            model.particleSystem.at(i)->v = (model.particleSystem.at(i)->v - 2.0f * dot(model.particleSystem.at(i)->v, n) * n) * (1.0f/(float)model.particlesNum);
            model.particleSystem.at(i)->P = model.particleSystem.at(i)->v * model.particleSystem.at(i)->m * (1.0f/(float)model.particlesNum);
        }
    }
}


bool checkForPlaneParticleCollision(vec3& particle_pos, vec3& n) {
    if (particle_pos.y < 0) {
        //correction
        float dis = -particle_pos.y;
        particle_pos += vec3(0, dis, 0);

        n = vec3(0, 1, 0);
    } else {
        return false;
    }
    return true;
}


// TODO
void handleStaircaseCubeCollision(Staircase& staircase, Cube& cube) {
    for (int i = 0; i < cube.particleSystem.size(); i++) {
        vec3 n;
        if (checkForStaircaseParticleCollision(staircase, cube.particleSystem.at(i)->x, n)) {
            // Define the velocity of the cube after the collision
            cube.particleSystem.at(i)->v = cube.particleSystem.at(i)->v - 2.0f * dot(cube.particleSystem.at(i)->v, n) * n;
            cube.particleSystem.at(i)->P = cube.particleSystem.at(i)->v * cube.particleSystem.at(i)->m;
        }
    }
}

void handleStaircaseFreeFormCollision(Staircase& staircase, FreeForm& model) {
    for (int i = 0; i < model.particleSystem.size(); i++) {
        vec3 n;
        if (checkForStaircaseParticleCollision(staircase, model.particleSystem.at(i)->x, n)) {
            // Define the velocity of the model after the collision
            model.particleSystem.at(i)->v = model.particleSystem.at(i)->v - 2.0f * dot(model.particleSystem.at(i)->v, n) * n;
            model.particleSystem.at(i)->P = model.particleSystem.at(i)->v * model.particleSystem.at(i)->m;
        }
    }
}


// TODO
bool checkForStaircaseParticleCollision(Staircase& staircase, vec3& p, vec3& n) {
    for (int i = 0; i < staircase.staircase->indexedVertices.size(); i++) {
        // Staircase vertex
        vec3 st_v = staircase.staircase->indexedVertices.at(i);
        if (p.y <= st_v.y && abs(p.x - st_v.x) <= 2.0f && abs(p.z - st_v.z) <= 2.0f ) {
            //correction
            cout << "staircase collision" << endl;
            float dis = st_v.y - p.y;
            p += vec3(0, dis, 0);
            n = vec3(0, 1, 0.01);
            return true;
        }
    }
    cout << "no staircase collision" << endl;

    return false;
}


void handleSphereSphereCollision(Sphere& sphere1, Sphere& sphere2)
{
    vec3 n;
    if (checkForSphereSphereCollision(sphere1.x, sphere1.r, sphere2.x, sphere2.r, n)) {
		sphere1.v = -sphere1.v;
		sphere1.P = sphere1.v * sphere1.m;
        sphere2.v = -sphere2.v;
		sphere2.P = sphere2.v * sphere2.m;
    }
}


bool checkForSphereSphereCollision(vec3& pos1, float& r1, vec3& pos2, float& r2, vec3& n)
{
	n = vec3(0, 0, 1);
    if (distance(pos1, pos2) < r1 + r2)
    {
        return true;
    }
    else
    {
		return false;
    }   
}


void handleParticleParticleCollision(Plane& plane, Sphere& sphere) {
    vec3 n;
    if (checkForPlaneSphereCollision(sphere.x, sphere.r, plane.size, n)) {
        // Define the velocity of the sphere after the collision
		sphere.v = sphere.v -2.0f * dot(sphere.v, n) * n;
		sphere.P = sphere.v * sphere.m;
    }
}


bool checkForParticleParticleCollision(vec3& pos, const float& r, const float& size, vec3& n) {
    bool insideXZ = true; // TODO: Check if sphere is inside XZ plane
    if (pos.y - r <= 0 && insideXZ) {
        //correction
        float dis = -(pos.y - r);
        pos = pos + vec3(0, dis, 0);

        n = vec3(0, -1, 0);
    } else {
        return false;
    }
    return true;
}


void handleStaircaseModelCollision(Staircase& staircase, DeformableModel& model) {
    for (int i = 0; i < model.model->particleSystem.size(); i++) {
        vec3 n;
        if (checkForStaircaseParticleCollision(staircase, model.model->particleSystem.at(i)->x, n)) {
            // Define the velocity of the model after the collision
            model.model->particleSystem.at(i)->v = model.model->particleSystem.at(i)->v - 2.0f * dot(model.model->particleSystem.at(i)->v, n) * n;
            model.model->particleSystem.at(i)->P = model.model->particleSystem.at(i)->v * model.model->particleSystem.at(i)->m;
        }
    }
}