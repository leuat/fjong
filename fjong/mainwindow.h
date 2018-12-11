#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QVector>
#include "source/forms/dialogeffects.h"
#include "source/documents/fjongdocument.h"
#include "source/documents/formfjongeditor.h"
#include <cinifile.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<FJONGDocument*> m_documents;
    CIniFile m_settings;
    FJONGDocument* m_currentDoc = nullptr;

private:
    Ui::MainWindow *ui;

    void LoadDocument(QString filename);

    void SetDarkPalette();

    void RemoveTab(int, bool save=true);


 public slots:
    void closeWindowSlot();

};


#endif // MAINWINDOW_H
