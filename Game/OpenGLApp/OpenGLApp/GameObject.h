#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <algorithm>

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

enum class DimensionType { TwoD, ThreeD };

class GameObject
{
public:
    DimensionType Dimension;

    static FlatMesh* fmesh; //unica fmesh per tutti gli sprite

    Model* model;
    TextureObject* Texture;

    glm::vec2 Position;
    glm::vec3 Size;

    static const glm::vec3 axisX;
    static const glm::vec3 axisY;
    static const glm::vec3 axisZ;

    float Rotation;
    glm::vec3 Axis = GameObject::axisY;
    bool usePivot = false;
    glm::vec2 Pivot;

    float FlipX;

    bool RepeatWidth;

    //Costruttore 3d
    GameObject(glm::vec2 position, glm::vec3 size, Model* model, bool repeatWidth);
    //Costruttore 2d
    GameObject(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth);

    void Render(const Shader& shader) const;

    //virtual void RenderFlat(const Shader& shader) const;

    Hitbox GetHitbox() const;

    //Hitbox GetHitboxFlat() const;
    void Print() const;

    float DistanceTo(GameObject* pObj) const;

    static glm::vec2 NDCPosition(glm::vec2 pixelPosition);
    static glm::vec3 NDCSize(glm::vec3 pixelSize);

protected:
    void Render(const Shader& shader, const glm::mat4 model_mat) const;
    void Render2D(const Shader& shader) const;
    void Render3D(const Shader& shader) const;
    Hitbox GetHitbox2D() const;
    Hitbox GetHitbox3D() const;
};