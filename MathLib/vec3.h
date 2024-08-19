#ifndef RAYTRACINGFROMSCRATCH_VEC3_H
#define RAYTRACINGFROMSCRATCH_VEC3_H

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

        bool near_zero() const {
            auto s = 1e-8;
            return (fabs(e[0]) < s && fabs(e[1]) < s && fabs(e[2]) < s);
        }

        static vec3 random()
        {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max)
        {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
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

inline vec3 random_in_unit_disk()
{
    while(true)
    {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

//this code is kinda weird???
inline vec3 random_in_unit_sphere()
{
    while(true)
    {
        auto p = vec3::random(-1,1);
        if (p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3&normal)
{
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // if < 0, not in same dir as hemisphere normal
    {
        return on_unit_sphere;
    }
    else
    {
        return -on_unit_sphere;
    }
}

inline vec3 reflect(const vec3&v, const vec3& n)
{
    return v - 2*dot(v,n) *n;
}

inline vec3 refract(const vec3& uv, const vec3&n, double etai_opver_etat)
{
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_opver_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif