
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

    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    auto material1 = make_shared<lambertian>(color(random_double(), random_double(), random_double()));

    transform cubeTransform1 (point3(-2.5, 3.0, 6.0), point3(30.0, 45.0, 130.0), point3(1.0, 0.2, 0.5));

    mesh* cube = new mesh ( projectDirectory + "cube.obj", cubeTransform1, world, material1);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 100;
    cam.samples_per_pixel = 5;
    cam.max_depth         = 10;

    cam.vfov     = 20;
    cam.lookfrom = point3(-22.0,17,0);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist    = 20.0;

    cam.render(world);




}