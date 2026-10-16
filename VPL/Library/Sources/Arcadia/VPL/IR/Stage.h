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

#if !defined(ARCADIA_VPL_IR_STAGE_H_INCLUDED)
#define ARCADIA_VPL_IR_STAGE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"
#include "Arcadia/VPL/IR/Function.h"

// The kinds of stages.
#define Arcadia_VPL_IR_Stage_Vertex (0)
#define Arcadia_VPL_IR_Stage_Fragment (1)

// A stage of the rasterizer program.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.Stage", Arcadia_VPL_IR_Stage,
                          u8"Arcadia.Object");

struct Arcadia_VPL_IR_StageDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_IR_Stage {
  Arcadia_Object _parent;
  /// The kind of this stage, one of Arcadia_VPL_IR_Stage_*.
  Arcadia_Natural8Value kind;
  /// The interface of this stage, a list of Arcadia.VPL.IR.StageIO objects.
  Arcadia_List* interface;
  /// The functions of this stage, a list of Arcadia.VPL.IR.Function objects.
  Arcadia_List* functions;
  /// Explicit entry function; NULL only while building an unfinished stage.
  Arcadia_VPL_IR_Function* entryFunction;
};

Arcadia_VPL_IR_Stage*
Arcadia_VPL_IR_Stage_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value kind,
    Arcadia_List* interface,
    Arcadia_List* functions
  );

Arcadia_VPL_IR_Stage*
Arcadia_VPL_IR_Stage_createWithEntryFunction
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value kind,
    Arcadia_List* interface,
    Arcadia_List* functions,
    Arcadia_VPL_IR_Function* entryFunction
  );

#endif // ARCADIA_VPL_IR_STAGE_H_INCLUDED
