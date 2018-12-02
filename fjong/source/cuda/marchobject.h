#ifndef MARCHOBJECTH
#define MARCHOBJECTH
#include "vec3.h"
#include "ray.h"

#define MO_SPHERE 0
#define MO_CUBE 1

class marchobject
{
    public:
        CUDA_CALLABLE_MEMBER marchobject() {}
        CUDA_CALLABLE_MEMBER marchobject(const vec3& pp, const vec3& pp1, const vec3& pp2) {
            pos=pp;
            p1=pp1;
            p2=pp2;
        }

        CUDA_CALLABLE_MEMBER virtual float intersect(const ray& p) {
            return 1;
        }

        vec3 pos, n;
        vec3 p1,p2;
        float type;

        static CUDA_CALLABLE_MEMBER float intersectSphere(const ray& r, const vec3& p, const float rad) {
            return (p+r.curPos).length() - rad;

        }


};

class mo_sphere : public marchobject {
public:

    CUDA_CALLABLE_MEMBER virtual float intersect(const ray& r) {
        return (pos+r.curPos).length() - p1.x();

    }
};

#endif
