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

#define ARCADIA_ENGINE_UI_PRIVATE (1)
#include "Arcadia/Engine/UI/Nodes/LayoutPanelNode.h"
#include "Arcadia/Engine/UI/Include.h"
#include <stdint.h>


static Arcadia_BooleanValue
hasInterface
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_InterfaceType* type
  )
{
  return object && NULL != Arcadia_ObjectType_getInterfaceDispatch(thread,
    (Arcadia_ObjectType*)Arcadia_Object_getType(thread, object), type);
}

static void
setHovered
  (
    Arcadia_Thread* thread,
    Arcadia_Object* child,
    Arcadia_BooleanValue value
  )
{
  if (!child) return;
  if (Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_ButtonNode_getType(thread))) {
    Arcadia_Engine_UI_ButtonNode_setHovered(thread, (Arcadia_Engine_UI_ButtonNode*)child, value);
  } else if (Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_SegmentedChoiceNode_getType(thread))) {
    Arcadia_Engine_UI_SegmentedChoiceNode_setHovered(thread, (Arcadia_Engine_UI_SegmentedChoiceNode*)child, value);
  } else if (Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_SliderNode_getType(thread))) {
    Arcadia_Engine_UI_SliderNode_setHovered(thread, (Arcadia_Engine_UI_SliderNode*)child, value);
  } else if (Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_ToggleNode_getType(thread))) {
    Arcadia_Engine_UI_ToggleNode_setHovered(thread, (Arcadia_Engine_UI_ToggleNode*)child, value);
  } else if (!value && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_LayoutPanelNode_getType(thread))) {
    Arcadia_Engine_UI_LayoutPanelNode_clearHover(thread, (Arcadia_Engine_UI_LayoutPanelNode*)child);
  }
}

// The weak receiver is this panel. The signal itself owns the connection.
static void
onChildGeometryChanged
  (
    Arcadia_Thread* thread
  )
{
  if (Arcadia_ValueStack_getNatural8Value(thread, 0) != 3) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_LayoutPanelNode* panel =
    (Arcadia_Engine_UI_LayoutPanelNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3,
      _Arcadia_Engine_UI_LayoutPanelNode_getType(thread));
  // Ignore bounds notifications caused by the active arrange pass.
  if (!panel->layoutInProgress) panel->layoutDirty = Arcadia_BooleanValue_True;
}

static void
focusChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Object* child
  )
{
  if (self->focusedChild == child) return;
  if (hasInterface(thread, self->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Focusable_getType(thread)))
    Arcadia_Engine_UI_Focusable_blur(thread, self->focusedChild);
  self->focusedChild = child;
  if (hasInterface(thread, child, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Focusable_getType(thread)))
    Arcadia_Engine_UI_Focusable_focus(thread, child);
}

static Arcadia_BooleanValue
focusNextChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  )
{
  Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children);
  if (!n) return Arcadia_BooleanValue_False;
  Arcadia_SizeValue start = 0;
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    if (Arcadia_List_getObjectReferenceValueAt(thread, self->children, i) == self->focusedChild) {
      start = i + 1;
      break;
    }
  }
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, (start + i) % n);
    if (hasInterface(thread, child, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Focusable_getType(thread))) {
      focusChild(thread, self, child);
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
hitTestImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  Arcadia_Engine_UI_WidgetNode* widget = (Arcadia_Engine_UI_WidgetNode*)object;
  return x >= widget->left && x < widget->left + widget->width &&
         y >= widget->bottom && y < widget->bottom + widget->height;
}

static Arcadia_BooleanValue
hasDirtyDescendant
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children); i < n; ++i) {
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i);
    if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_LayoutPanelNode_getType(thread))) {
      Arcadia_Engine_UI_LayoutPanelNode* panel = (Arcadia_Engine_UI_LayoutPanelNode*)child;
      if (panel->layoutDirty || hasDirtyDescendant(thread, panel)) return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_Object*
hitTestChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, self);
  if (!hitTestImpl(thread, (Arcadia_Object*)self, x, y)) return NULL;
  for (Arcadia_SizeValue i = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children); i > 0; --i) {
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i - 1);
    if (hasInterface(thread, child, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_HitTestable_getType(thread)) &&
        Arcadia_Engine_UI_HitTestable_hitTest(thread, child, x, y)) return child;
  }
  return NULL;
}

