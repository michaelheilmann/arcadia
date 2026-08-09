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

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Math/Include.h"
typedef struct Arcadia_Engine_Visuals_EnterPassResource Arcadia_Engine_Visuals_EnterPassResource;
typedef struct Arcadia_Engine_Visuals_BackendContext Arcadia_Engine_Visuals_Implementation_BackendContext;

// A "resource" is owned by a "backend context". That is, the "backend context" holds a STRONG reference to its "resources".
// In addition, the "backend context" retains a GC lock unless its "resources" such that they are only gc'ed if the "backend context" drops this lock.
Arcadia_declareObjectType(u8"Arcadia.Visuals.Resource", Arcadia_Engine_Visuals_Resource,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Visuals_ResourceDispatch {
  Arcadia_ObjectDispatch _parent;

  void (*load)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_Resource* self);
  void (*unload)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_Resource* self);
  void (*unlink)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_Resource* self);
  void (*render)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_Resource* self, Arcadia_Engine_Visuals_EnterPassResource*);
};

struct Arcadia_Engine_Visuals_Resource {
  Arcadia_Object _parent;
  // The reference count of this resources.
  Arcadia_Integer32Value referenceCount;
  // Unmanaged reference to the "backend context" or the null reference.
  Arcadia_Engine_Visuals_Implementation_BackendContext* context;
};

void
Arcadia_Engine_Visuals_Resource_load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  );

// Unload the "backend resource" of this resource.
// "Unloading" is a reversible action.
void
Arcadia_Engine_Visuals_Resource_unload
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  );

// Unlink the of this resource.
// "Unlinking" is an irreversible action.
// The resource must be unloaded before unlinking.
// Unlinking removes all references to:
// a) the backend context
// b) if this resource is referencing a resource:
//    A referencing resource increments the reference count of a referenced resource (usually).
//    This increment is reversed when unlinking the referencing resource.
void
Arcadia_Engine_Visuals_Resource_unlink
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  );

// Visit this resource during a "rendering".
// a) ensure the resource back representation is created / updated
// b) the "rendering context" resource is updated
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
