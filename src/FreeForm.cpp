#include "FreeForm.h"


FreeForm::FreeForm() {
  Grid3DParams gridParams;
  gridParams.width = 2.0f;
  gridParams.height = 10.0f;
  gridParams.depth = 3.0f;
  gridParams.n = 2;
  gridParams.m = 10;
  gridParams.l = 3;
  gridParams.position = vec3(1,5,0);
  Grid3D* grid = new Grid3D(gridParams);
  for (vec3 v : grid->vertices) vertices.push_back(v);
  for (uvec4 ind : grid->indices) indices.push_back(ind);

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
