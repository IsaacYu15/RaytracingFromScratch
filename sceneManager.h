#ifndef RAYTRACINGFROMSCRATCH_SCENEMANAGER_H
#define RAYTRACINGFROMSCRATCH_SCENEMANAGER_H

class sceneManager {
public:
    int image_width;
    double aspect_ratio;

    sceneManager (int width, double ratio): image_width(width), aspect_ratio(ratio) {};

    unsigned char * load_scene()
    {
        auto material = make_shared<lambertian>(color(0.2, 0.2, 0.2));

        world.add(make_shared<sphere>(point3(0,-1000,0), 1000, material));

        camera cam;

        cam.image_width       = image_width;
        cam.aspect_ratio      = aspect_ratio;
        cam.samples_per_pixel = 5;
        cam.max_depth         = 10;
        cam.background        = color(0.5,0.5,0.5);

        cam.vfov     = 30;
        cam.lookfrom = point3(20,2,5);
        cam.lookat   = point3(0,1.5,4.5);
        cam.vup      = vec3(0,1,0);

        cam.defocus_angle = 0;
        cam.focus_dist    = 20.0;

        return cam.render(world);
    }

    void add_sphere(point3 pos, double radius, color& pixel_color)
    {
        //why is red and blues mixed up?
        auto material  = make_shared<lambertian>(color(pixel_color.z(), pixel_color.y(), pixel_color.x()));
        world.add(make_shared<sphere>(pos, radius, material));
    }

private:
    hittable_list world;


};
#endif //RAYTRACINGFROMSCRATCH_SCENEMANAGER_H