static void
focusImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{ ((Arcadia_Engine_UI_LayoutPanelNode*)object)->focused = Arcadia_BooleanValue_True; }

static void
blurImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{
  Arcadia_Engine_UI_LayoutPanelNode* self = (Arcadia_Engine_UI_LayoutPanelNode*)object;
  self->focused = Arcadia_BooleanValue_False;
  focusChild(thread, self, NULL);
}

static Arcadia_BooleanValue
isFocusedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{ return ((Arcadia_Engine_UI_LayoutPanelNode*)object)->focused; }

static Arcadia_BooleanValue
handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  Arcadia_Engine_UI_LayoutPanelNode* self = (Arcadia_Engine_UI_LayoutPanelNode*)object;
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
      Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Tabulator) {
    if (focusNextChild(thread, self)) return Arcadia_BooleanValue_True;
  }
  if (hasInterface(thread, self->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_KeyboardInput_getType(thread)))
    return Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, self->focusedChild, event);
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  Arcadia_Engine_UI_LayoutPanelNode* self = (Arcadia_Engine_UI_LayoutPanelNode*)object;
  Arcadia_Object* child = event->action == Arcadia_Engine_Input_MousePointerAction_Exited ? NULL :
    hitTestChild(thread, self, event->x, (Arcadia_Integer32Value)self->lastCanvasHeight - event->y);
  if (event->action == Arcadia_Engine_Input_MousePointerAction_Scrolled) {
    if (hasInterface(thread, child, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread)))
      return Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, child, event);
    return Arcadia_BooleanValue_False;
  }
  if (self->hoveredChild != child) {
    setHovered(thread, self->hoveredChild, Arcadia_BooleanValue_False);
    setHovered(thread, child, Arcadia_BooleanValue_True);
  }
  self->hoveredChild = child;
  Arcadia_Object* receiver = self->pressedChild ? self->pressedChild : child;
  if (hasInterface(thread, receiver, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread)))
    return Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, receiver, event);
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  Arcadia_Engine_UI_LayoutPanelNode* self = (Arcadia_Engine_UI_LayoutPanelNode*)object;
  Arcadia_Object* child = hitTestChild(thread, self, event->x, (Arcadia_Integer32Value)self->lastCanvasHeight - event->y);
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Pressed) {
    self->pressedChild = child;
    if (child) focusChild(thread, self, child);
  }
  Arcadia_Object* receiver = self->pressedChild ? self->pressedChild : child;
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Released) self->pressedChild = NULL;
  if (hasInterface(thread, receiver, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread)))
    return Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, receiver, event);
  return Arcadia_BooleanValue_False;
}

static void
initializeFocusableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  )
{
  self->focus = &focusImpl;
  self->blur = &blurImpl;
  self->isFocused = &isFocusedImpl;
}

static void
initializeHitTestableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_HitTestableDispatch* self
  )
{ self->hitTest = &hitTestImpl; }

static void
initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  )
{ self->handleKeyboardKeyEvent = &handleKeyboardKeyEventImpl; }

static void
initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  )
{
  self->handleMousePointerEvent = &handleMousePointerEventImpl;
  self->handleMouseButtonEvent = &handleMouseButtonEventImpl;
}

static Arcadia_SizeValue
findChildIndex
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* panel,
    Arcadia_Object* child
  )
{
  Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)panel->children);
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    if (Arcadia_List_getObjectReferenceValueAt(thread, panel->children, i) == child) return i;
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
  return 0;
}

