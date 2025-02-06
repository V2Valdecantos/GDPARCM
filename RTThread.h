#pragma once
#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "random"
#include "thread"

class RTThread
{

public:
	RTThread();
	~RTThread();

	void start();
	void run();
	bool isRunning = false;

	int uCol, lCol;
	int uRow, lRow;
	int image_height;
	int image_width;
	int samples_per_pixel;
	hittable_list world;
	int bounces;
	rtimage* image;

	camera* cam;

	
};
