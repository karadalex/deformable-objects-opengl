#include "Collision.h"
#include "Box.h"
#include "Sphere.h"

using namespace glm;
using namespace std;

void handleBoxSphereCollision(Box& box, Sphere& sphere) {
    vec3 n;
    if (checkForBoxSphereCollision(sphere.x, sphere.r, box.size, n)) {
        // Task 2b: define the velocity of the sphere after the collision
		sphere.v = sphere.v -2.0f * dot(sphere.v, n) * n;
		sphere.P = sphere.v * sphere.m;
    }
}

bool checkForBoxSphereCollision(vec3& pos, const float& r, const float& size, vec3& n) {
    if (pos.x - r <= 0) {
        //correction
        float dis = -(pos.x - r);
        pos = pos + vec3(dis, 0, 0);

        n = vec3(-1, 0, 0);
    } else if (pos.x + r >= size) {
        //correction
        float dis = size - (pos.x + r);
        pos = pos + vec3(dis, 0, 0);

        n = vec3(1, 0, 0);
    } else if (pos.y - r <= 0) {
        //correction
        float dis = -(pos.y - r);
        pos = pos + vec3(0, dis, 0);

        n = vec3(0, -1, 0);
    } else if (pos.y + r >= size) {
        //correction
        float dis = size - (pos.y + r);
        pos = pos + vec3(0, dis, 0);

        n = vec3(0, 1, 0);
    } else if (pos.z - r <= 0) {
        //correction
        float dis = -(pos.z - r);
        pos = pos + vec3(0, 0, dis);

        n = vec3(0, 0, -1);
    } else if (pos.z + r >= size) {
        //correction
        float dis = size - (pos.z + r);
        pos = pos + vec3(0, 0, dis);

        n = vec3(0, 0, 1);
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