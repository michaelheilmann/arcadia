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

#if !defined(ARCADIA_ENGINE_UI_NODES_CANVASNODE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NODES_CANVASNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/WidgetNode.h"
#include "Arcadia/Engine/Include.h"

// The "UI canvas" node.
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.CanvasNode", Arcadia_Engine_UI_CanvasNode,
                          u8"Arcadia.Engine.UI.WidgetNode")

struct Arcadia_Engine_UI_CanvasNodeDispatch {
  Arcadia_Engine_UI_WidgetNodeDispatch _parent;
};

struct Arcadia_Engine_UI_CanvasNode {
  Arcadia_Engine_UI_WidgetNode _parent;
  /// The width of the visual canvas.
  Arcadia_Integer32Value visualsCanvasWidth;
  /// The height of the visual canvas.
  Arcadia_Integer32Value visualsCanvasHeight;
  /// The viewport node.
  Arcadia_Engine_Visuals_ViewportNode* viewportNode;
  /// The camera node.
  Arcadia_Engine_Visuals_CameraNode* cameraNode;
  /// The list of rectangles.
  Arcadia_List* rectangles;
};

Arcadia_Engine_UI_CanvasNode*
Arcadia_Engine_UI_CanvasNode_create
  (
    Arcadia_Thread* thread
  );

/// @brief Set the size of the visuals canvass.
/// @param self A pointer to this UI canvas.
/// @param width The width of the visuals canvas.
/// @param height The height of the visuals canvas.
void
Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

void
Arcadia_Engine_UI_CanvasNode_updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

#endif // ARCADIA_ENGINE_UI_NODES_CANVASNODE_H_INCLUDED
