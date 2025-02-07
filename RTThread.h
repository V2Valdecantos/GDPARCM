#pragma once
#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"

#include "random"
#include "iostream"
#include "thread"
#include "string"

class RTThread
{

public:
	RTThread();
	~RTThread();

	void start(const hittable& world);
	void run(const hittable& world);

	void setPixelColor(int i, int j, color pixel);
	bool isRunning = false;

	std::string name;
	int uCol, lCol;
	int uRow, lRow;
	int col;
	int image_height;
	int image_width;
	int samples_per_pixel;
	int bounces;
	rtimage* image;

	camera* cam;
	cv::String filename;
	 
	std::mutex* guard;
	int pixelsRendered = 0;

	
};
