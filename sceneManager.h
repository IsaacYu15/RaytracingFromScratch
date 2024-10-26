#ifndef RAYTRACINGFROMSCRATCH_SCENEMANAGER_H
#define RAYTRACINGFROMSCRATCH_SCENEMANAGER_H

class sceneManager {
public:
    int image_width;
    int image_height;
    double aspect_ratio;

    int* samples_per_pixel;
    int* ray_depth;

    double* lookfrom_x;
    double* lookfrom_y;
    double* lookfrom_z;

    double* lookat_x;
    double* lookat_y;
    double* lookat_z;

    double vfov;

    double defocus_angle;
    double focus_dist;

    sceneManager (int width, int height, int& samples, int& depth,
                  double& from_x, double& from_y, double& from_z,
                  double& at_x, double& at_y, double& at_z)
                    :image_width(width), image_height(height)
    {
        aspect_ratio = double(image_width) / double(image_height);

        samples_per_pixel = &samples;
        ray_depth = &depth;

        lookfrom_x = &from_x;
        lookfrom_y = &from_y;
        lookfrom_z = &from_z;

        lookat_x = &at_x;
        lookat_y = &at_y;
        lookat_z = &at_z;

    };

    unsigned char * load_scene()
    {
        auto material = make_shared<lambertian>(color(0.2, 0.2, 0.2));

        world.add(make_shared<sphere>(point3(0,-1000,0), 1000, material));
        transform trans (point3(0,0,0), point3(0,0,0), point3(1,1,1));
        mesh m ("cube.obj", trans, world, material);

        camera cam;

        cam.image_width       = image_width;
        cam.aspect_ratio      = aspect_ratio;
        cam.samples_per_pixel = *samples_per_pixel;
        cam.max_depth         = *ray_depth;
        cam.background        = color(0.5,0.5,0.5);

        cam.vfov     = 30;
        cam.lookfrom = point3(*lookfrom_x, *lookfrom_y, *lookfrom_z);
        cam.lookat   = point3(*lookat_x, *lookat_y, *lookat_z);
        cam.vup      = vec3(0,1,0);

        cam.defocus_angle = 0;
        cam.focus_dist    = 20;

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
