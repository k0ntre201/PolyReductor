#include "Histograms.h"

float compareHistograms(const Histograms & h1, const Histograms & h2)
{
	double value{ 0.0 };
	for (int i = 0;i< h1.histograms.size(); ++i)
	{
		std::cout << i << "\n";
		value += cv::compareHist(h1.histograms[i], h2.histograms[i], 0);
	}
	return value /h1.histograms.size();
}
