#include "FlatMesh.h"

unsigned int FlatMesh::VAO = 0;
unsigned int FlatMesh::VBO = 0;
bool FlatMesh::initialized = false;

/*
FlatMesh::FlatMesh(const char* path) : texture(path) {
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    if (!initialized)
    {
        setupMesh();
        initialized = true;
    }
}*/
FlatMesh::FlatMesh() {
    if (!initialized)
    {
        setupMesh();
        initialized = true;
    }
}
void FlatMesh::Draw(const Shader& shader, unsigned int textureID) const
{
    glActiveTexture(GL_TEXTURE0);
    shader.setInt("ourTexture", 0);
    //glBindTexture(GL_TEXTURE_2D, texture.TextureID)
    glBindTexture(GL_TEXTURE_2D, textureID);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);//per disegnare il quadratino (2 triangoli)
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    //glActiveTexture(GL_TEXTURE0);
}

void FlatMesh::setupMesh()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,

        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
    };

    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}
/*
glm::vec3 FlatMesh::getSize()
{
    //printf("%f %f\n", static_cast<float>(texture.Width), static_cast<float>(texture.Height));
    return glm::vec3(static_cast<float>(texture.Width), static_cast<float>(texture.Height), 1.0f);
}
float FlatMesh::getWidth()
{
    return static_cast<float>(texture.Width);
}

float FlatMesh::getHeigth()
{
    return static_cast<float>(texture.Height);
}
*/
