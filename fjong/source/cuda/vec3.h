#ifndef VEC3H
#define VEC3H



#ifdef __CUDACC__
#define CUDA_CALLABLE_MEMBER __host__ __device__
#else
#define CUDA_CALLABLE_MEMBER
#endif


#ifdef __CUDACC__
#define CUDA_CALLABLE_DEVICE  __device__
#else
#define CUDA_CALLABLE_DEVICE
#endif

#include <math.h>
#include <stdlib.h>
#include <iostream>




class vec3  {





public:

#ifndef __CUDACC__
    float __saturatef(const float& f) {
        return f;
    }
#endif

    float e[3];



    CUDA_CALLABLE_MEMBER vec3() {}
       CUDA_CALLABLE_MEMBER vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
       CUDA_CALLABLE_MEMBER inline float x() const { return e[0]; }
       CUDA_CALLABLE_MEMBER inline float y() const { return e[1]; }
       CUDA_CALLABLE_MEMBER inline float z() const { return e[2]; }
       CUDA_CALLABLE_MEMBER inline float r() const { return e[0]; }
       CUDA_CALLABLE_MEMBER inline float g() const { return e[1]; }
       CUDA_CALLABLE_MEMBER inline float b() const { return e[2]; }

       CUDA_CALLABLE_MEMBER inline const vec3& operator+() const { return *this; }
       CUDA_CALLABLE_MEMBER inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
       CUDA_CALLABLE_MEMBER inline float operator[](int i) const { return e[i]; }
       CUDA_CALLABLE_MEMBER inline float& operator[](int i) { return e[i]; };

       CUDA_CALLABLE_MEMBER inline vec3& operator+=(const vec3 &v2);
       CUDA_CALLABLE_MEMBER inline vec3& operator-=(const vec3 &v2);
       CUDA_CALLABLE_MEMBER inline vec3& operator*=(const vec3 &v2);
       CUDA_CALLABLE_MEMBER inline vec3& operator/=(const vec3 &v2);
       CUDA_CALLABLE_MEMBER inline vec3& operator*=(const float t);
       CUDA_CALLABLE_MEMBER inline vec3& operator/=(const float t);
       CUDA_CALLABLE_MEMBER inline vec3& operator=(const vec3 &v2);


       CUDA_CALLABLE_MEMBER inline float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
       CUDA_CALLABLE_MEMBER inline float squared_length() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
       CUDA_CALLABLE_MEMBER inline vec3 normalized() const { float l = length(); return vec3(e[0]/l, e[1]/l,e[2]/l); }
       CUDA_CALLABLE_MEMBER inline void make_unit_vector();


       static float CUDA_CALLABLE_MEMBER fract(const float x)
       {
           return x - trunc(x);
       }

    CUDA_CALLABLE_DEVICE inline vec3 clamp() {
        return vec3(__saturatef(e[0]),__saturatef(e[1]),__saturatef(e[2]));
    }


    CUDA_CALLABLE_MEMBER inline vec3 absolute() {
        return vec3(abs(e[0]),abs(e[1]),abs(e[2]));
    }


    static float CUDA_CALLABLE_MEMBER hash(vec3 p)  // replace this by something better
    {
        p  = vec3(fract( p.x()*0.3183099 ),fract( p.y()*0.3183099 ),fract( p.z()*0.3183099 ));
        p *= 17.0;
        return fract( p.x()*p.y()*p.z()*(p.x()+p.y()+p.z()) );
    }

    static float mix(const float& a, const float& b, const float& t) {
        return b*t + (1-t*a);
    }

    static CUDA_CALLABLE_MEMBER inline vec3 cross(const vec3 &v1, const vec3 &v2) {
        return vec3( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
                    (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
                    (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
    }



};



inline std::istream& operator>>(std::istream &is, vec3 &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const vec3 &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

CUDA_CALLABLE_MEMBER inline void vec3::make_unit_vector() {
    float k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}


CUDA_CALLABLE_MEMBER inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

CUDA_CALLABLE_MEMBER inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

CUDA_CALLABLE_MEMBER inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

CUDA_CALLABLE_MEMBER inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

CUDA_CALLABLE_MEMBER inline vec3 operator*(float t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

CUDA_CALLABLE_MEMBER inline vec3 operator/(vec3 v, float t) {
    return vec3(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}

CUDA_CALLABLE_MEMBER inline vec3 operator*(const vec3 &v, float t) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

CUDA_CALLABLE_MEMBER inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1.e[0] *v2.e[0] + v1.e[1] *v2.e[1]  + v1.e[2] *v2.e[2];
}




CUDA_CALLABLE_MEMBER inline vec3& vec3::operator+=(const vec3 &v){
    e[0]  += v.e[0];
    e[1]  += v.e[1];
    e[2]  += v.e[2];
    return *this;
}

/*vec3 &vec3::operator+(const vec3 &v2)
{
    e[0]+=v2.e[0];
    e[1]+=v2.e[1];
    e[2]+=v2.e[2];
    return *this;
//    return vec3(x()+v2.x(),y()+v2.y(),z()+v2.z());

}
*/
/*vec3 &vec3::operator-(const vec3 &v2)
{
    e[0]-=v2.e[0];
    e[1]-=v2.e[1];
    e[2]-=v2.e[2];
    return *this;

}
*/
/*vec3 vec3::operator+(const vec3 &v2)
{
    return vec3(x()+v2.x(),y()+v2.y(),z()+v2.z());
}
*/
CUDA_CALLABLE_MEMBER inline vec3& vec3::operator*=(const vec3 &v){
    e[0]  *= v.e[0];
    e[1]  *= v.e[1];
    e[2]  *= v.e[2];
    return *this;
}

CUDA_CALLABLE_MEMBER inline vec3& vec3::operator/=(const vec3 &v){
    e[0]  /= v.e[0];
    e[1]  /= v.e[1];
    e[2]  /= v.e[2];
    return *this;
}

/*vec3 vec3::operator*(const vec3 t)
{
    return vec3(e[0]*t.e[0],e[1]*t.e[1],e[2]*t.e[2]);
}
*/
CUDA_CALLABLE_MEMBER inline vec3& vec3::operator-=(const vec3& v) {
    e[0]  -= v.e[0];
    e[1]  -= v.e[1];
    e[2]  -= v.e[2];
    return *this;
}

CUDA_CALLABLE_MEMBER inline vec3& vec3::operator*=(const float t) {
    e[0]  *= t;
    e[1]  *= t;
    e[2]  *= t;
    return *this;
}

CUDA_CALLABLE_MEMBER inline vec3& vec3::operator/=(const float t) {
    float k = 1.0/t;

    e[0]  *= k;
    e[1]  *= k;
    e[2]  *= k;
    return *this;
}

CUDA_CALLABLE_MEMBER inline vec3 &vec3::operator=(const vec3 &v2) {
    e[0]=v2.e[0];
    e[1]=v2.e[1];
    e[2]=v2.e[2];
    return *this;
}

/*vec3& vec3::operator*(float t)
{
    e[0]=e[0]*t;
    e[1]=e[1]*t;
    e[2]=e[2]*t;
    return *this;
}
*/
CUDA_CALLABLE_MEMBER inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

CUDA_CALLABLE_MEMBER inline vec3 maxx(const vec3& v0, const vec3& v1) {
    return vec3(fmaxf(v0.x(), v1.x()), fmaxf(v0.y(), v1.y()),fmaxf(v0.z(), v1.z()));
;}



#endif
