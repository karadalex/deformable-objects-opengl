#ifndef DEFORMABLE_MODEL_H
#define DEFORMABLE_MODEL_H

#include "RigidBody.h"
#include "DeformableBody.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>

using namespace glm;
using namespace std;

class DeformableBody;

class DeformableModel : public RigidBody {
public:
    DeformableBody* model;
    glm::mat4 modelMatrix;
    float l;

    /**
     * @brief DeformableModel
     * @param modelFile
     * @param pos
     * @param vel
     * @param omega
     * @param length
     * @param mass
     * @param stiffness
     * @param damping
     */
    DeformableModel(string modelFile, vec3 pos, vec3 vel, vec3 omega, float length, float mass, float stiffness, float damping);
    ~DeformableModel();

    /**
     * @brief draw
     * @param showModelVertices
     */
    void draw(bool showModelVertices = false);

    /**
     * @brief update
     * @param t
     * @param dt
     */
    void update(float t = 0, float dt = 0);
};

#endif
