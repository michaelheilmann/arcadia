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

#if !defined(ARCADIA_ENGINE_VISUALS_NODES_PIXELBUFFERNODE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_NODES_PIXELBUFFERNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Engine/Visuals/Node.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/PixelBufferIO/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.PixelBufferNode", Arcadia_Engine_Visuals_PixelBufferNode,
                          u8"Arcadia.Engine.Visuals.Node")

struct Arcadia_Engine_Visuals_PixelBufferNodeDispatch {
  Arcadia_Engine_Visuals_NodeDispatch parent;
};

struct Arcadia_Engine_Visuals_PixelBufferNode {
  Arcadia_Engine_Visuals_Node parent;
  // The pixel buffer definition.
  Arcadia_ADL_PixelBufferDefinition* source;
  // The pixel buffer.
  Arcadia_Media_PixelBuffer* pixelBuffer;
  // The backend context.
  Arcadia_Engine_Visuals_BackendContext* backendContext;
};

/// @brief Create pixel buffer node.
/// @param thread A pointer to this thread.
/// @param backendContext A pointer to the backend context or a null pointer.
/// @return A pointer to the pixel buffer node.
Arcadia_Engine_Visuals_PixelBufferNode*
Arcadia_Engine_Visuals_PixelBufferNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_PixelBufferDefinition* source
  );

#endif // ARCADIA_ENGINE_VISUALS_NODES_PIXELBUFFERNODE_H_INCLUDED
