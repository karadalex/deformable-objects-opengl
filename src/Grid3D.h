#ifndef GRID_3D_H
#define GRID_3D_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace glm;


struct Grid3DParams {
  float width; 
  float height;
  float depth;
  int n;
  int m;
  int l;
  vec3 position;
};

class Grid3D {
public:
    vector<vec3> vertices;
    vector<uvec4> indices;
    Grid3DParams grid;

    Grid3D(Grid3DParams grid);

    void getData(vector<vec3> &exportedVertices, vector<uvec4> exportedIndices);

    int gridCoordsToInd(int i, int j, int k);

    ~Grid3D();
};

#endif
