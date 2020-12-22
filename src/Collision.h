#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

class Box;
class Sphere;

void handleBoxSphereCollision(Box& box, Sphere& sphere);
bool checkForBoxSphereCollision(glm::vec3& pos, const float& r, const float& size, glm::vec3& n);

void handleSphereSphereCollision(Sphere& sphere1, Sphere& sphere2);
bool checkForSphereSphereCollision(glm::vec3& pos1, float& r1, glm::vec3& pos2, float& r2, glm::vec3& n);

#endif
