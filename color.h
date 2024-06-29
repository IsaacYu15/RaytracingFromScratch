#ifndef RAYTRACINGFROMSCRATCH_COLOR_H
#define RAYTRACINGFROMSCRATCH_COLOR_H

#include "interval.h"
#include "vec3.h"

using color = vec3;

void write_color(std::ostream&out, const color&pixel_color) {


    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to color range[0,255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}



#endif //RAYTRACINGFROMSCRATCH_COLOR_H
