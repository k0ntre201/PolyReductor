#include "KeyboardGlfw.h"

using namespace MyEngine;
using namespace Enumerators;

MyEngine::Device::KeyboardGlfw::KeyboardGlfw(std::shared_ptr<MyEngine::Device::WindowGlfw> window)
{
	this->window = window->getWindow();
}

MyEngine::Enumerators::KeyboardKeys MyEngine::Device::KeyboardGlfw::getKey()
{
	if (glfwGetKey(window, GLFW_KEY_A))
		return A_KEY;
	if (glfwGetKey(window, GLFW_KEY_S))
		return S_KEY;
	if (glfwGetKey(window, GLFW_KEY_D))
		return D_KEY;
	if (glfwGetKey(window, GLFW_KEY_W))
		return W_KEY;
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))
		return LEFT_ALT_KEY;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
		return LEFT_CTRL_KEY;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		return ESCAPE_KEY;
	if (glfwGetKey(window, GLFW_KEY_C))
		return C_KEY;
	if (glfwGetKey(window, GLFW_KEY_ENTER))
		return ENTER_KEY;
	return NO_KEY;
}

MyEngine::Device::KeyboardGlfw::~KeyboardGlfw()
{
}
