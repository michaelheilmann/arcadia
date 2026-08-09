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

#if !defined(ARCADIA_ENGINE_VISUALS_NODES_VIEWPORTNODE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_NODES_VIEWPORTNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Engine/Visuals/Node.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.ViewportNode", Arcadia_Engine_Visuals_ViewportNode,
                          u8"Arcadia.Engine.Visuals.Node");

struct Arcadia_Engine_Visuals_ViewportNodeDispatch {
  Arcadia_Engine_Visuals_NodeDispatch parent;

  Arcadia_BooleanValue
  (*getClearColorBuffer)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self
    );

  void
  (*setClearColorBuffer)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self,
      Arcadia_BooleanValue clearColorBuffer
    );

  Arcadia_Math_Color4Real32*
  (*getClearColor)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self
    );

  void
  (*setClearColor)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self,
      Arcadia_Math_Color4Real32* clearColor
    );

  Arcadia_BooleanValue
  (*getClearDepthBuffer)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self
    );

  void
  (*setClearDepthBuffer)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self,
      Arcadia_BooleanValue clearDepthBuffer
    );

  Arcadia_Real32Value
  (*getClearDepth)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self
    );

  void
  (*setClearDepth)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self,
      Arcadia_Real32Value depth
    );

  void
  (*getRelativeViewportRectangle)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self,
      Arcadia_Real32Value* left,
      Arcadia_Real32Value* bottom,
      Arcadia_Real32Value* right,
      Arcadia_Real32Value* top
    );

  void
  (*setRelativeViewportRectangle)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self,
      Arcadia_Real32Value left,
      Arcadia_Real32Value bottom,
      Arcadia_Real32Value right,
      Arcadia_Real32Value top
    );

  void
  (*getCanvasSize)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self,
      Arcadia_Real32Value* width,
      Arcadia_Real32Value* height
    );

  void
  (*setCanvasSize)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_ViewportNode* self,
      Arcadia_Real32Value width,
      Arcadia_Real32Value height
    );
};

struct Arcadia_Engine_Visuals_ViewportNode {
  Arcadia_Engine_Visuals_Node _parent;

  Arcadia_Natural8Value dirtyBits;

  Arcadia_Engine_Visuals_BackendContext* backendContext;

  // If the color buffer should be cleared.
  // Default value is #Arcadia_BooleanValue_True.
  Arcadia_BooleanValue clearColorBuffer;
  // Default value is 193, 216, 195, 255.
  Arcadia_Math_Color4Real32* clearColor;

  // If the depth buffer should be cleared.
  // Default value is #Arcadia_BooleanValue_True.
  Arcadia_BooleanValue clearDepthBuffer;
  // Default is 1.
  Arcadia_Real32Value clearDepth;

  struct {
    // Default is 0.
    Arcadia_Real32Value left;
    // Default is 0.
    Arcadia_Real32Value bottom;
    // Default is 1.
    Arcadia_Real32Value right;
    // Default is 1.
    Arcadia_Real32Value top;
  } relativeViewportRectangle;

  struct {
    // The width of the canvas.
    // Default is 320.
    Arcadia_Real32Value width;
    // The height of the canvas.
    // Default is 240.
    Arcadia_Real32Value height;
  } canvasSize;
};

/// @brief Create a viewport node.
/// @param thread A pointer to this thread.
/// @param backendContext A pointer to the backend context or a null pointer.
/// @return A pointer to the viewport node.
Arcadia_Engine_Visuals_ViewportNode*
Arcadia_Engine_Visuals_ViewportNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

Arcadia_BooleanValue
Arcadia_Engine_Visuals_ViewportNode_getClearColorBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  );

/// Set if clearing the color buffer is enabled.
/// Default value is #Arcadia_BooleanValue_True.
void
Arcadia_Engine_Visuals_ViewportNode_setClearColorBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_BooleanValue clearColorBuffer
  );

Arcadia_Math_Color4Real32*
Arcadia_Engine_Visuals_ViewportNode_getClearColor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  );

/// Set the clear color.
/// Default values are red = 193, green = 216, blue = 195, and alpha = 255.
void
Arcadia_Engine_Visuals_ViewportNode_setClearColor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Math_Color4Real32* clearColor
  );

Arcadia_BooleanValue
Arcadia_Engine_Visuals_ViewportNode_getClearDepthBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  );

/// Set if clearing the depth buffer is enabled.
/// Default value is #Arcadia_BooleanValue_True.
void
Arcadia_Engine_Visuals_ViewportNode_setClearDepthBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_BooleanValue clearDepthBuffer
  );

Arcadia_Real32Value
Arcadia_Engine_Visuals_ViewportNode_getClearDepth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self
  );

/// Set the clear depth.
/// Default value is depth = 1.
void
Arcadia_Engine_Visuals_ViewportNode_setClearDepth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value depth
  );

void
Arcadia_Engine_Visuals_ViewportNode_getRelativeViewportRectangle
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value* left,
    Arcadia_Real32Value* bottom,
    Arcadia_Real32Value* right,
    Arcadia_Real32Value* top
  );

/// Set the viewport relative rectangle.
/// x axis going from left to right, y axis going from bottom to top
/// Default values are left = 0, bottom = 0, right = 1, and top  = 1.
void
Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangle
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top
  );

void
Arcadia_Engine_Visuals_ViewportNode_getCanvasSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value* width,
    Arcadia_Real32Value* height
  );

/// Set the canvas size.
/// Default values are width = 320 and height = 240.
void
Arcadia_Engine_Visuals_ViewportNode_setCanvasSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ViewportNode* self,
    Arcadia_Real32Value width,
    Arcadia_Real32Value height
  );

#endif // ARCADIA_ENGINE_VISUALS_NODES_VIEWPORTNODE_H_INCLUDED
