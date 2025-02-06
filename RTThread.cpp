#include "RTThread.h"


RTThread::RTThread()
{

}

RTThread::~RTThread()
{
}

void RTThread::start(const hittable& world)
{
	this->isRunning = true;
	std::thread t(&RTThread::run, this, std::ref(world));
	t.detach();
}

void RTThread::run(const hittable& world)
{
	std::cout << this->name << ": Start. \n";
	for (int j = uCol - 1; j >= lCol; j--) 
	{
		for (int i = lRow; i < uRow; i++) 
		{
			color pixels(0.0f, 0.0f, 0.0f);

			
			for (int s = 0; s < samples_per_pixel; s++) 
			{

				
				//float u = (float(i) + rand() / (image_width - 1));
				//float v = (float(j) + rand() / (image_height - 1));

				ray r = cam->get_ray(i, j);
				pixels = pixels + cam->ray_color(r, bounces, world);

			}

			vec3 color = cam->write_color(samples_per_pixel * pixels);

			pixel_guard.lock();
			this->image->setPixel(i, j, color.x(), color.y(), color.z(), samples_per_pixel);
			this->image->saveImage(filename);
			pixel_guard.unlock();
		}
	}

	//this->image->saveImage(filename);
	std::cout << this->name << ": Done. \n";
	this->isRunning = false;
}
