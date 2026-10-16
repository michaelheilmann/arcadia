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

#if !defined(ARCADIA_ENGINE_VISUALS_RESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_RESOURCE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Visuals/Resource.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_Engine_Visuals_BackendContext Arcadia_Engine_Visuals_BackendContext;
typedef struct Arcadia_Engine_Visuals_EnterPassResource Arcadia_Engine_Visuals_EnterPassResource;
typedef struct Arcadia_Engine_Visuals_Resource Arcadia_Engine_Visuals_Resource;

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.Resource", Arcadia_Engine_Visuals_Resource,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Visuals_ResourceDispatch {
  Arcadia_ObjectDispatch _parent;

  void (*load)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*);

  void (*unload)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*);

  void (*unlink)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*);

  void (*render)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*, Arcadia_Engine_Visuals_EnterPassResource*);
};

struct Arcadia_Engine_Visuals_Resource {
  Arcadia_Object _parent;
  Arcadia_Integer32Value referenceCount;
  Arcadia_Engine_Visuals_BackendContext* context;
};

void
Arcadia_Engine_Visuals_Resource_load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  );

void
Arcadia_Engine_Visuals_Resource_unload
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  );

void
Arcadia_Engine_Visuals_Resource_unlink
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  );

void
Arcadia_Engine_Visuals_Resource_render
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  );

void
Arcadia_Engine_Visuals_Resource_ref
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  );

void
Arcadia_Engine_Visuals_Resource_unref
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  );

#endif // ARCADIA_ENGINE_VISUALS_RESOURCE_H_INCLUDED
