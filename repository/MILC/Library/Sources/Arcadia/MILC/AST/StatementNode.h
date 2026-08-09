// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#if !defined(ARCADIA_MILC_AST_STATEMENTNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_STATEMENTNODE_H_INCLUDED

#include "Arcadia/MILC/AST/Node.h"

/// @code
/// class Arcadia.MILC.AST.StatementNode extends Arcadia.MILC.AST.Node {
///   ...
/// }
/// @endcode
/// represents
/// @code
/// definitionStatement
/// instructionStatement
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.StatementNode", Arcadia_MILC_AST_StatementNode,
                          u8"Arcadia.MILC.AST.Node");

struct Arcadia_MILC_AST_StatementNodeDispatch {
  Arcadia_MILC_AST_NodeDispatch _parent;
};

struct Arcadia_MILC_AST_StatementNode {
  Arcadia_MILC_AST_Node _parent;
};

#endif // ARCADIA_MILC_AST_STATEMENTNODE_H_INCLUDED
