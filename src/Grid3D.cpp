#include "Grid3D.h"


Grid3D::Grid3D(Grid3DParams grid) {
  int n = grid.n;
  int m = grid.m;
  int l = grid.l;
  float xStep = grid.width / n;
  float yStep = grid.height / m;
  float zStep = grid.depth / l;

  // Before each loop we set the axis initial dimension
  // Grid's position is set to be at the center of the grid
  float y = grid.position.y - grid.height / 2.0f;
  for (int k = 0; k < l; k++) {
    float z = grid.position.z - grid.depth / 2.0f;
    for(int j=0; j < m; j++) {
      float x = grid.position.x - grid.width / 2.0f;
      for(int i=0; i < n; i++) {
        vertices.push_back(vec3(x, y, z));
        x += xStep;
      }
      z += zStep;
    }
    y += yStep;
  }

  for (int k = 0; k < l-1; k++) {
    for(int j=0; j < m-1; j++) {
      for(int i=0; i < n-1; i++) {

        int row1 = k*m*n + j*n;
        int row2 = k*m*n + (j+1)*n;
        int row3 = (k+1)*m*n + j*n;
        int row4 = (k+1)*m*n + (j+1)*n;

        indices.push_back(uvec4(row1+i, row1+i+1, row1+i+1, row2+i+1));
        indices.push_back(uvec4(row2+i+1, row2+i, row2+i, row1+i));
        indices.push_back(uvec4(row1+i, row3+i, row3+i, row3+i+1));
        indices.push_back(uvec4(row1+i+1, row3+i+1, row3+i+1, row4+i+1));
        indices.push_back(uvec4(row2+i+1, row4+i+1, row4+i+1, row4+i));
        indices.push_back(uvec4(row2+i, row4+i, row4+i, row3+i));
      }
    }
  }
}

void Grid3D::getData(vector<vec3> &exportedVertices, vector<uvec4> exportedIndices) {
  for (auto v : vertices) exportedVertices.push_back(v);
  for (auto ind : indices) exportedIndices.push_back(ind);
}