static void
ensureChildLayoutSlotCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_SizeValue count
  )
{
  if (count <= self->childLayoutSlotCapacity) return;
  Arcadia_SizeValue limit = SIZE_MAX / sizeof(Arcadia_Engine_UI_StackLayoutSlot);
  if (count > limit) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue capacity = self->childLayoutSlotCapacity ? self->childLayoutSlotCapacity : 8;
  while (capacity < count) {
    if (capacity > limit / 2) { capacity = count; break; }
    capacity *= 2;
  }
  Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->childLayoutSlots,
    capacity * sizeof(Arcadia_Engine_UI_StackLayoutSlot));
  self->childLayoutSlotCapacity = capacity;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_LayoutPanelNode);
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->childLayoutSlots = NULL;
  self->childLayoutSlotCapacity = 0;
  self->children = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  // The derived visitor is not active until the constructor returns.
  // Root the first list while allocating the second one.
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self->children);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    self->childConnections = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_popValues(thread, 1);
  self->layout = NULL;
  self->backgroundPanelNode = NULL;
  self->backgroundBorderWidth = 0;
  self->hoveredChild = NULL;
  self->pressedChild = NULL;
  self->focusedChild = NULL;
  self->lastCanvasHeight = 0.f;
  self->focused = Arcadia_BooleanValue_False;
  ((Arcadia_Engine_UI_WidgetNode*)self)->sizePolicy = Arcadia_Engine_UI_WidgetNodeSizePolicy_Content;
  self->layoutDirty = Arcadia_BooleanValue_True;
  self->layoutInProgress = Arcadia_BooleanValue_False;
  self->hasArranged = Arcadia_BooleanValue_False;
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_LayoutPanelNode);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  )
{
  if (self->childLayoutSlots) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->childLayoutSlots);
    self->childLayoutSlots = NULL;
    self->childLayoutSlotCapacity = 0;
  }
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  )
{
  if (self->children) Arcadia_Object_visit(thread, (Arcadia_Object*)self->children);
  if (self->childConnections) Arcadia_Object_visit(thread, (Arcadia_Object*)self->childConnections);
  if (self->layout) Arcadia_Object_visit(thread, self->layout);
  if (self->backgroundPanelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->backgroundPanelNode);
  if (self->hoveredChild) Arcadia_Object_visit(thread, self->hoveredChild);
  if (self->pressedChild) Arcadia_Object_visit(thread, self->pressedChild);
  if (self->focusedChild) Arcadia_Object_visit(thread, self->focusedChild);
}

static Arcadia_Natural64Value
getChildCountImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  return Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_UI_LayoutPanelNode*)self)->children);
}

static Arcadia_Object*
getChildAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value index
  )
{
  return Arcadia_List_getObjectReferenceValueAt(thread, ((Arcadia_Engine_UI_LayoutPanelNode*)self)->children, index);
}

static void
addChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  )
{
  Arcadia_Engine_UI_LayoutPanelNode* panel = (Arcadia_Engine_UI_LayoutPanelNode*)self;
  Arcadia_SizeValue index = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)panel->children);
  ensureChildLayoutSlotCapacity(thread, panel, index + 1);
  Arcadia_Value connection = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_WidgetNode_getType(thread))) {
    Arcadia_Slot* slot = Arcadia_Signal_connect(thread,
      Arcadia_Engine_UI_WidgetNode_getPropertyChangedSignal(thread, (Arcadia_Engine_UI_WidgetNode*)child),
      self, &onChildGeometryChanged);
    connection = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)slot);
  }
  // The child list and observer list must be kept in lockstep on failure.
  Arcadia_BooleanValue connectionAdded = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_List_insertBack(thread, panel->childConnections, connection);
    connectionAdded = Arcadia_BooleanValue_True;
    Arcadia_List_insertBackObjectReferenceValue(thread, panel->children, child);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (connectionAdded) Arcadia_List_removeAt(thread, panel->childConnections,
      Arcadia_Collection_getSize(thread, (Arcadia_Collection*)panel->childConnections) - 1, 1);
    if (!Arcadia_Value_isVoidValue(&connection))
      Arcadia_Slot_disconnect(thread, (Arcadia_Slot*)Arcadia_Value_getObjectReferenceValue(&connection));
    Arcadia_Thread_jump(thread);
  }
  // Plain numeric initialization cannot jump after child insertion succeeds.
  panel->childLayoutSlots[index] = (Arcadia_Engine_UI_StackLayoutSlot){ 0 };
  panel->layoutDirty = Arcadia_BooleanValue_True;
}

