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

#if !defined(ARCADIA_MILC_AST_INVOKEINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_INVOKEINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/MILC/AST/InstructionNode.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_MILC_AST_VariableOperandNode Arcadia_MILC_AST_VariableOperandNode;

/// @code
/// class Arcadia.MIL.InvokeExpressionAst extends Arcadia.MIL.ExpressionAst
/// @endcode
/// Represents an unary epxression like
/// @code
/// 'invoke' target '(' (operand (',' operand)*)? ')'
/// target : name
/// operand : register
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.InvokeInstructionNode", Arcadia_MILC_AST_InvokeInstructionNode,
                          u8"Arcadia.MILC.AST.InstructionNode");

struct Arcadia_MILC_AST_InvokeInstructionNodeDispatch {
  Arcadia_MILC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_MILC_AST_InvokeInstructionNode {
  Arcadia_MILC_AST_InstructionNode parent;
  Arcadia_MILC_AST_VariableOperandNode* callee;
  Arcadia_List* operands;
};

Arcadia_MILC_AST_InvokeInstructionNode*
Arcadia_MILC_AST_InvokeInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_VariableOperandNode* callee,
    Arcadia_List* operands
  );

#endif // ARCADIA_MILC_AST_INVOKEINSTRUCTIONNODE_H_INCLUDED
