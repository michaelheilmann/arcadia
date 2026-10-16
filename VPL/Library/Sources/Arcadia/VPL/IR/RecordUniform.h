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

#if !defined(ARCADIA_VPL_IR_RECORDUNIFORM_H_INCLUDED)
#define ARCADIA_VPL_IR_RECORDUNIFORM_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/Uniform.h"
#include "Arcadia/VPL/IR/RecordType.h"

// A uniform of a record type.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.RecordUniform", Arcadia_VPL_IR_RecordUniform,
                          u8"Arcadia.VPL.IR.Uniform");

struct Arcadia_VPL_IR_RecordUniformDispatch {
  Arcadia_VPL_IR_UniformDispatch _parent;
};

struct Arcadia_VPL_IR_RecordUniform {
  Arcadia_VPL_IR_Uniform _parent;
  /// The record type of the uniform.
  Arcadia_VPL_IR_RecordType* recordType;
};

Arcadia_VPL_IR_RecordUniform*
Arcadia_VPL_IR_RecordUniform_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_VPL_IR_RecordType* recordType
  );

#endif // ARCADIA_VPL_IR_RECORDUNIFORM_H_INCLUDED