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

#if !defined(ARCADIA_MILC_AST_LOADFIELDINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_LOADFIELDINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/MILC/AST/InstructionNode.h"
typedef struct Arcadia_MILC_AST_OperandNode Arcadia_MILC_AST_OperandNode;

/// @code
/// class Arcadia.MILC.AST.LoadFieldInstructionNode extends Arcadia.MILC.AST.InstructionNode
/// @endcode
/// Represents
/// @code
/// 'ldf' target source name
/// target : register
/// source : register
/// name : name
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.LoadFieldInstructionNode", Arcadia_MILC_AST_LoadFieldInstructionNode,
                          u8"Arcadia.MILC.AST.InstructionNode");

struct Arcadia_MILC_AST_LoadFieldInstructionNodeDispatch {
  Arcadia_MILC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_MILC_AST_LoadFieldInstructionNode {
  Arcadia_MILC_AST_InstructionNode parent;
  Arcadia_MILC_AST_OperandNode* target;
  Arcadia_MILC_AST_OperandNode* source;
  Arcadia_MILC_AST_OperandNode* name;
};

/// @brief Create a MIL load field instruction AST node.
/// @param thread A pointer to this thread.
/// @param target The target.
/// @param source The source.
/// @param name The name.
/// @return A pointer to this MIL load field instruction AST node.
Arcadia_MILC_AST_LoadFieldInstructionNode*
Arcadia_MILC_AST_LoadFieldInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_MILC_AST_OperandNode* target,
    Arcadia_MILC_AST_OperandNode* source,
    Arcadia_MILC_AST_OperandNode* name
  );

#endif // ARCADIA_MILC_AST_LOADFIELDINSTRUCTIONNODE_H_INCLUDED
