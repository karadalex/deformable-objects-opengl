#ifndef STAIRCASE_H
#define STAIRCASE_H

#include <glm/glm.hpp>

class Drawable;

/**
 * Represents the bounding box
 */
class Staircase {
public:
    Drawable* staircase;
    float size;
    glm::mat4 modelMatrix;

    Staircase(float s = 1.0f);
    ~Staircase();

    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif
