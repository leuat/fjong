#include "raytracer1.h"

RaytracerThread::RaytracerThread()
{


//    qDebug() << "whoo";

}

void RaytracerThread::Initialize()
{
    //     m_rt.m_globals.m_lights.append(new DirectionalLight(QVector3D(-1,1,0),QVector3D(1,0.7,0)));
        m_rt.m_globals.m_lights[0]->m_color = QVector3D(1,1,0.7);
    //     SetParameters(0);
        m_img = QImage(1024,768,QImage::Format_ARGB32);
        //m_img = QImage(160,200,QImage::Format_ARGB32);
      //  m_img = QImage(400,300,QImage::Format_ARGB32);
     //   m_img = QImage(1980,1600,QImage::Format_ARGB32);
        m_rt.m_objects.clear();

     /*   m_rt.m_objects.append(new RayObjectSphere(QVector3D(0,0,0),QVector3D(1,1,1), QVector3D(1,0,1),50,0.5));
        m_rt.m_objects.append(new RayObjectSphere(QVector3D(1.5,2,0),QVector3D(1.0,1.0,1.0), QVector3D(0,1,0),100,0.7));
        m_rt.m_objects.append(new RayObjectSphere(QVector3D(-3.3,-3.3,0),QVector3D(2,2,2), QVector3D(0.1,0.2,1),80,0.6));

        */
    //    m_rt.m_objects.append(new RayObjectSphere(QVector3D(0,0,0),QVector3D(2,2,2), QVector3D(1,0.8,0.2),10,0));


        m_rt.m_camera.m_camera = QVector3D(0,0,-5);
        float a = 0.05;
        m_rt.m_globals.m_ambient = QVector3D(a,a,a);

        for (int i=0;i<48; i++) {
            int rw = 8;
            int rx = 30;
            QVector3D pos = QVector3D(rand()%rx-rx/2,rand()%rw-rw/2+1,rand()%rx-rx/2 );
            QVector3D col = QVector3D(rand()%rw,rand()%rw,rand()%rw)/(float)rw;
            int k= rand()%60;
     //       if (i==0) k=10;
       //     if (i==1) k=70;
            float pn = 0.0;
            float ps =3.2;
    //        k=45;
             //k=35;
            float ref = rand()%100/100.0;
    //        k=45;
           if (k>=0 && k<20)
                m_rt.m_objects.append(new RayObjectBox(pos,QVector3D(1,1,1), Material(col,rand()%100,ref, pn,ps,"")));
            else if
               (k>=20 && k<=40)
                m_rt.m_objects.append(new RayObjectSphere(pos,QVector3D(1,1,1), Material(col,rand()%100,ref, pn,ps,"")));
           else if
              (k>=40 && k<=60)
               m_rt.m_objects.append(new RayObjectTorus(pos,QVector3D(1,0.3,0), QVector3D(0,1,0),Material(col,rand()%100,ref, pn,ps,"")));
            else
               m_rt.m_objects.append(new RayObjectCylinder(pos,QVector3D(0.3,0.1,2), Material(col,rand()%100,ref, pn,ps,"")));

        }

    //    m_rt.m_objects.append(new RayObjectTorus(QVector3D(0,0,0),QVector3D(2,1,0), QVector3D(0,1,0), Material(col,rand()%150+100,0.4, 0.0f,19.123,"")));
    //    m_rt.m_objects.append(new RayObjectPlane(QVector3D(0,-4,0),QVector3D(0,1,0),
      //                                           Material(QVector3D(0.9, 0.7, 0.5),30,0.5,0.0f,3.0, "brick1.jpg")));

        m_rt.m_objects.append(new RayObjectPlane(QVector3D(0,-5,0),QVector3D(0,1,0),
                                                 Material(QVector3D(0.9, 0.7, 0.5),30,0.5,0.0f,3.0, "brick1_256.png")));

  /*      m_rt.m_objects.append(new RayObjectSphere(QVector3D(0,1,0),QVector3D(4,4,4),
                                                 Material(QVector3D(0.9, 0.7, 0.5),30,0.5,0.0f,3.0, "")));
*/
/*        m_rt.m_objects.append(new RayObjectPlane(QVector3D(0,5,0),QVector3D(0,1,0),
                                                 Material(QVector3D(0.9, 0.7, 0.5),30,0.5,0.0f,3.0, "brick1_256.png")));

*/

}