static void
removeChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  )
{
  Arcadia_Engine_UI_LayoutPanelNode* panel = (Arcadia_Engine_UI_LayoutPanelNode*)self;
  Arcadia_List* children = panel->children;
  Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)children);
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    if (Arcadia_List_getObjectReferenceValueAt(thread, children, i) == child) {
      Arcadia_Value connection = Arcadia_List_getAt(thread, panel->childConnections, i);
      if (!Arcadia_Value_isVoidValue(&connection))
        Arcadia_Slot_disconnect(thread, (Arcadia_Slot*)Arcadia_Value_getObjectReferenceValue(&connection));
      if (panel->hoveredChild == child) {
        setHovered(thread, child, Arcadia_BooleanValue_False);
        panel->hoveredChild = NULL;
      }
      if (panel->pressedChild == child) panel->pressedChild = NULL;
      if (panel->focusedChild == child) focusChild(thread, panel, NULL);
      Arcadia_List_removeAt(thread, children, i, 1);
      Arcadia_List_removeAt(thread, panel->childConnections, i, 1);
      for (Arcadia_SizeValue j = i + 1; j < n; ++j) {
        panel->childLayoutSlots[j - 1] = panel->childLayoutSlots[j];
      }
      panel->layoutDirty = Arcadia_BooleanValue_True;
      return;
    }
  }
}

static void
measureImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* widget,
    Arcadia_Integer32Value availableWidth,
    Arcadia_Integer32Value availableHeight
  )
{
  Arcadia_Engine_UI_LayoutPanelNode* self = (Arcadia_Engine_UI_LayoutPanelNode*)widget;
  if (!self->layout) {
    widget->desiredWidth = widget->requestedWidth;
    widget->desiredHeight = widget->requestedHeight;
    return;
  }
  Arcadia_Integer32Value width = 0, height = 0;
  // Always measure descendants, even when this panel has a fixed desired size.
  Arcadia_Engine_UI_Layout_measure(thread, self->layout, (Arcadia_Object*)self, availableWidth, availableHeight, &width, &height);
  if (widget->sizePolicy == Arcadia_Engine_UI_WidgetNodeSizePolicy_Fixed) {
    width = widget->requestedWidth;
    height = widget->requestedHeight;
  }
  if (width < 0 || width > Arcadia_Integer32Value_Maximum / 2 ||
      height < 0 || height > Arcadia_Integer32Value_Maximum / 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  widget->desiredWidth = width;
  widget->desiredHeight = height;
}

static void
arrangeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetNode* widget,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  Arcadia_Engine_UI_LayoutPanelNode* self = (Arcadia_Engine_UI_LayoutPanelNode*)widget;
  Arcadia_BooleanValue wasInProgress = self->layoutInProgress;
  self->layoutInProgress = Arcadia_BooleanValue_True;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_UI_WidgetNodeDispatch* baseDispatch =
      (Arcadia_Engine_UI_WidgetNodeDispatch*)Arcadia_ObjectType_getDispatch(_Arcadia_Engine_UI_WidgetNode_getType(thread));
    baseDispatch->arrange(thread, widget, left, bottom, width, height);
    if (self->layout) Arcadia_Engine_UI_Layout_arrange(thread, self->layout, (Arcadia_Object*)self, left, bottom, width, height);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    self->layoutInProgress = wasInProgress;
    self->layoutDirty = Arcadia_BooleanValue_True;
    Arcadia_Thread_jump(thread);
  }
  self->arrangedLeft = left;
  self->arrangedBottom = bottom;
  self->arrangedWidth = width;
  self->arrangedHeight = height;
  self->hasArranged = Arcadia_BooleanValue_True;
  self->layoutInProgress = wasInProgress;
  self->layoutDirty = Arcadia_BooleanValue_False;
}

