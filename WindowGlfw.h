#ifndef WINDOW_GLFW_H
#define WINDOW_GLFW_H

#include "Window.h"
#include "OpenGLLibaries.h"

namespace MyEngine
{
	namespace Device
	{
		class WindowGlfw:public Window
		{
		public:
			//WindowGlfw(const std::string& name);
			//WindowGlfw(const std::string& name, int flags);
			WindowGlfw(const std::string& name, int width, int height);
			virtual ~WindowGlfw();
		public:
			virtual bool isClosed() override;
			virtual void swapBuffer() override;
			virtual void waitForEvent() override;
			virtual void initViewport() override;
			virtual void clearWindow(float r, float g, float b, float a) override;
			virtual void updateWindow() override;

			GLFWwindow* getWindow();
		private:
			GLFWwindow* window;

		protected:
			virtual void createWindow() override;

		private:
			void initializeGlfw();
		};

	}
}

#endif // !WINDOW_GLFW_H
