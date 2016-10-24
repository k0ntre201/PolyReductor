#include "Window.h"

std::string MyEngine::Device::Window::getName()
{
	return name;
}

void MyEngine::Device::Window::setName(const std::string & name)
{
	this->name = name;
}

int MyEngine::Device::Window::getWidth()
{
	return width;
}

void MyEngine::Device::Window::setWidth(int width)
{
	this->width = width;
}

int MyEngine::Device::Window::getHeight()
{
	return height;
}

void MyEngine::Device::Window::setHeight(int height)
{
	this->height = height;
}
