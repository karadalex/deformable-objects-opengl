#ifndef STAIRCASE_H
#define STAIRCASE_H

#include <glm/glm.hpp>

class Drawable;

class Staircase {
public:
    Drawable* staircase;
    float size;
    glm::mat4 modelMatrix;

    /**
     * @brief Staircase
     * @param s Staircase scaling factor, defaults to 1.0f
     */
    Staircase(float s = 1.0f);
    ~Staircase();

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
