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

#if !defined(ARCADIA_MILC_AST_STRINGLITERALNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_STRINGLITERALNODE_H_INCLUDED

#include "Arcadia/MILC/AST/Node.h"
typedef struct Arcadia_MILC_Symbol Arcadia_MILC_Symbol;

/// @code
/// class Arcadia.MILC.AST.StringLiteralNode extends Arcadia.MILC.AST.Node
/// @endcode
/// represents
/// @code
/// stringLiteral : lexicalAnalysis.stringLiteral
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.StringLiteralNode", Arcadia_MILC_AST_StringLiteralNode,
                          u8"Arcadia.MILC.AST.Node");

struct Arcadia_MILC_AST_StringLiteralNodeDispatch {
  Arcadia_MILC_AST_NodeDispatch parent;
};

struct Arcadia_MILC_AST_StringLiteralNode {
  Arcadia_MILC_AST_Node parent;
  Arcadia_String* value;
  /// @brief The type of this node. Initially null. Computed in a later phase.
  Arcadia_MILC_Symbol* type;
};

Arcadia_MILC_AST_StringLiteralNode*
Arcadia_MILC_AST_StringLiteralNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* value
  );

#endif // ARCADIA_MILC_AST_STRINGLITERALNODE_H_INCLUDED
