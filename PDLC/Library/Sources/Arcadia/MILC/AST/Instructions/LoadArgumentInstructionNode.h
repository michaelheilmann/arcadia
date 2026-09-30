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

#if !defined(ARCADIA_MILC_AST_LOADARGUMENTINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_LOADARGUMENTINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/MILC/AST/InstructionNode.h"
typedef struct Arcadia_MILC_AST_OperandNode Arcadia_MILC_AST_OperandNode;

/// @code
/// class Arcadia.MILC.AST.LoadArgumentInstructionNode extends Arcadia.MILC.AST.InstructionNode
/// @endcode
/// Represents
/// @code
/// 'loadArgument' target source
/// target : register
/// source : name
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.LoadArgumentInstructionNode", Arcadia_MILC_AST_LoadArgumentInstructionNode,
                          u8"Arcadia.MILC.AST.InstructionNode");

struct Arcadia_MILC_AST_LoadArgumentInstructionNodeDispatch {
  Arcadia_MILC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_MILC_AST_LoadArgumentInstructionNode {
  Arcadia_MILC_AST_InstructionNode parent;
  Arcadia_MILC_AST_OperandNode* target;
  Arcadia_MILC_AST_OperandNode* source;
};

/// @brief Create a MIL load argument instruction AST node.
/// @param thread A pointer to this thread.
/// @param target The target.
/// @param source The source.
/// @return A pointer to this MIL load argument instruction AST node.
Arcadia_MILC_AST_LoadArgumentInstructionNode*
Arcadia_MILC_AST_LoadArgumentInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_MILC_AST_OperandNode* target,
    Arcadia_MILC_AST_OperandNode* source
  );

#endif // ARCADIA_MILC_AST_LOADARGUMENTINSTRUCTIONNODE_H_INCLUDED
