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

#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include <QMap>
//#include "data_pmm.h"
#include <QDebug>

class TokenType {
public:
    enum Type {
        INTEGER, PLUS, MINUS, MUL, DIV, LPAREN,
        RPAREN, TEOF, ID, ASSIGN, BEGIN,
        END, SEMI, DOT, VAR, REAL,
        PROGRAM, COLON, COMMA, INTEGER_CONST,
        REAL_CONST, PROCEDURE, STRING,
        IF, THEN, EQUALS,NOTEQUALS, GREATER, LESS,
        FOR, TO, DO, NADA, ADDRESS, WHILE,
        ARRAY, OF, BYTE, LBRACKET, RBRACKET, INCBIN, ASM, INCSID,
        INTERRUPT, BITAND, BITOR, ELSE, OR, AND, POINTER, AT, INCLUDE, DEFINE, PREPROCESSOR,
        IFDEF, ENDIF, IFNDEF, OFFPAGE, ONPAGE, STEP, UNROLL, LOOPX, LOOPY, CSTRING, USERDATA,
        STARTBLOCK, ENDBLOCK, IGNOREMETHOD, ERRORMESSAGE, WEDGE
    };

    static QString types[];
    static QString getType(Type t) {
        return types[t];
    }

    static Type getType(QString s) {
        for (int i=0;i<68;i++)
            if (types[i].toLower()==s.toLower()) {
//                qDebug() << s;
                return (Type)i;
            }

        return DIV;
    }

//    QMap<QString, TokenType::Type> m_stringType;

    QString m_value;
    Type m_type;

    TokenType() {}
    TokenType (Type t, QString v) {
        m_type = t;
        m_value = v;
    }
};


class Token
{
public:
    TokenType::Type m_type;
    QString m_value;
    int m_intVal=0;
    int m_lineNumber=0;
    QString m_currentLineText="";
    QString getType();

    Token();
    QString getNumAsHexString();

    Token(TokenType::Type t, QString val);
    Token(TokenType::Type t, int val);

};

#endif // TOKEN_H
