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
using std::fabs;

//constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//utility functions
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

//common headers
#include "Materials/color.h"
#include "RayGeneration/ray.h"
#include "vec3.h"
#include "interval.h"

#include <vector>

#endif //RAYTRACINGFROMSCRATCH_MATHLIBRARY_H
