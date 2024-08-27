#ifndef RAYTRACINGFROMSCRATCH_COLOR_H
#define RAYTRACINGFROMSCRATCH_COLOR_H

#include "MathLib/interval.h"
#include "MathLib/vec3.h"

using color = vec3;


inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}


void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000, 0.999);

    int rbyte = int(256 * intensity.clamp(r) );
    int gbyte = int(256 * intensity.clamp(g) );
    int bbyte = int(256 * intensity.clamp(b) );

//    if (r > 1 || g > 1 || b > 1)
//    {
//        double scalingFactor = std::max(r, std::max(g,b));
//        r /= scalingFactor;
//        g /= scalingFactor;
//        b /= scalingFactor;
//    }
//
//    int rbyte = int(256 * r);
//    int gbyte = int(256 * g);
//    int bbyte = int(256 * b);

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}


#endif //RAYTRACINGFROMSCRATCH_COLOR_H