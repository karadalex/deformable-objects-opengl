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
    vec3 n;
    if (checkForPlaneCubeCollision(cube.x, cube.l, plane.size, n)) {
        // Define the velocity of the cube after the collision
		cube.v = cube.v -2.0f * dot(cube.v, n) * n;
		cube.P = cube.v * cube.m;
    }
}

bool checkForPlaneCubeCollision(vec3& pos, const float& r, const float& size, vec3& n) {
    bool insideXZ = true; // TODO: Check if cube is inside XZ plane
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


// TODO
void handleStaircaseCubeCollision(Staircase& plane, Cube& cube) {

}


// TODO
bool checkForStaircaseCubeCollision(Staircase& plane, Cube& cube, glm::vec3& n) {

}