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

#if !defined(ARCADIA_VPL_IR_TYPE_H_INCLUDED)
#define ARCADIA_VPL_IR_TYPE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"

// See Arcadia.VPL.IR.VoidType for more information.
#define Arcadia_VPL_IR_TypeKind_Void (0)
// See Arcadia.VPL.IR.VectorType for more information.
#define Arcadia_VPL_IR_TypeKind_Vector (1)
// See Arcadia.VPL.IR.MatrixType for more information.
#define Arcadia_VPL_IR_TypeKind_Matrix (2)
// See Arcadia.VPL.IR.SamplerType for more information.
#define Arcadia_VPL_IR_TypeKind_Sampler (3)
// See Arcadia.VPL.IR.RecordType for more information.
#define Arcadia_VPL_IR_TypeKind_Record (4)
// See Arcadia.VPL.IR.ArrayType for more information.
#define Arcadia_VPL_IR_TypeKind_Array (5)
#define Arcadia_VPL_IR_TypeKind_Scalar (6)

// The scalar element kind of a vector or matrix type.
#define Arcadia_VPL_IR_ScalarElementKind_Boolean (0)
#define Arcadia_VPL_IR_ScalarElementKind_Integer (1)
#define Arcadia_VPL_IR_ScalarElementKind_Floating (2)

// The base of all intermediate-representation types.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.Type", Arcadia_VPL_IR_Type,
                          u8"Arcadia.Object");

struct Arcadia_VPL_IR_TypeDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_IR_Type {
  Arcadia_Object _parent;
  /// The kind of this type.
  Arcadia_Natural8Value kind;
};

/// @brief Create a type.
/// @param thread A pointer to this thread.
/// @param kind The kind of the type.
Arcadia_VPL_IR_Type*
Arcadia_VPL_IR_Type_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value kind
  );

/// @brief Get the kind of a type.
Arcadia_Natural8Value
Arcadia_VPL_IR_Type_getKind
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Type* self
  );

#endif // ARCADIA_VPL_IR_TYPE_H_INCLUDED
