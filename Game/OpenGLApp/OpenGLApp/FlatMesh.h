#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_s.h"

#include <string>
#include <vector>
#include "TextureObject.h"
using namespace std;

class FlatMesh {
public:
    // mesh Data
    //TextureObject texture;

    // constructor
    FlatMesh();
    FlatMesh(float xrepeat, float yrepeat);

    // render the mesh
    void Draw(const Shader& shader, unsigned int textureID) const;

    void setupMesh(float xrepeat, float yrepeat);

private:
    // render data 
    unsigned int VAO = 0;
    unsigned int VBO = 0;
};