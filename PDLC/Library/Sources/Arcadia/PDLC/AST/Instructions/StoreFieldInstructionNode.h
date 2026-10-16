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

#if !defined(ARCADIA_PDLC_AST_STOREFIELDINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_STOREFIELDINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/InstructionNode.h"
typedef struct Arcadia_PDLC_AST_OperandNode Arcadia_PDLC_AST_OperandNode;

/// @code
/// class Arcadia.PDLC.AST.StoreFieldInstructionNode extends Arcadia.PDLC.AST.InstructionNode
/// @endcode
/// Represents
/// @code
/// 'storeField' target name source
/// target : register
/// name : name
/// source : register
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.StoreFieldInstructionNode", Arcadia_PDLC_AST_StoreFieldInstructionNode,
                          u8"Arcadia.PDLC.AST.InstructionNode");

struct Arcadia_PDLC_AST_StoreFieldInstructionNodeDispatch {
  Arcadia_PDLC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_PDLC_AST_StoreFieldInstructionNode {
  Arcadia_PDLC_AST_InstructionNode parent;
  Arcadia_PDLC_AST_OperandNode* target;
  Arcadia_PDLC_AST_OperandNode* name;
  Arcadia_PDLC_AST_OperandNode* source;
};

/// @brief Create a MIL store field instruction AST node.
/// @param thread A pointer to this thread.
/// @param target The target.
/// @param name The name.
/// @param source The source.
/// @return A pointer to this MIL store field instruction AST node.
Arcadia_PDLC_AST_StoreFieldInstructionNode*
Arcadia_PDLC_AST_StoreFieldInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_PDLC_AST_OperandNode* target,
    Arcadia_PDLC_AST_OperandNode* name,
    Arcadia_PDLC_AST_OperandNode* source
  );

#endif // ARCADIA_PDLC_AST_STOREFIELDINSTRUCTIONNODE_H_INCLUDED
