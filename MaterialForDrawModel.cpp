#include "MaterialForDrawModel.h"
#include <glm\gtc\type_ptr.hpp>
/*
 *Constructor load default blin phong shader type, but in the future can be imiplements diffrent types of shading models.
 *Shaders are loaded and attached to drawing programs.
*/
PolyReductor::Renderer::MaterialForDrawModel::MaterialForDrawModel()
{
	shader.reset(new MyEngine::Renderer::ShaderGlsl);
	shader->loadVertexShader("./res/shaders/blinPhong.vs");
	shader->loadFragmentShade("./res/shaders/blinPhong.fs");
	shader->attachAllShader();
	shaderProgram = shader->getProgram();
}

/*Set material type to drawing program.*/
void PolyReductor::Renderer::MaterialForDrawModel::setMaterial(MaterialType type)
{
	mat = PolyReductor::Renderer::getMaterial(type);
}

/*
 *Tell opengl that we use shader for this drawing model. Tell our shader what mateial we use,
 *and where ale light and eye position.
*/
void PolyReductor::Renderer::MaterialForDrawModel::bindMatrial(const glm::vec3& lightPosition /*the best will be eye position*/,const glm::vec3& eyePosition)
{
	shader->useShader();
	glUniform3fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(mat.ambient));
	glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(mat.diffuse));
	glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(mat.specular));
	glUniform1f(glGetUniformLocation(shaderProgram, "material.specular"), mat.shiness*128.0f);/*For blin phong light model shiness must be multiply by 128*/
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPosition"),1, glm::value_ptr(lightPosition));
	glUniform3fv(glGetUniformLocation(shaderProgram, "eyePosition"), 1, glm::value_ptr(eyePosition));
}
