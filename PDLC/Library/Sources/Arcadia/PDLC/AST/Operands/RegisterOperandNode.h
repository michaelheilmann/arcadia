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

#if !defined(ARCADIA_PDLC_AST_OPERANDS_REGISTEROPERANDNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_OPERANDS_REGISTEROPERANDNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/OperandNode.h"

/// @code
/// class Arcadia.PDLC.AST.RegisterOperandNode extends Arcadia.PDLC.AST.OperandNode {
///   ...
/// }
/// @endcode
/// represents
/// @code
/// register
/// @endcode
/// in the @a operand context.
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.RegisterOperandNode", Arcadia_PDLC_AST_RegisterOperandNode,
                          u8"Arcadia.PDLC.AST.OperandNode");

struct Arcadia_PDLC_AST_RegisterOperandNodeDispatch {
  Arcadia_PDLC_AST_OperandNodeDispatch parent;
};

struct Arcadia_PDLC_AST_RegisterOperandNode {
  Arcadia_PDLC_AST_OperandNode parent;
  Arcadia_String* value;
};

Arcadia_PDLC_AST_RegisterOperandNode*
Arcadia_PDLC_AST_RegisterOperandNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* value
  );

#endif // ARCADIA_PDLC_AST_OPERANDS_REGISTEROPERANDNODE_H_INCLUDED
