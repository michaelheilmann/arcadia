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

#if !defined(ARCADIA_VPL_IR_FIELD_H_INCLUDED)
#define ARCADIA_VPL_IR_FIELD_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/Type.h"

// A named field of a record type or record uniform.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.Field", Arcadia_VPL_IR_Field,
                          u8"Arcadia.Object");

struct Arcadia_VPL_IR_FieldDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_IR_Field {
  Arcadia_Object _parent;
  /// The name of the field.
  Arcadia_String* name;
  /// The type of the field.
  Arcadia_VPL_IR_Type* type;
};

Arcadia_VPL_IR_Field*
Arcadia_VPL_IR_Field_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_VPL_IR_Type* type
  );

#endif // ARCADIA_VPL_IR_FIELD_H_INCLUDED