#ifndef GRID_3D_H
#define GRID_3D_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace glm;


/**
 * @brief The Grid3DParams struct
 */
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

    /**
     * @brief Create a 3D Grid, by specifying all parameters (width, height, subdivisions, etc.)
     * with a Grid3DParamsparams instance
     * @param grid
     */
    Grid3D(Grid3DParams grid);

    /**
     * @brief Export 3D Grid vertices data
     * @param exportedVertices
     * @param exportedIndices
     */
    void getData(vector<vec3> &exportedVertices, vector<uvec4> exportedIndices);

    /**
     * @brief Get index of vertex when knowing the i,j,k coordinates of the
     * vertex in the Grid
     * @param i
     * @param j
     * @param k
     * @return int index
     */
    int gridCoordsToInd(int i, int j, int k);

    ~Grid3D();
};

#endif
