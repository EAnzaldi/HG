#include "TextObject.h"

TextObject::TextObject(FT_Library ft, const char* path) : Path(path), FT(ft)
{
    this->LoadFont();
    this->initRenderData();
}

void TextObject::LoadFont()
{
    FT_Face face;
    // load the font
    if (FT_New_Face(this->FT, this->Path, 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    // Sets font's width and height parameters
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // I caratteri vengono caricati un'unica volta
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
            face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        this->Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);//libera la Face
}

 // Inizializzo i dati nei buffer
 void TextObject::initRenderData() {
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
 }
 // Renderizza il testo
 void TextObject::Render(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color, Alignment alignment){
     if (x >= static_cast<float>(SCR_WIDTH) || y >= static_cast<float>(SCR_HEIGHT)) {
         std::cout << "ERROR::FREETYPE: Out of range coordinates" << std::endl;
     }

     shader.use();

     glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),
                                        0.0f, static_cast<float>(SCR_HEIGHT));
     shader.setMat4("projection", projection);

     shader.setVec3("textColor", color);//passa il colore allo shader

     // Abilita la trasparenza dei glifi
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     glActiveTexture(GL_TEXTURE0);
     glBindVertexArray(this->VAO);

     if (alignment != Alignment::Left) {
         //Calcola dimensione del testo per ricavare l'offset per l'allineamento
         float text_width = 0.0f;
         for (char c : text) {
             auto iterator = this->Characters.find(c);
             if (iterator != this->Characters.end()) {
                 Character ch = iterator->second;
                 text_width += (ch.Advance >> 6) * scale;
             }
         }
         if (alignment == Alignment::Center)
             x -= text_width / 2.0f;
         else if (alignment == Alignment::Right)
             x -= text_width;
     }

     // Itera su ogni carattere
     for (char c : text) {
         auto iterator = this->Characters.find(c);//cerca il carattere
         if (iterator != this->Characters.end()) {//trovato
             Character ch = iterator->second;
             float xpos = x + ch.Bearing.x * scale;
             float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

             float w = ch.Size.x * scale;
             float h = ch.Size.y * scale;

             // Aggiorna il VBO usando glBufferSubData
             float vertices[6][4] = {
                 { xpos,     ypos + h,   0.0f, 0.0f },
                 { xpos,     ypos,       0.0f, 1.0f },
                 { xpos + w, ypos,       1.0f, 1.0f },

                 { xpos,     ypos + h,   0.0f, 0.0f },
                 { xpos + w, ypos,       1.0f, 1.0f },
                 { xpos + w, ypos + h,   1.0f, 0.0f }
             };

             glBindTexture(GL_TEXTURE_2D, ch.TextureID);

             glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
             glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
             glBindBuffer(GL_ARRAY_BUFFER, 0);

             // Renderizza il carattere
             glDrawArrays(GL_TRIANGLES, 0, 6);

             // Avanza per il carattere successivo (Advance è in 1/64 di pixel)
             x += (ch.Advance >> 6) * scale;
             // (larghezza_glifo * 64 ) * scale
         }
     }

     glBindVertexArray(0);
     glBindTexture(GL_TEXTURE_2D, 0);
 }
