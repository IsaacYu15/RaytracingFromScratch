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

        if (det < 0.0001) return false;

        // ray and triangle are parallel if det is close to 0
        if (fabs(det) < epsilon) return false;
        float invDet = 1 / det;

        //cramer's rule
        vec3 tvec = r.origin() - vertices[0];
        auto u = dot(tvec,pvec) * invDet;
        if (u < 0 || u > 1) return false;

        vec3 qvec = cross(tvec, v0v1);
        auto v = dot(r.direction(), qvec) * invDet;
        if (v < 0 || u + v > 1) return false;

        auto t = dot(v0v2, qvec) * invDet;

        //solution is too close / triangle is behind the camera
        if (t < 0 || fabs(t) < epsilon) return false;

        //front facing or back facing
        if (det > 0) {
            // Front-facing triangle
            rec.front_face = true;
            rec.set_face_normal(r, unit_vector(normal));
        } else {
            // Back-facing triangle
            rec.front_face = false;
            rec.set_face_normal(r, -unit_vector(normal));
        }

        //update hit record
        rec.t = t;
        rec.p = r.at(rec.t);
        rec.mat = mat;


        return true;
    }

private:
    std::vector<point3>vertices;
    shared_ptr<material> mat;
};

#endif //RAYTRACINGFROMSCRATCH_TRIANGLE_H
