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

#if !defined(ARCADIA_PDLC_AST_LOADFIELDINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_LOADFIELDINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/InstructionNode.h"
typedef struct Arcadia_PDLC_AST_OperandNode Arcadia_PDLC_AST_OperandNode;

/// @code
/// class Arcadia.PDLC.AST.LoadFieldInstructionNode extends Arcadia.PDLC.AST.InstructionNode
/// @endcode
/// Represents
/// @code
/// 'loadField' target ',' source ',' name
/// target : register
/// source : register
/// name : name
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.LoadFieldInstructionNode", Arcadia_PDLC_AST_LoadFieldInstructionNode,
                          u8"Arcadia.PDLC.AST.InstructionNode");

struct Arcadia_PDLC_AST_LoadFieldInstructionNodeDispatch {
  Arcadia_PDLC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_PDLC_AST_LoadFieldInstructionNode {
  Arcadia_PDLC_AST_InstructionNode parent;
  Arcadia_PDLC_AST_OperandNode* target;
  Arcadia_PDLC_AST_OperandNode* source;
  Arcadia_PDLC_AST_OperandNode* name;
};

/// @brief Create a MIL load field instruction AST node.
/// @param thread A pointer to this thread.
/// @param target The target.
/// @param source The source.
/// @param name The name.
/// @return A pointer to this MIL load field instruction AST node.
Arcadia_PDLC_AST_LoadFieldInstructionNode*
Arcadia_PDLC_AST_LoadFieldInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_PDLC_AST_OperandNode* target,
    Arcadia_PDLC_AST_OperandNode* source,
    Arcadia_PDLC_AST_OperandNode* name
  );

#endif // ARCADIA_PDLC_AST_LOADFIELDINSTRUCTIONNODE_H_INCLUDED
