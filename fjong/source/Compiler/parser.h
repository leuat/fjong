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

#ifndef PARSER_H
#define PARSER_H

#include "source/Compiler/ast/ast.h"
#include "source/Compiler/lexer.h"
#include <QRegularExpression>
#include "source/Compiler/ast/nodebinop.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodestring.h"
#include "source/Compiler/ast/nodeunaryop.h"
#include "source/Compiler/ast/nodecompound.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodeassign.h"
#include "source/Compiler/ast/nodevardecl.h"
#include "source/Compiler/ast/nodeblock.h"
#include "source/Compiler/ast/nodeprogram.h"
#include "source/Compiler/ast/nodevartype.h"
#include "source/Compiler/ast/nodeproceduredecl.h"
#include "source/Compiler/ast/nodeprocedure.h"
#include "source/Compiler/ast/nodeconditional.h"
#include "source/Compiler/ast/nodeforloop.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodewhileloop.h"
#include "source/Compiler/ast/nodeasm.h"
#include "source/Compiler/ast/nodebinaryclause.h"

class ParserBlock {
public:
    int m_blockID;
    QString pos;
};



class Parser {
public:

    QVector<ParserBlock> m_parserBlocks;

//    int m_currentParserBlock=-1;

    QMap<QString, Node*> m_procedures;
    QMap<QString, QString> m_preprocessorDefines;
    QVector<Node*> m_proceduresOnly;
    QVector<QString> m_ignoreMethods;
    Lexer* m_lexer;
    Token m_currentToken;
    int m_pass = 0;
    bool m_ignoreAll = false;
    QStringList  m_initJumps;
    SymbolTable* m_symTab;
    Parser();
    Parser(Lexer* l) {
        m_lexer = l;
    }
    ~Parser() {
        Delete();
    }

    void Delete();

    void Eat(TokenType::Type t);
    void VerifyToken(Token t);

    void HandlePreprocessorInParsing();
    void Preprocess();
    void PreprocessReplace();
    void PreprocessIfDefs(bool ifdef);

    int findPage();

    Node* Parse(bool removeUnusedDecls, QString param);
    Node* Variable();
    Node* Empty();
    Node* AssignStatement();
    Node* Statement();
    QVector<Node*> StatementList();
    Node* CompoundStatement();
    Node* Program(QString param);
    Node* Factor();
    Node* Expr();
    Node* Term();
    Node* FindProcedure();
    Node* BinaryClause();
    //Node* LogicalClause();
    Node* Block(bool useOwnSymTab);
    QVector<Node*> Parameters();
    Node* ForLoop();
//    Node* WhileLoop();
    Node* String();

    Node* Conditional(bool isWhileLoop=false);
//    QVector<Node*> Procedure();
    QVector<Node*> Declarations(bool isMain);
    QVector<Node*> VariableDeclarations();
    Node* TypeSpec();
    Node* BuiltinFunction();
    Node* Constant();
    Node* InlineAssembler();
    void Eat();


    int findSymbolLineNumber(QString symbol);





};


#endif // PARSER_H
