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

#if !defined(ARCADIA_PDLC_AST_INVOKEINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_INVOKEINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/InstructionNode.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_PDLC_AST_OperandNode Arcadia_PDLC_AST_OperandNode;
typedef struct Arcadia_PDLC_AST_VariableOperandNode Arcadia_PDLC_AST_VariableOperandNode;

/// @code
/// class Arcadia.PDLC.AST.InvokeInstructionNode extends Arcadia.PDLC.AST.InstructionNode
/// @endcode
/// Represents
/// @code
/// 'invoke' operand name operand operand*
/// target  : operand
/// callee  : name
/// operands : operand
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.InvokeInstructionNode", Arcadia_PDLC_AST_InvokeInstructionNode,
                          u8"Arcadia.PDLC.AST.InstructionNode");

struct Arcadia_PDLC_AST_InvokeInstructionNodeDispatch {
  Arcadia_PDLC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_PDLC_AST_InvokeInstructionNode {
  Arcadia_PDLC_AST_InstructionNode parent;
  Arcadia_PDLC_AST_OperandNode* target;
  Arcadia_PDLC_AST_VariableOperandNode* callee;
  Arcadia_List* operands;
  /// True for the explicit procedure-only `call` spelling.
  Arcadia_BooleanValue procedureOnly;
};

Arcadia_PDLC_AST_InvokeInstructionNode*
Arcadia_PDLC_AST_InvokeInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_PDLC_AST_OperandNode* target,
    Arcadia_PDLC_AST_VariableOperandNode* callee,
    Arcadia_List* operands
  );

#endif // ARCADIA_PDLC_AST_INVOKEINSTRUCTIONNODE_H_INCLUDED
