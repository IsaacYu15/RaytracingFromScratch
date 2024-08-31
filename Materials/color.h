#ifndef RAYTRACINGFROMSCRATCH_COLOR_H
#define RAYTRACINGFROMSCRATCH_COLOR_H

#include <array>
#include <valarray>
#include "MathLib/interval.h"
#include "MathLib/vec3.h"

using color = vec3;


inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}


std::array<unsigned char, 3>  write_color(const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000, 0.999);

    auto rbyte = static_cast<unsigned char>(256 * intensity.clamp(r) );
    auto gbyte = static_cast<unsigned char>(256 * intensity.clamp(g) );
    auto bbyte = static_cast<unsigned char>(256 * intensity.clamp(b) );

    return {rbyte, gbyte, bbyte};
}


#endif //RAYTRACINGFROMSCRATCH_COLOR_H