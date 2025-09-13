#pragma once
#include "GameObject.h"
#include "TextureObject.h"

class MouseObject {
public:
    MouseObject(GLFWwindow* window, TextureObject* texture, glm::vec3 size) : Window(window), Texture(texture), Size(size) {
        fmesh = new FlatMesh();
        Position.x = 0.0f;
        Position.y = 0.0f;
    };

    void Render(const Shader& shader) const {
        shader.use();
        glm::mat4 model_mat = glm::mat4(1.0f);
        model_mat = glm::translate(model_mat, glm::vec3(Position.x, Position.y, 0.0f));
        model_mat = glm::scale(model_mat, glm::vec3(Size.x, Size.y, 1.0f));
        shader.setMat4("model", model_mat);

        fmesh->Draw(shader, Texture->TextureID);
    }

    void Move(double xpos, double ypos) {
        Position.x = (float) xpos;
        Position.y = (float) ypos;
    }

    /*
    void Move() {
        
        int fbWidth, fbHeight;
        double xpos, ypos;
        glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);
        glfwGetCursorPos(Window, &xpos, &ypos);//getting cursor position
        ypos = fbHeight - ypos;
        Move(Position.x, Position.y);
    }*/

    void Hide() {
        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);//nasconde il mouse
    }

    void Show() {
        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//mostra il mouse
    }

private:
    GLFWwindow* Window;
    FlatMesh* fmesh;
    TextureObject* Texture;
    glm::vec2 Position;
    glm::vec3 Size;
};