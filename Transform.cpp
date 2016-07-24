#include "Transform.h"
#include <glm/gtx/transform.hpp>

MyEngine::Renderer::Transform::Transform()
{
}

MyEngine::Renderer::Transform::Transform(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale)
{
	pos = position;
	rot = rotation;
	scal = scale;
}

MyEngine::Renderer::Transform::~Transform()
{
}

void MyEngine::Renderer::Transform::move(const glm::vec3 & dxyz)
{
	pos += dxyz;
}

void MyEngine::Renderer::Transform::setPosition(const glm::vec3 position)
{
	pos = position;
}

glm::vec3 MyEngine::Renderer::Transform::getPosiotion()
{
	return pos;
}

const glm::vec3 & MyEngine::Renderer::Transform::getPosition() const
{
	return pos;
}

void MyEngine::Renderer::Transform::rotate(const glm::vec3 & dxyz)
{
	rot += dxyz;
}

void MyEngine::Renderer::Transform::setRotate(const glm::vec3 rotate)
{
	rot = rotate;
}

glm::vec3 MyEngine::Renderer::Transform::getRotate()
{
	return rot;
}

const glm::vec3 & MyEngine::Renderer::Transform::getRotate() const
{
	return rot;
}

void MyEngine::Renderer::Transform::scale(const glm::vec3 & dxyz)
{
	scal += dxyz;
}

void MyEngine::Renderer::Transform::setScale(const glm::vec3 scale)
{
	scal = scale;
}

glm::vec3 MyEngine::Renderer::Transform::getScale()
{
	return scal;
}

const glm::vec3 & MyEngine::Renderer::Transform::getScale() const
{
	return scal;
}

void MyEngine::Renderer::Transform::update()
{
	glm::mat4 mov = glm::translate(pos);
	glm::mat4 sc = glm::scale(scal);
	glm::mat4 rx = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 ry = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rz = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 r = rz*ry*rx;
	modelMatrix = mov*r*sc;
}
