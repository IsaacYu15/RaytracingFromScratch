#ifndef RAYTRACINGFROMSCRATCH_MATRIX_H
#define RAYTRACINGFROMSCRATCH_MATRIX_H

#include "vec3.h"

class matrix
{
public:
    double m[3][3];

    matrix() : m{ {0,0,0}, {0,0,0}, {0,0,0} } {};

    matrix(double m0[3][3]) {
        *this=m0;
    }

    matrix& operator = (const double m0 [3][3])
    {
        std::copy(&m0[0][0], &m0[0][0] + 3*3, &m[0][0]);
        return *this;
    }

    //3x3 matrix * 3x3 matrix
    matrix& operator *=(const matrix& M)
    {
        matrix res;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j ++)
            {
                double sum = 0.0;

                //perform row col multiplication
                for (int k = 0; k < 3; k ++)
                {
                    sum += m[i][k] * M.m[k][j];
                }

                res.m[i][j] = sum;
            }
        }

        *this=res;
        return *this;
    }

    //3x3 matrix * 3x1 matrix
    vec3 operator * (const vec3& v)
    {
        vec3 res;

        for (int i = 0; i < 3; i++)
        {
            res.e[i] = m[i][0] * v.e[0] + m[i][1] * v.e[1] + m[i][2] * v.e[2];
        }

        return res;
    }

    vec3 getRow (int r)
    {
        return vec3 (m[r][0], m[r][1],m[r][2]);
    }

    vec3 getColumn (int c)
    {
        return vec3 (m[0][c], m[1][c],m[2][c]);
    }

    matrix& operator +=(const matrix& M)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j ++)
            {
                m[i][j] += M.m[i][j];
            }
        }

        return *this;
    }

    matrix& operator -=(const matrix& M)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j ++)
            {
                m[i][j] -= M.m[i][j];
            }
        }

        return *this;
    }


};

#endif //RAYTRACINGFROMSCRATCH_MATRIX_H