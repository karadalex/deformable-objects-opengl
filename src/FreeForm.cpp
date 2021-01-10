#include "FreeForm.h"


FreeForm::FreeForm(vec3 position, vec3 vel, vec3 omega, float mass, float stiffness, float damping) {
  Grid3DParams gridParams;
  gridParams.width = 5.0f;
  gridParams.height = 5.0f;
  gridParams.depth = 5.0f;
  gridParams.n = 5;
  gridParams.m = 5;
  gridParams.l = 5;
  gridParams.position = position;
  Grid3D* grid = new Grid3D(gridParams);
  for (vec3 v : grid->vertices) vertices.push_back(v);
  for (uvec4 ind : grid->indices) indices.push_back(ind);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uvec4), &indices[0], GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  particlesNum = vertices.size();
  float particleMass = mass / particlesNum;
  // Assign a Particle to each vertex
  for (int i = 0; i < vertices.size(); i++) {
    vec3 vertex = vertices.at(i);
    Particle* particle = new Particle(vertex, vel, particleMass);
    particleSystem.push_back(particle);
  }
  
}

void FreeForm::update(float t, float dt) {
  for (int i = 0; i < particlesNum; i++) {
    Particle* prt1 = particleSystem.at(i);
    // TODO: Calculate total force for particle:
    // Forces: gravity, spring-damp, shear, bending, collision
    vec3 gravity = vec3(
      0,
      -prt1->m * g,
      0
    );
    
    vec3 damping = -b*prt1->v; 
    vec3 elastic = -k*vec3(0, prt1->x.y, 0);
    // vec3 damping = vec3(0,0,0);
    // vec3 elastic = vec3(0,0,0);
    // for (int j = 0; j < prt1->structNeighbors.size(); j++) {
    //     Particle* prt2 = prt1->structNeighbors.at(j);
    //     vec3 p1p2 = prt1->x - prt2->x;
    //     float p1p2d = sqrt(p1p2.x*p1p2.x + p1p2.y*p1p2.y + p1p2.z*p1p2.z);
    //     if (p1p2d >= 0.0000001f) {
    //         vec3 unit_vec = p1p2 / p1p2d;
    //         elastic += -k * (p1p2d - prt1->structDistances.at(j)) * unit_vec;
            
    //         vec3 vel = (dot(prt2->v, unit_vec) - dot(prt1->v, unit_vec)) * unit_vec;
    //         damping += -b*vel;
    //     }
    // }

    vec3 force = gravity + elastic + damping;
    prt1->forcing = [&](float t, const vector<float>& y)->vector<float> {
      vector<float> f(6, 0.0f);
      f[0] = force.x;
      f[1] = force.y;
      f[2] = force.z;
      return f;
    };
    // Update particle's physics state
    prt1->update(t, dt);
  }

  // Get new position of particle and save it in the vertex
  for (int i = 0; i < vertices.size(); i++) {
    vertices.at(i) = particleSystem.at(i)->x;
  }

  // Update Vertices with new positions
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vec3), &vertices[0]);

  // Draw grid
  glEnable(GL_DEPTH_TEST);
  glBindVertexArray(VAO);
  glDrawElements(GL_LINES, (GLuint)indices.size()*4, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glDisable(GL_DEPTH_TEST);
}
