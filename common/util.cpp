#include <GL/glew.h>
#include <iostream>
#include <cmath>
using namespace std;
#include "util.h"

void logGLParameters() {
    GLenum params[] = {
        GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
        GL_MAX_CUBE_MAP_TEXTURE_SIZE,
        GL_MAX_DRAW_BUFFERS,
        GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
        GL_MAX_TEXTURE_IMAGE_UNITS,
        GL_MAX_TEXTURE_SIZE,
        GL_MAX_VARYING_FLOATS,
        GL_MAX_VERTEX_ATTRIBS,
        GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
        GL_MAX_VERTEX_UNIFORM_COMPONENTS,
        GL_MAX_VIEWPORT_DIMS
    };
    const char* names[] = {
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
        "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
        "GL_MAX_DRAW_BUFFERS",
        "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
        "GL_MAX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_TEXTURE_SIZE",
        "GL_MAX_VARYING_FLOATS",
        "GL_MAX_VERTEX_ATTRIBS",
        "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
        "GL_MAX_VIEWPORT_DIMS"
    };

    cout << "GL Context Parameters:" << endl;

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);;
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported: " << version << endl;

    // integers - only works if the order is 0-10 integer return types
    for (int i = 0; i < 10; i++) {
        int v = 0;
        glGetIntegerv(params[i], &v);
        cout << names[i] << " " << v << endl;
    }
    // others
    int v[2];
    v[0] = v[1] = 0;
    glGetIntegerv(params[10], v);
    cout << names[10] << " " << v[0] << " " << v[1] << endl;
    cout << "-----------------------------" << endl;
}

std::string getBaseDir(const std::string & filepath) {
    if (filepath.find_last_of("/\\") != std::string::npos) {
        return filepath.substr(0, filepath.find_last_of("/\\"));
    }
    return "";
}

bool fileExists(const std::string & abs_filename) {
    bool ret;
    FILE *fp = fopen(abs_filename.c_str(), "rb");
    if (fp) {
        ret = true;
        fclose(fp);
    } else {
        ret = false;
    }

    return ret;
}


std::string selectObject() {
    std::string obj_models[20] = {
		"armadillo_low_low.obj",
		"b66_L2.obj",
		"bone.obj",
		"bunny_low.obj",
		"cube.obj",
		"dolphin.obj",
		"dragon_low_low.obj",
		"flashlight.obj",
		"flashlightNoCentered.obj",
		"hand2.obj",
		"icosahedron.obj",
		"Phone_v02.obj",
		"pins.obj",
		"polyhedron.obj",
        "sphere.obj",
        "spring.obj",
		"suzanne.obj",
		"teapotMultiMesh.obj",
		"unicorn_low.obj",
		"unicorn_low_low.obj"
	};
	cout << "Select model (1-20): " << endl;
    for (int i = 0; i < 20; i++) {
		cout << i+1 << " " << obj_models[i] << endl;
	}
	string model_file;
	int ind;
	cin >> ind;
	model_file = obj_models[ind - 1];
    
    return model_file;
}