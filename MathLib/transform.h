#ifndef RAYTRACINGFROMSCRATCH_TRANSFORM_H
#define RAYTRACINGFROMSCRATCH_TRANSFORM_H

#include "matrix.h"

class transform
{
public:
    vec3 position;
    vec3 rotation;
    vec3 scaling;

    matrix rotationalMatrix;
    matrix scalingMatrix;

    transform() : position(0,0,0), rotation(0,0,0), scaling(0,0,0) {};
    transform(vec3 pos, vec3 rot, vec3 scale) {
        position = pos;
        rotation = rot;
        scaling = scale;

        rotationalMatrix = setRotationalMatrix(rotation);
        scalingMatrix = setScalingMatrix(scaling);
    }

    matrix setRotationalMatrix (point3 r) const
    {
        //to unfortunately, radians (enums would be nice here to make it so we can only put in degrees :) )
        double xRot = r[0] * (pi / 180);
        double yRot = r[1] * (pi / 180);
        double zRot = r[2] * (pi / 180);

        //counter-clockwise about the x-axis
        double ccw_x [3][3] =  { {1,     0,         0      },
                                 {0, cos(xRot), -sin(xRot) },
                                 {0, sin(xRot), cos(xRot)  }
                               };

        //counter-clockwise about the y-axis
        double ccw_y [3][3] =  { {cos(yRot), 0, sin(yRot)},
                                 {0,         1,        0 },
                                 {-sin(yRot),0, cos(yRot)}
                                };

        //counter-clockwise about the z-axis
        double ccw_z [3][3] =  { {cos(zRot), -sin(zRot), 0},
                                 {sin(zRot), cos(zRot),  0},
                                 {0,            0,       1}
                                };

        matrix m0 (ccw_x);
        matrix m1 (ccw_y);
        matrix m2 (ccw_z);

        m0 *= m1;
        m0 *= m2;

        return m0;
    }

    matrix setScalingMatrix (point3 s) const {
        double scale [3][3] = { {s.e[0], 0, 0},
                                {0, s.e[1], 0},
                                {0, 0, s.e[2]}
                               };

        matrix s0 (scale);
        return s0;
    }
};


#endif //RAYTRACINGFROMSCRATCH_TRANSFORM_H
