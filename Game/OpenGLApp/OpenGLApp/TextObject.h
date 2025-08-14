#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <map>
#include <algorithm>
#include <string>

#include "shader_s.h"
#include "utils.h"

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextObject
{
public:
    const char* Path;
    FT_Library FT;
    std::map<char, Character> Characters;//128 ASCII characters

    TextObject(FT_Library ft, const char* path);
    
    void Render(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

private:
    unsigned int VAO, VBO;

    void LoadFont();
    void initRenderData();
};
