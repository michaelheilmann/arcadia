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

#if !defined(ARCADIA_MILC_AST_RETURNINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_RETURNINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/MILC/AST/InstructionNode.h"
typedef struct Arcadia_MILC_AST_OperandNode Arcadia_MILC_AST_OperandNode;

/// @code
/// class Arcadia.MILC.AST.ReturnInstructionNode extends Arcadia.MILC.AST.InstructionNode
/// @endcode
/// Represents
/// @code
/// 'return' (BooleanLiteral | NumberLiteral | StringLiteral | VariableName)
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.ReturnInstructionNode", Arcadia_MILC_AST_ReturnInstructionNode,
                          u8"Arcadia.MILC.AST.InstructionNode");

struct Arcadia_MILC_AST_ReturnInstructionNodeDispatch {
  Arcadia_MILC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_MILC_AST_ReturnInstructionNode {
  Arcadia_MILC_AST_InstructionNode parent;
  Arcadia_MILC_AST_OperandNode* operand;
};

/// @brief Create a MIL return instruction AST node.
/// @return A pointer to this MIL return instruction AST node.
Arcadia_MILC_AST_ReturnInstructionNode*
Arcadia_MILC_AST_ReturnInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_OperandNode* operand
  );

#endif // ARCADIA_MILC_AST_RETURNINSTRUCTIONNODE_H_INCLUDED
