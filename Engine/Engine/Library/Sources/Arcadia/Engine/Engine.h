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

#if !defined(ARCADIA_ENGINE_ENGINE_H_INCLUDED)
#define ARCADIA_ENGINE_ENGINE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Engine.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_Deque Arcadia_Deque;
typedef struct Arcadia_Engine Arcadia_Engine;
typedef struct Arcadia_Engine_BackendContext Arcadia_Engine_BackendContext;
typedef struct Arcadia_Engine_Event Arcadia_Engine_Event;
typedef struct Arcadia_Engine_NodeFactory Arcadia_Engine_NodeFactory;
typedef struct Arcadia_Set Arcadia_Set;

Arcadia_declareObjectType(u8"Arcadia.Engine", Arcadia_Engine,
                          u8"Arcadia.Object");

struct Arcadia_EngineDispatch {
  Arcadia_ObjectDispatch _parent;

  void (*update)(Arcadia_Thread*, Arcadia_Engine*);
};

struct Arcadia_Engine {
  Arcadia_Object _parent;
  Arcadia_Set* visualsBackendTypes;
  Arcadia_Set* visualsNodeFactoryTypes;
  Arcadia_Set* audialsBackendTypes;
  Arcadia_Set* audialsNodeFactoryTypes;
  Arcadia_Engine_BackendContext* visualsBackendContext;
  Arcadia_Engine_NodeFactory* visualsNodeFactory;
  Arcadia_Engine_BackendContext* audialsBackendContext;
  Arcadia_Engine_NodeFactory* audialsNodeFactory;
  Arcadia_Deque* events;
};

void
Arcadia_Engine_enqueEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self,
    Arcadia_Engine_Event* event
  );

Arcadia_Engine_Event*
Arcadia_Engine_dequeEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  );

void
Arcadia_Engine_update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  );

#endif // ARCADIA_ENGINE_ENGINE_H_INCLUDED
