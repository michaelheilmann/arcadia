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

#if !defined(ARCADIA_PDLC_AST_RAISEINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_RAISEINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/InstructionNode.h"
typedef struct Arcadia_PDLC_AST_OperandNode Arcadia_PDLC_AST_OperandNode;

/// @code
/// class Arcadia.PDLC.AST.RaiseInstructionNode extends Arcadia.PDLC.AST.InstructionNode
/// @endcode
/// Represents
/// @code
/// 'raise'
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.RaiseInstructionNode", Arcadia_PDLC_AST_RaiseInstructionNode,
                          u8"Arcadia.PDLC.AST.InstructionNode");

struct Arcadia_PDLC_AST_RaiseInstructionNodeDispatch {
  Arcadia_PDLC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_PDLC_AST_RaiseInstructionNode {
  Arcadia_PDLC_AST_InstructionNode parent;
  Arcadia_PDLC_AST_OperandNode* operand;
};

/// @brief Create a MIL raise instruction AST node.
/// @return A pointer to this MIL raise instruction AST node.
Arcadia_PDLC_AST_RaiseInstructionNode*
Arcadia_PDLC_AST_RaiseInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset
  );

#endif // ARCADIA_PDLC_AST_RAISEINSTRUCTIONNODE_H_INCLUDED
