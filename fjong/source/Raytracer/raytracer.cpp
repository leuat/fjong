#include "raytracer.h"
#include <omp.h>

RayTracer::RayTracer()
{

}

int cmpfunc (const AbstractRayObject * a, AbstractRayObject * b) {
   return ( a->m_localPos.length() > b->m_localPos.length() );
}


void RayTracer::Raytrace(QImage &img)
{
    m_globals.m_camera = &m_camera;

    for (AbstractRayObject* aro : m_objects)
        aro->m_localPos = aro->m_position-m_camera.m_camera;

//    qSort(m_objects.begin(), m_objects.end(), cmpfunc);

//    QVector<AbstractRayObject*> objs;



//#pragma omp parallel for
    for (int j=0;j<img.height();j++)
        for (int i=0;i<img.width();i++) {
            QVector3D dir = m_camera.coord2ray(i,j,img.width());
            Ray ray(m_camera.m_camera,dir);

            float m_z = 1E20;
            AbstractRayObject *winner = nullptr;
            QVector3D isp;
            for (AbstractRayObject* ro: m_objects) {
                if (ro->RayTrace(&ray, m_globals, isp,1, m_objects)) {
                    if (ray.m_z<m_z) {
                        m_z = ray.m_z;
                        winner = ro;
                    }


                }
            }
            float shadow = 1;

            if (winner!=nullptr) {


                QVector3D tmp;
                ray.m_z = 1E20;
                winner->RayTrace(&ray, m_globals, isp,0, m_objects);

                for (AbstractLight* al: m_globals.m_lights) {
                    Ray shadowRay(isp+winner->m_position,al->m_direction);
                    for (AbstractRayObject* sh: m_objects) {
                        if (sh==winner)
                            continue;
                        if (sh->RayTrace(&shadowRay, m_globals, tmp,1,m_objects)) {
                            shadow*=0.25;
                        }

                    }
                }

            }
            else
                m_globals.Sky(&ray, m_globals.m_skyScale);

            QColor c = Util::toColor(ray.m_intensity*256*shadow + m_globals.m_ambient);
            //                if (i<img.width() && j<img.height())
            img.setPixel(i,j,c.rgba());





        }
}







void RayTracer::Raymarch(QImage &img)
{
    m_globals.m_camera = &m_camera;

    for (AbstractRayObject* aro : m_objects)
        aro->m_localPos = aro->m_position-m_camera.m_camera;

//    qSort(m_objects.begin(), m_objects.end(), cmpfunc);

//    QVector<AbstractRayObject*> objs;


       float aspect = img.width()/(float)img.height();
//#pragma omp parallel for
    for (int j=0;j<img.height();j++)
        for (int i=0;i<img.width();i++) {

            float x = i;
            float y = j*aspect;

            QVector3D dir = m_camera.coord2ray(x,y,img.width());
            Ray ray(m_camera.m_camera,dir);
            ray.m_reflect=3;

//            float m_z = 1E20;
            int tid = omp_get_thread_num();

            RayMarchSingle(ray, Image, nullptr,80,tid);
            QColor c = Util::toColor(ray.m_intensity*256 + m_globals.m_ambient);
            img.setPixel(i,j,c.rgba());

        }

}

