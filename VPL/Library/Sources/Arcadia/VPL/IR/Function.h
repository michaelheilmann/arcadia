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

#if !defined(ARCADIA_VPL_IR_FUNCTION_H_INCLUDED)
#define ARCADIA_VPL_IR_FUNCTION_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"
#include "Arcadia/VPL/IR/Type.h"
#include "Arcadia/VPL/IR/Block.h"

// A function of a stage.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.Function", Arcadia_VPL_IR_Function,
                          u8"Arcadia.Object");

struct Arcadia_VPL_IR_FunctionDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_IR_Function {
  Arcadia_Object _parent;
  /// The name of the function.
  Arcadia_String* name;
  /// The return type of the function.
  Arcadia_VPL_IR_Type* returnType;
  /// The parameters of the function, a list of Arcadia.VPL.IR.ReferenceValue objects.
  Arcadia_List* parameters;
  /// The basic blocks of the function, a list of Arcadia.VPL.IR.Block objects.
  Arcadia_List* blocks;
};

Arcadia_VPL_IR_Function*
Arcadia_VPL_IR_Function_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_VPL_IR_Type* returnType,
    Arcadia_List* parameters,
    Arcadia_List* blocks
  );

#endif // ARCADIA_VPL_IR_FUNCTION_H_INCLUDED