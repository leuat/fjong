#include "dialogeffects.h"
#include "ui_dialogeffects.h"

DialogEffects::DialogEffects(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEffects)
{
    ui->setupUi(this);

    Create();
}

DialogEffects::~DialogEffects()
{

    Abort();

    delete ui;
}

void DialogEffects::Abort()
{
    if (m_effect!=nullptr) {
        m_effect->m_abort = true;
        m_effect->quit();
        m_effect->wait();
        m_effect->terminate();
        m_effect->wait();
        m_effect->deleteLater();
//        delete m_effect;
    }

}

void DialogEffects::closeEvent(QCloseEvent *e)
{
    Abort();
    exit(1);
}

void DialogEffects::Create()
{
    if (m_effect!=nullptr) {
        Abort();
        delete m_effect;
    }

    m_effect = new RaytracerThread();

    connect(m_effect,SIGNAL(SignalImageUpdate()),this,SLOT(UpdateImage()));
    m_effect->start();
}

void DialogEffects::UpdateImage()
{
    if (m_effect==nullptr)
        return;
    QPixmap p = QPixmap::fromImage(m_effect->m_img);
    ui->lblImage->setPixmap(p);

    m_avg = (int)(0.2*m_effect->m_elapsedTime +0.8*m_avg) ;
    ui->lblTime->setText(QString::number(m_avg));

}
