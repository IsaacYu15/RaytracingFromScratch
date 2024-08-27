#ifndef RAYTRACINGFROMSCRATCH_HITTABLE_LIST_H
#define RAYTRACINGFROMSCRATCH_HITTABLE_LIST_H

#include "hittable.h"
#include "Lights/light.h"

#include <vector>

class hittable_list : public hittable {

public:
    std::vector<shared_ptr<hittable>> objects;
    std::vector<shared_ptr<light>> lights;

    hittable_list() {};
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear()
    {
        objects.clear();
        lights.clear();
    }

    void add(shared_ptr<hittable>object)
    {
        objects.push_back(object);
    }

    void add(shared_ptr<light>object)
    {
        lights.push_back(object);
    }

    bool hit (const ray & r, interval ray_t, hit_record&rec ) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object: objects)
        {
            if (object->hit (r, interval(ray_t.min, closest_so_far), temp_rec) && temp_rec.t < closest_so_far)
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    color shouldIlluminate(const point3 pos) const
    {
        color illumination(0,0,0);

        for (const auto& light: lights)
        {
            //randomize light source position to simulate how light in reality, doesn't come from directly one point
            auto dir = light->getSource() + point3( (random_double()  - 0.5),
                                                    (random_double()  - 0.5),
                                                    (random_double()  - 0.5)) - pos;

            ray r (pos, dir);
            hit_record rec;

            if(!hit(r, interval(0.001, infinity), rec))
            {
                light->illuminate(illumination, pos);
            }
        }


        return illumination;
    }

};

#endif //RAYTRACINGFROMSCRATCH_HITTABLE_LIST_H
