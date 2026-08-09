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

#if !defined(ARCADIA_VPL_ENTERPHASE_H_INCLUDED)
#define ARCADIA_VPL_ENTERPHASE_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/VPL/Symbols/Include.h"

// 1st phase.
// Enter symbols into scopes.
Arcadia_declareObjectType(u8"Arcadia.VPL.EnterPhase", Arcadia_VPL_EnterPhase,
                          Arcadia_Object);

struct Arcadia_VPL_EnterPhaseDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_EnterPhase {
  Arcadia_Object _parent;
  // Constant records and procedures are added to a work list.
  // They are taken out of the queue for processing.
  Arcadia_Deque* worklist;
};

Arcadia_VPL_EnterPhase*
Arcadia_VPL_EnterPhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  );

// - create scope of the program
// - populate that scope with default symbols (e.g., the built-in types)
// - for each constant record defn:
//   - create constant record symbol
//   - create scope of constant record symbol
//   - add constant record symbol to scope of program
//   - for each variable defn of the constant record symbol / constant record defn:
//    - create variable symbol
//    - add variable symbol to scope of constant record symbol
void
Arcadia_VPL_EnterPhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_EnterPhase* self,
    Arcadia_VPL_Symbols_Program* program
  );

#endif // ARCADIA_VPL_ENTERPHASE_H_INCLUDED
