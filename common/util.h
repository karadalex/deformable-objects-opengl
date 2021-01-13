#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

/* We can use a function like this to print some GL capabilities of our adapter
to the log file. handy if we want to debug problems on other people's computers
*/
void logGLParameters();

/**
* Slice a vector<T>.
*/
template<typename T>
std::vector<T> slice(const std::vector<T>& v, int start = 0, int end = -1) {
    int oldlen = v.size();
    int newlen;

    if (end == -1 && end >= oldlen) {
        newlen = oldlen - start;
    } else {
        newlen = end - start;
    }

    std::vector<T> nv(newlen);

    for (int i = 0; i < newlen; i++) {
        nv[i] = v[start + i];
    }
    return nv;
}

/**
* Get base directory from file path.
*/
std::string getBaseDir(const std::string& filepath);

/**
* Check if file exists.
*/
bool fileExists(const std::string& abs_filename);

/**
 * Show list of avaialable models and select model
 */
std::string selectObject();


/**
 * Get Elasticity parameters from user's input
 */ 
void getElasticityParameters(float &stiffness, float &damping);


/**
 * Get model's mass from user's input
 */ 
float getMass();


/**
 * Show list of available textures, slect texture and return directory name containing
 * all texture's files
 */
std::string selectModelTexture();


void printVec3(glm::vec3 v, const char *name = "Vector");

#endif