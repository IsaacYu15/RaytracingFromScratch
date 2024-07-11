#ifndef RAYTRACINGFROMSCRATCH_TRIANGLE_H
#define RAYTRACINGFROMSCRATCH_TRIANGLE_H

#include "hittable.h"

class triangle: public hittable {
public:
    triangle(std::vector<point3>&vertices, shared_ptr<material> mat)
            : mat(mat) , vertices(vertices) {}

    bool hit(const ray&r, interval ray_t, hit_record&rec) const override {
        //triangle ray intersection
    }

private:
    std::vector<point3>vertices;
    shared_ptr<material> mat;
};

#endif //RAYTRACINGFROMSCRATCH_TRIANGLE_H
