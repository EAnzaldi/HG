#include "FlatMesh.h"

FlatMesh::FlatMesh() {
    setupMesh(1.0f, 1.0f);
}
FlatMesh::FlatMesh(float xrepeat, float yrepeat) {
    setupMesh(xrepeat, yrepeat);
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
void FlatMesh::setupMesh(float xrepeat, float yrepeat)
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,  xrepeat, 0.0f,
        0.5f,  0.5f, 0.0f,  xrepeat, yrepeat,

        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
        0.5f,  0.5f, 0.0f,  xrepeat, yrepeat,
        -0.5f,  0.5f, 0.0f,  0.0f, yrepeat
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
