#ifndef COLLISION_H
#define COLLISION_H

#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Staircase.h"
#include <glm/glm.hpp>

class Box;
class Sphere;

void handlePlaneSphereCollision(Plane& plane, Sphere& sphere);
bool checkForPlaneSphereCollision(glm::vec3& pos, const float& r, const float& size, glm::vec3& n);

void handlePlaneCubeCollision(Plane& plane, Cube& cube);
bool checkForPlaneCubeCollision(glm::vec3& pos, const float& l, const float& size, glm::vec3& n);

void handleSphereSphereCollision(Sphere& sphere1, Sphere& sphere2);
bool checkForSphereSphereCollision(glm::vec3& pos1, float& r1, glm::vec3& pos2, float& r2, glm::vec3& n);

void handleStaircaseCubeCollision(Staircase& plane, Cube& cube);
bool checkForStaircaseCubeCollision(Staircase& plane, Cube& cube, glm::vec3& n);

#endif
