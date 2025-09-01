#include "TextureObject.h"

TextureObject::TextureObject(const char* path) : Path(path)
{
    this->LoadTexture(path);
}

void TextureObject::LoadTexture(const char* path)
{
    int nrChannels;

    glGenTextures(1, &this->TextureID);
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true);

    std::string extension = std::string(path).substr(std::string(path).find_last_of('.') + 1);

    unsigned char* data = stbi_load(path, &this->Width, &this->Height, &nrChannels, 0);
    if (data)
    {
        if (nrChannels == 3)//RGB only
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->Width, this->Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (nrChannels == 4)//RGB + alpha
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->Width, this->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}
glm::vec3 TextureObject::getSize() const
{
    //printf("%f %f\n", static_cast<float>(texture.Width), static_cast<float>(texture.Height));
    return glm::vec3(static_cast<float>(Width), static_cast<float>(Height), 1.0f);
}

float TextureObject::getWidth() const
{
    return static_cast<float>(Width);
}

float TextureObject::getHeight() const
{
    return static_cast<float>(Height);
}
float TextureObject::getAspect() const
{
    return (float) Width / (float) Height;
}