static void
renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Engine_UI_LayoutPanelNode* panel = (Arcadia_Engine_UI_LayoutPanelNode*)self;
  Arcadia_Real32Value canvasWidth;
  Arcadia_Real32Value canvasHeight;
  Arcadia_Engine_Visuals_ViewportNode_getCanvasSize(thread, enterPassNode->viewportNode, &canvasWidth, &canvasHeight);
  panel->lastCanvasHeight = canvasHeight;
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  if (panel->backgroundPanelNode) {
    Arcadia_Engine_UI_WidgetNode* widget = (Arcadia_Engine_UI_WidgetNode*)panel;
    Arcadia_Engine_UI_PanelNode_updateBackgroundGeometry(thread, panel->backgroundPanelNode,
      widget->left, widget->bottom, widget->width, widget->height, panel->backgroundBorderWidth);
    Arcadia_Engine_UI_Renderable_render(thread, (Arcadia_Object*)panel->backgroundPanelNode, enterPassNode);
  }
  Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)panel->children);
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, panel->children, i);
    Arcadia_Engine_UI_RenderableDispatch* dispatch =
      (Arcadia_Engine_UI_RenderableDispatch*)Arcadia_ObjectType_getInterfaceDispatch
        (
          thread,
          (Arcadia_ObjectType*)Arcadia_Object_getType(thread, child),
          (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Renderable_getType(thread)
        );
    if (dispatch && dispatch->render) Arcadia_Engine_UI_Renderable_render(thread, child, enterPassNode);
  }
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNodeDispatch* self
  )
{
  ((Arcadia_Engine_UI_WidgetNodeDispatch*)self)->measure = &measureImpl;
  ((Arcadia_Engine_UI_WidgetNodeDispatch*)self)->arrange = &arrangeImpl;
}

static void
initializeContainerDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ContainerDispatch* self
  )
{
  self->getChildCount = &getChildCountImpl;
  self->getChildAt = &getChildAtImpl;
  self->addChild = &addChildImpl;
  self->removeChild = &removeChildImpl;
}

static void
initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  )
{
  self->render = &renderImpl;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.LayoutPanelNode", Arcadia_Engine_UI_LayoutPanelNode,
    u8"Arcadia.Engine.UI.WidgetNode", Arcadia_Engine_UI_WidgetNode, &typeOperations,
    { .interfaceType = _Arcadia_Engine_UI_Container_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_ContainerDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeContainerDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_Renderable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_RenderableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeRenderableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_Focusable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_FocusableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeFocusableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_HitTestable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_HitTestableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeHitTestableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_KeyboardInput_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_KeyboardInputDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeKeyboardInputDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_PointerInput_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_PointerInputDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializePointerInputDispatchImpl }
  )

Arcadia_Engine_UI_LayoutPanelNode*
Arcadia_Engine_UI_LayoutPanelNode_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_LayoutPanelNode);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_UI_LayoutPanelNode);
}

void
Arcadia_Engine_UI_LayoutPanelNode_setLayout
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Object* layout
  )
{
  if (layout && !Arcadia_ObjectType_getInterfaceDispatch(thread,
      (Arcadia_ObjectType*)Arcadia_Object_getType(thread, layout),
      (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Layout_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->layout = layout;
  self->layoutDirty = Arcadia_BooleanValue_True;
}

void
Arcadia_Engine_UI_LayoutPanelNode_setChildMargin
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Object* child,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value right,
    Arcadia_Integer32Value top
  )
{
  if (left < 0 || bottom < 0 || right < 0 || top < 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_StackLayoutSlot* slot = &self->childLayoutSlots[findChildIndex(thread, self, child)];
  slot->marginLeft = left;
  slot->marginBottom = bottom;
  slot->marginRight = right;
  slot->marginTop = top;
  self->layoutDirty = Arcadia_BooleanValue_True;
}

void
Arcadia_Engine_UI_LayoutPanelNode_setChildCrossAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Object* child,
    Arcadia_Engine_UI_LayoutAlignment alignment
  )
{
  if (alignment < Arcadia_Engine_UI_LayoutAlignment_Start ||
      alignment > Arcadia_Engine_UI_LayoutAlignment_Stretch) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_StackLayoutSlot* slot = &self->childLayoutSlots[findChildIndex(thread, self, child)];
  slot->crossAlignment = alignment;
  slot->hasAlignmentOverride = Arcadia_BooleanValue_True;
  self->layoutDirty = Arcadia_BooleanValue_True;
}

void
Arcadia_Engine_UI_LayoutPanelNode_clearChildCrossAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Object* child
  )
{
  self->childLayoutSlots[findChildIndex(thread, self, child)].hasAlignmentOverride = Arcadia_BooleanValue_False;
  self->layoutDirty = Arcadia_BooleanValue_True;
}

Arcadia_Engine_UI_StackLayoutSlot const*
Arcadia_Engine_UI_LayoutPanelNode_getChildLayoutSlots
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  )
{ return self->childLayoutSlots; }

