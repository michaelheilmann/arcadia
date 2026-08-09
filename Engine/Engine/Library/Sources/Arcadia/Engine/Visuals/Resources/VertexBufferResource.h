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

#include "Arcadia/Media/Include.h"
#include "Arcadia/Engine/Visuals/Resource.h"

/// @brief Flag indicating the vertex data of a vertex buffer resource is dirty.
#define Arcadia_Engine_Visuals_VertexBufferResource_VertexDataDirty (1)

/// @brief Flag indicating the vertex descriptor of a vertex buffer resource is dirty.
#define Arcadia_Engine_Visuals_VertexBufferResource_VertexDescriptorDirty (2)

Arcadia_declareObjectType(u8"Arcadia.Visuals.VertexBufferResource", Arcadia_Engine_Visuals_VertexBufferResource,
                          u8"Arcadia.Visuals.Resource");

struct Arcadia_Engine_Visuals_VertexBufferResourceDispatch {
  Arcadia_Engine_Visuals_ResourceDispatch _parent;
  
  Arcadia_Media_VertexDescriptor* (*getVertexDescriptor)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_VertexBufferResource* self);
  void (*setData)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_VertexBufferResource* self, Arcadia_Media_VertexDescriptor* vertexDescriptor, Arcadia_SizeValue numberOfVertices, const void* bytes, Arcadia_SizeValue numberOfBytes);
  Arcadia_SizeValue (*getNumberOfVertices)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_VertexBufferResource* self);
};

struct Arcadia_Engine_Visuals_VertexBufferResource {
  Arcadia_Engine_Visuals_Resource _parent;

  Arcadia_Natural8Value dirty;

  Arcadia_Media_VertexDescriptor* vertexDescriptor;

  Arcadia_SizeValue numberOfVertices;

  void* bytes;
  Arcadia_SizeValue numberOfBytes;
};

Arcadia_SizeValue
Arcadia_Engine_Visuals_VertexBufferResource_getNumberOfVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  );

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

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_VERTEXBUFFERRESOURCE_H_INCLUDED
