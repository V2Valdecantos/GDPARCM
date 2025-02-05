#pragma once
#include "rtweekend.h"
#include "hittable.h"
#include "material.h"
#include "camera.h"
#include "rtimage.h"
#include "thread"

class RTThread
{
	public:
		RTThread();
		~RTThread();

		void RenderPixel(camera* cam, int i, int j, const hittable& world);
		void RenderLine(camera* cam, int j, const hittable& world);
		bool GetIsDone();

		bool isDone = false;
};

