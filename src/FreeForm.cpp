#include "FreeForm.h"


FreeForm::FreeForm(string modelFile, vec3 position, vec3 vel, vec3 omega, float mass, float stiffness, float damping) {
  drawable = new Drawable(modelFile);
  
  pos = position;
  k0 = stiffness;
  b = damping;

  // Calculate AABB of drawable object
	aabb.min_x = aabb.max_x = drawable->indexedVertices[0].x;
	aabb.min_y = aabb.max_y = drawable->indexedVertices[0].y;
	aabb.min_z = aabb.max_z = drawable->indexedVertices[0].z;

	for (int i = 0; i < drawable->indexedVertices.size(); i++) {
    vec3 vertex = drawable->indexedVertices.at(i);
		if (vertex.x < aabb.min_x) aabb.min_x = vertex.x;
		if (vertex.y < aabb.min_y) aabb.min_y = vertex.y;
		if (vertex.z < aabb.min_z) aabb.min_z = vertex.z;

		if (vertex.x > aabb.max_x) aabb.max_x = vertex.x;
		if (vertex.y > aabb.max_y) aabb.max_y = vertex.y;
		if (vertex.z > aabb.max_z) aabb.max_z = vertex.z;
	}
  aabbCenterTranslation = vec3((aabb.max_x - aabb.min_x)/2.0f, (aabb.max_y - aabb.min_y)/2.0f, (aabb.max_z - aabb.min_z)/2.0f);

  Grid3DParams gridParams;
  gridParams.width = 1.1f*(aabb.max_x - aabb.min_x);
  gridParams.height = 1.3f*(aabb.max_y - aabb.min_y);
  gridParams.depth = 1.1f*(aabb.max_z - aabb.min_z);
  gridParams.n = 5;
  gridParams.m = 5;
  gridParams.l = 5;
  gridParams.position = position;
  Grid3D* grid = new Grid3D(gridParams);
  for (vec3 v : grid->vertices) {
    vertices.push_back(v);
    prevVertices.push_back(v);
  }
  translations = vector<vec3>(vertices.size(), vec3(0,0,0));
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
  
  // Setup structural neighbors
  // Add connections with previous and next vertices from the particleSystem array
  for (int k = 0; k < gridParams.l; k++) {    
    for(int j = 0; j < gridParams.m; j++) {
      for(int i = 0; i < gridParams.n; i++) { 
        if (i < gridParams.n-1)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i+1,j,k)), STRUCT_NEIGHBOR);
        if (i > 0)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i-1,j,k)), STRUCT_NEIGHBOR);
        if (j < gridParams.m-1)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i,j+1,k)), STRUCT_NEIGHBOR);
        if (j > 0)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i,j-1,k)), STRUCT_NEIGHBOR);
        if (k < gridParams.l-1)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i,j,k+1)), STRUCT_NEIGHBOR);
        if (k > 0)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i,j,k-1)), STRUCT_NEIGHBOR);
      }
    }
  }

  // Setup shear neighbors
  for (int k = 0; k < gridParams.l; k++) {    
    for(int j = 0; j < gridParams.m; j++) {
      for(int i = 0; i < gridParams.n; i++) { 
        if (i < gridParams.n-1 && j < gridParams.m-1)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i+1,j+1,k)), SHEAR_NEIGHBOR);
        if (i > 0 && j > 0)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i-1,j-1,k)), SHEAR_NEIGHBOR);
        if (j < gridParams.m-1 && k < gridParams.l-1)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i,j+1,k+1)), SHEAR_NEIGHBOR);
        if (j > 0 && k > 0)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i,j-1,k-1)), SHEAR_NEIGHBOR);
        if (i < gridParams.n-1 && k < gridParams.l-1)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i+1,j,k+1)), SHEAR_NEIGHBOR);
        if (i > 0 && k > 0)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i-1,j,k-1)), SHEAR_NEIGHBOR);
      }
    }
  }

  // Setup bending neighbors
  for (int k = 0; k < gridParams.l; k++) {    
    for(int j = 0; j < gridParams.m; j++) {
      for(int i = 0; i < gridParams.n; i++) { 
        if (i < gridParams.n-2)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i+2,j,k)), BEND_NEIGHBOR);
        if (i > 1)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i-2,j,k)), BEND_NEIGHBOR);
        if (j < gridParams.m-2)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i,j+2,k)), BEND_NEIGHBOR);
        if (j > 1)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i,j-2,k)), BEND_NEIGHBOR);
        if (k < gridParams.l-2)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i,j,k+2)), BEND_NEIGHBOR);
        if (k > 1)
          particleSystem.at(grid->gridCoordsToInd(i,j,k))->addNeighbor(particleSystem.at(grid->gridCoordsToInd(i,j,k-2)), BEND_NEIGHBOR);
      }
    }
  }

  // Last step of pre-processing
  // Map model vertices to the 4 nearest points
  mapModelVerticesToControlPoints();
  printStdVector(modelVerticesToControlPts, "modelVerticesToControlPts");
}


void FreeForm::mapModelVerticesToControlPoints() {
  for (int i = 0; i < drawable->indexedVertices.size(); i++) {
    vec3 modelVertex = drawable->indexedVertices.at(i);
    float minDist = length(modelVertex - vertices.at(0));
    // the index of the grid vertex that is closest to the model vertex
    int minDistInd = 0;
    for (int j = 1; j < vertices.size(); j++) {
      vec3 gridVertex = vertices.at(j);
      float dist = length(modelVertex - gridVertex);
      if (dist < minDist) minDistInd = j;
    }
    modelVerticesToControlPts.push_back(minDistInd);
  } 
}


