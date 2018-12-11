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

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <QString>
#include <QMap>
#include <QDebug>
#include "errorhandler.h"
#include "pvar.h"
#include "token.h"
#include "syntax.h"
#include <util.h>
class Symbol {
public:
    QString m_name;
    QString m_type;
    int m_org = 0;
    int m_size = 0;

    PVar* m_value = nullptr;
    TokenType::Type getTokenType();
    Symbol(QString name, QString type="");
    Symbol(QString name, QString type, float var);
    Symbol(QString name, QString type, QString var);

};


class BuiltInTypeSymbol : public Symbol {
public:
    BuiltInTypeSymbol(QString name, QString type) : Symbol(name, type) {
    }
};

class VarSymbol : public Symbol {
public:
    VarSymbol(QString name, QString type) : Symbol(name, type) {

    }
};

class SymbolTable
{
public:
    QMap<QString, Symbol*> m_symbols;
    static QMap<QString, Symbol*> m_constants;
    QString m_name;
    SymbolTable();
    static SymbolTable s;


    static int m_currentSid;

    static void DefineSid(unsigned int initAddress, unsigned int playAddress);


    static bool isInitialized;
    static void Initialize();

    void Define(Symbol* s) {
        m_symbols[s->m_name] = s;
    }
    void Delete();

    void setName(QString s);

    void InitBuiltins();

    Symbol* Lookup(QString name, int lineNumber, bool isAddress=false);
    Symbol* LookupVariables(QString name, int lineNumber);

    Symbol* LookupConstants(QString name);

};

#endif // SYMBOLTABLE_H
