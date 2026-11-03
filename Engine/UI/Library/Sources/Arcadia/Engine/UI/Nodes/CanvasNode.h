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

// Optional keyboard interceptor for the currently active generic popup.
// It is owned by the canvas and automatically cleared when the popup closes.
typedef Arcadia_BooleanValue (*Arcadia_Engine_UI_CanvasPopupKeyboardHandler)
  (Arcadia_Thread*, Arcadia_Object*, Arcadia_Engine_Input_KeyboardKeyEvent*);

// A single capture handler may intercept keyboard and mouse-button input
// before focus traversal and hit testing. The caller retains ownership of its
// modal state; CanvasNode keeps the managed callback owner alive.
typedef Arcadia_BooleanValue (*Arcadia_Engine_UI_CanvasCaptureKeyboardHandler)
  (Arcadia_Thread*, Arcadia_Object*, Arcadia_Engine_Input_KeyboardKeyEvent*);
typedef Arcadia_BooleanValue (*Arcadia_Engine_UI_CanvasCaptureMouseHandler)
  (Arcadia_Thread*, Arcadia_Object*, Arcadia_Engine_Input_MouseButtonEvent*);

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
  /// The child widgets.
  Arcadia_List* children;
  Arcadia_Object* hoveredChild;
  Arcadia_Object* pressedChild;
  Arcadia_Object* focusedChild;
  // One canvas-owned modal popup, rendered above all normal children.
  Arcadia_Object* popup;
  Arcadia_Object* focusBeforePopup;
  // An outside press dismisses the popup but its release must not click through.
  Arcadia_BooleanValue consumePopupRelease;
  Arcadia_Object* popupKeyboardOwner;
  Arcadia_Engine_UI_CanvasPopupKeyboardHandler popupKeyboardHandler;
  Arcadia_Object* captureOwner;
  Arcadia_Engine_UI_CanvasCaptureKeyboardHandler captureKeyboardHandler;
  Arcadia_Engine_UI_CanvasCaptureMouseHandler captureMouseHandler;
  // Matching releases are swallowed even after capture has ended.
  Arcadia_Engine_Input_KeyboardKey suppressedCaptureKey;
  Arcadia_Natural64Value suppressedCaptureButton;
  Arcadia_BooleanValue suppressCaptureKeyRelease;
  Arcadia_BooleanValue suppressCaptureButtonRelease;
};

Arcadia_Engine_UI_CanvasNode*
Arcadia_Engine_UI_CanvasNode_create
  (
    Arcadia_Thread* thread
  );

/// @brief Open a single modal popup above ordinary canvas children.
/// @remarks Popup is retained by the canvas and must be a widget implementing
///          HitTestable, Renderable, KeyboardInput and PointerInput.
///          Popup must not be a direct canvas child. Reopening the same popup
///          is a no-op; a different popup replaces the old one.
void
Arcadia_Engine_UI_CanvasNode_showPopup
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Object* popup
  );

/// @brief Dismiss the active popup, optionally preserving a previous focus target.
/// @remarks A popup button may call this from within its own event callback.
void
Arcadia_Engine_UI_CanvasNode_hidePopup
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self
  );

/// @brief Return the current canvas modal overlay, or NULL.
Arcadia_Object*
Arcadia_Engine_UI_CanvasNode_getPopup
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self
  );

// Install a keyboard event handler for the active popup. It is invoked before
// the popup's normal KeyboardInput interface for keys other than Escape/Tab.
// The canvas retains owner while open; closing removes the callback and owner.
void
Arcadia_Engine_UI_CanvasNode_setPopupKeyboardHandler
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Object* owner,
    Arcadia_Engine_UI_CanvasPopupKeyboardHandler handler
  );

// The capture owner receives keyboard and mouse-button events before normal
// canvas routing. Only one owner is permitted; a popup and capture cannot
// both be modal. Capture cannot be installed without both callbacks.
void
Arcadia_Engine_UI_CanvasNode_beginInputCapture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Object* owner,
    Arcadia_Engine_UI_CanvasCaptureKeyboardHandler keyboard,
    Arcadia_Engine_UI_CanvasCaptureMouseHandler mouse
  );

void
Arcadia_Engine_UI_CanvasNode_endInputCapture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Object* owner
  );

void
Arcadia_Engine_UI_CanvasNode_suppressKeyboardRelease
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Engine_Input_KeyboardKey key
  );

void
Arcadia_Engine_UI_CanvasNode_suppressMouseButtonRelease
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Natural64Value button
  );

/// @brief Place a popup beneath an anchor when possible, otherwise above it.
/// @remarks Coordinates are in bottom-left canvas space. The popup is clamped
///          within the canvas; a caller may put a ScrollPanel inside it.
void
Arcadia_Engine_UI_CanvasNode_placePopup
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Engine_UI_WidgetNode* anchor,
    Arcadia_Engine_UI_WidgetNode* popup,
    Arcadia_Integer32Value requestedWidth,
    Arcadia_Integer32Value requestedHeight
  );

Arcadia_Engine_UI_CanvasNode*
Arcadia_Engine_UI_CanvasNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_CanvasDefinition* source
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
