#ifndef RAYH
#define RAYH
#include "vec3.h"

class ray
{
    public:
        CUDA_CALLABLE_MEMBER ray() {}
        CUDA_CALLABLE_MEMBER ray(const vec3& a, const vec3& b) { org = a; dir = b; }
        CUDA_CALLABLE_MEMBER vec3 origin() const       { return org; }
        CUDA_CALLABLE_MEMBER vec3 direction() const    { return dir; }
        CUDA_CALLABLE_MEMBER vec3 point_at_parameter(float t) const { return org + t*dir; }

        vec3 org, curPos;
        vec3 dir;
        vec3 intensity;
        int reflect;
};

#endif
