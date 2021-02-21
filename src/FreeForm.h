#ifndef FREE_FORM_H
#define FREE_FORM_H

#include "Grid3D.h"
#include "Particle.h"
#include <common/model.h>
#include <vector>
#include <string>
#include <iostream>
#include <common/util.h>

using namespace std;
using namespace glm;

// Standard acceleration due to gravity
#define g 9.80665f

struct AABB {
  double min_x, min_y, min_z, max_x, max_y, max_z;
};

class FreeForm {
public:
    GLuint VAO, VBO, EBO;
    vector<vec3> vertices, prevVertices, translations;
    vector<uvec4> indices;
    Drawable* drawable;
    glm::mat4 modelMatrix = mat4();
    AABB aabb;
    vec3 aabbCenterTranslation;

    std::vector<Particle*> particleSystem; 
    vector<int> modelVerticesToControlPts;
    int particlesNum;  
    vec3 pos; 
    float k0 = 0.5; // stiffness
    float b = 0.01; // damping

    /**
     * @brief FreeForm
     * @param modelFile
     * @param position
     * @param vel
     * @param omega
     * @param mass
     * @param stiffness
     * @param damping
     */
    FreeForm(string modelFile, vec3 position, vec3 vel, vec3 omega, float mass, float stiffness, float damping);

    /**
     * @brief mapModelVerticesToControlPoints
     */
    void mapModelVerticesToControlPoints();

    /* Bind VAO before calling draw */    
    /**
     * @brief draw
     * @param modelMatrixLocation
     * @param mode
     */
    void draw(GLuint modelMatrixLocation, int mode = GL_TRIANGLES);

    /**
     * @brief translateAllVertices: Translate all control points and model vertices by given amount of translation
     * @param translation : A 3D vector with the amount of displacement/translation we want to apply all
     * the control points and model vertices
     */
    void translateAllVertices(vec3 translation);

    /**
     * @brief transformWithControlPoint
     * @param controlPtIndex : The index of the control point that will be moved
     * @param translation : A 3D vector with the amount of displacement/translation we want to apply to the
     * control point and thus to the controlled model vertices
     */
    void transformWithControlPoint(int controlPtIndex, vec3 translation);

    /**
     * @brief update
     * @param t
     * @param dt
     */
    void update(float t = 0, float dt = 0);

    ~FreeForm();
};

#endif
