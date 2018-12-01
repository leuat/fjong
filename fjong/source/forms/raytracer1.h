#ifndef RAYTRACERTHREAD_H
#define RAYTRACERTHREAD_H

#include <cinifile.h>
#include <QMatrix3x3>
#include <QImage>
#include <QThread>
#include "source/Raytracer/raytracer.h"

class RaytracerThread  : public QThread
{
    Q_OBJECT
public:
    RaytracerThread();
    QImage m_img;
    float m_time;
//    CIniFile m_file;
    int m_elapsedTime;
    QElapsedTimer m_timer;
    bool m_abort = false;
    RayTracer m_rt;

    void run() override;
    void Init();
signals:
    void SignalImageUpdate();
};

#endif // DEMOEFFECTRAYTRACER_H
