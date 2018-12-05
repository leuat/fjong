#ifndef MARCHOBJECTH
#define MARCHOBJECTH
#include "vec3.h"
#include "ray.h"

#define MO_SPHERE 0
#define MO_CUBE 1

class World;

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
        CUDA_CALLABLE_MEMBER virtual float intersect(const vec3& p) {
            return 1;
        }

        vec3 pos, n, mat_color;
        vec3 p1,p2;
        float reflectivity = 0.5;
        float type;

/*        static vec3 e;

        static vec3 exyy;
        static vec3 eyyx;
        static vec3 eyxy;
        static vec3 exxx;
*/
/*
        static CUDA_CALLABLE_MEMBER  void Init() {
            e = vec3(1.0,-1.0,0)*0.1773*0.05;

            exyy = vec3(e.x(), e.y(),e.y());
            eyyx = vec3(e.y(), e.y(),e.x());
            eyxy = vec3(e.y(), e.x(),e.y());
            exxx = vec3(e.x(), e.x(),e.x());

        }
*/

/*
        static CUDA_CALLABLE_MEMBER float intersectSphere(const ray& r, const vec3& p, const float rad) {
            return (p+r.curPos).length() - rad;

        }

*/
        vec3 CUDA_CALLABLE_MEMBER CalcMarchNormal(const vec3 &pos)
        {

            vec3 e = vec3(1.0,-1.0,0)*0.1773*0.05;

            vec3 exyy = vec3(e.x(), e.y(),e.y());
            vec3 eyyx = vec3(e.y(), e.y(),e.x());
            vec3 eyxy = vec3(e.y(), e.x(),e.y());
            vec3 exxx = vec3(e.x(), e.x(),e.x());

        //    vec2( sdSphere(    pos-vec3( 0.0,0.25, 0.0), 0.25 ), 46.9 ) );
        //    if (rand()%100==0) qDebug() << "POS: " << pos;
            return ( exyy*intersect( pos + exyy ) +
                              eyyx*intersect( pos + eyyx ) +
                              eyxy*intersect( pos + eyxy ) +
                              exxx*intersect( pos + exxx ) ).normalized();

        }

//        void CUDA_CALLABLE_MEMBER  CalculateLight(Ray* ray, vec3& normal, vec3& tangent, vec3& isp, RayTracerGlobals &globals,vec3 reflectDir, QVector<AbstractRayObject*>& objects, int pass)
        void CUDA_CALLABLE_DEVICE  CalculateLight(ray* ray, vec3& normal, vec3& tangent, vec3& isp, World *world,vec3 reflectDir, marchobject** objects, int pass)
        {

            //double l = (ray->m_origin-(isp+m_position)).length();

            //if (l<ray->m_z)
            {

//                vec3 col = m_material.m_color;
                //vec3 col = m_material.m_color;
                vec3 col = mat_color;//   vec3(0.4, 0.6, 1.0);
                vec3 light = vec3(1,1,1).normalized();
                if (pass==0)
                {
                    //m_material.m_hasTexture=false;
/*                    if (m_material.m_hasTexture) {
                        vec3 uv = CalculateUV(isp, normal, tangent);

                        float lvl = pow(0.1*l,0.9);
                        QImage* img = m_material.m_texture.get(lvl);
                        float uu = abs((int)(uv.x()*(float)img->width())%img->width());
                        float vv = abs((int)(uv.y()*(float)img->height())%img->height());
                        QColor org = img->pixel(uu,vv);
                        vec3 c = Util::fromColor(org)/256.;
                        col.setX(col.x()*c.x());
                        col.setY(col.y()*c.y());
                        col.setZ(col.z()*c.z());
                    }*/
        //            col.setX(1);
/*                    ray->intensity = col*ApplyDirectionalLight(normal,globals) + globals.m_ambient;

                    ray->m_intensity += ApplySpecularLight(normal,ray->m_direction,  globals, m_material);*/
                    ray->intensity = (col*(dot(normal, light))).clamp();


                    vec3 H = ((light-ray->dir.normalized())).normalized();
                    ray->intensity+=  vec3(1,1,1)*max(pow(dot(H,normal),100),0.0f);

//                    ray->intensity += ApplySpecularLight(normal,ray->m_direction,  globals, m_material);
                }
//                ray->m_z = l;
            }

        }


};

class mo_sphere : public marchobject {
public:

    CUDA_CALLABLE_MEMBER virtual float intersect(const ray& r) {
        return (pos+r.curPos).length() - p1.x();

    }
    CUDA_CALLABLE_MEMBER virtual float intersect(const vec3& p) {
        return (pos+p).length() - p1.x();

    }
};

class mo_plane : public marchobject {
public:

    CUDA_CALLABLE_MEMBER virtual float intersect(const ray& r) {
        return (-pos.y() + r.curPos.y());

    }
    CUDA_CALLABLE_MEMBER virtual float intersect(const vec3& p) {
        return (-pos.y() + p.y());

    }
};

#endif
