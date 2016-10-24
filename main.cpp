#include <iostream>
#include <memory>
#include <string>
#include "OpenGLEngine.h"

int main()
{
	try
	{
		std::unique_ptr<MyEngine::Engine> engine(new MyEngine::OpenGLEngine());
		engine->initializeFramework();
		engine->run();
	}
	catch (std::string exception)
	{
		std::cout << exception << "\n";
	}

	std::cin.get();
	return 0;
}