#include "RTThread.h"


RTThread::RTThread()
{
}

RTThread::~RTThread()
{
}

void RTThread::start()
{
	this->isRunning = true;
	std::thread t(&RTThread::run, this);
	t.detach();
}

void RTThread::run()
{

	for (int j = uCol - 1; j >= lCol; j--) 
	{
		for (int i = lRow; i < uRow; i++) 
		{
			color pixels(0.0f, 0.0f, 0.0f);

			for (int s = 0; s < samples_per_pixel; s++) 
			{
				float u = (float(i) + rand() / (image_width - 1));
				float v = (float(j) + rand() / (image_height - 1));

				ray r = cam->get_ray(u, v);
				pixels = pixels + cam->ray_color(r, bounces, world);

			}

			this->image->setPixel(i, j, pixels.x(), pixels.y(), pixels.z(), samples_per_pixel);

		}
	}

	this->isRunning = false;
}
