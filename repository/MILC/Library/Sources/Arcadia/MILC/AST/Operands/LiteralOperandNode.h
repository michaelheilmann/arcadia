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

#if !defined(ARCADIA_MILC_AST_OPERANDS_LITERALOPERANDNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_OPERANDS_LITERALOPERANDNODE_H_INCLUDED

#include "Arcadia/MILC/AST/OperandNode.h"

/// @code
/// class Arcadia.MILC.AST.LiteralOperandNode extends Arcadia.MILC.AST.OperandNode {
///   ...
/// }
/// @endcode
/// represents
/// @code
/// lexical.literal
/// @endcode
/// in the @a operand context.
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.LiteralOperandNode", Arcadia_MILC_AST_LiteralOperandNode,
                          u8"Arcadia.MIL.OperandAst");

struct Arcadia_MILC_AST_LiteralOperandNodeDispatch {
  Arcadia_MILC_AST_OperandNodeDispatch parent;
};

struct Arcadia_MILC_AST_LiteralOperandNode {
  Arcadia_MILC_AST_OperandNode parent;
  Arcadia_MILC_AST_Node* literal;
};

Arcadia_MILC_AST_LiteralOperandNode*
Arcadia_MILC_AST_LiteralOperandNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_MILC_AST_Node* literal
  );

#endif // ARCADIA_MILC_AST_OPERANDS_LITERALOPERANDNODE_H_INCLUDED
