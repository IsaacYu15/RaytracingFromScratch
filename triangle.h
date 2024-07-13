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
        vec3 pvec = cross(r.direction(), v0v2);
        vec3 normal = cross(v0v1, v0v2);

        float det = dot(v0v1, pvec);

        //back facing triangle - THIS DOESN'T REALLY WROK
        if (det < 0.0001) return false;

        // ray and triangle are parallel if det is close to 0
        if (fabs(det) < 0.0001) return false;
        float invDet = 1 / det;

        //cramer's rule
        vec3 tvec = r.origin() - vertices[0];
        auto u = dot(tvec,pvec) * invDet;
        if (u < 0 || u > 1) return false;

        vec3 qvec = cross(tvec, v0v1);
        auto v = dot(r.direction(), qvec) * invDet;
        if (v < 0 || u + v > 1) return false;

        auto t = dot(v0v2, qvec) * invDet;

        //ray is behind triangle -HANDLE BACK FACIN TRIANGLES!!
        if (t < 0 || fabs(t) < 0.001) {return false;}

        //update hit record
        rec.t = t;
        rec.p = r.at(rec.t);
        rec.set_face_normal(unit_vector(normal));
        rec.mat = mat;


        return true;
    }

private:
    std::vector<point3>vertices;
    shared_ptr<material> mat;
};

#endif //RAYTRACINGFROMSCRATCH_TRIANGLE_H
