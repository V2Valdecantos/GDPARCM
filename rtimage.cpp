#include "rtimage.h"

rtimage::rtimage(const int imageWidth, const int imageHeight)
{
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;

	this->pixels = std::make_unique<cv::Mat>(cv::Mat::zeros(this->imageHeight, this->imageWidth, CV_8UC3));

}

void rtimage::setPixel(int x, int y, float r, float g, float b, int samplesPerPixel)
{
	cv::Mat imgChannels[3];
	cv::split(*this->pixels, imgChannels);

	imgChannels[0].at<uchar>(this->imageHeight - 1 - y, x) = b;
	imgChannels[1].at<uchar>(this->imageHeight - 1 - y, x) = g;
	imgChannels[2].at<uchar>(this->imageHeight - 1 - y, x) = r;

	cv::merge(imgChannels, 3, *this->pixels);
}

void rtimage::saveImage(cv::String& filename) const
{
	cv::Mat flipped;
	cv::flip(*this->pixels, flipped, 0);  // Flip vertically (flipCode = 0)
	cv::imwrite(filename, flipped);
}

