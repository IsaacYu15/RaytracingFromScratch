#ifndef RAYTRACINGFROMSCRATCH_CAMERA_H
#define RAYTRACINGFROMSCRATCH_CAMERA_H

#include "MathLib/mathLibrary.h"
#include "Hittables/hittable.h"
#include "Hittables/hittable_list.h"
#include "Materials/material.h"

#include <chrono>
#include <thread>

using namespace std::chrono;

class camera {
public:
    unsigned char * final_image;
    double aspect_ratio = 1.0; //image width over height
    int image_width = 100;     //width pixel count
    int samples_per_pixel = 10; //count random samples for each pixel
    int max_depth = 10; //max ray bounces to avoid infinite recursion
    color background;

    double vfov = 90; //vertical field of view
    point3 lookfrom = point3(0,0,0); //camera's starting position
    point3 lookat = point3 (0,0, -1); //camera is looking at
    vec3 vup = vec3(0,1,0); //relative "up direction"

    double defocus_angle = 0;
    double focus_dist = 10;

    void render_subImage (const hittable_list& world, int startRow, int threads, unsigned char * sub_render)
    {
        for (int j = startRow; j < image_height; j+=threads) {

            for (int i = 0; i < image_width; i++) {

                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                std::array<unsigned char, 3> colors = write_color(pixel_samples_scale * pixel_color);
                unsigned int index = (j * image_width + i) * 4;
                sub_render[index]     = colors[0];
                sub_render[index + 1] = colors[1];
                sub_render[index + 2] = colors[2];
                sub_render[index + 3] = 255;
            }
        }

        std::clog << "Thread: " << startRow << " done! \n" << std::flush;

    }

    unsigned char * render(const hittable_list& world) {

        initialize();

        //this should be move to main to avoid multiple final_images being created
        final_image = new unsigned char[image_height*image_width*4];

        const auto threads = 1;
        std::vector<std::thread>thread_list;
        thread_list.reserve(threads);

        for (int i = 0 ; i < threads; i ++)
        {
            thread_list.emplace_back([this, &world, i, threads]() {
                render_subImage(world, i, threads, final_image);
            });
        }

        for(auto& t: thread_list)
        {
            t.join();
        }


        return final_image;
    }

private:
    int image_height;
    point3 camera_center;
    double pixel_samples_scale; //color scale factor for a sum of pixel samples
    point3 pixel00_loc;  //location of the (0,0) pixel
    vec3 pixel_delta_u;  //pixel right direction
    vec3 pixel_delta_v;  //pixel left direction
    vec3 u,v,w;          //coordinate system in respect to camera
    vec3 defocus_disk_u; //horizontal radius of defocus lens
    vec3 defocus_disk_v;//vertical radius of defocus lens

    void initialize(){
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        camera_center = lookfrom;

        //camera
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        //basis vector for camera
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        //scan the viewport left to right and up to down
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        //pixel spacing
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        //location of upper left pixel
        auto viewport_upper_left = camera_center - (focus_dist*w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

        //calculate the camera defocus basis vectors
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                            + ((i + offset.x()) * pixel_delta_u)
                            + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }


    color ray_color(const ray& r, int depth, const hittable_list& world) const
    {
        //avoid rays for recursing infinitely
        if (depth <= 0)
            return color(0,0,0);

        hit_record rec;


        if(!world.hit(r, interval(0.001, infinity), rec))
        {
            return background + world.shouldIlluminate(r.origin());
        }

        //objs
        ray scattered;
        color attenuation;
        color emission_color = rec.mat->emitted() + world.shouldIlluminate(r.origin());

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return emission_color;

        color scatter_color = attenuation * ray_color(scattered, depth-1, world);

        return emission_color + scatter_color;
    }
};


#endif //RAYTRACINGFROMSCRATCH_CAMERA_H