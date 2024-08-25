#ifndef RAYTRACINGFROMSCRATCH_LIGHT_H
#define RAYTRACINGFROMSCRATCH_LIGHT_H

#include "MathLib/mathLibrary.h"

class light {
public:
    virtual ~light() = default;

    virtual bool illuminate(color& current_colour, const point3 pos) const {return false; }

};

class pointLight : public light {
public:
    pointLight (const color& emit, double intensity, const point3&source): emission(emit), intensity(intensity), source(source) {}

    bool illuminate(color& current_colour, const point3 pos) const override
    {
        current_colour += emission * (intensity / distance_squared(pos, source));
        return true;
    }

private:
    color emission;
    point3 source;
    double intensity;

};

#endif //RAYTRACINGFROMSCRATCH_LIGHT_H
