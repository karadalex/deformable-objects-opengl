#ifndef CUBE_H
#define CUBE_H

#include "RigidBody.h"
#include "DeformableBody.h"

class DeformableBody;

class Cube : public RigidBody {
public:
    DeformableBody* cube;
    float l;
    glm::mat4 modelMatrix;

    /**
     * @brief Cube
     * @param pos
     * @param vel
     * @param omega
     * @param length
     * @param mass
     * @param stiffness
     * @param damping
     */
    Cube(glm::vec3 pos, glm::vec3 vel, glm::vec3 omega, float length, float mass, float stiffness, float damping);

    ~Cube();

    /**
     * @brief draw
     * @param showCubeVertices
     */
    void draw(bool showCubeVertices = false);

    /**
     * @brief update
     * @param t
     * @param dt
     */
    void update(float t = 0, float dt = 0);
};

#endif
