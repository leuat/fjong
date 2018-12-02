#include "material.h"

Material::Material()
{

}

Material::Material(QVector3D col, float shin, float ref, float perlin, float ps, QString texture) {
    m_color = col;
    m_shininess = shin;
    m_reflectivity = ref;
    m_perlinness = perlin;
    m_perlinScale = ps;



    if (texture!="") {
        m_hasTexture = true;
        QString f = "textures/"+texture;
        if (!QFile::exists(f)) {
            qDebug() << "Could not find file: " << f;
            exit(1);
        }
//        qDebug() << "File exists!";
        m_texture.Load(f);

      //  qDebug() << "DONE";

    }

}

void Texture::GenerateMipMaps() {
    m_images.append(m_image);
    QImage* cur = m_image;


    int ww = cur->width();
    int hh = cur->height();
    for (int i=0;i<8;i++) {
        //qDebug() << ww;
        ww = std::max(ww/2,1);
        hh = std::max(hh/2,1);
        QImage* nxt = new QImage(m_image->width(),m_image->height(),QImage::Format_ARGB32);
        nxt->fill(QColor(0,0,0,1));
        QImage small = (cur->scaled(ww,hh,Qt::KeepAspectRatio));
        /*for (int x=0;x<xw;x++)
            for (int y=0;y<yw;y++) {
                nxt->setPixel(x,y,cur->pixel(x*2,y*2));
            }*/
        *nxt = (small.scaled(m_image->width(),m_image->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
//        qDebug() <<nxt->width();

        m_images.append(nxt);
        cur = nxt;
    }
}
