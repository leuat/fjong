#ifndef WORLDH
#define WORLDH

#include "vec3.h"
#include "marchobject.h"

class World  {


public:
//    __host__ __device__


    marchobject* objects;

    vec3 lower_left_corner,horizontal, vertical, origin;
    vec3 light0 = vec3(1,1,1).normalized();
    float skyScale = 0.9;
    int rnd[1024];

    void CUDA_CALLABLE_DEVICE sky(ray &r) {
      float sun = min(max(dot(light0,r.dir), 0.0f), 1.0f );
      vec3 col = vec3(0.6,0.71,0.75) - r.dir.y()*0.1*vec3(1.0,0.5,1.0) + 0.05*0.5*vec3(1,1,1);
      col += 1.0*vec3(1.0,.6,0.1)*pow( sun, 20.0f );


      r.intensity = col*skyScale;

    }


    int length;

};

#endif
