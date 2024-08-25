
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
    world.add(make_shared<sphere>(point3(0, 2, 0), 1.7, material3));

    auto emissive = make_shared<diffuse_light>(color(random_double(), random_double(), random_double()), 200.0);

    vector<point3>vertices = {point3 (0,5,2), point3 (0,6,3), point3 (0,5,4)};
    world.add(make_shared<triangle>(vertices, emissive));
    vector<point3>vertices2 = {point3 (0,5,4), point3 (0,4,3), point3 (0,5,2)};
    world.add(make_shared<triangle>(vertices2, emissive));

    camera cam;

    cam.image_width       = 400;
    cam.aspect_ratio      = 9.0/16.0;
    cam.samples_per_pixel = 1000;
    cam.max_depth         = 10;
    cam.background        = color(0.1,0.1,0.1);

    cam.vfov     = 50;
    cam.lookfrom = point3(15,15,15);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist    = 20.0;

    cam.render(world);




}