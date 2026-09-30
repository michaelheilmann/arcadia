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

#if !defined(ARCADIA_MILC_AST_OPERANDS_REGISTEROPERANDNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_OPERANDS_REGISTEROPERANDNODE_H_INCLUDED

#include "Arcadia/MILC/AST/OperandNode.h"

/// @code
/// class Arcadia.MILC.AST.RegisterOperandNode extends Arcadia.MILC.AST.OperandNode {
///   ...
/// }
/// @endcode
/// represents
/// @code
/// register
/// @endcode
/// in the @a operand context.
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.RegisterOperandNode", Arcadia_MILC_AST_RegisterOperandNode,
                          u8"Arcadia.MILC.AST.OperandNode");

struct Arcadia_MILC_AST_RegisterOperandNodeDispatch {
  Arcadia_MILC_AST_OperandNodeDispatch parent;
};

struct Arcadia_MILC_AST_RegisterOperandNode {
  Arcadia_MILC_AST_OperandNode parent;
  Arcadia_String* value;
};

Arcadia_MILC_AST_RegisterOperandNode*
Arcadia_MILC_AST_RegisterOperandNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* value
  );

#endif // ARCADIA_MILC_AST_OPERANDS_REGISTEROPERANDNODE_H_INCLUDED
