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

#if !defined(ARCADIA_PDLC_AST_JUMPINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_JUMPINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/InstructionNode.h"
typedef struct Arcadia_PDLC_AST_OperandNode Arcadia_PDLC_AST_OperandNode;

typedef enum Arcadia_PDLC_AST_JumpInstructionKind {
  Arcadia_PDLC_AST_JumpInstructionKind_Always,
  Arcadia_PDLC_AST_JumpInstructionKind_IfTrue,
  Arcadia_PDLC_AST_JumpInstructionKind_IfFalse,
} Arcadia_PDLC_AST_JumpInstructionKind;

/// @code
/// class Arcadia.PDLC.AST.JumpInstructionNode extends Arcadia.PDLC.AST.InstructionNode
/// @endcode
/// Represents
/// @code
/// jumpInstruction : 'jump' name
///                 | 'jumpIfTrue' operand ',' name
///                 | 'jumpIfFalse' operand ',' name
/// name : name
/// operand : operand
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.JumpInstructionNode", Arcadia_PDLC_AST_JumpInstructionNode,
                          u8"Arcadia.PDLC.AST.InstructionNode");

struct Arcadia_PDLC_AST_JumpInstructionNodeDispatch {
  Arcadia_PDLC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_PDLC_AST_JumpInstructionNode {
  Arcadia_PDLC_AST_InstructionNode parent;
  Arcadia_PDLC_AST_JumpInstructionKind kind;
  Arcadia_PDLC_AST_OperandNode* condition;
  /// The name of the label.
  Arcadia_String* labelName;
};

/// @brief Create a MIL jump instruction AST node.
/// @return A pointer to this MIL jump instruction AST node.
Arcadia_PDLC_AST_JumpInstructionNode*
Arcadia_PDLC_AST_JumpInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_PDLC_AST_JumpInstructionKind kind,
    Arcadia_PDLC_AST_OperandNode* condition,
    Arcadia_String* labelName
  );

#endif // ARCADIA_PDLC_AST_JUMPINSTRUCTIONNODE_H_INCLUDED
