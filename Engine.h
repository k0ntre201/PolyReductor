#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include "Window.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace MyEngine
{
	class Engine
	{
	public:
		virtual void run();
		virtual void initializeFramework();
		virtual ~Engine();
	private:
		
	protected:
		std::shared_ptr<MyEngine::Device::Window> window;
		std::shared_ptr<MyEngine::Device::Mouse> mouse;
		std::shared_ptr<MyEngine::Device::Keyboard> keyboard;

	};
}

#endif //ENGINE_H