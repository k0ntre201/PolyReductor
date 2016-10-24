#ifndef OPENGLENGINE_H
#define OPENGLENGINE_H

#include "Engine.h"
#include "OpenGLLibaries.h"
#include "Camera.h"
#include "ShaderGLSL.h"
#include "Transform.h"

//model reduction system
#include "ModelForReduction.h"
#include "MaterialForDrawModel.h"

namespace MyEngine
{
	class OpenGLEngine:public MyEngine::Engine
	{
	public:
		OpenGLEngine();
		virtual ~OpenGLEngine();
		void run();
		void initializeFramework();
	private:
		glm::ivec2 getMousePosition();
		void moveCamera(MyEngine::Enumerators::KeyboardKeys keys);
		void mouseMoveCamera();
		void keyboardMoveCamera(MyEngine::Enumerators::KeyboardKeys keys);


	protected:
		void initializeObjects();
	private:
		std::shared_ptr<MyEngine::Renderer::Camera> camera;
		std::shared_ptr<MyEngine::Renderer::ShaderGlsl> shader;
		std::shared_ptr<MyEngine::Renderer::Transform> transform;
		int width, height;

		float dt;

		glm::mat4 VP;

		//-- ModelReductionSystem

		std::shared_ptr<PolyReductor::Renderer::ModelForReduction> modelForReduce;
		std::shared_ptr<PolyReductor::Renderer::MaterialForDrawModel> materialForDrawingModell;
	};
}
#endif // !OPENGLENGINE_H

