#include "CubeCustom.h"

CubeCustom::CubeCustom(vec3 position, int density) : position(position), density(density)
{
    previousTime = glfwGetTime();
    // Create and compile our GLSL program from the shaders
    shaderProgram = loadShaders("deformable.vert",
                                "deformable.frag");
}

void CubeCustom::createContext()
{

    // Get a pointer location to model matrix in the vertex shader
    MVPLocation = glGetUniformLocation(shaderProgram, "MVP");

    // Define cube VAO
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    // Define cube vertices. Three consecutive floats give a 3D vertex; Three
    // consecutive vertices give a triangle.
    GLfloat range[2] = {-1.0f, 1.0f};
    GLfloat step = 2.0f/density;
    GLfloat x, y, z;
    for (int r = 0; r < 2; r++)
    {
        x = range[r];
        for (int j = 0; j < density; j++)
        {
            for (int k = 0; k < density; k++)
            {
                // 1st triangle
                y = range[0] + j*step;
                z = range[0] + k*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                z = range[0] + (k+1)*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                y = range[0] + (j+1)*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);

                // 2nd triangle
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                z = range[0] + k*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                y = range[0] + j*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
            }
        }
    }
    for (int r = 0; r < 2; r++)
    {
        y = range[r];
        for (int i = 0; i < density; i++)
        {
            for (int k = 0; k < density; k++)
            {
                // 1st triangle
                x = range[0] + i*step;
                z = range[0] + k*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                z = range[0] + (k+1)*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                x = range[0] + (i+1)*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);

                // 2nd triangle
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                z = range[0] + k*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                x = range[0] + i*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
            }
        }
    }
    for (int r = 0; r < 2; r++)
    {
        z = range[r];
        for (int i = 0; i < density; i++)
        {
            for (int j = 0; j < density; j++)
            {
                // 1st triangle
                x = range[0] + i*step;
                y = range[0] + j*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                y = range[0] + (j+1)*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                x = range[0] + (i+1)*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);

                // 2nd triangle
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                y = range[0] + j*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
                x = range[0] + i*step;
                cubeVertices.push_back(x); cubeVertices.push_back(y); cubeVertices.push_back(z);
            }
        }
    }

    glGenBuffers(1, &cubeVerticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVerticesVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(GLfloat), static_cast<void *>(cubeVertices.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // color VBO
    vector<GLfloat> cubeColors;
    vector<GLfloat> faceColors = {
        0.583f, 0.771f, 0.014f,
        0.609f, 0.115f, 0.436f,
        0.327f, 0.483f, 0.844f,

        0.583f, 0.771f, 0.014f,
        0.609f, 0.115f, 0.436f,
        0.327f, 0.483f, 0.844f
    };
    for (int r = 0; r < 2; r++)
    {
        x = range[r];
        for (int j = 0; j < density; j++)
        {
            for (int k = 0; k < density; k++)
            {
                cubeColors.insert(cubeColors.end(), faceColors.begin(), faceColors.end());
            }
        }
    }
    faceColors = {
        0.597f, 0.770f, 0.761f,
        0.559f, 0.436f, 0.730f,
        0.359f, 0.583f, 0.152f,

        0.483f, 0.596f, 0.789f,
        0.559f, 0.861f, 0.639f,
        0.195f, 0.548f, 0.859f
    };
    for (int r = 0; r < 2; r++)
    {
        y = range[r];
        for (int i = 0; i < density; i++)
        {
            for (int k = 0; k < density; k++)
            {
                cubeColors.insert(cubeColors.end(), faceColors.begin(), faceColors.end());
            }
        }
    }
    faceColors = {
        0.014f, 0.184f, 0.576f,
        0.771f, 0.328f, 0.970f,
        0.406f, 0.615f, 0.116f,

        0.676f, 0.977f, 0.133f,
        0.971f, 0.572f, 0.833f,
        0.140f, 0.616f, 0.489f
    };
    for (int r = 0; r < 2; r++)
    {
        z = range[r];
        for (int i = 0; i < density; i++)
        {
            for (int j = 0; j < density; j++)
            {
                cubeColors.insert(cubeColors.end(), faceColors.begin(), faceColors.end());
            }
        }
    }

    glGenBuffers(1, &cubeColorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeColorsVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeColors.size() * sizeof(GLfloat), static_cast<void *>(cubeColors.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    cout << "cubevertices " << cubeVertices.size() << endl;
    cout << "cubecolors " << cubeColors.size() << endl;
}

void CubeCustom::draw()
{
    glUseProgram(shaderProgram);

    // CubeCustom model matrix
    mat4 cubeScaling = glm::scale(mat4(), vec3(1.0, 1.0, 1.0));
    mat4 cubeRotation = glm::rotate(mat4(), 3.14f / 4, vec3(0, 1, 0));
    cubeRotation = cubeRotation * glm::rotate(mat4(), 3.14f / 4, vec3(0, 0, 1));
    mat4 cubeTranslation = glm::translate(mat4(), position);

    // Projection matrix: 45deg Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    mat4 projection = perspective(radians(45.f), 4.0f / 3.0f, 0.1f, 100.0f);

    cubeRotation = cubeRotation * glm::rotate(mat4(), 3.14f * dt, vec3(0.5, 0.5, 0.8));
    cubeScaling = glm::scale(mat4(), vec3(1.0, 1.0, 1.0));
    // Rotate camera
    mat4 view = lookAt(
        vec3(sin(glfwGetTime()), 0, 10),
        vec3(0, 0, 0),
        vec3(0, 1, 0));

    // Draw cube
    glBindVertexArray(cubeVAO);

    // Define cube MVP
    mat4 cubeModel = cubeTranslation * cubeRotation * cubeScaling;
    mat4 cubeMVP = projection * view * cubeModel;
    glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &cubeMVP[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, cubeVertices.size());

    dt = glfwGetTime() - previousTime;
}

CubeCustom::~CubeCustom()
{
    glDeleteBuffers(1, &cubeVerticesVBO);
    glDeleteBuffers(1, &cubeColorsVBO);
    glDeleteVertexArrays(1, &cubeVAO);

    glDeleteProgram(shaderProgram);
}
