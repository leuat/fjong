#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetDarkPalette();

//    DialogEffects* de= new DialogEffects();
  //  de->exec();

    LoadDocument("test.fjong");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadDocument(QString filename)
{
    if (!QFile::exists(filename))
        return;

    FJONGDocument* doc = nullptr;

    if (filename.toLower().endsWith(".fjong")) {

        QFile f(filename);
        f.open(QFile::ReadOnly| QFile::Text);
        QString text = f.readAll();
        f.close();

        doc = new FormFjongEditor(this);
        dynamic_cast<FormFjongEditor*>(doc)->setText(text);

    }

    if (doc==nullptr)
        return;

    doc->m_currentSourceFile = filename;
   m_documents.append(doc);
   doc->InitDocument(&m_settings);
   ui->tabMain->addTab(doc, filename);


   connect(m_currentDoc, SIGNAL(updatePaletteSignal()), this, SLOT(updatePalette()));
   connect(m_currentDoc, SIGNAL(requestSaveAs()), this, SLOT(SaveAs()));
   connect(m_currentDoc, SIGNAL(requestCloseWindow()), this, SLOT(closeWindowSlot()));
   connect(m_currentDoc, SIGNAL(emitFindFile()), this, SLOT(FindFileDialog()));
   connect(m_currentDoc, SIGNAL(requestBuild()), this, SLOT(acceptBuild()));


}

void MainWindow::SetDarkPalette() {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    QApplication::setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: #ffE0C0; background-color: #000000; border: 0px; }");


}

void MainWindow::RemoveTab(int idx, bool save)
{
        if (idx==0)
            return;


        idx--;
        FJONGDocument* doc = m_documents[idx];
        if (!doc->SaveChanges())
            return;

        doc->PrepareClose();

        if (doc==nullptr)
            return;



        ui->tabMain->removeTab(idx+1);

        m_documents[idx]->Destroy();
        m_documents[idx];
        m_documents.remove(idx);


        ui->tabMain->currentWidget()->setFocus();

    }


void MainWindow::closeWindowSlot()
{
    int idx = ui->tabMain->currentIndex();
    RemoveTab(idx);
    //qDebug() << "DONE";
}

