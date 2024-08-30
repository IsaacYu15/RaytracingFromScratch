
#ifndef RAYTRACINGFROMSCRATCH_SCENEMANAGER_H
#define RAYTRACINGFROMSCRATCH_SCENEMANAGER_H

#include "MathLib/mathLibrary.h"

#include "RayGeneration/camera.h"
#include "Hittables/hittable.h"
#include "Hittables/hittable_list.h"
#include "Materials/material.h"
#include "Hittables/sphere.h"

class sceneManager {
public:
    void LoadScene()
    {
        hittable_list world;

        auto material1 = make_shared<dielectric>(1.5);
        world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

        auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
        world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

        auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
        world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

        camera cam;

        cam.aspect_ratio      = 16.0 / 9.0;
        cam.image_width       = 1200;
        cam.samples_per_pixel = 500;
        cam.max_depth         = 50;

        cam.vfov     = 20;
        cam.lookfrom = point3(13,2,3);
        cam.lookat   = point3(0,0,0);
        cam.vup      = vec3(0,1,0);

        cam.defocus_angle = 0.6;
        cam.focus_dist    = 10.0;

        cam.render(world);
    }
};


#endif //RAYTRACINGFROMSCRATCH_SCENEMANAGER_H
