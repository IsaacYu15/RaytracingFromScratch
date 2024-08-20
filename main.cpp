
#include "MathLib/mathLibrary.h"

#include "RayGeneration/camera.h"
#include "Hittables/hittable.h"
#include "Materials/material.h"
#include "Hittables/hittable_list.h"
#include "Hittables/sphere.h"
#include "Hittables/triangle.h"
#include "Hittables/mesh.h"


//test read obj
#include <sstream>
#include "fstream"
#include "stdlib.h"
using namespace std;
//end test


int main() {

    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    auto material = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
    auto material1 = make_shared<lambertian>(color(random_double(), random_double(), random_double()));

    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material));
    world.add(make_shared<sphere>(point3(0.0,    0.0, -1.0),   0.4, material1));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 5;
    cam.max_depth         = 10;

    cam.vfov     = 30;
    cam.lookfrom = point3(-2,2,1);
    cam.lookat   = point3(0,0,-1);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist    = 20.0;

    cam.render(world);




}