#ifndef RAYH
#define RAYH
#include "vec3.h"
#include "mat3.h"

class ray
{
    public:
        CUDA_CALLABLE_MEMBER ray() {intensity=vec3(0,0,0);reflect=1;}
        CUDA_CALLABLE_MEMBER ray(const vec3& a, const vec3& b) { org = a; dir = b; }
        CUDA_CALLABLE_MEMBER vec3 origin() const       { return org; }
        CUDA_CALLABLE_MEMBER vec3 direction() const    { return dir; }
        CUDA_CALLABLE_MEMBER vec3 point_at_parameter(float t) const { return org + t*dir; }

        vec3 org, curPos;
        vec3 dir;
        vec3 intensity = vec3(0,0,0);
        int reflect=3;


        CUDA_CALLABLE_MEMBER ray Rotate(mat3& rotmat, const vec3& pos) {
            ray r(rotmat*(curPos+pos)-pos,rotmat*dir);
            r.curPos = r.org;
            return r;
        }


        bool CUDA_CALLABLE_MEMBER  IntersectSphere(const vec3& pos, vec3 r, vec3 &isp1, vec3 &isp2, float &t0, float &t1) {

            vec3 o = org-pos;
            vec3 d = dir;

            r = vec3(1.0/(r.x()*r.x()),1.0/(r.y()*r.y()),1.0/(r.z()*r.z()));


            vec3 rD = vec3(d.x()*r.x(), d.y()*r.y(), d.z()*r.z());
            vec3 rO = vec3(o.x()*r.x(), o.y()*r.y(), o.z()*r.z());


            float A = dot(d,rD);
            float B = 2.0*(dot(d, rO));
            float C = dot(o, rO) - 1.0;

            float S = (B*B - 4.0*A*C);

            if (S<=0) {
                isp1 = vec3(0,0,0);
                isp2 = vec3(0,0,0);
                t0 = 0;
                t1 = 0;
                return false;
            }

            t0 =  (-B - sqrt(S))/(2.0*A);
            t1 =  (-B + sqrt(S))/(2.0*A);

            isp1 = o+d*t0;
            isp2 = o+d*t1;

            return true;
        }


};

#endif
