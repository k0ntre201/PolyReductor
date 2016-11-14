/*
Keyboard interface
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H


namespace MyEngine
{
	namespace Enumerators
	{
		enum KeyboardKeys
		{
			A_KEY, S_KEY, D_KEY, W_KEY, LEFT_ALT_KEY, ESCAPE_KEY, C_KEY, ENTER_KEY, LEFT_CTRL_KEY,NO_KEY
		};
	}
	namespace Device
	{
		class Keyboard
		{
		public:
			Keyboard();
			virtual ~Keyboard();

			virtual MyEngine::Enumerators::KeyboardKeys getKey() = 0;
			
		private:

		};
	}
}
#endif // !KEYBOARD_H
