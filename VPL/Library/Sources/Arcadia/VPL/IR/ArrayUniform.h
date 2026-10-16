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

#if !defined(ARCADIA_VPL_IR_ARRAYUNIFORM_H_INCLUDED)
#define ARCADIA_VPL_IR_ARRAYUNIFORM_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/Uniform.h"
#include "Arcadia/VPL/IR/Type.h"

// A uniform of an array type.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.ArrayUniform", Arcadia_VPL_IR_ArrayUniform,
                          u8"Arcadia.VPL.IR.Uniform");

struct Arcadia_VPL_IR_ArrayUniformDispatch {
  Arcadia_VPL_IR_UniformDispatch _parent;
};

struct Arcadia_VPL_IR_ArrayUniform {
  Arcadia_VPL_IR_Uniform _parent;
  /// The element type of the uniform.
  Arcadia_VPL_IR_Type* elementType;
  /// The number of elements of the uniform, or 0 for an unsized array.
  Arcadia_SizeValue count;
};

Arcadia_VPL_IR_ArrayUniform*
Arcadia_VPL_IR_ArrayUniform_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_VPL_IR_Type* elementType,
    Arcadia_SizeValue count
  );

#endif // ARCADIA_VPL_IR_ARRAYUNIFORM_H_INCLUDED