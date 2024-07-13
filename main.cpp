
#include "mathLibrary.h"

#include "camera.h"
#include "hittable.h"
#include "material.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "mesh.h"


//test read obj
#include <sstream>
#include "fstream"
#include "stdlib.h"
using namespace std;
//end test

int main() {
    string projectDirectory = "C:\\Users\\yuisa\\CLionProjects\\RaytracingFromScratch\\obj\\";

    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    //world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
//    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
//    world.add(make_shared<sphere>(point3(-1.0,0.0, -1.0), 0.5, material_left));
//    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

//    std::vector<point3> v { point3(0.5, 0.0, -1.0), point3(-0.5, 0.0, -1.0), point3(0.0, -0.5, -1.0)};
//    world.add(make_shared<triangle>(v, material_center));
//
//    std::vector<point3> v2 { point3(0.5, 0.0, -1.0), point3(-0.5, 0.0, -1.0), point3(0.0, 0.5, -1.0)};
//    world.add(make_shared<triangle>(v2, material_ground));

    world.add(make_shared<mesh>(projectDirectory.append("monkey.obj"), point3(0.0,0.0, -3.0), world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 80;
    cam.lookfrom = point3(-1.5,2,0.5);
    cam.lookat = point3(0,0,-3.0);
    cam.vup = vec3 (0,1,0);

    cam.render(world);

}