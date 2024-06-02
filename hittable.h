#ifndef RAYTRACINGFROMSCRATCH_HITTABLE_H
#define RAYTRACINGFROMSCRATCH_HITTABLE_H

#include "ray.h"

class hit_record {
    public:
        point3 p;
        vec3 normal;
        double t;
};

//abstract classes, indicated by virtual
class hittable {
    public:
        virtual ~hittable() = default;
        virtual bool hit(const ray&r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif //RAYTRACINGFROMSCRATCH_HITTABLE_H
