#ifndef DIALOGEFFECTS_H
#define DIALOGEFFECTS_H

#include <QDebug>
#include <QDialog>
#include "source/forms/raytracer1.h"
#include <QElapsedTimer>
namespace Ui {
class DialogEffects;
}

class DialogEffects : public QDialog
{
    Q_OBJECT

    RaytracerThread* m_effect = nullptr;

public:
    explicit DialogEffects(QWidget *parent = nullptr);
    ~DialogEffects();

    void Abort();

    void closeEvent(QCloseEvent *e )override ;
private:
    void Create();
    Ui::DialogEffects *ui;
    int m_avg=0;

public slots:
    void UpdateImage();
private slots:
};

#endif // DIALOGEFFECTS_H