void
Arcadia_Engine_UI_LayoutPanelNode_invalidateLayout
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  )
{ self->layoutDirty = Arcadia_BooleanValue_True; }

void
Arcadia_Engine_UI_LayoutPanelNode_updateLayout
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  )
{
  if (!self->layout || self->layoutInProgress) return;
  Arcadia_Engine_UI_WidgetNode* widget = (Arcadia_Engine_UI_WidgetNode*)self;
  if (self->hasArranged && !self->layoutDirty && !hasDirtyDescendant(thread, self) &&
      self->arrangedLeft == widget->left && self->arrangedBottom == widget->bottom &&
      self->arrangedWidth == widget->width && self->arrangedHeight == widget->height) return;
  Arcadia_Engine_UI_WidgetNode_measure(thread, widget, widget->width, widget->height);
  Arcadia_Engine_UI_WidgetNode_arrange(thread, widget, widget->left, widget->bottom, widget->width, widget->height);
}

void
Arcadia_Engine_UI_LayoutPanelNode_clearHover
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  )
{
  if (self->hoveredChild) setHovered(thread, self->hoveredChild, Arcadia_BooleanValue_False);
  self->hoveredChild = NULL;
}

static Arcadia_Engine_UI_LayoutAlignment
parseAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_String* value
  )
{
  if (Arcadia_String_isEqualTo_pn(thread, value, "Start", 5)) return Arcadia_Engine_UI_LayoutAlignment_Start;
  if (Arcadia_String_isEqualTo_pn(thread, value, "Center", 6)) return Arcadia_Engine_UI_LayoutAlignment_Center;
  if (Arcadia_String_isEqualTo_pn(thread, value, "End", 3)) return Arcadia_Engine_UI_LayoutAlignment_End;
  if (Arcadia_String_isEqualTo_pn(thread, value, "Stretch", 7)) return Arcadia_Engine_UI_LayoutAlignment_Stretch;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

Arcadia_Engine_UI_LayoutPanelNode*
Arcadia_Engine_UI_LayoutPanelNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_PanelDefinition* source
  )
{
  // Keep the panel and layout alive across collecting factory and population calls.
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_String_isEqualTo_pn(thread, source->layoutType, "UI.StackLayout", 14)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_LayoutPanelNode* panel = source->tabBehavior
    ? (Arcadia_Engine_UI_LayoutPanelNode*)Arcadia_Engine_UI_TabBarNode_create(thread, source->vertical, source->gap)
    : Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)panel);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)panel,
    source->left, source->bottom, (Arcadia_Integer32Value)source->width, (Arcadia_Integer32Value)source->height);
  Arcadia_Engine_UI_StackLayout* layout = Arcadia_Engine_UI_StackLayout_create(thread, source->vertical, source->gap);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)layout);
  Arcadia_Engine_UI_StackLayout_setPadding(thread, layout,
    source->paddingLeft, source->paddingBottom, source->paddingRight, source->paddingTop);
  Arcadia_Engine_UI_StackLayout_setAlignment(thread, layout,
    parseAlignment(thread, source->mainAlignment), parseAlignment(thread, source->crossAlignment));
  Arcadia_Engine_UI_LayoutPanelNode_setLayout(thread, panel, (Arcadia_Object*)layout);
  Arcadia_ValueStack_popValues(thread, 1);
  if (source->sizePolicy && Arcadia_String_isEqualTo_pn(thread, source->sizePolicy, "Fixed", 5)) {
    Arcadia_Engine_UI_LayoutPanelNode_setSizePolicy(thread, panel, Arcadia_Engine_UI_WidgetNodeSizePolicy_Fixed);
  }
  panel->backgroundBorderWidth = source->backgroundBorderWidth;
  if (source->backgroundModelName) {
    Arcadia_ADL_Definition* model = Arcadia_ADL_Definitions_getDefinitionOrNull(thread,
      ((Arcadia_ADL_Definition*)source)->definitions, source->backgroundModelName);
    if (!model || !Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)model, _Arcadia_ADL_ModelDefinition_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    panel->backgroundPanelNode = Arcadia_Engine_UI_PanelNode_create(thread, backendContext, (Arcadia_ADL_ModelDefinition*)model);
  }
  Arcadia_Engine_UI_WidgetNode_setName(thread, (Arcadia_Engine_UI_WidgetNode*)panel, ((Arcadia_ADL_Definition*)source)->name);
  Arcadia_Engine_UI_populateContainerFromDefinition(thread, backendContext, (Arcadia_Object*)panel, (Arcadia_Object*)source);
  if (source->selectedChildName) {
    Arcadia_Engine_UI_ButtonNode* selected = NULL;
    Arcadia_Value expectedName = Arcadia_Value_makeObjectReferenceValue(source->selectedChildName);
    Arcadia_Natural64Value n = Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)panel);
    for (Arcadia_Natural64Value i = 0; i < n; ++i) {
      Arcadia_Engine_UI_ButtonNode* button = (Arcadia_Engine_UI_ButtonNode*)Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)panel, i);
      Arcadia_String* name = Arcadia_Engine_UI_WidgetNode_getName(thread, (Arcadia_Engine_UI_WidgetNode*)button);
      if (name && Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)name, &expectedName)) {
        selected = button;
        break;
      }
    }
    if (!selected || !selected->enabled) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Engine_UI_TabBarNode_select(thread, (Arcadia_Engine_UI_TabBarNode*)panel, selected);
  }
  if (source->childLayouts) {
    if (Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)panel) != source->childLayoutCount) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    for (Arcadia_SizeValue i = 0; i < source->childLayoutCount; ++i) {
      Arcadia_ADL_UI_PanelChildLayout const* slot = &source->childLayouts[i];
      if (!slot->configured) continue;
      Arcadia_Object* child = Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)panel, i);
      Arcadia_Engine_UI_LayoutPanelNode_setChildMargin(thread, panel, child,
        slot->marginLeft, slot->marginBottom, slot->marginRight, slot->marginTop);
      if (slot->hasCrossAlignment) {
        Arcadia_Engine_UI_LayoutAlignment alignment = Arcadia_Engine_UI_LayoutAlignment_Center;
        switch (slot->crossAlignment) {
          case Arcadia_ADL_UI_PanelChildCrossAlignment_Start: alignment = Arcadia_Engine_UI_LayoutAlignment_Start; break;
          case Arcadia_ADL_UI_PanelChildCrossAlignment_Center: alignment = Arcadia_Engine_UI_LayoutAlignment_Center; break;
          case Arcadia_ADL_UI_PanelChildCrossAlignment_End: alignment = Arcadia_Engine_UI_LayoutAlignment_End; break;
          case Arcadia_ADL_UI_PanelChildCrossAlignment_Stretch: alignment = Arcadia_Engine_UI_LayoutAlignment_Stretch; break;
          default:
            Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
            Arcadia_Thread_jump(thread);
        }
        Arcadia_Engine_UI_LayoutPanelNode_setChildCrossAlignment(thread, panel, child, alignment);
      }
    }
  }
  Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel,
    (Arcadia_Integer32Value)source->width, (Arcadia_Integer32Value)source->height);
  Arcadia_Engine_UI_WidgetNode_arrange(thread, (Arcadia_Engine_UI_WidgetNode*)panel,
    source->left, source->bottom, (Arcadia_Integer32Value)source->width, (Arcadia_Integer32Value)source->height);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return panel;
}

void
Arcadia_Engine_UI_LayoutPanelNode_setSizePolicy
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Engine_UI_WidgetNodeSizePolicy policy
  )
{
  if (policy != Arcadia_Engine_UI_WidgetNodeSizePolicy_Content &&
      policy != Arcadia_Engine_UI_WidgetNodeSizePolicy_Fixed) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_WidgetNode* widget = (Arcadia_Engine_UI_WidgetNode*)self;
  if (widget->sizePolicy != policy) {
    widget->sizePolicy = policy;
    self->layoutDirty = Arcadia_BooleanValue_True;
  }
}

Arcadia_Object*
Arcadia_Engine_UI_LayoutPanelNode_getLayout
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  )
{ return self->layout; }
