#ifndef MAT3H
#define MAT3H


#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "vec3.h"

class mat3  {
public:

    CUDA_CALLABLE_MEMBER mat3() {}
    CUDA_CALLABLE_MEMBER mat3(const vec3& e0, const vec3& e1, const vec3& e2) {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }


/*    x x x   a
    y y y   b    =
    z z z   c
*/
    CUDA_CALLABLE_MEMBER inline vec3 operator*(const vec3 &v2) {
        return vec3(dot(e[0],v2),dot(e[1],v2),dot(e[2],v2));
    }

    vec3 e[3];
};




#endif
