#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

#include "stb_image.h"
#include "shader_s.h"
#include "TextureObject.h"
#include "model.h"
#include "FlatMesh.h"
#include "utils.h"

struct Hitbox 
{
    glm::vec2 Min; // Punto in basso a sinistra
    glm::vec2 Max; // Punto in alto a destra
};

class GameObject 
{
public:
    glm::vec2 Position;
    glm::vec3 Size;
    float Rotation;
    float FlipX;

    bool RepeatWidth;

    Model* model;

    FlatMesh* fmesh;

    TextureObject* Texture;

    //Costruttore 3d
    GameObject(glm::vec2 position, glm::vec3 size, Model* model, TextureObject* texture, bool repeatWidth);
    //Costruttore 2d
    GameObject(glm::vec2 position, glm::vec3 size, FlatMesh* fmesh, bool repeatWidth);

    virtual void Render(const Shader& shader) const;

    virtual void RenderFlat(const Shader& shader) const;

    Hitbox GetHitbox() const;

    Hitbox GetHitboxFlat() const;

    void Print() const;

};