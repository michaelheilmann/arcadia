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

#if !defined(ARCADIA_VPL_IR_REFERENCEVALUE_H_INCLUDED)
#define ARCADIA_VPL_IR_REFERENCEVALUE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/Value.h"
#include "Arcadia/VPL/IR/Type.h"

#define Arcadia_VPL_IR_StorageKind_Local (0)
#define Arcadia_VPL_IR_StorageKind_Parameter (1)
#define Arcadia_VPL_IR_StorageKind_Uniform (2)
#define Arcadia_VPL_IR_StorageKind_StageIO (3)
#define Arcadia_VPL_IR_StorageKind_Member (4)
#define Arcadia_VPL_IR_StorageKind_Index (5)

// A reference to a storage location: a uniform field, a stage input/output, a parameter, or a local.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.ReferenceValue", Arcadia_VPL_IR_ReferenceValue,
                          u8"Arcadia.VPL.IR.Value");

struct Arcadia_VPL_IR_ReferenceValueDispatch {
  Arcadia_VPL_IR_ValueDispatch _parent;
};

struct Arcadia_VPL_IR_ReferenceValue {
  Arcadia_VPL_IR_Value _parent;
  /// The root storage namespace or derived reference kind, one of Arcadia_VPL_IR_StorageKind_*.
  Arcadia_Natural8Value storageKind;
  /// The root storage slot; zero for a derived reference.
  Arcadia_SizeValue slot;
  /// The type of the referenced value.
  Arcadia_VPL_IR_Type* type;
};

Arcadia_VPL_IR_ReferenceValue*
Arcadia_VPL_IR_ReferenceValue_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue slot,
    Arcadia_VPL_IR_Type* type
  );

/// Construct a reference in an explicit storage namespace.
Arcadia_VPL_IR_ReferenceValue*
Arcadia_VPL_IR_ReferenceValue_createWithStorage
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value storageKind,
    Arcadia_SizeValue slot,
    Arcadia_VPL_IR_Type* type
  );

#endif // ARCADIA_VPL_IR_REFERENCEVALUE_H_INCLUDED