bool RayTracer::RayMarchSingle(Ray& ray, Pass pass, AbstractRayObject* ignore, int cnt, int tid)
{
    QVector3D isp;
    float shadow = 1;
    float t = 1;
    AbstractRayObject *winner = nullptr;
 //   Ray rotated;
    ray.m_currentPos = ray.m_origin;

    // Generate list with bb
    QVector<AbstractRayObject*> culled;
    for (AbstractRayObject* o: m_objects) {
        QVector3D isp1, isp2;
        double t0, t1;
        if (ray.IntersectSphere(o->m_position*-1,QVector3D(1,1,1)*o->m_bbRadius,isp1, isp2, t0,t1)) {
            //if (( t1>0) || dynamic_cast<RayObjectPlane*>(o)!=nullptr)
                culled.append(o);
        }


    }

//    culled = m_objects;

    if (culled.length()==0) {
        m_globals.Sky(&ray, m_globals.m_skyScale);
        return false;
    }

    for (AbstractRayObject* ro: culled) {
        ro->m_localRay[tid] = ray.Rotate(ro->m_rotmat, ro->m_position);
    }


    for (int i=0;i<cnt;i++) {
        float precis = 0.004*t;
        float keep=1000.0;
        ray.m_curStep =t; //(ray.m_origin-m_objects[j]->m_position).length();
        ray.setCurrent(t);




        //rotated = ray;
        AbstractRayObject* w= nullptr;
        for (AbstractRayObject* ro: culled) {

            if (ro==ignore)
                continue;
            ro->m_localRay[tid].setCurrent(t);

            float keep2 =ro->intersect(&ro->m_localRay[tid]);
            if (keep2<keep) {
                keep = keep2;
                w = ro;

            }

            if (keep2<precis) {
                winner = w;
                i=cnt;
                if (pass==Shadow)
                    return true;
                break;

            }
        }
            t=t+keep;

    }



    if (winner!=nullptr) {
        Ray rotated = winner->m_localRay[tid];//ray.Rotate(winner->m_rotmat, winner->m_position);

        //ray.m_currentPos = isp;
//                exit(1);
        isp = rotated.m_currentPos;
        QVector3D normal = winner->CalcMarchNormal(rotated.m_currentPos);
        normal = winner->m_rotmatInv*normal;
        QVector3D tt(1,2,-213.123);
        QVector3D tangent = QVector3D::crossProduct(tt,normal).normalized();
        QVector3D bi = QVector3D::crossProduct(tangent,normal).normalized();

        normal  = winner->GetPerturbedNormal(isp,normal,tangent,m_globals);


//        ray.m_reflect = 0;
        QVector3D reflectionDir = ray.m_direction-2*QVector3D::dotProduct(ray.m_direction, normal)*normal;
        QVector3D lp = ray.m_currentPos;//-winner->m_localPos;
 //       ray.m_z=10000;
        winner->CalculateLight(&ray,normal,tangent,lp,m_globals,reflectionDir,m_objects,0);


        if (winner->m_material.m_reflectivity>0 && ray.m_reflect>0) {
            Ray nxt(lp,reflectionDir);
            nxt.m_reflect=ray.m_reflect-1;
            RayMarchSingle(nxt, Reflect, winner,24,tid);

            ray.m_intensity = ray.m_intensity*(1-winner->m_material.m_reflectivity) + winner->m_material.m_reflectivity*nxt.m_intensity;
        }

//        if (pass==Image)

        for (AbstractLight* al: m_globals.m_lights) {
//            Ray shadowRay(isp,winner->m_rotmat*al->m_direction*1);
            Ray shadowRay(lp,al->m_direction);
            AbstractRayObject* o= nullptr;
            if (dynamic_cast<RayObjectBox*>(winner)!=nullptr)
                o=winner;
            if (RayMarchSingle(shadowRay, Shadow, o,14,tid)) {
                shadow*=0.5;
            }

        }

        ray.m_intensity*=shadow;


        return true;

    }
    m_globals.Sky(&ray,m_globals.m_skyScale);

    return false;
}
World RayTracer::toWorld()
{
    World w;
    int cnt = 0;
    QVector<marchobject*> objs;
    for (AbstractRayObject* o: m_objects) {
        RayObjectSphere* s = dynamic_cast<RayObjectSphere*>(o);
        if (s!=nullptr) {
            marchobject* mo = new mo_sphere();
            mo->pos = vec3(s->m_position.x(),s->m_position.y(),s->m_position.z());
            mo->p1 = vec3(s->m_radius.x(),s->m_radius.y(),s->m_radius.z());
            mo->type = 0;
            objs.append(mo);
        }
    }
    w.length = objs.count();
    w.objects = new marchobject[w.length];
    for (int i=0;i<w.length;i++)
        w.objects[i] = *objs[i];
    return w;
}
