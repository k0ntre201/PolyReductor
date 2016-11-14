/*
Class of FPS clocker.
*/

#ifndef FPS_TICKER_H
#define FPS_TICKER_H

#include <chrono>

class FPSTicker
{
public:
	void start();//measure first time to calculate FPS and deltatime
	float getDt();//get delta time between prevorius and actual timestamp's
	float getFPSValue();//get FPS. it's refreshed for 0.1s
private:
	std::chrono::high_resolution_clock::time_point prevoriusTime;
	float actualFps;
	int counter;
	float delta;//actual delta time for calculate FPS
	float returnDelta;//real delta time
};

#endif // !FPS_TICKER_H
