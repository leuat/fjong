#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector3D>
#include <QString>
#include <QDebug>

#include <QImage>
#include <QFile>
#include <QVector>
#include <math.h>

class Texture {
public:
    QString m_name="";
    QVector<QImage*> m_images;

    QImage* m_image = nullptr;

    void Load(QString f) {
        m_image = new QImage();
        if (!m_image->load(f)) {
            qDebug() << "Unsupported image format: " << f;
            exit(1);

        }
        if (m_image->width()==0) {
            qDebug() << "Error loading file: " << f;
            qDebug() << QFile::exists(f);
            exit(1);
        }
        GenerateMipMaps();
    }
    QImage* get(int lvl) {
        if (lvl>=m_images.count()) lvl = m_images.count()-1;
        if (lvl<0) lvl = 0;
        return m_images[lvl];
    }

    void GenerateMipMaps();

};


class Material
{
public:
    Material();

    QVector3D m_color =QVector3D(1,0.5,0.5);
    float m_shininess=0;
    float m_reflectivity=0.5;
    float m_perlinness = 0.3;
    float m_uvScale = 0.03;
    float m_perlinScale = 5;
    Texture m_texture;
    bool m_hasTexture = false;

    Material(QVector3D col, float shin, float ref, float perlin, float ps, QString texture);

};

#endif // MATERIAL_H
