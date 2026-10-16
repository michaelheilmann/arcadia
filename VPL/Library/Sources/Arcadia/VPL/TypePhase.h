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

#if !defined(ARCADIA_VPL_TYPEPHASE_H_INCLUDED)
#define ARCADIA_VPL_TYPEPHASE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Languages/Include.h"
#include "Arcadia/VPL/IR/Include.h"
#include "Arcadia/VPL/Tree/Node.h"

typedef struct Arcadia_VPL_Symbols_Program Arcadia_VPL_Symbols_Program;

Arcadia_declareObjectType(u8"Arcadia.VPL.TypePhase", Arcadia_VPL_TypePhase,
                          u8"Arcadia.Object");

struct Arcadia_VPL_TypePhaseDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_TypePhase {
  Arcadia_Object _parent;
  Arcadia_Languages_Diagnostics* diagnostics;
  /// Node to IR type. Only complete, successful runs publish this map.
  Arcadia_Map* types;
  Arcadia_Map* symbolTypes;
  Arcadia_VPL_Symbols_Program* program;
};

Arcadia_VPL_TypePhase*
Arcadia_VPL_TypePhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  );

void
Arcadia_VPL_TypePhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Symbols_Program* program
  );

/// Returns NULL for a node not typed by the last successful run.
Arcadia_VPL_IR_Type*
Arcadia_VPL_TypePhase_getType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Tree_Node* node
  );

Arcadia_Languages_Diagnostics*
Arcadia_VPL_TypePhase_getDiagnostics
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self
  );

#endif // ARCADIA_VPL_TYPEPHASE_H_INCLUDED
