#include "WindowGlfw.h"
#include <iostream>

MyEngine::Device::WindowGlfw::WindowGlfw(const std::string & name, int width, int height)
{
	setName(name);
	setWidth(width);
	setHeight(height);
	try
	{
	initializeGlfw();
	createWindow();
	}
	catch (std::string e)
	{
		std::cout << e << "\n";
		throw "Problem in WindowGlfw class!";
	}

}

MyEngine::Device::WindowGlfw::~WindowGlfw()
{
	if (window != nullptr)
		glfwDestroyWindow(window);
}

bool MyEngine::Device::WindowGlfw::isClosed()
{
	return glfwWindowShouldClose(window);
}

void MyEngine::Device::WindowGlfw::swapBuffer()
{
	glfwSwapBuffers(window);
}

GLFWwindow * MyEngine::Device::WindowGlfw::getWindow()
{
	return window;
}

void MyEngine::Device::WindowGlfw::waitForEvent()
{
	//glfwWaitEvents();
	glfwPollEvents();
}

void MyEngine::Device::WindowGlfw::initViewport()
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
}

void MyEngine::Device::WindowGlfw::clearWindow(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MyEngine::Device::WindowGlfw::updateWindow()
{
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

void MyEngine::Device::WindowGlfw::createWindow()
{
	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	if (window == NULL)
		throw "Can not create window";
	glfwMakeContextCurrent(window);
}

void MyEngine::Device::WindowGlfw::initializeGlfw()
{
	if (glfwInit())
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}
	else
		throw "Can not initialize glfw";
}
