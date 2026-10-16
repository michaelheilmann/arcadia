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

#if !defined(ARCADIA_VPL_IR_BINARYINSTRUCTION_H_INCLUDED)
#define ARCADIA_VPL_IR_BINARYINSTRUCTION_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/Instruction.h"
#include "Arcadia/VPL/IR/RegisterValue.h"
#include "Arcadia/VPL/IR/Value.h"

// The binary operators.
#define Arcadia_VPL_IR_BinaryOperator_Add (0)
#define Arcadia_VPL_IR_BinaryOperator_Subtract (1)
#define Arcadia_VPL_IR_BinaryOperator_Multiply (2)
#define Arcadia_VPL_IR_BinaryOperator_Divide (3)
#define Arcadia_VPL_IR_BinaryOperator_Equal (4)
#define Arcadia_VPL_IR_BinaryOperator_NotEqual (5)
#define Arcadia_VPL_IR_BinaryOperator_Less (6)
#define Arcadia_VPL_IR_BinaryOperator_LessEqual (7)
#define Arcadia_VPL_IR_BinaryOperator_Greater (8)
#define Arcadia_VPL_IR_BinaryOperator_GreaterEqual (9)

// Applies a binary operator to two values and writes the result into a register.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.BinaryInstruction", Arcadia_VPL_IR_BinaryInstruction,
                          u8"Arcadia.VPL.IR.Instruction");

struct Arcadia_VPL_IR_BinaryInstructionDispatch {
  Arcadia_VPL_IR_InstructionDispatch _parent;
};

struct Arcadia_VPL_IR_BinaryInstruction {
  Arcadia_VPL_IR_Instruction _parent;
  /// The destination register.
  Arcadia_VPL_IR_RegisterValue* result;
  /// The operator, one of Arcadia_VPL_IR_BinaryOperator_*.
  Arcadia_Natural8Value operatorKind;
  /// The left-hand side.
  Arcadia_VPL_IR_Value* lhs;
  /// The right-hand side.
  Arcadia_VPL_IR_Value* rhs;
};

Arcadia_VPL_IR_BinaryInstruction*
Arcadia_VPL_IR_BinaryInstruction_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_RegisterValue* result,
    Arcadia_Natural8Value operatorKind,
    Arcadia_VPL_IR_Value* lhs,
    Arcadia_VPL_IR_Value* rhs
  );

#endif // ARCADIA_VPL_IR_BINARYINSTRUCTION_H_INCLUDED
