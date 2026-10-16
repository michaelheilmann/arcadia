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

#if !defined(ARCADIA_VPL_IR_INSTRUCTION_H_INCLUDED)
#define ARCADIA_VPL_IR_INSTRUCTION_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"

// See Arcadia.VPL.IR.DeclareLocalInstruction for more information.
#define Arcadia_VPL_IR_InstructionKind_DeclareLocal (0)
// See Arcadia.VPL.IR.LoadInstruction for more information.
#define Arcadia_VPL_IR_InstructionKind_Load (1)
// See Arcadia.VPL.IR.StoreInstruction for more information.
#define Arcadia_VPL_IR_InstructionKind_Store (2)
// See Arcadia.VPL.IR.BinaryInstruction for more information.
#define Arcadia_VPL_IR_InstructionKind_Binary (3)
// See Arcadia.VPL.IR.CallInstruction for more information.
#define Arcadia_VPL_IR_InstructionKind_Call (4)
// See Arcadia.VPL.IR.ConstructInstruction for more information.
#define Arcadia_VPL_IR_InstructionKind_Construct (5)
// See Arcadia.VPL.IR.SwizzleInstruction for more information.
#define Arcadia_VPL_IR_InstructionKind_Swizzle (6)
// See Arcadia.VPL.IR.MemberInstruction for more information.
#define Arcadia_VPL_IR_InstructionKind_Member (7)
// See Arcadia.VPL.IR.IndexInstruction for more information.
#define Arcadia_VPL_IR_InstructionKind_Index (8)
// See Arcadia.VPL.IR.DiscardInstruction for more information.
#define Arcadia_VPL_IR_InstructionKind_Discard (9)
#define Arcadia_VPL_IR_InstructionKind_Copy (10)

// The base of all intermediate-representation instructions.
// Instructions writing a result have an explicit destination register.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.Instruction", Arcadia_VPL_IR_Instruction,
                          u8"Arcadia.Object");

struct Arcadia_VPL_IR_InstructionDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_IR_Instruction {
  Arcadia_Object _parent;
  /// The kind of this instruction.
  Arcadia_Natural8Value kind;
};

Arcadia_VPL_IR_Instruction*
Arcadia_VPL_IR_Instruction_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value kind
  );

#endif // ARCADIA_VPL_IR_INSTRUCTION_H_INCLUDED
