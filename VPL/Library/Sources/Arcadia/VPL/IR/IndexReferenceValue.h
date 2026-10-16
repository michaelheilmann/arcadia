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

#if !defined(ARCADIA_VPL_IR_INDEXREFERENCEVALUE_H_INCLUDED)
#define ARCADIA_VPL_IR_INDEXREFERENCEVALUE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/ReferenceValue.h"

Arcadia_declareObjectType
  (
    u8"Arcadia.VPL.IR.IndexReferenceValue",
    Arcadia_VPL_IR_IndexReferenceValue,
    u8"Arcadia.VPL.IR.ReferenceValue"
  );

struct Arcadia_VPL_IR_IndexReferenceValueDispatch {
  Arcadia_VPL_IR_ReferenceValueDispatch _parent;
};

struct Arcadia_VPL_IR_IndexReferenceValue {
  Arcadia_VPL_IR_ReferenceValue _parent;
  /// The reference to the containing array storage.
  Arcadia_VPL_IR_ReferenceValue* base;
  /// A register or scalar constant, evaluated when a load/store uses this path.
  Arcadia_VPL_IR_Value* index;
};

Arcadia_VPL_IR_IndexReferenceValue*
Arcadia_VPL_IR_IndexReferenceValue_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReferenceValue* base,
    Arcadia_VPL_IR_Value* index,
    Arcadia_VPL_IR_Type* type
  );

#endif // ARCADIA_VPL_IR_INDEXREFERENCEVALUE_H_INCLUDED
