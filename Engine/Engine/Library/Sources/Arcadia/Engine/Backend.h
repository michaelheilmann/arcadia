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

#if !defined(ARCADIA_ENGINE_BACKEND_H_INCLUDED)
#define ARCADIA_ENGINE_BACKEND_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Backend.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_Engine_Backend Arcadia_Engine_Backend;
typedef struct Arcadia_Engine_BackendContext Arcadia_Engine_BackendContext;

Arcadia_declareObjectType(u8"Arcadia.Engine.Backend", Arcadia_Engine_Backend,
                          u8"Arcadia.Object");

struct Arcadia_Engine_BackendDispatch {
  Arcadia_ObjectDispatch _parent;

  Arcadia_String* (*getName)(Arcadia_Thread*, Arcadia_Engine_Backend*);

  Arcadia_Engine_BackendContext* (*createBackendContext)(Arcadia_Thread*, Arcadia_Engine_Backend*);
};

struct Arcadia_Engine_Backend {
  Arcadia_Object _parent;
};

Arcadia_String*
Arcadia_Engine_Backend_getName
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Backend* self
  );

Arcadia_Engine_BackendContext*
Arcadia_Engine_Backend_createBackendContext
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Backend* self
  );

#endif // ARCADIA_ENGINE_BACKEND_H_INCLUDED
