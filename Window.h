#ifndef WINDOW_H
#define WINDOW_H

#include <string>


namespace MyEngine
{
	namespace Device
	{
		class Window
		{
		public:
			Window() {};
			virtual ~Window() {};
			virtual bool isClosed() = 0;
			virtual void swapBuffer() = 0;
			virtual void waitForEvent() = 0;
			virtual void initViewport() = 0;
			virtual void clearWindow(float r, float g, float b, float a) = 0;
			virtual void updateWindow() = 0;
		public:
			std::string getName();
			void setName(const std::string& name);
			int getWidth();
			void setWidth(int width);
			int getHeight();
			void setHeight(int height);
		protected:
			std::string name;
			int width;
			int height;

			virtual void createWindow() = 0;
		private:

		};
	}
}
#endif // !WINDOW_H

