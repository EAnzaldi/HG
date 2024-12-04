#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "shader_s.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class TextureObj {
public:
    const char* Path;
    unsigned int TextureID;
    int Width;
    int Height;

    TextureObj(const char* path);

private:
    void LoadTexture(const char* path);
};

