#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>

class Drawable;

/**
 * Represents the bounding box
 */
class Plane {
public:
    Drawable* plane;
    float size;
    glm::mat4 modelMatrix;

    /**
     * @brief Plane
     * @param s scaling factor of Plane
     */
    Plane(float s);
    ~Plane();

    /**
     * @brief draw
     * @param drawable
     */

    /**
     * @brief draw
     * @param drawable
     */
    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif
