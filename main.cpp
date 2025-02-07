#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include "RTThread.h"
#include "vector"
#include "iostream"

int main()
{
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    //for (int a = -50; a < 50; a++) 
    //{
    //    for (int b = -50; b < 50; b++) 
    //    {
    //        auto choose_mat = random_double();
    //        point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

    //        if ((center - point3(4, 0.2, 0)).length() > 0.9) 
    //        {
    //            shared_ptr<material> sphere_material;

    //            if (choose_mat < 0.8) 
    //            {
    //                // diffuse
    //                auto albedo = color::random() * color::random();
    //                sphere_material = make_shared<lambertian>(albedo);
    //                world.add(make_shared<sphere>(center, 0.2, sphere_material));
    //            }
    //            else if (choose_mat < 0.95) 
    //            {
    //                // metal
    //                auto albedo = color::random(0.5, 1);
    //                auto fuzz = random_double(0, 0.5);
    //                sphere_material = make_shared<metal>(albedo, fuzz);
    //                world.add(make_shared<sphere>(center, 0.2, sphere_material));
    //            }
    //            else 
    //            {
    //                // glass
    //                sphere_material = make_shared<dielectric>(1.5);
    //                world.add(make_shared<sphere>(center, 0.2, sphere_material));
    //            }
    //        }
    //    }
    //}

    //auto material1 = make_shared<dielectric>(1.5);
    //world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    //auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    //world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));   

    //auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    //world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    //auto material4 = make_shared<lambertian>(color(0.87, 0, 0));
    //world.add(make_shared<sphere>(point3(-8, 1, 0), 1.0, material4));


    auto material4 = make_shared<lambertian>(color(0.87, 0, 0));
    world.add(make_shared<sphere>(point3(0, 1, 0), 0.2, material4));
    world.add(make_shared<sphere>(point3(0, 1.4, 0), 0.2, material4));
    world.add(make_shared<sphere>(point3(0, 1.8, 0), 0.2, material4));
    world.add(make_shared<sphere>(point3(0, 2.2, 0), 0.2, material4));
    world.add(make_shared<sphere>(point3(0, 2.6, 0), 0.2, material4));
    world.add(make_shared<sphere>(point3(0, 3, 0), 0.2, material4));

    camera cam;
    
    int image_width = 1080;
    int image_height = 1080;
    int num_cores = 2;

    cam.aspect_ratio = 1;
    cam.image_width = image_width;
    cam.image_height = image_height;
    cam.samples_per_pixel = 200;
    cam.max_depth = 50;

    cam.vfov = 40;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0; 
    
    cam.initialize();
    //cam.render(world);

    rtimage* image = new rtimage(image_width, image_height);
    std::mutex* guard = new std::mutex();

    std::vector<RTThread*> threads;
    cv::String filename = "C:/Users/valde/Desktop/GDPARCM/AssetLoader/Png/3.png";

    int widthWindow = rint(image_width / num_cores);
    int lRow = 0;
    int uRow = widthWindow;

    int totalpixels = image_width * image_height;
    int pixelsRendered = 0;

    for (int i = 0; i < num_cores; i++) 
    {

        RTThread* rtThread = new RTThread();
        rtThread->name = "Thread " + std::to_string(i);
        rtThread->image_height = image_height;
        rtThread->image_width = image_width;
        rtThread->image = image;
        rtThread->cam = &cam;
        rtThread->bounces = 50;
        rtThread->samples_per_pixel = 200;
        rtThread->filename = filename;
        rtThread->guard = guard;
        //rtThread->pixelsRendered = pixelsRendered;

        //auto compute
        rtThread->lRow = lRow;
        rtThread->uRow = uRow;

        //rtThread->lCol = 0;
        //rtThread->uCol = image_height;

        //manual set - per row thread
        //rtThread->lRow = 0;
        //rtThread->uRow = i;

        rtThread->lCol = 0;
        rtThread->uCol = image_height;

        rtThread->start(world);

        threads.push_back(rtThread);

        lRow = lRow + widthWindow;

        if (i + 2 == num_cores)
            uRow = image_width;
        else
            uRow = uRow + widthWindow;
        


    }
   

    bool standby = true;
    while (standby) 
    {
        std::clog << "\Pixels Rendered: " << pixelsRendered << " out of " << totalpixels << std::flush;
        system("cls");

        pixelsRendered = 0;

        for (int i = 0; i < threads.size(); i++) 
        {
            pixelsRendered += threads[i]->pixelsRendered;
            standby = false;
            if (threads[i]->isRunning) {
                standby = true;
                break;
            }
        }
    }

    image->saveImage(filename);
    std::cout << "Done." << std::endl;
}

