#pragma once

#include <vector>
#include "opencv2/core.hpp"
#include <opencv2/imgcodecs.hpp> 

class rtimage 
{
public:
	rtimage(const int imageWidth, const int imageHeight);
	void setPixel(int x, int y, float r, float g, float b, int samplesPerPixel);

	void saveImage(cv::String& filename) const;

private:
	std::unique_ptr<cv::Mat> pixels;
	int imageHeight;
	int imageWidth;
};


