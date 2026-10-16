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

#if !defined(ARCADIA_VPL_IR_STOREINSTRUCTION_H_INCLUDED)
#define ARCADIA_VPL_IR_STOREINSTRUCTION_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/Instruction.h"
#include "Arcadia/VPL/IR/ReferenceValue.h"
#include "Arcadia/VPL/IR/Value.h"

// Stores a value into a storage location.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.StoreInstruction", Arcadia_VPL_IR_StoreInstruction,
                          u8"Arcadia.VPL.IR.Instruction");

struct Arcadia_VPL_IR_StoreInstructionDispatch {
  Arcadia_VPL_IR_InstructionDispatch _parent;
};

struct Arcadia_VPL_IR_StoreInstruction {
  Arcadia_VPL_IR_Instruction _parent;
  /// The storage location.
  Arcadia_VPL_IR_ReferenceValue* target;
  /// The value to store.
  Arcadia_VPL_IR_Value* value;
};

Arcadia_VPL_IR_StoreInstruction*
Arcadia_VPL_IR_StoreInstruction_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReferenceValue* target,
    Arcadia_VPL_IR_Value* value
  );

#endif // ARCADIA_VPL_IR_STOREINSTRUCTION_H_INCLUDED