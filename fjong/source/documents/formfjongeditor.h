#ifndef FORMFJONGEDITOR_H
#define FORMFJONGEDITOR_H

#include <QWidget>
#include "fjongdocument.h"
#include "source/documents/highlighter.h"

namespace Ui {
class FormFjongEditor;
}

class FormFjongEditor : public FJONGDocument
{
    Q_OBJECT

public:
    explicit FormFjongEditor(QWidget *parent = nullptr);
    ~FormFjongEditor();

    void setText(QString txt);

private:
    QFont m_font;
    Ui::FormFjongEditor *ui;
    Highlighter* highlighter = nullptr;

    void InitDocument(CIniFile* ini) override;

    void SetupHighlighter();

    void Save(QString filename) override;


};


#endif // FORMFJONGEDITOR_H
