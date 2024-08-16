
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

const static string projectDirectory = "C:\\Users\\yuisa\\CLionProjects\\RaytracingFromScratch\\obj\\";

int main() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 50;
    cam.max_depth         = 20;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world);


//    std::vector<point3> v { point3(0.5, 0.0, -1.0), point3(-0.5, 0.0, -1.0), point3(0.0, -0.5, -1.0)};
//    world.add(make_shared<triangle>(v, material_center));
//
//    std::vector<point3> v2 { point3(0.5, 0.0, -1.0), point3(-0.5, 0.0, -1.0), point3(0.0, 0.5, -1.0)};
//    world.add(make_shared<triangle>(v2, material_ground));

//    mesh* cube = new mesh (projectDirectory.append("cube.obj"), point3(0.0,-1.0, 0.0), world, material_left);



}