void FreeForm::update(float t, float dt) {
  for (int i = 0; i < particlesNum; i++) {
    Particle* prt1 = particleSystem.at(i);
    // TODO: Calculate total force for particle:
    // Forces: gravity, spring-damp, shear, bending, collision
    vec3 gravity = vec3(0, -prt1->m * g, 0);
    
    // vec3 damping = -b*prt1->v; 
    // vec3 elastic = -k0*vec3(0, prt1->x.y, 0);
    vec3 damping = vec3(0,0,0);
    vec3 elastic = vec3(0,0,0);
    for (int j = 0; j < prt1->structNeighbors.size(); j++) {
      Particle* prt2 = prt1->structNeighbors.at(j);
      vec3 p1p2 = prt1->x - prt2->x;
      float p1p2d = sqrt(p1p2.x*p1p2.x + p1p2.y*p1p2.y + p1p2.z*p1p2.z);
      vec3 unit_vec = p1p2 / p1p2d;
      elastic += -k0 * (p1p2d - prt1->structDistances.at(j)) * unit_vec;
      
      vec3 vel = (dot(prt1->v, unit_vec) - dot(prt2->v, unit_vec)) * unit_vec;
      damping += -b*vel;

      // Disable gravity if particles are in rest
      // if (p1p2d - prt1->structDistances.at(j) <= 0.0001f) {
      //   gravity = vec3(0,0,0);
      // } 
    }

    for (int j = 0; j < prt1->shearNeighbors.size(); j++) {
      Particle* prt2 = prt1->shearNeighbors.at(j);
      vec3 p1p2 = prt1->x - prt2->x;
      float p1p2d = sqrt(p1p2.x*p1p2.x + p1p2.y*p1p2.y + p1p2.z*p1p2.z);
      vec3 unit_vec = p1p2 / p1p2d;
      elastic += -k0 * (p1p2d - prt1->shearDistances.at(j)) * unit_vec;
      
      vec3 vel = (dot(prt1->v, unit_vec) - dot(prt2->v, unit_vec)) * unit_vec;
      damping += -b*vel;
    }

    for (int j = 0; j < prt1->bendNeighbors.size(); j++) {
      Particle* prt2 = prt1->bendNeighbors.at(j);
      vec3 p1p2 = prt1->x - prt2->x;
      float p1p2d = sqrt(p1p2.x*p1p2.x + p1p2.y*p1p2.y + p1p2.z*p1p2.z);
      vec3 unit_vec = p1p2 / p1p2d;
      elastic += -k0 * (p1p2d - prt1->bendDistances.at(j)) * unit_vec;
      
      vec3 vel = (dot(prt1->v, unit_vec) - dot(prt2->v, unit_vec)) * unit_vec;
      damping += -b*vel;
    }

    // vec3 force = gravity + elastic + damping;
    vec3 force = elastic + damping;
    if (i == 0) {
      printVec3(gravity, "gravity");
      printVec3(elastic, "elastic");
      // printVec3(damping, "damping");
    }
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
  // and save previous value as well as the displacement
  for (int i = 0; i < vertices.size(); i++) {
    prevVertices.at(i) = vertices.at(i);
    vertices.at(i) = particleSystem.at(i)->x;

    float normalization = 1.0f/length(prevVertices.at(i));
    float amplitude = 1.0f;
    translations.at(i) = amplitude * normalization * (vertices.at(i) - prevVertices.at(i));
  }

  // Update model vertices positions based on translations of the grid vertices
  for (int i = 0; i < drawable->indexedVertices.size(); i++) {
    drawable->indexedVertices.at(i) += translations.at(modelVerticesToControlPts.at(i));
  }
  
}


void FreeForm::draw(GLuint modelMatrixLocation, int mode) {
  // Calculate modelMatrix transformation for the actual model separately from the
  // grid, so that they are aligned
  vec3 x = 0.5f*(particleSystem.at(0)->x + particleSystem.at(particleSystem.size()-1)->x);
  mat4 translate = glm::translate(mat4(), vec3(x.x, x.y, x.z));
  mat4 scale = mat4();
  modelMatrix = translate * scale;
  glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

  glBindBuffer(GL_ARRAY_BUFFER, drawable->verticesVBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, drawable->indexedVertices.size() * sizeof(vec3), &drawable->indexedVertices[0]);
  drawable->bind();
  drawable->draw();
  // glBindVertexArray(0);

  // Undo modelMatrix trnasformation
  modelMatrix = mat4();
  glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

  // Update Vertices with new positions
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vec3), &vertices[0]);

  // Draw grid
  // glEnable(GL_DEPTH_TEST);
  glBindVertexArray(VAO);
  glDrawElements(GL_LINES, (GLuint)indices.size()*4, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  // glDisable(GL_DEPTH_TEST);
}


void FreeForm::translateAllVertices(vec3 translation) {
  for (int i = 0; i < vertices.size(); i++) 
    vertices.at(i) += translation;
  for (int i = 0; i < drawable->indexedVertices.size(); i++) 
    drawable->indexedVertices.at(i) += translation;
}


void FreeForm::transformWithControlPoint(int controlPtIndex, vec3 translation) {
  particleSystem.at(controlPtIndex)->x += translation;
  vertices.at(controlPtIndex) += translation;

  // Update model vertices that are controlled by the given control point
  for (int i = 0; i < drawable->indexedVertices.size(); i++) {
    if (modelVerticesToControlPts.at(i) == controlPtIndex) {
      drawable->indexedVertices.at(i) += translation;
    }
  }
}