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

#if !defined(ARCADIA_VPL_RESOLVEPHASE_H_INCLUDED)
#define ARCADIA_VPL_RESOLVEPHASE_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/VPL/Symbols/Include.h"

// 2nd phase.
// Resolve types.
Arcadia_declareObjectType(u8"Arcadia.VPL.ResolvePhase", Arcadia_VPL_ResolvePhase,
                          Arcadia_Object);

struct Arcadia_VPL_ResolvePhaseDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_ResolvePhase {
  Arcadia_Object _parent;
};

Arcadia_VPL_ResolvePhase*
Arcadia_VPL_ResolvePhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  );

void
Arcadia_VPL_ResolvePhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Program* program
  );

#endif // ARCADIA_VPL_RESOLVEPHASE_H_INCLUDED
