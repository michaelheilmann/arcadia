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

#if !defined(ARCADIA_ENGINE_NODE_H_INCLUDED)
#define ARCADIA_ENGINE_NODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Node.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_Engine_Audials_BackendContext Arcadia_Engine_Audials_BackendContext;
typedef struct Arcadia_Engine_Node Arcadia_Engine_Node;
typedef struct Arcadia_Engine_Visuals_BackendContext Arcadia_Engine_Visuals_BackendContext;

Arcadia_declareObjectType(u8"Arcadia.Engine.Node", Arcadia_Engine_Node,
                          u8"Arcadia.Object");

struct Arcadia_Engine_NodeDispatch {
  Arcadia_ObjectDispatch _parent;

  void (*setAudialsBackendContext)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Audials_BackendContext*);

  void (*setVisualsBackendContext)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*);
};

struct Arcadia_Engine_Node {
  Arcadia_Object _parent;
};

void
Arcadia_Engine_Node_setAudialsBackendContext
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Node* self,
    Arcadia_Engine_Audials_BackendContext* backendContext
  );

void
Arcadia_Engine_Node_setVisualsBackendContext
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Node* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

#endif // ARCADIA_ENGINE_NODE_H_INCLUDED
