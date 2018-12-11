/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (FJONG, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#include "fjongdocument.h"

QPalette FJONGDocument::m_defaultPalette;

FJONGDocument::FJONGDocument(QWidget* parent) : QWidget(parent)
{

}

bool FJONGDocument::SaveChanges()
{
//    qDebug() << "Document changed: " << m_documentIsChanged;
    if (!m_documentIsChanged)
        return true;
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setWindowTitle("Save document");
    int ret = msgBox.exec();
    if (ret==QMessageBox::Save)
        SaveCurrent();
    if (ret==QMessageBox::Cancel)
        return false;

    return true;

}

void FJONGDocument::keyPressEvent(QKeyEvent *e) {


    if (e->key() == Qt::Key_S &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        SaveCurrent();
    }
    if (e->key() == Qt::Key_B &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        Build();
    }

    if (e->key() == Qt::Key_T &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        UserDefined();
    }

    if (e->key() == Qt::Key_L &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        emit emitFindFile();
    }

}

void FJONGDocument::UserDefined()
{
    QProcess p;

    QString s = m_iniFile->getString("user_defined_command").trimmed();
    if (s=="")
        return;

    QString src = m_currentSourceFile;

    s=s.replace("@prg", src.replace(".ras", ".prg"));
    QStringList params = s.split(" ");
    QString cmd = params[0];
    params.removeFirst();
    p.execute(cmd,params);
    p.waitForFinished();

}
