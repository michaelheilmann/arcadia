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

#if !defined(ARCADIA_PDLC_AST_REALLITERALNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_REALLITERALNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/Node.h"
typedef struct Arcadia_PDLC_Symbol Arcadia_PDLC_Symbol;

/// @code
/// class Arcadia.MIL.RealLiteralOperandAst extends Arcadia.MIL.OperandAst
/// @endcode
/// represents
/// @code
/// realLiteral : lexicalAnalysis.realLiteral
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.RealLiteralNode", Arcadia_PDLC_AST_RealLiteralNode,
                          u8"Arcadia.PDLC.AST.Node");

struct Arcadia_PDLC_AST_RealLiteralNodeDispatch {
  Arcadia_PDLC_AST_NodeDispatch parent;
};

struct Arcadia_PDLC_AST_RealLiteralNode {
  Arcadia_PDLC_AST_Node parent;
  Arcadia_String* value;
  /// @brief The type of this node. Initially null. Computed in a later phase.
  Arcadia_PDLC_Symbol* type;
};

/// @undefined @a value is not a real literal
Arcadia_PDLC_AST_RealLiteralNode*
Arcadia_PDLC_AST_RealLiteralNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* value
  );

#endif // ARCADIA_PDLC_AST_REALLITERALNODE_H_INCLUDED
