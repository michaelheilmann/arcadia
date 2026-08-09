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

#if !defined(ARCADIA_ENGINE_VISUALS_NODES_FRAMEBUFFERNODE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_NODES_FRAMEBUFFERNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Engine/Visuals/Node.h"
typedef struct Arcadia_Engine_Visuals_ViewportNode Arcadia_Engine_Visuals_ViewportNode;

// A "framebuffer" node may be attached the "rendering context" node.
// If a "framebuffer" node is attached, rendering is performed to the framebuffer of that node.
// Otherwise rendering is performed to the default framebuffer.
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.FrameBufferNode", Arcadia_Engine_Visuals_FrameBufferNode,
                          u8"Arcadia.Engine.Visuals.Node")

struct Arcadia_Engine_Visuals_FrameBufferNodeDispatch {
  Arcadia_Engine_Visuals_NodeDispatch parent;
  void (*setSize)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_FrameBufferNode*, Arcadia_Integer32Value, Arcadia_Integer32Value);
  void (*getSize)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_FrameBufferNode*, Arcadia_Integer32Value*, Arcadia_Integer32Value*);
};

struct Arcadia_Engine_Visuals_FrameBufferNode {
  Arcadia_Engine_Visuals_Node _parent;

  /// The width, in pixels, of the frame buffer.
  /// The default value is 320.
  Arcadia_Integer32Value width;

  /// The height, in pixels, of the frame buffer.
  /// The default value is 240.
  Arcadia_Integer32Value height;

  /// The backend context.
  Arcadia_Engine_Visuals_BackendContext* backendContext;

  /// The frame buffer resource.
  Arcadia_Engine_Visuals_FrameBufferResource* frameBufferResource;
};

/// @brief Create an frame buffer mode.
/// @param backendContext A pointer to the backend context or the null pointer.
/// @return A pointer to the frame buffer node.
Arcadia_Engine_Visuals_FrameBufferNode*
Arcadia_Engine_Visuals_FrameBufferNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

void
Arcadia_Engine_Visuals_FrameBufferNode_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

void
Arcadia_Engine_Visuals_FrameBufferNode_setSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

#endif // ARCADIA_ENGINE_VISUALS_NODES_FRAMEBUFFERNODE_H_INCLUDED
