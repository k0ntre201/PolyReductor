#ifndef KEYBOARD_GLFW_H
#define KEYBOARD_GLFW_H

#include "Keyboard.h"
#include "WindowGlfw.h"
#include <memory>

namespace MyEngine
{
	namespace Device
	{
		class KeyboardGlfw:public MyEngine::Device::Keyboard
		{
		public:
			
			KeyboardGlfw(std::shared_ptr<MyEngine::Device::WindowGlfw> window);

			virtual MyEngine::Enumerators::KeyboardKeys getKey() override;

			virtual ~KeyboardGlfw();
		private:
			GLFWwindow* window;
		};
	}
}

#endif // !KEYBOARD_GLFW_H
