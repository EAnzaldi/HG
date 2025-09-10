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
const glm::mat4 ShaderManager::projectionPixels = glm::ortho(0.0f, SCR_WIDTH_F, 0.0f, SCR_HEIGHT_F);

void ShaderManager::SetProjection(const Shader& shader, GLFWwindow* window, ProjectionType type) {
    shader.use();
    if (type == ProjectionType::NDC) {
        shader.setMat4("projection", projectionNDC);
    }
    else if (type == ProjectionType::Pixels) {
        //int fbWidth, fbHeight;
        //glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        //glm::mat4 projectionPixels = glm::ortho(0.0f, static_cast<float>(fbWidth), 0.0f, static_cast<float>(fbHeight));
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