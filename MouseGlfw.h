#ifndef MOUSEGLFW_H
#define MOUSEGLFW_H

#include "Mouse.h"
#include "WindowGlfw.h"
#include <memory>

namespace MyEngine
{
	namespace Device
	{
		class MouseGlfw: public MyEngine::Device::Mouse
		{
		public:
			MouseGlfw(std::shared_ptr<MyEngine::Device::WindowGlfw> window);
			
			template<typename T>
			void setCursorPosition(const T& position);

			template<typename T>
			T getCursorPosition();

			template<typename T>
			const  T& getCursorPosition() const;

			template<typename T>
			void setCursorPosition(const T& x, const T& y);
			
			virtual Enumerators::MouseButtons getBurtton() override;

			virtual ~MouseGlfw();
		private:
			glm::vec2 cursorPosition;
			GLFWwindow* window;
		};
		template<typename T>
		inline void MouseGlfw::setCursorPosition(const T & position)
		{
			cursorPosition[0] = static_cast<double>(position[0]);
			cursorPosition[1] = static_cast<double>(position[1]);
			glfwSetCursorPos(window, cursorPosition[0],cursorPosition[1]);
		}
		template<typename T>
		inline T MouseGlfw::getCursorPosition()
		{
			double x{ 0.0 }, y{ 0.0 };
			glfwGetCursorPos(window, &x, &y);
			cursorPosition[0] = x;
			cursorPosition[1] = y;
			return T(cursorPosition[0],cursorPosition[1]);
		}
		template<typename T>
		inline const T & MouseGlfw::getCursorPosition() const
		{
			double x{ 0.0 }, y{ 0.0 };
			glfwGetCursorPos(window, &x, &y);
			cursorPosition[0] = x;
			cursorPosition[1] = y;
			return T(cursorPosition[0], cursorPosition[1]);
		}
		template<typename T>
		inline void MouseGlfw::setCursorPosition(const T & x, const T & y)
		{
			cursorPosition[0] = static_cast<double>(x);
			cursorPosition[1] = static_cast<double>(x);
			glfwSetCursorPos(window, , cursorPosition[0], cursorPosition[1]);
		}
	}
}

#endif //!MOUSEGLFW_H
