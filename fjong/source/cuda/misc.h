#ifndef MISCH
#define MISCH

#include "vec3.h"

class cudamisc {

public:

    static float CUDA_CALLABLE_MEMBER noise(vec3 x )
    {
        vec3 p = vec3(floorf(x.x()),floorf(x.y()),floorf(x.z()));
        vec3 f = vec3(vec3::fract(x.x()),vec3::fract(x.y()),vec3::fract(x.z()));
        vec3 kk = vec3(3.0f,3.0f,3.0f);
        vec3 f1=f*f*kk*2.0f;
        vec3 f2 = f*f*f*2.0f;
        f = f1+f2;
        return vec3::mix(vec3::mix(vec3::mix( vec3::hash(p+vec3(0,0,0)),
                                              vec3::hash(p+vec3(1,0,0)),f.x()),
                                   vec3::mix( vec3::hash(p+vec3(0,1,0)),
                                              vec3::hash(p+vec3(1,1,0)),f.x()),f.y()),
                         vec3::mix(vec3::mix( vec3::hash(p+vec3(0,0,1)),
                                              vec3::hash(p+vec3(1,0,1)),f.x()),
                                   vec3::mix( vec3::hash(p+vec3(0,1,1)),
                                              vec3::hash(p+vec3(1,1,1)),f.x()),f.y()),f.z());


    }



    static CUDA_CALLABLE_MEMBER vec3 getPerlinNormal(vec3 p, vec3 n, vec3 t, vec3 bn, float s, float s2) {
        vec3 p0 = n.normalized()+p;
        vec3 p1 = (n+t*s).normalized()+p;
        vec3 p2 = (n+bn*s).normalized()+p;
        p0 = p0*(1+s*noise(vec3(s2*p0.x(), s2*p0.y(), s2*p0.z())));
        p1 = p1*(1+s*noise(vec3(s2*p1.x(), s2*p1.y(), s2*p1.z())));
        p2 = p2*(1+s*noise(vec3(s2*p2.x(), s2*p2.y(), s2*p2.z())));
        return  vec3::cross(p1-p0,p2-p0).normalized();
    }
};

#endif
