#include "ShaderManager.h"

// Valori impostati seguendo http://devernay.free.fr/cours/opengl/materials.html
const MaterialType ShaderManager::Gold(
	glm::vec3(0.24725f, 0.1995f, 0.0745f),
	glm::vec3(0.75164f, 0.60648f, 0.22648f),
	glm::vec3(0.628281f, 0.555802f, 0.366065f),
	128.0f * 0.4f
);
const MaterialType ShaderManager::Silver(
	glm::vec3(0.19225f, 0.19225f, 0.19225f),
	glm::vec3(0.50754f, 0.50754f, 0.50754f),
	glm::vec3(0.508273f, 0.508273f, 0.508273f),
	128.0f * 0.4f
);

const glm::mat4 ShaderManager::projectionNDC = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

void ShaderManager::SetProjection(const Shader& shader, GLFWwindow* window, ProjectionType type) {
    shader.use();
    if (type == ProjectionType::NDC) {
        shader.setMat4("projection", projectionNDC);
    }
    else if (type == ProjectionType::Pixels) {
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glm::mat4 projectionPixels = glm::ortho(0.0f, static_cast<float>(fbWidth), 0.0f, static_cast<float>(fbHeight));
        shader.setMat4("projection", projectionPixels);
    }
}
void ShaderManager::SetMaterial(const Shader& shader, const MaterialType& material) {
	shader.use();
	shader.setVec3("material.ambient", material.ambient);
	shader.setVec3("material.diffuse", material.diffuse);
	shader.setVec3("material.specular", material.specular);
	shader.setFloat("material.shininess", material.shininess);

}
void ShaderManager::SetLight(const Shader& shader, glm::vec3 position, glm::vec3 color) {
	shader.use();
	shader.setVec3("light.position", position);
	shader.setVec3("light.ambient", color * glm::vec3(0.3f));
	shader.setVec3("light.diffuse", color * glm::vec3(0.6f));
	shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
}
void ShaderManager::PrintMaterialUniforms(const Shader& shader) {
    shader.use();  // Assicurati che lo shader sia attivo

    const std::string& prefix = "material";
    GLint loc;
    GLfloat temp[3];

    // Ambient
    loc = glGetUniformLocation(shader.ID, (prefix + ".ambient").c_str());
    if (loc != -1) {
        glGetUniformfv(shader.ID, loc, temp);
        std::cout << "Ambient: " << temp[0] << ", " << temp[1] << ", " << temp[2] << std::endl;
    }

    // Diffuse
    loc = glGetUniformLocation(shader.ID, (prefix + ".diffuse").c_str());
    if (loc != -1) {
        glGetUniformfv(shader.ID, loc, temp);
        std::cout << "Diffuse: " << temp[0] << ", " << temp[1] << ", " << temp[2] << std::endl;
    }

    // Specular
    loc = glGetUniformLocation(shader.ID, (prefix + ".specular").c_str());
    if (loc != -1) {
        glGetUniformfv(shader.ID, loc, temp);
        std::cout << "Specular: " << temp[0] << ", " << temp[1] << ", " << temp[2] << std::endl;
    }

    // Shininess
    loc = glGetUniformLocation(shader.ID, (prefix + ".shininess").c_str());
    if (loc != -1) {
        float shininess;
        glGetUniformfv(shader.ID, loc, &shininess);
        std::cout << "Shininess: " << shininess << std::endl;
    }
}
void ShaderManager::PrintLightUniforms(const Shader & shader) {
    shader.use();  // Assicurati che lo shader sia attivo

    const std::string& prefix = "light";
    GLint locPosition = glGetUniformLocation(shader.ID, (prefix + ".position").c_str());
    if (locPosition != -1) {
        GLfloat pos[3];
        glGetUniformfv(shader.ID, locPosition, pos);
        std::cout << "Position: " << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
    }

    GLint locAmbient = glGetUniformLocation(shader.ID, (prefix + ".ambient").c_str());
    if (locAmbient != -1) {
        GLfloat amb[3];
        glGetUniformfv(shader.ID, locAmbient, amb);
        std::cout << "Ambient: " << amb[0] << ", " << amb[1] << ", " << amb[2] << std::endl;
    }

    GLint locDiffuse = glGetUniformLocation(shader.ID, (prefix + ".diffuse").c_str());
    if (locDiffuse != -1) {
        GLfloat diff[3];
        glGetUniformfv(shader.ID, locDiffuse, diff);
        std::cout << "Diffuse: " << diff[0] << ", " << diff[1] << ", " << diff[2] << std::endl;
    }

    GLint locSpecular = glGetUniformLocation(shader.ID, (prefix + ".specular").c_str());
    if (locSpecular != -1) {
        GLfloat spec[3];
        glGetUniformfv(shader.ID, locSpecular, spec);
        std::cout << "Specular: " << spec[0] << ", " << spec[1] << ", " << spec[2] << std::endl;
    }
}