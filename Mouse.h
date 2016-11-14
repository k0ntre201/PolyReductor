/*
Interface of mouse class. All generic functions must will be overrided.
*/

#ifndef MOUSE_H
#define MOUSE_H

namespace MyEngine
{
	namespace Enumerators
	{
		enum MouseButtons
		{
			LEFT_BUTTON, RIGHT_BUTTON, MIDDLE_BUTTON,NONE_BUTTON
		};
	}

	namespace Device
	{
		class Mouse
		{
		public:
			Mouse();
			virtual ~Mouse();

			template<typename T>
			void setCursorPosition(const T& position) {}

			template<typename T>
			void setCursorPosition(const T& x, const T& y) {}

			template<typename T>
			T getCursorPosition() { return T(); }

			template<typename T>
			const T& getCursorPosition() const { return T(); }

			virtual Enumerators::MouseButtons getBurtton() = 0;

		private:

		};
	}
}

#endif //!MOUSE_H
