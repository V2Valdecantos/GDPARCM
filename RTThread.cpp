#include "RTThread.h"

RTThread::RTThread()
{
}

RTThread::~RTThread()
{
}

void RTThread::RenderPixel(camera* cam, int i, int j, const hittable& world)
{

    color pixel_color(0, 0, 0);

    for (int sample = 0; sample < cam->samples_per_pixel; sample++)
    {
        ray r = cam->get_ray(i, j);
        pixel_color += cam->ray_color(r, cam->max_depth, world);;
    }

    vec3 color = write_color(cam->pixel_samples_scale * pixel_color);


    cam->image->setPixel(i, j, int(color.x()), int(color.y()), int(color.z()), cam->samples_per_pixel);

    this->isDone = true;

    //cam->image->saveImage(filename);
}

void RTThread::RenderLine(camera* cam, int j, const hittable& world)
{
    color pixel_color(0, 0, 0);

    for (int i = 0; i < cam->image_width; i++)
    {
        for (int sample = 0; sample < cam->samples_per_pixel; sample++)
        {
            ray r = cam->get_ray(i, j);
            pixel_color += cam->ray_color(r, cam->max_depth, world);;
        }

        vec3 color = write_color(cam->pixel_samples_scale * pixel_color);


        cam->image->setPixel(i, j, int(color.x()), int(color.y()), int(color.z()), cam->samples_per_pixel);
    }
}

bool RTThread::GetIsDone()
{
    return this->isDone;
}
