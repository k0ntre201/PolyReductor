#include "MouseGlfw.h"
#include <iostream>

MyEngine::Device::MouseGlfw::MouseGlfw(std::shared_ptr<MyEngine::Device::WindowGlfw> window)
{
	this->window = window->getWindow();
	std::cout << "Create glfwMouse.\n";
}

MyEngine::Enumerators::MouseButtons MyEngine::Device::MouseGlfw::getBurtton()
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		return Enumerators::LEFT_BUTTON;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		return Enumerators::RIGHT_BUTTON;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		return Enumerators::MIDDLE_BUTTON;
	return Enumerators::NONE_BUTTON;
}

MyEngine::Device::MouseGlfw::~MouseGlfw()
{
}
