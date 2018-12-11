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

#include "node.h"

int Node::m_currentLineNumber;

void Node::Delete() {
    if (m_left!=nullptr) {
        m_left->Delete();
        delete m_left;
        m_left = nullptr;
    }
    if (m_right!=nullptr) {
        m_right->Delete();
        delete m_right;
        m_left = nullptr;

    }
}


void Node::RequireAddress(Node *n, QString name, int ln) {
    if (!n->isAddress())
        ErrorHandler::e.Error(name + " requires parameter to be memory address. Did you forget a '^' symbol such as ^$D800?", ln);
}

