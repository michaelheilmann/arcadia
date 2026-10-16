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

#if !defined(ARCADIA_VPL_IR_MODULE_H_INCLUDED)
#define ARCADIA_VPL_IR_MODULE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"
#include "Arcadia/VPL/IR/Stage.h"

// A rasterizer program in the IR.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.Module", Arcadia_VPL_IR_Module,
                          u8"Arcadia.Object");

struct Arcadia_VPL_IR_ModuleDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_IR_Module {
  Arcadia_Object _parent;
  /// The uniforms of this module, a list of Arcadia.VPL.IR.Uniform objects.
  Arcadia_List* uniforms;
  /// The vertex stage of this module.
  Arcadia_VPL_IR_Stage* vertex;
  /// The fragment stage of this module.
  Arcadia_VPL_IR_Stage* fragment;
};

Arcadia_VPL_IR_Module*
Arcadia_VPL_IR_Module_create
  (
    Arcadia_Thread* thread,
    Arcadia_List* uniforms,
    Arcadia_VPL_IR_Stage* vertex,
    Arcadia_VPL_IR_Stage* fragment
  );

#endif // ARCADIA_VPL_IR_MODULE_H_INCLUDED