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
#include "Arcadia/Math/Include.h"
#include "Arcadia/Engine/Visuals/Resources/ConstantBufferResource.h"
#include "Arcadia/Engine/Visuals/Resources/VertexBufferResource.h"
#include "Arcadia/Media/Include.h"

// A "mesh node" consists of a
// - a geometry: The geometry to render, providing the vertices of the mesh and its ambient color.
// - an ambient mesh color (required):
//   The ambient color used for each of the vertices if a) the vertices provide no color data and b) the material specifies the mesh as an ambient color source.
// - a vertex buffer: The vertex data of the geometry.
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.MeshNode", Arcadia_Engine_Visuals_MeshNode,
                          u8"Arcadia.Engine.Visuals.Node")

struct Arcadia_Engine_Visuals_MeshNodeDispatch {
  Arcadia_Engine_Visuals_NodeDispatch parent;
};

struct Arcadia_Engine_Visuals_MeshNode {
  Arcadia_Engine_Visuals_Node _parent;

  /// The dirty flags.
  Arcadia_Natural8Value dirtyFlags;

  /// The geometry to render.
  Arcadia_Media_Geometry* geometry;
  // The ambient color of this mesh.
  // This is a copy of the ambient color of the geometry, such that changing it does not affect the geometry or other mesh nodes.
  Arcadia_Math_Color4Real32* ambientColor;
  // The vertex buffer of this mesh.
  // This is a copy of the vertex data of the geometry, because mesh nodes sharing a geometry do not necessarily render the same vertices.
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
/// @param geometry A pointer to the geometry to render.
/// @return A pointer to the mesh node.
Arcadia_Engine_Visuals_MeshNode*
Arcadia_Engine_Visuals_MeshNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_Media_Geometry* geometry
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

/// @brief Get the vertex buffer of this mesh node.
/// @details This mesh node owns its vertex buffer, and rewrites its vertices when it renders something else. The vertex
///          buffer is therefore mutable and specific to this mesh node; it is not the vertex buffer of the geometry, which
///          stays unchanged.
/// @param thread A pointer to this thread.
/// @param self A pointer to this mesh node.
/// @return A pointer to the vertex buffer of this mesh node.
Arcadia_Media_VertexBuffer*
Arcadia_Engine_Visuals_MeshNode_getVertexBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self
  );

/// Mark the mesh node's mutable vertex buffer as changed so rendering uploads it again.
void
Arcadia_Engine_Visuals_MeshNode_markVerticesDirty
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self
  );

#endif // ARCADIA_ENGINE_VISUALS_NODES_MESHNODE_H_INCLUDED
