#include "Plane.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>

using namespace glm;

Plane::Plane(float s) {
    size = s;
    plane = new Drawable("models/plane.obj");
}

Plane::~Plane() {
    delete plane;
}

void Plane::draw(unsigned int drawable) {
    plane->bind();
    plane->draw();
}

void Plane::update(float t, float dt) {
    mat4 translate = glm::translate(mat4(), vec3(size / 2, 0, size / 2));
    mat4 scale = glm::scale(mat4(), vec3(size, size, size));
    modelMatrix = translate * scale;
}