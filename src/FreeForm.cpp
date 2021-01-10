#include "FreeForm.h"


FreeForm::FreeForm() {
  int width, height, depth;
  width = height = depth = 10;
  int n, m, l;
  n = m = l = 10;
  float xStep = width / n;
  float yStep = height / m;
  float zStep = depth / l;

  float y = 0.0f;
  for (int k = 0; k < l; k++) {
    float z = 0.0f;
    for(int j=0; j < m; j++) {
      float x = 0.0f;
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

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3), &vertices[0], GL_STATIC_DRAW );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

  glGenBuffers( 1, &EBO );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uvec4), &indices[0], GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void FreeForm::update(float t, float dt) {
  glEnable(GL_DEPTH_TEST);

  glBindVertexArray(VAO);

  glDrawElements(GL_LINES, (GLuint)indices.size()*4, GL_UNSIGNED_INT, NULL);

  glBindVertexArray(0);

  glDisable(GL_DEPTH_TEST);
}
