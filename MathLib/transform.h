#ifndef RAYTRACINGFROMSCRATCH_TRANSFORM_H
#define RAYTRACINGFROMSCRATCH_TRANSFORM_H

#include "vec3.h"

class transform
{
public:
    vec3 position;
    vec3 rotation;
    vec3 scaling;

    transform() : position(0,0,0), rotation(0,0,0), scaling(0,0,0) {};
    transform(vec3 pos, vec3 rot, vec3 scale) : position(pos), rotation(rot), scaling(scale) {};

    void setRotation (point3 r) const
    {

    }

};


#endif //RAYTRACINGFROMSCRATCH_TRANSFORM_H
