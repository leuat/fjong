#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QMatrix3x3>
#include "camera.h"
#include "raytracerglobals.h"
#include "rayobject.h"
#include <QImage>
#include "source/cuda/world.h"



class RayTracer
{
public:
    RayTracer();
    QVector<AbstractRayObject*> m_objects;
    Camera m_camera;
    RayTracerGlobals m_globals;
    enum Pass { Image, Reflect, Shadow};
    void Raytrace(QImage& img);
    void Raymarch(QImage& img);

    bool RayMarchSingle(Ray& ray, Pass pass, AbstractRayObject* ignore, int cnt, int tid);

    World toWorld();

};

#endif // RAYTRACER_H
