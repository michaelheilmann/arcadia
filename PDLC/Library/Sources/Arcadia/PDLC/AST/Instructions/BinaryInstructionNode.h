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

#if !defined(ARCADIA_PDLC_AST_BINARYINSTRUCTIONNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_BINARYINSTRUCTIONNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/InstructionNode.h"
#include "Arcadia/PDLC/AST/Instructions/BinaryInstructionKind.h"
typedef struct Arcadia_PDLC_AST_OperandNode Arcadia_PDLC_AST_OperandNode;

/// @code
/// class Arcadia.PDLC.AST.BinaryInstructionNode extends Arcadia.PDLC.AST.InstructionNode
/// @endcode
/// Represents instructions with two operands and a result.
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.BinaryInstructionNode", Arcadia_PDLC_AST_BinaryInstructionNode,
                          u8"Arcadia.PDLC.AST.InstructionNode");

struct Arcadia_PDLC_AST_BinaryInstructionNodeDispatch {
  Arcadia_PDLC_AST_InstructionNodeDispatch parent;
};

struct Arcadia_PDLC_AST_BinaryInstructionNode {
  Arcadia_PDLC_AST_InstructionNode parent;
  Arcadia_PDLC_AST_BinaryInstructionKind kind;
  Arcadia_PDLC_AST_OperandNode* target;
  Arcadia_PDLC_AST_OperandNode* operand1;
  Arcadia_PDLC_AST_OperandNode* operand2;
};

Arcadia_PDLC_AST_BinaryInstructionNode*
Arcadia_PDLC_AST_BinaryInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_PDLC_AST_BinaryInstructionKind kind,
    Arcadia_PDLC_AST_OperandNode* target,
    Arcadia_PDLC_AST_OperandNode* operand1,
    Arcadia_PDLC_AST_OperandNode* operand2
  );

#endif // ARCADIA_PDLC_AST_BINARYINSTRUCTIONNODE_H_INCLUDED
