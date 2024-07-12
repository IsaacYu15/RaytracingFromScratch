#ifndef RAYTRACINGFROMSCRATCH_TRIANGLE_H
#define RAYTRACINGFROMSCRATCH_TRIANGLE_H

#include "hittable.h"

class triangle: public hittable {
public:
    triangle(std::vector<point3>&vertices, shared_ptr<material> mat)
            : mat(mat) , vertices(vertices) {}

    bool hit(const ray&r, interval ray_t, hit_record&rec) const override {
        vec3 v0v1 = vertices[1] - vertices[0];
        vec3 v0v2 = vertices[2] - vertices[0];
        vec3 normal = cross(v0v1, v0v2);

        auto D = -dot(normal, vertices[0]);
        auto t = -(D + dot(normal, r.origin())) / dot (normal, r.direction());

        //edge cases - triangle behind ray, ray and triangle parallel
        if (fabs(dot(r.direction(), normal)) < 0.001 || t < 0) {return false;}

        auto p = r.at(t);

        //is the triangle in the ray?
        vec3 edge0 = vertices[1] - vertices[0];
        vec3 p0 = p - vertices[0];

        vec3 edge1 = vertices[2] - vertices[1];
        vec3 p1 = p - vertices[1];

        vec3 edge2 = vertices[0] - vertices[2];
        vec3 p2 = p - vertices[2];

        if (dot(cross(edge0, p0), normal) < 0 ||
            dot(cross(edge1, p1), normal) < 0 ||
            dot(cross(edge2, p2), normal) < 0)
        {
            return false;
        }

        //update hit record
        rec.t = t;
        rec.p = r.at(rec.t);
        rec.normal = normal;
        rec.mat = mat;

//        //determine dir of normal
//        vec3 outward_normal = (rec.p - center) / radius;
//        rec.set_face_normal(r, outward_normal);


        return true;


    }

private:
    std::vector<point3>vertices;
    shared_ptr<material> mat;
};

#endif //RAYTRACINGFROMSCRATCH_TRIANGLE_H
