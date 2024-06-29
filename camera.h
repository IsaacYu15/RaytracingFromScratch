#ifndef RAYTRACINGFROMSCRATCH_CAMERA_H
#define RAYTRACINGFROMSCRATCH_CAMERA_H

#include "mathLibrary.h"
#include "hittable.h"

class camera {
    public:
        double aspect_ratio = 1.0; //image width over height
        int image_width = 100;     //width pixel count
        int samples_per_pixel = 10; //count random samples for each pixel

        void render(const hittable& world) {

            initialize();

            // Render ppm image
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

                for (int i = 0; i < image_width; i++) {

                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, world);
                    }
                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }

            std::clog << "\rDone.                 \n";
        }

    private:
        int image_height;
        point3 camera_center;
        double pixel_samples_scale; //color scale factor for a sum of pixel samples
        point3 pixel00_loc;  //location of the (0,0) pixel
        vec3 pixel_delta_u;  //pixel right direction
        vec3 pixel_delta_v;  //pixel left direction

        void initialize(){
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;
            camera_center = point3(0,0,0);

            //camera
            auto focal_length = 1.0;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            //scan the viewport left to right and up to down
            auto viewport_u = vec3(viewport_width,0,0);
            auto viewport_v = vec3(0, -viewport_height, 0);

            //pixel spacing
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            //location of upper left pixel
            auto viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);
        }

        ray get_ray(int i, int j) const {
            // Construct a camera ray originating from the origin and directed at randomly sampled
            // point around the pixel location i, j.

            auto offset = sample_square();
            auto pixel_sample = pixel00_loc
                                + ((i + offset.x()) * pixel_delta_u)
                                + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = camera_center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }


        color ray_color(const ray& r, const hittable& world)
        {
            //objs
            hit_record rec;
            if (world.hit(r, interval(0, infinity), rec))
            {
                vec3 direction = random_on_hemisphere(rec.normal);
                //we call this ray colour recursively to allow a ray to bounce multiple times and intersect multiple surfaces
                return 0.3 * ray_color(ray(rec.p, direction), world);
            }

            //sky gradient
            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        }
};


#endif //RAYTRACINGFROMSCRATCH_CAMERA_H
