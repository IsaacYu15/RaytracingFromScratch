
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


    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, material));
    world.add(make_shared<sphere>(point3(0, 1.2, 0), 1.0, material1));
    world.add(make_shared<spotLight>(color(1.0,1.0,1.0), 10.0, point3(2,5,5), point3(0, 1.2, 0), 20, 3.5));

    camera cam;

    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 10;
    cam.background        = color(0.0,0.0,0.0);

    cam.vfov     = 40;
    cam.lookfrom = point3(10,2,5);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist    = 20.0;

    cam.render(world);




}