
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
    auto material2 = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
    auto material3 = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
    auto material4 = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
    auto material5 = make_shared<lambertian>(color(random_double(), random_double(), random_double()));

    transform cubeTransform1 (point3(-2.5, 3.0, 6.0), point3(30.0, 45.0, 130.0), point3(1.0, 0.2, 0.5));
    transform cubeTransform2 (point3(-2.5, 3.0, 3.0), point3(45.0, 0.0, 0.0), point3(0.7, 0.7, 0.7));
    transform cubeTransform3 (point3(-2.5, 3.0, 0.0), point3(0.0, 0.0, 45.0), point3(1.0, 1.0, 1.0));
    transform cubeTransform4 (point3(-2.5, 3.0, -3.0), point3(0.0, 45.0, 0.0), point3(1.2, 1.2, 1.2));
    transform cubeTransform5 (point3(-2.5, 3.0, -6.0), point3(45.0, 45.0, 45.0), point3(1.0, 0.5, 0.5));

    mesh* cube = new mesh ( projectDirectory + "cube.obj", cubeTransform1, world, material1);
    mesh* cube1 = new mesh (projectDirectory + "cube.obj", cubeTransform2, world, material2);
    mesh* cube2 = new mesh (projectDirectory + "cube.obj", cubeTransform3, world, material3);
    mesh* cube3 = new mesh (projectDirectory + "cube.obj", cubeTransform4, world, material4);
    mesh* cube4 = new mesh (projectDirectory + "cube.obj", cubeTransform5, world, material5);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 3;
    cam.max_depth         = 10;

    cam.vfov     = 20;
    cam.lookfrom = point3(-22.0,17,0);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist    = 20.0;

    cam.render(world);




}