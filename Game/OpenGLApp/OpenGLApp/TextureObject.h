#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "shader_s.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class TextureObject 
{
public:
    const char* Path;
    unsigned int TextureID;
    int Width;
    int Height;

    TextureObject(const char* path);

    glm::vec3 getSize() const;
    float getWidth() const;
    float getHeigth() const;

private:
    void LoadTexture(const char* path);
};

