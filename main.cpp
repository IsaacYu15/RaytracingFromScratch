
#include "MathLib/mathLibrary.h"

#include "RayGeneration/camera.h"
#include "Hittables/hittable.h"
#include "Materials/material.h"
#include "Hittables/hittable_list.h"
#include "Hittables/sphere.h"
#include "Hittables/triangle.h"
#include "Hittables/mesh.h"

#include <sstream>
#include "fstream"
#include "stdlib.h"

using namespace std;

int main() {

    hittable_list world;

    auto material = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
    auto material1 = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
    auto material2 = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
    auto material3 = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
    auto material4 = make_shared<lambertian>(color(random_double(), random_double(), random_double()));

    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, material));
    world.add(make_shared<sphere>(point3(0, 2, 0), 1.7, material1));
    world.add(make_shared<sphere>(point3(0, 8, 0), 1.7, material2));
    world.add(make_shared<sphere>(point3(0, 5, 3), 1.7, material3));
    world.add(make_shared<sphere>(point3(0, 5, -3), 1.7, material4));

    auto emissive = make_shared<diffuse_light>(color(1.0,1.0,1.0), 5.0);

    world.add(make_shared<sphere>(point3(0, 5, 8), 1, emissive));

    world.add(make_shared<pointLight>(color(0.1,0.1,0.8), 7.5, point3(0,5,0)));


    camera cam;

    cam.image_width       = 400;
    cam.samples_per_pixel = 1000;
    cam.max_depth         = 10;
    cam.background        = color(0.0,0.0,0.0);

    cam.vfov     = 50;
    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist    = 20.0;

    cam.render(world);




}