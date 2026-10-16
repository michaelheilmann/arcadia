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

#if !defined(ARCADIA_PDLC_AST_RETURNINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_RETURNINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/InstructionNode.h"
typedef struct Arcadia_PDLC_AST_OperandNode Arcadia_PDLC_AST_OperandNode;

/// @code
/// class Arcadia.PDLC.AST.ReturnInstructionNode extends Arcadia.PDLC.AST.InstructionNode
/// @endcode
/// Represents
/// @code
/// 'return' operand?
/// operand : operand
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.ReturnInstructionNode", Arcadia_PDLC_AST_ReturnInstructionNode,
                          u8"Arcadia.PDLC.AST.InstructionNode");

struct Arcadia_PDLC_AST_ReturnInstructionNodeDispatch {
  Arcadia_PDLC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_PDLC_AST_ReturnInstructionNode {
  Arcadia_PDLC_AST_InstructionNode parent;
  Arcadia_PDLC_AST_OperandNode* operand;
};

/// @brief Create a MIL return instruction AST node.
/// @return A pointer to this MIL return instruction AST node.
Arcadia_PDLC_AST_ReturnInstructionNode*
Arcadia_PDLC_AST_ReturnInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_PDLC_AST_OperandNode* operand
  );

#endif // ARCADIA_PDLC_AST_RETURNINSTRUCTIONNODE_H_INCLUDED
