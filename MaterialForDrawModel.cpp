#include "MaterialForDrawModel.h"
#include <glm\gtc\type_ptr.hpp>

PolyReductor::Renderer::MaterialForDrawModel::MaterialForDrawModel()
{
	shader.reset(new MyEngine::Renderer::ShaderGlsl);
	shader->loadVertexShader("./res/shaders/blinPhong.vs");
	shader->loadFragmentShade("./res/shaders/blinPhong.fs");
	shader->attachAllShader();
	shaderProgram = shader->getProgram();
}

void PolyReductor::Renderer::MaterialForDrawModel::setMaterial(MaterialType type)
{
	mat = PolyReductor::Renderer::getMaterial(type);
}

void PolyReductor::Renderer::MaterialForDrawModel::bindMatrial(const glm::vec3& lightPosition /*the best will be eye position*/,const glm::vec3& eyePosition)
{
	shader->useShader();
	glUniform3fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(mat.ambient));
	glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(mat.diffuse));
	glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(mat.specular));
	glUniform1f(glGetUniformLocation(shaderProgram, "material.specular"), mat.shiness*128.0f);
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPosition"),1, glm::value_ptr(lightPosition));
	glUniform3fv(glGetUniformLocation(shaderProgram, "eyePosition"), 1, glm::value_ptr(eyePosition));
}
