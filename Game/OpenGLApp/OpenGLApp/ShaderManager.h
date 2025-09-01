#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "shader_s.h"

class MaterialType {
public:
	//MaterialName name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	MaterialType(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
		: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {
	};
};

enum class ProjectionType {NDC, Pixels};

class ShaderManager {
public:
	static const MaterialType Gold;
	static const MaterialType Silver;
	
	static void SetProjection(const Shader& shader, GLFWwindow* window, ProjectionType type);
	static void SetMaterial(const Shader& shader, const MaterialType& material);
	static void SetLight(const Shader& shader, glm::vec3 position, glm::vec3 color);

	static void PrintMaterialUniforms(const Shader& shader);
	static void PrintLightUniforms(const Shader& shader);

	private:
	static const glm::mat4 projectionNDC;


};

