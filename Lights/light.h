#ifndef RAYTRACINGFROMSCRATCH_LIGHT_H
#define RAYTRACINGFROMSCRATCH_LIGHT_H

#include "MathLib/mathLibrary.h"

class light {
public:
    virtual ~light() = default;

    virtual bool illuminate(color& current_colour, const point3 pos) const {return false; }

    point3 getSource() {return source; }

protected:
    point3 source;
    color emission;
    double intensity;

};

class pointLight : public light {
public:
    pointLight (const color& emit, double inten, const point3&src) {
        source = src;
        emission = emit;
        intensity = inten;
    }

    bool illuminate(color& current_colour, const point3 pos) const override
    {
        current_colour += emission * (intensity / distance_squared(pos, source));
        return true;
    }



};

class spotLight: public light{
public:
    spotLight (const color& emit, double inten, const point3&src, const point3&lookAt, double width, double fallOff):
    angle_width(width), angle_fallOff(fallOff)
    {
        angle_width = std::max(angle_width, angle_fallOff);
        angle_width = degrees_to_radians(angle_width);
        angle_fallOff = degrees_to_radians(angle_fallOff);

        source = src;
        emission = emit;
        intensity = inten;

        direction = (lookAt - source);
        direction /= direction.length();
    }

    bool illuminate(color& current_colour, const point3 pos) const override
    {
        vec3 pointToSource = pos - source;
        pointToSource /= pointToSource.length();

        //as we normalized the vectors
        double cosTheta = acos(dot(pointToSource, direction));

        //outside of cone
        if (cosTheta > angle_width)
            return false;

        //point is within cone recieving full illumination
        if (cosTheta < angle_fallOff)
            current_colour += emission * (intensity / distance_squared(pos, source));

        //points in cone that are in fall off angle
        double delta = (cosTheta - angle_width) / (angle_fallOff - angle_width);

        current_colour += (delta * delta) * (delta * delta) * emission;

        return true;
    }

private:
    vec3 direction;

    double angle_width;
    double angle_fallOff;
};

#endif //RAYTRACINGFROMSCRATCH_LIGHT_H
