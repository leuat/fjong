#ifndef RAYTRACERTHREAD_H
#define RAYTRACERTHREAD_H

#include <cinifile.h>
#include <QMatrix3x3>
#include <QImage>
#include <QThread>
#include "source/Raytracer/raytracer.h"
#include <QVector3D>
#include "source/cuda/world.h"

void RaytraceImage(int nx, int ny, int* img, World* w);

class RaytracerThread  : public QThread
{
    Q_OBJECT
public:
    RaytracerThread();

    void Initialize();

    QImage m_img;
    int* img = nullptr;
    bool m_rendering = true;
    float m_time=0;
//    CIniFile m_file;
    int m_elapsedTime;
    QElapsedTimer m_timer;
    bool m_abort = false;
    RayTracer m_rt;

    void run() override;
    void Init();
    void PerformCUDA();
    void PerformCPU();
signals:
    void SignalImageUpdate();
};

#endif // DEMOEFFECTRAYTRACER_H
