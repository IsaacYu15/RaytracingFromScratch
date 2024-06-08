#ifndef RAYTRACINGFROMSCRATCH_COLOR_H
#define RAYTRACINGFROMSCRATCH_COLOR_H

#include "vec3.h"

using color = vec3;

void write_color(std::ostream&out, const color&pixel_color) {


    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to color range[0,255]
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}



#endif //RAYTRACINGFROMSCRATCH_COLOR_H