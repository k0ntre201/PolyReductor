#include "OpenGLEngine.h"
#include "OpenGLLibaries.h"
#include <string>
#include <iostream>
#include "WindowGlfw.h"
#include "MouseGlfw.h"
#include "KeyboardGlfw.h"
#include <glm\gtc\type_ptr.hpp>
#include "FPSTicker.h"
#include <opencv2\opencv.hpp>

MyEngine::OpenGLEngine::OpenGLEngine()
{
	std::cout << "Create OpenGL Engine.\n";
	width = 800;
	height = 600;
}

MyEngine::OpenGLEngine::~OpenGLEngine()
{
	std::cout << "Destroy OpenGL Engine\n";
	glfwTerminate();
}

void MyEngine::OpenGLEngine::run()
{
	window->initViewport();
	bool cloase = false;

	MyEngine::Enumerators::KeyboardKeys keys;

	double fps = 0;
	

	FPSTicker fpsTicker;
	fpsTicker.start();

	//unsigned char* buffer = new unsigned char[3 * width*height];

	//prepare model to reduce and compare

	PolyReductor::Renderer::MaterialType mat = PolyReductor::Renderer::MaterialType::COPPER;

	materialForDrawingModell->setMaterial(mat);
	materialForDrawingModell->bindMatrial(camera->getCameraPosition()*10.0f, camera->getCameraPosition());

	modelForReduce->createHistograms(materialForDrawingModell->getMaterialProgram(), transform, camera->getViewMatrix(), camera->getProjectionMatrix(),width,height);

	Histograms h1 = modelForReduce->getHistograms();

	modelForReduce->reduce(2000);
	modelForReduce->deleteBUffers();
	modelForReduce->prepareModelToDraw();
	modelForReduce->createHistograms(materialForDrawingModell->getMaterialProgram(), transform, camera->getViewMatrix(), camera->getProjectionMatrix(), width, height);
	std::cout << "Comparasion oryginal:reduced " << compareHistograms(h1, modelForReduce->getHistograms()) << "\n";


	while(!window->isClosed() && !cloase)
	{
		glm::mat4 m = transform->getModelMatrix();
		glm::mat4 v = camera->getViewMatrix();
		glm::mat4 p = camera->getProjectionMatrix();
		window->clearWindow(0.0, 0.0, 0.0, 1.0);
		PolyReductor::Renderer::MaterialType mat = PolyReductor::Renderer::MaterialType::COPPER;

		materialForDrawingModell->setMaterial(mat);
		materialForDrawingModell->bindMatrial(camera->getCameraPosition()*10.0f, camera->getCameraPosition());
		modelForReduce->draw(materialForDrawingModell->getMaterialProgram(), m, v, p);
		
		keys = keyboard->getKey();
		if (keys == MyEngine::Enumerators::ESCAPE_KEY)
			cloase = true;

		dt = fpsTicker.getDt();
		moveCamera(keys);
		window->swapBuffer();
		window->waitForEvent();

		/*glReadBuffer(GL_BACK_LEFT);
		glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, buffer);
		cv::Mat image(height, width, CV_8UC3, buffer);

		cv::namedWindow("name");
		cv::imshow("name", image);*/

		window->updateWindow();
		camera->update();
		
		fpsTicker.getFPSValue();

		transform->rotate(glm::vec3(dt/10.0f, 0.0, 0.0));
		transform->update();
	}
	//delete[] buffer;
}

void MyEngine::OpenGLEngine::initializeFramework()
{
	std::cout << "Initialize openGl engine framework.\n";
	
	window.reset(new MyEngine::Device::WindowGlfw("OpenglWindow", width, height));
	mouse.reset(new MyEngine::Device::MouseGlfw(std::dynamic_pointer_cast<MyEngine::Device::WindowGlfw>(window)));
	keyboard.reset(new MyEngine::Device::KeyboardGlfw(std::dynamic_pointer_cast<MyEngine::Device::WindowGlfw>(window)));

	camera.reset(new Renderer::Camera(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), (float)width / (float)height, 45.0f, 0.1f, 1000.0f));
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::string exception = "GLEW isn't initialized!\n";
		throw exception;
	}

	std::string OglVers(std::string("Opengl version ") + std::string((char*)glGetString(GL_VERSION)) + "\n");
	std::cout.write(OglVers.c_str(), OglVers.size());
	std::string OglSLVers(std::string("Opengl version ") + std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION)) + "\n");
	std::cout.write(OglSLVers.c_str(), OglVers.size());

	try
	{
		initializeObjects();
	}
	catch (std::string ex)
	{
		std::cout << ex;
		throw std::string("Problem in object initialization\n");
	}
}

glm::ivec2 MyEngine::OpenGLEngine::getMousePosition()
{
	return std::dynamic_pointer_cast<MyEngine::Device::MouseGlfw>(mouse)->getCursorPosition<glm::ivec2>();
}

void MyEngine::OpenGLEngine::moveCamera(MyEngine::Enumerators::KeyboardKeys keys)
{
	mouseMoveCamera();
	keyboardMoveCamera(keys);
}

void MyEngine::OpenGLEngine::mouseMoveCamera()
{
	glm::ivec2 oldMousePos = glm::ivec2(window->getWidth()/2, window->getHeight()/2);
	glm::ivec2 newMousePos = getMousePosition();
	std::dynamic_pointer_cast<MyEngine::Device::MouseGlfw>(mouse)->setCursorPosition<glm::ivec2>(oldMousePos);
}

void MyEngine::OpenGLEngine::keyboardMoveCamera(MyEngine::Enumerators::KeyboardKeys keys)
{
	using namespace MyEngine;
	using namespace Enumerators;
	if (keys == W_KEY)
		camera->moveForward(dt/10);
	else if (keys == S_KEY)
		camera->moveBack(dt/10);
	else if (keys == A_KEY)
		camera->moveLeft(dt/10);
	else if (keys == D_KEY)
		camera->moveRight(dt/10);
}

void MyEngine::OpenGLEngine::initializeObjects()
{
	transform.reset(new MyEngine::Renderer::Transform);
	transform->setScale(glm::vec3(1.0f));
	transform->update();

	//test for ModelReduction

	modelForReduce.reset(new PolyReductor::Renderer::ModelForReduction);
	modelForReduce->loadModel("./res/Models/skull.obj");
	//
	//modelForReduce->reduce(3200);
	modelForReduce->prepareModelToDraw();

	materialForDrawingModell.reset(new PolyReductor::Renderer::MaterialForDrawModel);
}
