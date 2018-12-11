#include "formfjongeditor.h"
#include "ui_formfjongeditor.h"

FormFjongEditor::FormFjongEditor(QWidget *parent) :
    FJONGDocument(parent),
    ui(new Ui::FormFjongEditor)
{
    ui->setupUi(this);
}

FormFjongEditor::~FormFjongEditor()
{
    delete ui;
}

void FormFjongEditor::setText(QString txt)
{
    ui->txtEditor->setPlainText(txt);
    ui->txtEditor->m_textChanged = false;
}

void FormFjongEditor::InitDocument(CIniFile *ini) {
    FJONGDocument::InitDocument(ini);

    m_font.setFamily("Courier");
    m_font.setFixedPitch(true);
    m_font.setPointSize(12);

    QFontMetrics metrics(m_font);

    ui->txtEditor->setFont(m_font);
    ui->txtEditor->setTabStopWidth(4 * metrics.width(' '));


    SetupHighlighter();
}


void FormFjongEditor::SetupHighlighter()
{
    if (highlighter != nullptr)
        delete highlighter;
    CIniFile colors;
    colors.Load("themes/october.ini");//" + m_iniFile->getString("theme"));
    ui->txtEditor->InitColors(colors);

    QPalette p = ui->txtEditor->palette();
    p.setColor(QPalette::Base, colors.getColor("backgroundcolor"));
    p.setColor(QPalette::Text, colors.getColor("textcolor"));
    ui->txtEditor->setPalette(p);
    highlighter = new Highlighter(colors, ui->txtEditor->document());

    //    qDebug() << "UPDATE " << m_iniFile->getString("theme");

}

void FormFjongEditor::Save(QString filename)
{
    if (QFile::exists(filename))
        QFile::remove(filename);
    QString txt = ui->txtEditor->document()->toPlainText();
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << txt;
    }
    file.close();
//    m_iniFile->Save();
    //       ui->txtEditor->m_textChanged = false;
}
