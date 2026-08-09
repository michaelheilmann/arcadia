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

#if !defined(ARCADIA_VPL_CONTEXT_H_INCLUDED)
#define ARCADIA_VPL_CONTEXT_H_INCLUDED

#include "Arcadia/Languages/Include.h"
typedef struct Arcadia_VPL_EnterPhase Arcadia_VPL_EnterPhase;
typedef struct Arcadia_VPL_ResolvePhase Arcadia_VPL_ResolvePhase;

Arcadia_declareObjectType(u8"Arcadia.VPL.Context", Arcadia_VPL_Context,
                          Arcadia_Object);

struct Arcadia_VPL_ContextDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_Context {
  Arcadia_Object _parent;
  Arcadia_Languages_Diagnostics* diagnostics;
  Arcadia_VPL_EnterPhase* enterPhase;
  Arcadia_VPL_ResolvePhase* resolvePhase;
};

Arcadia_VPL_Context*
Arcadia_VPL_Context_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_VPL_CONTEXT_H_INCLUDED
