/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
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

#include "compiler.h"
#include <QDebug>

Compiler::Compiler(Parser* p, CIniFile* ini, CIniFile* pIni)
{
    m_parser = p;
    m_ini = ini;
    m_projectIni = pIni;
}

void Compiler::Parse()
{
    m_tree = nullptr;
    m_parser->m_preprocessorDefines[m_projectIni->getString("system").toUpper()]=1;
    m_parser->m_preprocessorDefines[m_ini->getString("assembler").toUpper()]=1;
    //qDebug() << "******" << m_ini->getString("assembler").toUpper();
    try {

        m_tree = m_parser->Parse(m_ini->getdouble("optimizer_remove_unused_symbols")==1.0,m_projectIni->getString("vic_memory_config"));
        //qDebug() << m_parser->m_preprocessorDefines["ORGASM"];
        //exit(1);
    } catch (FatalErrorException e) {
        qDebug() << "ERROR parse " << e.message;
        HandleError(e, "Error during parsing:");
    }

}


bool Compiler::Build(Compiler::Type type, QString project_dir)
{
    if (m_tree==nullptr) {
        qDebug() << "Compiler::Build : tree not parsed!";
        return false;
    }

    if (m_tree!=nullptr)
        try {
            dynamic_cast<NodeProgram*>(m_tree)->m_initJumps = m_parser->m_initJumps;
            //m_dispatcher->as = m_assembler;
//            m_dispatcher->dispatch(m_tree);
            m_tree->Accept(m_dispatcher);

            //m_tree->Build(m_assembler);
        } catch (FatalErrorException e) {
            HandleError(e,"Error during build");
            return false;
         }
 /*       try {
           // m_tree->ExecuteSym(m_assembler->m_symTab);
        } catch (FatalErrorException e) {
            HandleError(e,"Error during symbolic check");
            return false;
    }*/

    return true;

}

void Compiler::HandleError(FatalErrorException fe, QString e)
{
    QString msg = "";
    int linenr = fe.linenr;
    QString file = "";
    FindLineNumberAndFile(fe.linenr, file, linenr);
    //linenr = fe.linenr;


    QString line = " on line: " + QString::number(linenr+1);
    if (file!="")
        msg+=" in file : " + file + "\n";

    fe.file=file;

    msg +="\nFatal error " + line;
    if (linenr<m_parser->m_lexer->m_lines.count() && linenr>=0)
        msg+="\nSource: " + m_parser->m_lexer->m_lines[linenr];
    msg+="\n\nMessage: ";
    //Pmm::Data::d.lineNumber = linenr+1;

    recentError = fe;
    ErrorHandler::e.CatchError(fe, e + msg);

}

void Compiler::FindLineNumberAndFile(int inLe, QString& file, int& outle)
{
    file="";
    outle = inLe;
    if (m_parser->m_lexer->m_includeFiles.count()==0) {
        return;
    }

    int cur = inLe;

    qDebug() << "input line number: " << inLe;
    qDebug() << "Start line: " << m_parser->m_lexer->m_includeFiles[0].m_startLine;
    if (cur<=m_parser->m_lexer->m_includeFiles[0].m_startLine) {
        return;
    }


    for (FilePart fp: m_parser->m_lexer->m_includeFiles) {
        if (inLe >= fp.m_startLine && inLe<fp.m_endLine) {
            file = fp.m_name;
            outle = inLe - fp.m_startLine;
            return;
        }
        qDebug() << "Include file size : " << (fp.m_endLine-fp.m_startLine);
        cur=cur - (fp.m_endLine-fp.m_startLine);
    }
    outle = cur;
}
