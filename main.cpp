
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

    auto material = make_shared<lambertian>(color(0.2, 0.2, 0.2));
    auto material1  = make_shared<lambertian>( color(1.0, 1.0, 1.0));
    auto material2  = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
    auto material3  = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
    auto material4  = make_shared<lambertian>(color(random_double(), random_double(), random_double()));

    auto material5 = make_shared<diffuse_light>(color(1.0,1.0,1.0), 1.0);

    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, material));
    world.add(make_shared<sphere>(point3(0, 1.0, 0), 1.0, material1));
    world.add(make_shared<sphere>(point3(0, 1.0, 3), 1.0, material2));
    world.add(make_shared<sphere>(point3(0, 1.0, 9), 1.0, material3));
    world.add(make_shared<sphere>(point3(0, 1.0, 6), 1.0, material4));

    world.add(make_shared<spotLight>(color(0.8,0.2,0.5), point3(2,4,5), point3(0,1.0,0), 40.0, 5.0, 6.0, 3.5));
    world.add(make_shared<pointLight>(color(0.2,0.5,0.8), point3(0,3,4.5), 5.0, 5.0));

    transform t (point3(1.0, 2.0, 11), point3(0,0,0), point3(0.2,2.0,0.2));
    mesh cube("cube.obj", t, world, material5);

    camera cam;

    cam.image_width       = 1200;
    cam.aspect_ratio      = 16.0/9.0;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 10;
    cam.background        = color(0.05,0.05,0.05);

    cam.vfov     = 30;
    cam.lookfrom = point3(20,2,5);
    cam.lookat   = point3(0,1.5,4.5);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist    = 20.0;

    cam.render(world);




}