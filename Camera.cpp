#include "Camera.h"
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace MyEngine;

Renderer::Camera::Camera(const glm::vec3 & pos, const glm::vec3 & target, float aspectRatio, float FOV, float zNear, float zFar, glm::vec3 UP)
{
	this->aspectRatio = aspectRatio;
	this->FOV = glm::radians(FOV);
	this->pos = pos;
	this->up = UP;
	this->zFar = zFar;
	this->zNear = zNear;
	this->target = target;
	updateViewMatrix();
	updatePerspectiveMatrix();
}

Renderer::Camera::~Camera()
{
}

const glm::mat4 & Renderer::Camera::getViewMatrix()
{
	return viewMatrix;
}

const glm::mat4 & Renderer::Camera::getProjectionMatrix()
{
	return _projectionMatrix;
}

const glm::mat4 & Renderer::Camera::getViewProjectionMatrix()
{
	return _projectionMatrix*viewMatrix;
}

void Renderer::Camera::moveForward(float dt)
{
	pos += target*speed*dt;
}

void Renderer::Camera::moveBack(float dt)
{
	pos -= target*speed*dt;
}

void Renderer::Camera::moveLeft(float dt)
{
	pos -= right*speed*dt;
}

void Renderer::Camera::moveRight(float dt)
{
	pos += right*speed*dt;
}

const glm::vec3 & Renderer::Camera::getCameraPosition() const
{
	return pos;
}

float Renderer::Camera::getCameraFOV()
{
	return FOV;
}

float Renderer::Camera::getCameraAspectrario()
{
	return aspectRatio;
}

void Renderer::Camera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	updatePerspectiveMatrix();
}

void Renderer::Camera::setSpeed(float s)
{
	speed = s;
}

void Renderer::Camera::addYaw(float yawOffsed)
{
	yawOff = yawOffsed;
	yaw += yawOffsed;
	if (yaw > 360.0f || yaw < -360.0f)
		yaw = 0.0f;
}

void Renderer::Camera::addPitch(float pithOffsed)
{
	pitchOff = pithOffsed;
	pith += pithOffsed;
	if (pith > 89.0f)
	{
		pitchOff = 0.0f;
		pith =89.0f;
	}
	else if (pith < -89.0f)
	{
		pitchOff = 0.0f;
		pith = -89.0f;
	}
}

void Renderer::Camera::update()
{
	right = glm::normalize(glm::cross(target, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::quat yawQuat = glm::angleAxis(glm::radians(yawOff), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat pitchQuat = glm::angleAxis(glm::radians(pitchOff), right);
	yawOff = 0.0f;
	pitchOff = 0.0f;
	glm::quat rot = glm::normalize(glm::cross(yawQuat, pitchQuat));
	target = glm::normalize(glm::rotate(rot, target));
	up = glm::normalize(glm::cross(right, target));
	updateViewMatrix();
}

void Renderer::Camera::updatePerspectiveMatrix()
{
	_projectionMatrix = glm::perspective(FOV, aspectRatio, zNear, zFar);
}

void Renderer::Camera::updateViewMatrix()
{
	viewMatrix = glm::lookAt(pos,pos + target, up);
}
