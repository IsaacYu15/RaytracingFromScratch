
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

    auto material = make_shared<dielectric>(1.5);
    auto material1 = make_shared<dielectric>(1.0 / 1.5);

    transform cubeTransform1 (point3(-2.5, 3.0, 0.0), point3(0,0,0), point3(0.9, 0.9,0.9));
    transform cubeTransform2 (point3(-2.5, 3.0, 0.0), point3(0,0,0), point3(1.0, 1.0,1.0));

    mesh* cube = new mesh ("cube.obj", cubeTransform1, world, material);
    mesh* cube2 = new mesh ("cube.obj", cubeTransform2, world, material);


    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 25;
    cam.max_depth         = 10;

    cam.vfov     = 20;
    cam.lookfrom = point3(-7,7,3);
    cam.lookat   = point3(-2.5, 3.0, 0.0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist    = 20.0;

    cam.render(world);




}