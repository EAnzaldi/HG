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
    TextureObject texture;

    // constructor
    FlatMesh(const char* path);

    // render the mesh
    void Draw(const Shader& shader) const;

    // get sprite size
    glm::vec3 getSize();
    float getWidth();
    float getHeigth();

private:
    // render data 
    static unsigned int VAO;
    static unsigned int VBO;
    static bool initialized;

    // initializes all the buffer objects/arrays
    static void setupMesh();
};