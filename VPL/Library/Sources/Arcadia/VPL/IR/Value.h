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

#if !defined(ARCADIA_VPL_IR_VALUE_H_INCLUDED)
#define ARCADIA_VPL_IR_VALUE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"

// See Arcadia.VPL.IR.RegisterValue for more information.
#define Arcadia_VPL_IR_ValueKind_Register (0)
// See Arcadia.VPL.IR.ConstantValue for more information.
#define Arcadia_VPL_IR_ValueKind_Constant (1)
// See Arcadia.VPL.IR.ReferenceValue for more information.
#define Arcadia_VPL_IR_ValueKind_Reference (2)

// The base of all intermediate-representation values.
// Every value carries an explicit type; see Arcadia.VPL.IR.Type.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.Value", Arcadia_VPL_IR_Value,
                          u8"Arcadia.Object");

struct Arcadia_VPL_IR_ValueDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_IR_Value {
  Arcadia_Object _parent;
  /// The kind of this value.
  Arcadia_Natural8Value kind;
};

Arcadia_VPL_IR_Value*
Arcadia_VPL_IR_Value_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value kind
  );

#endif // ARCADIA_VPL_IR_VALUE_H_INCLUDED