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

#if !defined(ARCADIA_ENGINE_VISUALS_NODES_ENTERPASSNODE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_NODES_ENTERPASSNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Engine/Visuals/Node.h"
typedef struct Arcadia_Engine_Visuals_CameraNode Arcadia_Engine_Visuals_CameraNode;
typedef struct Arcadia_Engine_Visuals_ViewportNode Arcadia_Engine_Visuals_ViewportNode;
typedef struct Arcadia_Engine_Visuals_FrameBufferNode Arcadia_Engine_Visuals_FrameBufferNode;

// An enter pass node.
// Inputs:
//  - viewport
//  - camera
//  - canvas size
//
// Actions:
// - Updates the viewport and the camera with the canvas size information.
// - Writes the viewport information and the camera information into a pass buffer.
// - Viewport clears the depth buffer (if enabled).
// - Viewport clears the color buffer (if enabled).
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.EnterPassNode", Arcadia_Engine_Visuals_EnterPassNode,
                          u8"Arcadia.Engine.Visuals.Node");

struct Arcadia_Engine_Visuals_EnterPassNodeDispatch {
  Arcadia_Engine_Visuals_NodeDispatch parent;
};

struct Arcadia_Engine_Visuals_EnterPassNode {
  Arcadia_Engine_Visuals_Node parent;

  // A pointer to the camera node.
  Arcadia_Engine_Visuals_CameraNode* cameraNode;
  // A pointer to the viewport node.
  Arcadia_Engine_Visuals_ViewportNode* viewportNode;

  // A pointer to the frame buffer node (render to frame buffer) or null (render to primary frame buffer).
  Arcadia_Engine_Visuals_FrameBufferNode* frameBufferNode;

  struct {
    // @todo Remove this. We can obtain that from the rendering context node -> camera node -> viewport node path.
    // Default is 193.
    Arcadia_Real32Value red;
    // @todo Remove this. We can obtain that from the rendering context node -> camera node -> viewport node path.
    // Default is 216.
    Arcadia_Real32Value green;
    // @todo Remove this. We can obtain that from the rendering context node -> camera node -> viewport node path.
    // Default is 195.
    Arcadia_Real32Value blue;
    // @todo Remove this. We can obtain that from the rendering context node -> camera node -> viewport node path.
    // Default is 255.
    Arcadia_Real32Value alpha;
  } viewportClearColor;

  // @todo Remove this. We can obtain that from the rendering context node -> camera node -> viewport node path.
  // The default value is 1.
  Arcadia_Real32Value viewportClearDepth;

  // @todo Remove this. We can obtain that from the rendering context node -> camera node -> viewport node path.
  // The relative viewport rectangle.
  // The default value is 0, 0, 1, 1.
  struct {
    Arcadia_Real32Value left, bottom, right, top;
  } relativeViewportRectangle;

  // @todo Remove this. We can obtain that from the rendering context node -> camera node -> viewport node path.
  // The size of the canvas.
  // The default value is 320 x 240.
  struct {
    Arcadia_Real32Value width, height;
  } canvasSize;

  // @todo Remove this. We can obtain that from the rendering context node -> camera node.
  // The view matrix.
  // The default value is the identity matrix.
  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix;

  // @todo Remove this. We can obtain that from the rendering context node -> camera node.
  // The projection matrix.
  // The default value is the identity matrix.
  Arcadia_Math_Matrix4Real32* worldToViewMatrix;

  // The backend context.
  Arcadia_Engine_Visuals_BackendContext* backendContext;

  // Theenter pass resource.
  Arcadia_Engine_Visuals_EnterPassResource* enterPassResource;
};

/// @brief Create an enter pass mode.
/// @param thread A pointer to this thread.
/// @param backendContext A pointer to the backend context or a null pointer.
/// @return A pointer to the enter pass node.
Arcadia_Engine_Visuals_EnterPassNode*
Arcadia_Engine_Visuals_EnterPassNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

void
Arcadia_Engine_Visuals_EnterPassNode_setViewToProjectionMatrix
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassNode* self,
    Arcadia_Math_Matrix4Real32* viewToProjectionMatrix
  );

void
Arcadia_Engine_Visuals_EnterPassNode_setWorldToViewMatrix
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassNode* self,
    Arcadia_Math_Matrix4Real32* worldToViewMatrix
  );

void
Arcadia_Engine_Visuals_EnterPassNode_setFrameBufferNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassNode* self,
    Arcadia_Engine_Visuals_FrameBufferNode* frameBufferNode
  );

Arcadia_Engine_Visuals_FrameBufferNode*
Arcadia_Engine_Visuals_EnterPassNode_getFrameBufferNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassNode* self
  );

void
Arcadia_Engine_Visuals_EnterPassNode_setViewportNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassNode* self,
    Arcadia_Engine_Visuals_ViewportNode* viewportNode
  );

Arcadia_Engine_Visuals_ViewportNode*
Arcadia_Engine_Visuals_EnterPassNode_getViewportNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassNode* self
  );

void
Arcadia_Engine_Visuals_EnterPassNode_setCameraNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassNode* self,
    Arcadia_Engine_Visuals_CameraNode* cameraNode
  );

Arcadia_Engine_Visuals_CameraNode*
Arcadia_Engine_Visuals_EnterPassNode_getCameraNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_EnterPassNode* self
  );

#endif // ARCADIA_ENGINE_VISUALS_NODES_ENTERPASSNODE_H_INCLUDED
