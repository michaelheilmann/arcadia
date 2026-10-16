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

#if !defined(ARCADIA_VPL_IR_CALLINSTRUCTION_H_INCLUDED)
#define ARCADIA_VPL_IR_CALLINSTRUCTION_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/Instruction.h"
#include "Arcadia/VPL/IR/RegisterValue.h"
#include "Arcadia/VPL/IR/Function.h"

// The built-in functions.
#define Arcadia_VPL_IR_BuiltinFunction_None (0)
#define Arcadia_VPL_IR_BuiltinFunction_Texture (1)

// Calls a function, either a function of the module or a built-in function.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.CallInstruction", Arcadia_VPL_IR_CallInstruction,
                          u8"Arcadia.VPL.IR.Instruction");

struct Arcadia_VPL_IR_CallInstructionDispatch {
  Arcadia_VPL_IR_InstructionDispatch _parent;
};

struct Arcadia_VPL_IR_CallInstruction {
  Arcadia_VPL_IR_Instruction _parent;
  /// The destination register, or NULL for a void call.
  Arcadia_VPL_IR_RegisterValue* result;
  /// The callee function of the module, or the null pointer if the callee is a built-in function.
  Arcadia_VPL_IR_Function* calleeFunction;
  /// The built-in function, one of Arcadia_VPL_IR_BuiltinFunction_*.
  Arcadia_Natural8Value builtin;
  /// The arguments, a list of Arcadia.VPL.IR.Value objects.
  Arcadia_List* arguments;
};

Arcadia_VPL_IR_CallInstruction*
Arcadia_VPL_IR_CallInstruction_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_RegisterValue* result,
    Arcadia_VPL_IR_Function* calleeFunction,
    Arcadia_Natural8Value builtin,
    Arcadia_List* arguments
  );

#endif // ARCADIA_VPL_IR_CALLINSTRUCTION_H_INCLUDED
