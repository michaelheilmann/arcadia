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

#if !defined(ARCADIA_VPL_IR_SCALARTYPE_H_INCLUDED)
#define ARCADIA_VPL_IR_SCALARTYPE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/Type.h"

Arcadia_declareObjectType(u8"Arcadia.VPL.IR.ScalarType", Arcadia_VPL_IR_ScalarType,
                          u8"Arcadia.VPL.IR.Type");

struct Arcadia_VPL_IR_ScalarTypeDispatch {
  Arcadia_VPL_IR_TypeDispatch _parent;
};

struct Arcadia_VPL_IR_ScalarType {
  Arcadia_VPL_IR_Type _parent;
  Arcadia_Natural8Value elementKind;
};

Arcadia_VPL_IR_ScalarType*
Arcadia_VPL_IR_ScalarType_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value elementKind
  );

#endif // ARCADIA_VPL_IR_SCALARTYPE_H_INCLUDED
