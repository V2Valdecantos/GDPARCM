#include "RTThread.h"


RTThread::RTThread()
{

}

RTThread::~RTThread()
{
	guard->lock();
	this->image->saveImage(filename);
	guard->unlock();
}

void RTThread::start(const hittable& world)
{
	this->isRunning = true;
	std::thread t(&RTThread::run, this, std::ref(world));
	t.detach();
}

void RTThread::run(const hittable& world)
{
	//std::cout << this->name << ": Start. \n";
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

			//pixels *= samples_per_pixel;

			//this->setPixelColor(i, j, pixels);
			//guard->lock();
			this->image->setPixel(i, j, int(pixels.x()), int(pixels.y()), int(pixels.z()), samples_per_pixel);
			//guard->unlock();
			this->pixelsRendered += 1;
		}
	}

	//guard->lock();
	this->image->saveImage(filename);
	//guard->unlock();

	//std::cout << this->name << ": Done. \n";
	this->isRunning = false;
}

void RTThread::setPixelColor(int i, int j, color pixel)
{
	//std::lock_guard<std::mutex> lock(*guard);
	guard->lock();
	this->image->setPixel(i, j, int(pixel.x()), int(pixel.y()), int(pixel.z()), samples_per_pixel);
	this->image->saveImage(filename);
	guard->unlock();
}
