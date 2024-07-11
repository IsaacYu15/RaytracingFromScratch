#ifndef RAYTRACINGFROMSCRATCH_HITTABLE_H
#define RAYTRACINGFROMSCRATCH_HITTABLE_H

class material;

class hit_record {
    public:
        point3 p;
        vec3 normal;
        shared_ptr<material> mat;
        double t;
        bool front_face;

        void set_face_normal(const ray&r, const vec3& outward_normal)
        {
            //negative => ray is outside the sphere
            //positive => ray is inside the sphere
            //ray and normal should be opposite
            front_face = dot(r.direction(), outward_normal) < 0;

            if (front_face)
            {
                normal = outward_normal;
            } else
            {
                normal = -outward_normal;
            }

        }
};

//abstract classes, indicated by virtual
class hittable {
    public:
        virtual ~hittable() = default;
        virtual bool hit(const ray&r, interval ray_t, hit_record& rec) const = 0;
};

#endif //RAYTRACINGFROMSCRATCH_HITTABLE_H
