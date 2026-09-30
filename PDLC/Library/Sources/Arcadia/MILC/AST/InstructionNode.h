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

#if !defined(ARCADIA_MILC_AST_INSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_INSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/MILC/AST/InstructionStatementNode.h"

/// @code
/// class Arcadia.MILC.AST.InstructionMode extends Arcadia.MILC.AST.InstructionStatementNode
/// @endcode
/// represents
/// @code
/// instruction : binaryInstruction
///             | unaryInstruction
///             | invokeInstruction
///             | jumpInstruction
///             | raiseInstruction
///             | returnInstruction
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.InstructionNode", Arcadia_MILC_AST_InstructionNode,
                          u8"Arcadia.MILC.AST.InstructionStatementNode");

struct Arcadia_MILC_AST_InstructionNodeDispatch {
  Arcadia_MILC_AST_InstructionStatementNodeDispatch _parent;
};

struct Arcadia_MILC_AST_InstructionNode {
  Arcadia_MILC_AST_InstructionStatementNode _parent;
};

#endif // ARCADIA_MILC_AST_INSTRUCTIONNODE_H_INCLUDED
