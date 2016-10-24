#include "FPSTicker.h"

void FPSTicker::start()
{
	this->prevoriusTime = std::chrono::high_resolution_clock::now();
	actualFps = 100;
	delta = 0;
	returnDelta = 0;
}

float FPSTicker::getDt()
{
	return delta;
}

float FPSTicker::getFPSValue()
{
	using namespace std::chrono;
	auto actualTime = std::chrono::high_resolution_clock::now();
	delta = duration_cast<duration<double>>(actualTime - prevoriusTime).count();
	counter++;
	if (delta > 0.1)
	{
		actualFps = counter / delta;
		prevoriusTime = actualTime;
		counter = 0;
	}
	return actualFps;
}