void RaytracerThread::run()
{
    img = new int[m_img.width()*m_img.height()*3];
    for (int i=0;i<m_img.width()*m_img.height()*3;i++)
        img[i]=rand()%255;

    while (!m_abort) {
        m_time+=1;
        // m_mc->Clear();
        if (!m_rendering)
            emit SignalImageUpdate();
        this->msleep(20);
    }

    delete img;
}


void RaytracerThread::Init()
{
   float t = m_time/60;
    static_cast<DirectionalLight*>(m_rt.m_globals.m_lights[0])->m_direction =
            QVector3D(sin(t),1,-cos(t*1.23123)).normalized();

    t = m_time/91.32;
    float r= 11;
//    m_rt.m_camera.m_camera =
  //          QVector3D(sin(t)*r,200.2,cos(t)*r);

    int i=0;
    t = m_time/3.53;
//    qDebug() << "t" << m_time;
/*    m_rt.m_objects[0]->SetRotation(QVector3D(i+t,-2*t+2*i, 3.12*i+t));
    m_rt.m_objects[1]->SetRotation(QVector3D(i+t,2*t+2*i, 3.12*i+t*2));*/
    for (AbstractRayObject* a: m_rt.m_objects) {
        if (i!=m_rt.m_objects.count()-1)
        a->SetRotation(QVector3D(i*63.1345+t,-2.13*t+2.134*i, 3.12*i+t));
        i++;
    }


    QVector3D rot = QVector3D(0,1,0);
    //m_rt.m_camera.setRotation(rot);
    m_rt.m_camera.m_rotMatrix.setToIdentity();
    //m_rt.m_camera.m_camera = QVector3D(-10,0,10);
    t=m_time/110.0;
    float rad = 14 + sin(m_time/71.23)*4;
    m_rt.m_camera.m_camera = QVector3D(rad*sin(t),4,rad*cos(t));

    m_rt.m_camera.m_target = QVector3D(0,sin(t)*3-2,0);
    m_rt.m_camera.m_fov = 80;
    m_rt.m_camera.setupViewmatrix();
//    m_rt.m_camera.m_target.setY(-2);



    m_rt.m_globals.m_skyScale = sin(m_time/38.12)/3.0 + 0.5;


/*    for (int i=0;i<m_rt.m_objects.count()-1;i++) {
        float t = m_time/12.3 + i*542;
        m_rt.m_objects[i]->m_material.m_reflectivity = 0.1;//sin(t)/2.0+1;
    }*/
}

void RaytracerThread::PerformCUDA()
{
//    m_img.fill(QColor(0,0,0,255));
    m_rendering=true;
    Init();
    m_timer = QElapsedTimer();
    m_timer.start();
//        m_timer.
//        m_rt.Raytrace(m_img);
   // m_rt.Raymarch(m_img);
//    qDebug() << "l: " <<sizeof(World)+world.length*sizeof(marchobject);

    World world = m_rt.toWorld(m_img.width(),m_img.height());

    uchar* data = RaytraceImage(m_img.width(), m_img.height(), img, &world);

//    for (int i=0;i<m_img.width()*m_img.height()*3;i++)
  //      img[i]=rand()%255;

    int k=0;

    m_img = QImage(data,m_img.width(), m_img.height(),QImage::Format_RGB888);

/*#pragma omp parallel for
        for (int j=0;j<m_img.height();j++) {
            for (int i=0;i<m_img.width();i++) {
                k=(m_img.width()*j+i);
//                m_img.setPixel(i,j,Util::toColor(QVector3D(data[k].x()*255,data[k].y()*255,data[k].z()*255)).rgba());
                m_img.setPixel(i,j,QColor(data[k].x()*255,data[k].y()*255,data[k].z()*255).rgba());
//                m_img.setPixel(i,j,QColor(img[3*k+0],img[3*k+1],img[3*k+2]).rgba());

            }
        }*/
        m_rendering=false;
        m_elapsedTime = m_timer.elapsed();


}

void RaytracerThread::PerformCPU()
{
//    m_img.fill(QColor(0,0,0,255));
    m_rendering=true;
    Init();
    m_timer = QElapsedTimer();
    m_timer.start();
//        m_timer.
//        m_rt.Raytrace(m_img);
    m_rt.Raymarch(m_img);
//    qDebug() << "l: " <<sizeof(World)+world.length*sizeof(marchobject);

    m_elapsedTime = m_timer.elapsed();
    m_rendering=false;

}

