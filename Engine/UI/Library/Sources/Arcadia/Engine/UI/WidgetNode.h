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

#if !defined(ARCADIA_ENGINE_UI_WIDGETNODE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_WIDGETNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/Node.h"

typedef struct Arcadia_Engine_UI_WidgetEvent Arcadia_Engine_UI_WidgetEvent;

// A persistent sizing policy for widgets participating in layout.
// Fixed uses requestedWidth/requestedHeight; Content uses the measured content.
// Widgets without a content layout naturally have fixed sizing.
typedef enum Arcadia_Engine_UI_WidgetNodeSizePolicy {
  Arcadia_Engine_UI_WidgetNodeSizePolicy_Content,
  Arcadia_Engine_UI_WidgetNodeSizePolicy_Fixed,
} Arcadia_Engine_UI_WidgetNodeSizePolicy;

// The "UI widget" node.
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.WidgetNode", Arcadia_Engine_UI_WidgetNode,
                          u8"Arcadia.Engine.UI.Node")

struct Arcadia_Engine_UI_WidgetNodeDispatch {
  Arcadia_Engine_UI_NodeDispatch _parent;
  void (*render)(Arcadia_Thread* thread, Arcadia_Engine_UI_WidgetNode* self, Arcadia_Engine_Visuals_EnterPassNode* enterPassNode);
  void (*getPosition)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Integer32Value*, Arcadia_Integer32Value*);
  void (*getSize)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Integer32Value*, Arcadia_Integer32Value*);
  void (*setPosition)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Integer32Value, Arcadia_Integer32Value);
  void (*setSize)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Integer32Value, Arcadia_Integer32Value);
  void (*raiseSignal)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Engine_UI_WidgetEvent*);
  void (*measure)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Integer32Value, Arcadia_Integer32Value);
  void (*arrange)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Integer32Value, Arcadia_Integer32Value, Arcadia_Integer32Value, Arcadia_Integer32Value);
};

struct Arcadia_Engine_UI_WidgetNode {
  Arcadia_Engine_UI_Node _parent;
  Arcadia_Integer32Value left, bottom;
  Arcadia_Integer32Value width, height;
  // setSize/setBounds specify the requested size; arrange assigns actual bounds.
  Arcadia_Integer32Value requestedWidth, requestedHeight;
  // Desired size produced by the most recent measure call.
  Arcadia_Integer32Value desiredWidth, desiredHeight;
  // The general sizing semantics; containers choose how to measure content.
  Arcadia_Engine_UI_WidgetNodeSizePolicy sizePolicy;
  Arcadia_Signal* propertyChanged;
  Arcadia_String* name;
};

Arcadia_Engine_UI_WidgetNode*
Arcadia_Engine_UI_WidgetNode_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_Engine_UI_WidgetNode_render
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

/// @brief Get the position of this widget.
/// @param thread A pointer to this thread.
/// @param self A pointer to this widget.
/// @param left A pointer to a variable which is assigned the left of this widget.
/// @param bottom A pointer to a variable which is assigned the bottom of this widget.
void
Arcadia_Engine_UI_WidgetNode_getPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* bottom
  );

/// @brief Get the size of this widget.
/// @param thread A pointer to this thread.
/// @param self A pointer to this widget.
/// @param width A pointer to a variable which is assigned the width of this widget.
/// @param height A pointer to a variable which is assigned the height of this widget.
void
Arcadia_Engine_UI_WidgetNode_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

/// @brief Set the position of this widget.
/// @param thread A pointer to this thread.
/// @param self A pointer to this widget.
/// @param left The left of this widget.
/// @param bottom The bottom of this widget.
/// @error Arcadia_Status_ArgumentValueInvalid left is smaller than Arcadia_Integer32Value_Minimum / 2  or greater than Arcadia_Integer32Value_Maximum / 2
/// @error Arcadia_Status_ArgumentValueInvalid bottom is smaller than Arcadia_Integer32Value_Minimum / 2  or greater than Arcadia_Integer32Value_Maximum / 2
void
Arcadia_Engine_UI_WidgetNode_setPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom
  );

/// @brief Set the size of this widget.
/// @param thread A pointer to this thread.
/// @param self A pointer to this widget.
/// @param width The width of this widget.
/// @param height The height of this widget.
/// @error Arcadia_Status_ArgumentValueInvalid width is smaller than Arcadia_Integer32Value_Minimum / 2  or greater than Arcadia_Integer32Value_Maximum / 2
/// @error Arcadia_Status_ArgumentValueInvalid height is smaller than Arcadia_Integer32Value_Minimum / 2  or greater than Arcadia_Integer32Value_Maximum / 2
void
Arcadia_Engine_UI_WidgetNode_setSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

/// @brief Measure a widget within the available size without changing its actual bounds.
/// @remarks The default implementation reports the requested size (fixed sizing).
///          Derived widgets can override the layout dispatch slot.
void
Arcadia_Engine_UI_WidgetNode_measure
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value availableWidth,
    Arcadia_Integer32Value availableHeight
  );

/// @brief Read the size specified by the last setSize/setBounds call.
void
Arcadia_Engine_UI_WidgetNode_getRequestedSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

/// @brief Read the result of the most recent measure call (zero before the first call).
void
Arcadia_Engine_UI_WidgetNode_getDesiredSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

/// @brief Assign final bounds without overwriting the requested size.
/// @remarks The default implementation retains the existing position/size event semantics.
void
Arcadia_Engine_UI_WidgetNode_arrange
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

void
Arcadia_Engine_UI_WidgetNode_getBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* bottom,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

void
Arcadia_Engine_UI_WidgetNode_setBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

/// @brief Initialize bounds in a derived widget constructor.
/// @remarks Sets actual and requested geometry without emitting signals or
///          allocating managed objects while the derived visitor is inactive.
void
Arcadia_Engine_UI_WidgetNode_setInitialBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

Arcadia_Signal*
Arcadia_Engine_UI_WidgetNode_getPropertyChangedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self
  );

/// @brief Get the name of this widget.
/// @param thread A pointer to this thread.
/// @param self A pointer to this widget.
/// @return The name of this widget, or a null pointer if it has none.
Arcadia_String*
Arcadia_Engine_UI_WidgetNode_getName
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self
  );

/// @brief Set the name of this widget.
/// @param thread A pointer to this thread.
/// @param self A pointer to this widget.
/// @param name The name of this widget, or a null pointer to clear it.
void
Arcadia_Engine_UI_WidgetNode_setName
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_String* name
  );

/// @brief Raise the widget event signal.
/// @param thread A pointer to this thread.
/// @param self A pointer to this widget.
/// @param event The event.
void
Arcadia_Engine_UI_WidgetNode_raiseSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* self,
    Arcadia_Engine_UI_WidgetEvent* event
  );

#endif // ARCADIA_ENGINE_UI_WIDGETNODE_H_INCLUDED
