
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

string projectDirectory = "C:\\Users\\yuisa\\CLionProjects\\RaytracingFromScratch\\obj\\";

int main() {
    double arr[3][3] = { {1,1,1}, {1,1,1}, {1,1,1} };
    double arr2[3][3] = { {6,2,1}, {5,5,6}, {1,0,0} };
    matrix m (arr);
    matrix m2 (arr);
    m -= m2;


//    hittable_list world;
//
//    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
//    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));
//
//    auto material1 = make_shared<dielectric>(1.5);
//    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
//    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
//
//    mesh* cube = new mesh (projectDirectory.append("cube.obj"), point3(0.0,1.1, 0.0), world, material2);
//
//    camera cam;
//
//    cam.aspect_ratio      = 16.0 / 9.0;
//    cam.image_width       = 1200;
//    cam.samples_per_pixel = 10;
//    cam.max_depth         = 10;
//
//    cam.vfov     = 20;
//    cam.lookfrom = point3(-15.0,3,0);
//    cam.lookat   = point3(0,0,0);
//    cam.vup      = vec3(0,1,0);
//
//    cam.defocus_angle = 0;
//    cam.focus_dist    = 15.0;
//
//    cam.render(world);



}