#ifndef SPHERE_H
#define SPHERE_H

#include "RigidBody.h"

class Drawable;

class Sphere : public RigidBody {
public:
    Drawable* sphere;
    float r;
    glm::mat4 modelMatrix;

    /**
     * @brief Sphere
     * @param pos
     * @param vel
     * @param radius
     * @param mass
     */
    Sphere(glm::vec3 pos, glm::vec3 vel, float radius, float mass);
    ~Sphere();

    /**
     * @brief draw
     * @param drawable
     */
    void draw(unsigned int drawable = 0);

    /**
     * @brief update
     * @param t
     * @param dt
     */
    void update(float t = 0, float dt = 0);
};

#endif
