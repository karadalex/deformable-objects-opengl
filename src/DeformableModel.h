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

    DeformableModel(string modelFile, vec3 pos, vec3 vel, vec3 omega, float length, float mass);
    ~DeformableModel();

    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif