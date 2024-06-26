#ifndef RAYTRACINGFROMSCRATCH_MATHLIBRARY_H
#define RAYTRACINGFROMSCRATCH_MATHLIBRARY_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

//common std
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

//constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//utility functions
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

//common headers
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"


#endif //RAYTRACINGFROMSCRATCH_MATHLIBRARY_H
