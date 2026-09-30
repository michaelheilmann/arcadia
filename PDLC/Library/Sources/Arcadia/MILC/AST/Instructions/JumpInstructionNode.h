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

#if !defined(ARCADIA_MILC_AST_JUMPINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_JUMPINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/MILC/AST/InstructionNode.h"

/// @code
/// class Arcadia.MILC.AST.JumpInstructionNode extends Arcadia.MILC.AST.InstructionNode
/// @endcode
/// Represents
/// @code
/// jumpStatement : 'jump' operand labelName
/// labelName : lexical.string | lexical.name
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.JumpInstructionNode", Arcadia_MILC_AST_JumpInstructionNode,
                          u8"Arcadia.MILC.AST.InstructionNode");

struct Arcadia_MILC_AST_JumpInstructionNodeDispatch {
  Arcadia_MILC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_MILC_AST_JumpInstructionNode {
  Arcadia_MILC_AST_InstructionNode parent;
  /// The name of the label.
  Arcadia_String* labelName;
};

/// @brief Create a MIL jump instruction AST node.
/// @return A pointer to this MIL jump instruction AST node.
Arcadia_MILC_AST_JumpInstructionNode*
Arcadia_MILC_AST_JumpInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* labelName
  );

#endif // ARCADIA_MILC_AST_JUMPINSTRUCTIONNODE_H_INCLUDED
