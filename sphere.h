#ifndef RAYTRACINGFROMSCRATCH_SPHERE_H
#define RAYTRACINGFROMSCRATCH_SPHERE_H

#include "hittable.h"

class sphere: public hittable {
public:
    sphere(const point3& center, double radius): center(center), radius(fmax(0, radius)) {}

    bool hit(const ray&r, double ray_tmin, double ray_tmax, hit_record&rec) const override {

        //sphere ray intersection via quadratic formula (# roots  == #intersections)
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = sqrt(discriminant);

        //accepts roots that lie within an acceptable range
        auto root = (h - sqrtd) / a;
        if (root <= ray_tmin || root >= ray_tmax) {
            root = (h + sqrtd) / a;
            if (root <= ray_tmin || root >= ray_tmax)
                return false;
        }

        //point of intersection
        rec.t = root;
        rec.p = r.at(rec.t);

        //determine dir of normal
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

private:
    point3 center;
    double radius;
};

#endif //RAYTRACINGFROMSCRATCH_SPHERE_H
