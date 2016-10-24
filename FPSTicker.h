#ifndef FPS_TICKER_H
#define FPS_TICKER_H

#include <chrono>

class FPSTicker
{
public:
	void start();
	float getDt();
	float getFPSValue();
private:
	std::chrono::high_resolution_clock::time_point prevoriusTime;
	float actualFps;
	int counter;
	float delta;
	float returnDelta;
};

#endif // !FPS_TICKER_H
