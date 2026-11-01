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

#if !defined(ARCADIA_ENGINE_VISUALS_RESOURCES_VERTEXBUFFERRESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_RESOURCES_VERTEXBUFFERRESOURCE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Visuals/Resources/VertexBufferResource.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/Visuals/Resource.h"

typedef struct Arcadia_Engine_Visuals_VertexBufferResource Arcadia_Engine_Visuals_VertexBufferResource;
typedef struct Arcadia_Media_VertexDescriptor Arcadia_Media_VertexDescriptor;

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.VertexBufferResource", Arcadia_Engine_Visuals_VertexBufferResource,
                          u8"Arcadia.Engine.Visuals.Resource");

struct Arcadia_Engine_Visuals_VertexBufferResourceDispatch {
  Arcadia_Engine_Visuals_ResourceDispatch _parent;

  Arcadia_Media_VertexDescriptor* (*getVertexDescriptor)(Arcadia_Thread*, Arcadia_Engine_Visuals_VertexBufferResource*);

  void (*setData)(Arcadia_Thread*, Arcadia_Engine_Visuals_VertexBufferResource*, Arcadia_Media_VertexDescriptor*, Arcadia_SizeValue, const void*, Arcadia_SizeValue);

  Arcadia_SizeValue (*getNumberOfVertices)(Arcadia_Thread*, Arcadia_Engine_Visuals_VertexBufferResource*);
};

struct Arcadia_Engine_Visuals_VertexBufferResource {
  Arcadia_Engine_Visuals_Resource _parent;
  Arcadia_Natural8Value dirty;
  Arcadia_Media_VertexDescriptor* vertexDescriptor;
  Arcadia_SizeValue numberOfVertices;
  void* bytes;
  Arcadia_SizeValue numberOfBytes;
};

#define Arcadia_Engine_Visuals_VertexBufferResource_VertexDataDirty INT32_C(1)

#define Arcadia_Engine_Visuals_VertexBufferResource_VertexDescriptorDirty INT32_C(2)

Arcadia_Media_VertexDescriptor*
Arcadia_Engine_Visuals_VertexBufferResource_getVertexDescriptor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  );

void
Arcadia_Engine_Visuals_VertexBufferResource_setData
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self,
    Arcadia_Media_VertexDescriptor* vertexDescriptor,
    Arcadia_SizeValue numberOfVertices,
    const void* bytes,
    Arcadia_SizeValue numberOfBytes
  );

Arcadia_SizeValue
Arcadia_Engine_Visuals_VertexBufferResource_getNumberOfVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  );

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_VERTEXBUFFERRESOURCE_H_INCLUDED
