/*
Component for histogram contener.
*/

#ifndef HISTOGRAMS_H
#define HISTOGRAMS_H

#include <array>
#include <glm\glm.hpp>
//#include "ModelForReduction.h"
#include <memory>
#include "Transform.h"
#include <opencv2\opencv.hpp>

struct Histograms
{
	std::array<cv::MatND, 16> histograms;//contain 16 histograms. For all posibly directions
};

/*get two histograms to compare. Return simillarity value*/
float compareHistograms(const Histograms& h1, const Histograms& h2);

#endif // !HISTOGRAMS_H
