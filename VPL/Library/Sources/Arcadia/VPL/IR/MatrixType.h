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

#if !defined(ARCADIA_VPL_IR_MATRIXTYPE_H_INCLUDED)
#define ARCADIA_VPL_IR_MATRIXTYPE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/Type.h"

// A matrix type with the given scalar element kind and dimensions.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.MatrixType", Arcadia_VPL_IR_MatrixType,
                          u8"Arcadia.VPL.IR.Type");

struct Arcadia_VPL_IR_MatrixTypeDispatch {
  Arcadia_VPL_IR_TypeDispatch _parent;
};

struct Arcadia_VPL_IR_MatrixType {
  Arcadia_VPL_IR_Type _parent;
  /// The scalar element kind, one of Arcadia_VPL_IR_ScalarElementKind_*.
  Arcadia_Natural8Value elementKind;
  /// The number of rows.
  Arcadia_Natural8Value rows;
  /// The number of columns.
  Arcadia_Natural8Value columns;
};

Arcadia_VPL_IR_MatrixType*
Arcadia_VPL_IR_MatrixType_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value elementKind,
    Arcadia_Natural8Value rows,
    Arcadia_Natural8Value columns
  );

#endif // ARCADIA_VPL_IR_MATRIXTYPE_H_INCLUDED