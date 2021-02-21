#ifndef COLLISION_H
#define COLLISION_H

#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Staircase.h"
#include "Particle.h"
#include "DeformableModel.h"
#include "FreeForm.h"
#include <glm/glm.hpp>

class Box;
class Sphere;

/**
 * @brief handlePlaneSphereCollision
 * @param plane
 * @param sphere
 */
void handlePlaneSphereCollision(Plane& plane, Sphere& sphere);

/**
 * @brief checkForPlaneSphereCollision
 * @param pos
 * @param r
 * @param size
 * @param n
 * @return
 */
bool checkForPlaneSphereCollision(glm::vec3& pos, const float& r, const float& size, glm::vec3& n);

/**
 * @brief handlePlaneCubeCollision
 * @param plane
 * @param cube
 */
void handlePlaneCubeCollision(Plane& plane, Cube& cube);

/**
 * @brief handlePlaneModelCollision
 * @param plane
 * @param model
 * @return
 */
void handlePlaneModelCollision(Plane& plane, DeformableModel& model);

/**
 * @brief handlePlaneFreeFormCollision
 * @param plane
 * @param model
 * @return
 */
void handlePlaneFreeFormCollision(Plane& plane, FreeForm& model);

/**
 * @brief checkForPlaneParticleCollision
 * @param particle_pos
 * @param n
 * @return
 */
bool checkForPlaneParticleCollision(vec3& particle_pos, vec3& n);

/**
 * @brief handleSphereSphereCollision
 * @param sphere1
 * @param sphere2
 */
void handleSphereSphereCollision(Sphere& sphere1, Sphere& sphere2);

/**
 * @brief checkForSphereSphereCollision
 * @param pos1
 * @param r1
 * @param pos2
 * @param r2
 * @param n
 * @return
 */
bool checkForSphereSphereCollision(glm::vec3& pos1, float& r1, glm::vec3& pos2, float& r2, glm::vec3& n);

/**
 * @brief handleStaircaseCubeCollision
 * @param staircase
 * @param cube
 */
void handleStaircaseCubeCollision(Staircase& staircase, Cube& cube);

/**
 * @brief handleStaircaseFreeFormCollision
 * @param staircase
 * @param model
 */
void handleStaircaseFreeFormCollision(Staircase& staircase, FreeForm& model) ;

/**
 * @brief checkForStaircaseParticleCollision
 * @param staircase
 * @param p: particle position
 * @param n
 * @return
 */
bool checkForStaircaseParticleCollision(Staircase& staircase, vec3& p, vec3& n);

void handleStaircaseModelCollision(Staircase& staircase, DeformableModel& model);

#endif
