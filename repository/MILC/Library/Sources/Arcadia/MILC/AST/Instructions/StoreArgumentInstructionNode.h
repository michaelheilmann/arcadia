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

#if !defined(ARCADIA_MILC_AST_STOREARGUMENTINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_STOREARGUMENTINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/MILC/AST/InstructionNode.h"
typedef struct Arcadia_MILC_AST_OperandNode Arcadia_MILC_AST_OperandNode;

/// @code
/// class Arcadia.MILC.AST.StoreArgumentInstructionNode extends Arcadia.MILC.AST.InstructionNode
/// @endcode
/// Represents
/// @code
/// 'storeArgument' target name source
/// target : register
/// name : name
/// source : register
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.StoreArgumentInstructionNode", Arcadia_MILC_AST_StoreArgumentInstructionNode,
                          u8"Arcadia.MILC.AST.InstructionNode");

struct Arcadia_MILC_AST_StoreArgumentInstructionNodeDispatch {
  Arcadia_MILC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_MILC_AST_StoreArgumentInstructionNode {
  Arcadia_MILC_AST_InstructionNode parent;
  Arcadia_MILC_AST_OperandNode* target;
  Arcadia_MILC_AST_OperandNode* name;
  Arcadia_MILC_AST_OperandNode* source;
};

/// @brief Create a MIL store argument instruction AST node.
/// @param thread A pointer to this thread.
/// @param target The target.
/// @param name The name.
/// @param source The source.
/// @return A pointer to this MIL store argument instruction AST node.
Arcadia_MILC_AST_StoreArgumentInstructionNode*
Arcadia_MILC_AST_StoreArgumentInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_OperandNode* target,
    Arcadia_MILC_AST_OperandNode* name,
    Arcadia_MILC_AST_OperandNode* source
  );

#endif // ARCADIA_MILC_AST_STOREARGUMENTINSTRUCTIONNODE_H_INCLUDED
