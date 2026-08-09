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

#if !defined(ARCADIA_ENGINE_VISUALS_NODES_MESHNODE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_NODES_MESHNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Engine/Visuals/Node.h"
#include "Arcadia/Engine/Visuals/Resources/ConstantBufferResource.h"
#include "Arcadia/Engine/Visuals/Resources/VertexBufferResource.h"
#include "Arcadia/ADL/Include.h"

// A "mesh node" consists of a
// - an ambient mesh color (required):
//   The ambient color used for each of the vertices if a) the vertices provide no color data and b) the material specifies the mesh as an ambient color source.
// - a vertex descriptor: Must be of the form [xyz/position, rgba/ambient, uv/ambient]. Describes the data in the vertex buffer.
// - a vertex buffer: The vertex data. Either of the form [xyz/position] or [xyz/position, rgba/ambient].
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.MeshNode", Arcadia_Engine_Visuals_MeshNode,
                          u8"Arcadia.Engine.Visuals.Node")

struct Arcadia_Engine_Visuals_MeshNodeDispatch {
  Arcadia_Engine_Visuals_NodeDispatch parent;
};

struct Arcadia_Engine_Visuals_MeshNode {
  Arcadia_Engine_Visuals_Node _parent;

  /// The dirty flags.
  Arcadia_Natural8Value dirtyFlags;

  /// The mesh definition.
  Arcadia_ADL_MeshDefinition* source;
  // The ambient color of this mesh.
  Arcadia_Math_Color4Real32* ambientColor;
  // The vertex buffer of this mesh.
  Arcadia_Media_VertexBuffer* vertexBuffer;
  /// The backend context or a null pointer.
  Arcadia_Engine_Visuals_BackendContext* backendContext;
  /// The constant buffer of this mesh.
  /// This contains the mesh ambient color.
  Arcadia_Engine_Visuals_ConstantBufferResource* constantBufferResource;
  /// The vertex buffer of this mesh.
  /// This contains the vertices of the mesh.
  Arcadia_Engine_Visuals_VertexBufferResource* vertexBufferResource;
};

/// @brief Create a mesh node.
/// @param thread A pointer to this thread.
/// @param backendContext A pointer to the backend context or a null pointer.
/// @return A pointer to the mesh node.
Arcadia_Engine_Visuals_MeshNode*
Arcadia_Engine_Visuals_MeshNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MeshDefinition* source
  );

void
Arcadia_Engine_Visuals_MeshNode_getNumberOfVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self,
    Arcadia_SizeValue* numberOfVertices
  );

void
Arcadia_Engine_Visuals_MeshNode_setAmbientColor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self,
    Arcadia_Math_Color4Real32* baseColor
  );

Arcadia_Math_Color4Real32*
Arcadia_Engine_Visuals_MeshNode_getAmbientColor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self
  );

#endif // ARCADIA_ENGINE_VISUALS_NODES_MESHNODE_H_INCLUDED
