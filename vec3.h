#ifndef RAYTRACINGFROMSCRATCH_VEC3_H
#define RAYTRACINGFROMSCRATCH_VEC3_H

#include <cmath>
#include <iostream>

class vec3 {

    public:
        double e[3];

        vec3 () : e{0,0,0} {};
        vec3 (double e0, double e1, double e2): e{e0, e1, e2} {};

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const {return e[i]; } //returns a constant variable that can't be mutated
        double& operator[] (int i) {return e[i]; }     //returns a non-constant variable that can be changed

        vec3& operator +=(const vec3& v){ //returns a reference to this object, we don't want to modify input vector
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this; //dereferencing pointer to the current object
        }

        vec3& operator *=(double t)
        {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator /=(double t)
        {
            return (*this *= 1/t);      //using the overloaded function, *= defined before
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }
};

//using aliases to help us determine which vector we are dealing with in code
using point3 = vec3;

//utility functions
inline std::ostream& operator<<(std::ostream&out, const vec3& v) { //printing out vectors
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+(const vec3&u, const vec3& v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3&u, const vec3& v)
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3&u, const vec3& v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double s, const vec3& v) //for scalars
{
    return vec3(s * v.e[0], s * v.e[1], s * v.e[2]);
}

inline vec3 operator*(const vec3& v, double s)
{
    return s*v; //referencing the previous function (in case users put in s*v or v*s
}

inline vec3 operator/(const vec3&v, double s)
{
    return (1/s)*v;
}

inline double dot(const vec3& u, const vec3& v) {
    return   u.e[0] * v.e[0]
           + u.e[1] * v.e[1]
           + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) { //we need to return a vec3 here as cross products return vectors where as dots return scalars
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3&v) {
    return v / v.length();
}


#endif