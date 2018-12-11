#ifndef MARCHOBJECTH
#define MARCHOBJECTH
#include "misc.h"
#include "vec3.h"
#include "mat3.h"
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
        CUDA_CALLABLE_MEMBER virtual float intersect(const vec3& p) {
            return 1;
        }

        vec3 pos, n, mat_color;
        vec3 p1,p2;
        float perlinness = 0.4;
        float perlinScale = 3.23;
        float bbRadius = 1;
        float reflectivity = 0.5;
        float glossiness = 1;
        float type;
        mat3 rotMat;
        mat3 rotMatInv;

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
        void CUDA_CALLABLE_DEVICE  CalculateLight(ray* ray, vec3& normal, vec3& tangent, vec3& isp, const vec3& light,vec3 reflectDir, marchobject** objects, int pass)
        {

            //double l = (ray->m_origin-(isp+m_position)).length();

            //if (l<ray->m_z)
            {

//                vec3 col = m_material.m_color;
                //vec3 col = m_material.m_color;
                vec3 col = mat_color;//   vec3(0.4, 0.6, 1.0);
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
#ifndef IGNORE_CUDA
                    ray->intensity+=  vec3(1,1,1)*max(pow(dot(H,normal),100),0.0f);
#endif

//                    ray->intensity += ApplySpecularLight(normal,ray->m_direction,  globals, m_material);
                }
//                ray->m_z = l;
            }

        }


         CUDA_CALLABLE_MEMBER vec3 GetPerturbedNormal(vec3 pos, vec3 normal, vec3 tangent)
        {
            if (perlinness==0)
                return normal;
        //    return normal;

                vec3 bn = vec3::cross(tangent, normal).normalized();
                vec3 perlin = cudamisc::getPerlinNormal(pos,normal,tangent,bn,1,perlinScale);
                vec3 dir = (1-perlinness)*normal + (perlinness)*perlin;
                normal = (dir + normal).normalized();

            return normal;
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

class mo_box : public marchobject {
public:
    vec3 box;
    CUDA_CALLABLE_MEMBER virtual float intersect(const ray& r) {
        return intersect(r.curPos);
    }
    CUDA_CALLABLE_MEMBER virtual float intersect(const vec3& p) {
        vec3 d = (pos+p).absolute() - box;// +ray->m_currentPos;
        float r = 0.0;
        return fminf(fmaxf(d.x()-r,fmaxf(d.y()-r,d.z()-r)),0.0f) + maxx(d,vec3(0,0,0)).length();

    }
};

class mo_torus : public marchobject {
public:
    vec3 box;
    CUDA_CALLABLE_MEMBER virtual float intersect(const ray& r) {
        return intersect(r.curPos);
    }
    CUDA_CALLABLE_MEMBER virtual float intersect(const vec3& p) {
        vec3 disp = pos + p;
        vec3 pp = disp;
        pp.e[1] = 0;
        vec3 q = vec3(pp.length()-p1.x(),disp.y(),0);
         return q.length()-p1.y();

    }
};

class mo_cylinder : public marchobject {
public:
    vec3 box;
    CUDA_CALLABLE_MEMBER virtual float intersect(const ray& r) {
        return intersect(r.curPos);
    }
    CUDA_CALLABLE_MEMBER virtual float intersect(const vec3& p) {
        vec3 d = pos+p;
        vec3 pp = vec3(d.x(), d.z(),0);
        vec3 d2 = vec3(pp.length()-2*p1.x() + p1.y()*1.0, abs(d.y())-p1.z(),0);

        return min(max(d2.x(),d2.y()),0.0f) + maxx(d2,vec3(0,0,0)).length() - p1.y();



/*        QVector3D pos = m_position+ray->m_currentPos;
        QVector3D p = QVector3D(pos.x(), pos.z(),0);
        QVector3D d = QVector3D(p.length()-2*m_radius.x() + m_radius.y()*1.0, abs(pos.y())-m_radius.z(),0);

        return min(max(d.x(),d.y()),0.0f) + Util::maxx(d,QVector3D(0,0,0)).length() - m_radius.y();
  */

    }
};

#endif
