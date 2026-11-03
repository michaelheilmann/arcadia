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
#include "Arcadia/Engine/UI/Nodes/CanvasNode.h"

#include "Arcadia/Engine/UI/DefinitionFactory.h"
#include "Arcadia/Engine/UI/Include.h"

static void
Arcadia_Engine_UI_CanvasNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self
  );

static void
Arcadia_Engine_UI_CanvasNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self
  );

static void
Arcadia_Engine_UI_CanvasNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNodeDispatch* self
  );

static void
Arcadia_Engine_UI_CanvasNode_initializeContainerDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ContainerDispatch* self
  );

static void
Arcadia_Engine_UI_CanvasNode_initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  );

static void
Arcadia_Engine_UI_CanvasNode_initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  );

static Arcadia_Natural64Value
Arcadia_Engine_UI_CanvasNode_getChildCountImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static Arcadia_Object*
Arcadia_Engine_UI_CanvasNode_getChildAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value index
  );

static void
Arcadia_Engine_UI_CanvasNode_addChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  );

static void
Arcadia_Engine_UI_CanvasNode_removeChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_CanvasNode_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_CanvasNode_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_CanvasNode_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_UI_CanvasNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_UI_CanvasNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_CanvasNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.CanvasNode",
    Arcadia_Engine_UI_CanvasNode,
    u8"Arcadia.Engine.UI.WidgetNode",
    Arcadia_Engine_UI_WidgetNode,
    &_typeOperations,
    {
      .interfaceType = _Arcadia_Engine_UI_Container_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_ContainerDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_CanvasNode_initializeContainerDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_KeyboardInput_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_KeyboardInputDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_CanvasNode_initializeKeyboardInputDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_PointerInput_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_PointerInputDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_CanvasNode_initializePointerInputDispatchImpl
    }
  )

static void
Arcadia_Engine_UI_CanvasNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_CanvasNode);
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  self->viewportNode = Arcadia_Engine_Visuals_NodeFactory_createViewportNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL);
  self->cameraNode = Arcadia_Engine_Visuals_NodeFactory_createCameraNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL);
  self->children = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->hoveredChild = NULL;
  self->pressedChild = NULL;
  self->focusedChild = NULL;
  self->popup = NULL;
  self->focusBeforePopup = NULL;
  self->consumePopupRelease = Arcadia_BooleanValue_False;
  self->popupKeyboardOwner = NULL;
  self->popupKeyboardHandler = NULL;
  self->captureOwner = NULL;
  self->captureKeyboardHandler = NULL;
  self->captureMouseHandler = NULL;
  self->suppressCaptureKeyRelease = Arcadia_BooleanValue_False;
  self->suppressCaptureButtonRelease = Arcadia_BooleanValue_False;
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_CanvasNode);
}

static void
Arcadia_Engine_UI_CanvasNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self
  )
{
  if (self->cameraNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cameraNode);
  }
  if (self->viewportNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->viewportNode);
  }
  if (self->children) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->children);
  }
  if (self->hoveredChild) {
    Arcadia_Object_visit(thread, self->hoveredChild);
  }
  if (self->pressedChild) {
    Arcadia_Object_visit(thread, self->pressedChild);
  }
  if (self->focusedChild) {
    Arcadia_Object_visit(thread, self->focusedChild);
  }
  if (self->popup) Arcadia_Object_visit(thread, self->popup);
  if (self->focusBeforePopup) Arcadia_Object_visit(thread, self->focusBeforePopup);
  if (self->popupKeyboardOwner) Arcadia_Object_visit(thread, self->popupKeyboardOwner);
  if (self->captureOwner) Arcadia_Object_visit(thread, self->captureOwner);
}

static void
Arcadia_Engine_UI_CanvasNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_UI_CanvasNode_initializeContainerDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ContainerDispatch* self
  )
{
  self->getChildCount = &Arcadia_Engine_UI_CanvasNode_getChildCountImpl;
  self->getChildAt = &Arcadia_Engine_UI_CanvasNode_getChildAtImpl;
  self->addChild = &Arcadia_Engine_UI_CanvasNode_addChildImpl;
  self->removeChild = &Arcadia_Engine_UI_CanvasNode_removeChildImpl;
}

static void
Arcadia_Engine_UI_CanvasNode_initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  )
{ self->handleKeyboardKeyEvent = &Arcadia_Engine_UI_CanvasNode_handleKeyboardKeyEventImpl; }

static void
Arcadia_Engine_UI_CanvasNode_initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  )
{
  self->handleMousePointerEvent = &Arcadia_Engine_UI_CanvasNode_handleMousePointerEventImpl;
  self->handleMouseButtonEvent = &Arcadia_Engine_UI_CanvasNode_handleMouseButtonEventImpl;
}

static void
focusChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Object* child
  );

static void
setHovered
  (
    Arcadia_Thread* thread,
    Arcadia_Object* child,
    Arcadia_BooleanValue hovered
  );

static Arcadia_BooleanValue
hasInterface
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_InterfaceType* interfaceType
  )
{
  return NULL != Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)Arcadia_Object_getType(thread, object), interfaceType);
}

// An overlay is not in children. Validate its capabilities before replacing
// an existing popup so an invalid request never destroys a valid overlay.
static Arcadia_BooleanValue
hasPopupCapabilities
  (
    Arcadia_Thread* thread,
    Arcadia_Object* popup
  )
{
  return popup &&
    Arcadia_Object_isInstanceOf(thread, popup, _Arcadia_Engine_UI_WidgetNode_getType(thread)) &&
    hasInterface(thread, popup, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_HitTestable_getType(thread)) &&
    hasInterface(thread, popup, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Renderable_getType(thread)) &&
    hasInterface(thread, popup, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_KeyboardInput_getType(thread)) &&
    hasInterface(thread, popup, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread)) &&
    hasInterface(thread, popup, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Focusable_getType(thread));
}

Arcadia_Object*
Arcadia_Engine_UI_CanvasNode_getPopup
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self
  )
{ return self->popup; }

void
Arcadia_Engine_UI_CanvasNode_setPopupKeyboardHandler
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Object* owner,
    Arcadia_Engine_UI_CanvasPopupKeyboardHandler handler
  )
{
  if (!self->popup || !owner || !handler) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->popupKeyboardOwner = owner;
  self->popupKeyboardHandler = handler;
}

void
Arcadia_Engine_UI_CanvasNode_hidePopup
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self
  )
{
  if (!self->popup) return;
  // Clear owned references first; focus callbacks may reenter and show a new
  // popup, which must not be overwritten by this closing operation.
  Arcadia_Object* previous = self->focusBeforePopup;
  if (previous) Arcadia_ValueStack_pushObjectReferenceValue(thread, previous);
  else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_Object* oldPopup = self->popup;
  Arcadia_ValueStack_pushObjectReferenceValue(thread, oldPopup);
  self->popup = NULL;
  self->focusBeforePopup = NULL;
  self->popupKeyboardOwner = NULL;
  self->popupKeyboardHandler = NULL;
  if (self->focusedChild == oldPopup) focusChild(thread, self, previous);
  Arcadia_ValueStack_popValues(thread, 2);
}

void
Arcadia_Engine_UI_CanvasNode_showPopup
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Object* popup
  )
{
  if (self->captureOwner) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!hasPopupCapabilities(thread, popup)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children); i < n; ++i) {
    if (Arcadia_List_getObjectReferenceValueAt(thread, self->children, i) == popup) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
  if (self->popup == popup) return;
  if (self->popup) Arcadia_Engine_UI_CanvasNode_hidePopup(thread, self);
  self->popup = popup;
  self->focusBeforePopup = self->focusedChild;
  if (self->hoveredChild) setHovered(thread, self->hoveredChild, Arcadia_BooleanValue_False);
  self->hoveredChild = NULL;
  self->pressedChild = NULL;
  focusChild(thread, self, popup);
}

void
Arcadia_Engine_UI_CanvasNode_beginInputCapture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Object* owner,
    Arcadia_Engine_UI_CanvasCaptureKeyboardHandler keyboard,
    Arcadia_Engine_UI_CanvasCaptureMouseHandler mouse
  )
{
  if (!owner || !keyboard || !mouse || self->captureOwner || self->popup || self->consumePopupRelease ||
      self->suppressCaptureKeyRelease || self->suppressCaptureButtonRelease) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // Clear old pointer capture but leave normal keyboard focus untouched.
  self->pressedChild = NULL;
  self->captureOwner = owner;
  self->captureKeyboardHandler = keyboard;
  self->captureMouseHandler = mouse;
}

void
Arcadia_Engine_UI_CanvasNode_endInputCapture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Object* owner
  )
{
  if (owner != self->captureOwner) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->captureOwner = NULL;
  self->captureKeyboardHandler = NULL;
  self->captureMouseHandler = NULL;
}

void
Arcadia_Engine_UI_CanvasNode_suppressKeyboardRelease
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Engine_Input_KeyboardKey key
  )
{
  self->suppressedCaptureKey = key;
  self->suppressCaptureKeyRelease = Arcadia_BooleanValue_True;
}

void
Arcadia_Engine_UI_CanvasNode_suppressMouseButtonRelease
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Natural64Value button
  )
{
  self->suppressedCaptureButton = button;
  self->suppressCaptureButtonRelease = Arcadia_BooleanValue_True;
}

// Widget bounds are in the local content coordinate space of their nearest
// ScrollPanelNode. Descendants of a LayoutPanelNode retain that space. Find
// the anchor through the actual container tree before placing a canvas-level
// popup; merely using anchor->left/bottom ignores every enclosing scroll
// viewport and sends the popup to an unrelated location on the screen.
static Arcadia_BooleanValue
findPopupAnchor
  (
    Arcadia_Thread* thread,
    Arcadia_Object* node,
    Arcadia_Engine_UI_WidgetNode* anchor,
    Arcadia_Integer32Value offsetX,
    Arcadia_Integer32Value offsetY,
    Arcadia_Integer32Value* canvasX,
    Arcadia_Integer32Value* canvasY,
    Arcadia_Natural32Value depth
  )
{
  if (!node || depth > 64) return Arcadia_BooleanValue_False;
  if (node == (Arcadia_Object*)anchor) {
    *canvasX = anchor->left + offsetX;
    *canvasY = anchor->bottom + offsetY;
    return Arcadia_BooleanValue_True;
  }
  if (!hasInterface(thread, node, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Container_getType(thread)))
    return Arcadia_BooleanValue_False;
  if (Arcadia_Object_isInstanceOf(thread, node, _Arcadia_Engine_UI_ScrollPanelNode_getType(thread))) {
    Arcadia_Engine_UI_ScrollPanelNode* scroll = (Arcadia_Engine_UI_ScrollPanelNode*)node;
    Arcadia_Engine_UI_WidgetNode* widget = (Arcadia_Engine_UI_WidgetNode*)node;
    offsetX += widget->left - scroll->scrollX;
    offsetY += widget->bottom - scroll->scrollY;
  }
  Arcadia_Natural64Value count = Arcadia_Engine_UI_Container_getChildCount(thread, node);
  for (Arcadia_Natural64Value i = 0; i < count; ++i) {
    Arcadia_Object* child = Arcadia_Engine_UI_Container_getChildAt(thread, node, i);
    if (findPopupAnchor(thread, child, anchor, offsetX, offsetY, canvasX, canvasY, depth + 1))
      return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False;
}

void
Arcadia_Engine_UI_CanvasNode_placePopup
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Engine_UI_WidgetNode* anchor,
    Arcadia_Engine_UI_WidgetNode* popup,
    Arcadia_Integer32Value requestedWidth,
    Arcadia_Integer32Value requestedHeight
  )
{
  if (!anchor || !popup || requestedWidth <= 0 || requestedHeight <= 0 ||
      self->visualsCanvasWidth <= 0 || self->visualsCanvasHeight <= 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Integer32Value width = requestedWidth < self->visualsCanvasWidth ? requestedWidth : self->visualsCanvasWidth;
  Arcadia_Integer32Value height = requestedHeight < self->visualsCanvasHeight ? requestedHeight : self->visualsCanvasHeight;
  Arcadia_Integer32Value anchorX = anchor->left;
  Arcadia_Integer32Value anchorY = anchor->bottom;
  // Keep the old canvas-space behavior for detached anchors, but normalize
  // anchors in the canvas tree through each enclosing scroll viewport.
  Arcadia_Natural64Value count = Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)self);
  for (Arcadia_Natural64Value i = 0; i < count; ++i) {
    Arcadia_Object* child = Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)self, i);
    if (findPopupAnchor(thread, child, anchor, 0, 0, &anchorX, &anchorY, 0)) break;
  }
  Arcadia_Integer32Value x = anchorX;
  if (x < 0) x = 0;
  if (x > self->visualsCanvasWidth - width) x = self->visualsCanvasWidth - width;
  Arcadia_Integer32Value below = anchorY;
  Arcadia_Integer32Value above = self->visualsCanvasHeight - anchorY - anchor->height;
  Arcadia_Integer32Value y;
  if (below >= height) y = anchorY - height;
  else if (above >= height) y = anchorY + anchor->height;
  else if (below >= above) y = below > height ? anchorY - height : 0;
  else y = anchorY + anchor->height;
  if (y < 0) y = 0;
  if (y > self->visualsCanvasHeight - height) y = self->visualsCanvasHeight - height;
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, popup, x, y, width, height);
}

static Arcadia_Object*
hitTest
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children);
  for (Arcadia_SizeValue i = n; i > 0; --i) {
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i - 1);
    if (hasInterface(thread, child, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_HitTestable_getType(thread)) &&
        Arcadia_Engine_UI_HitTestable_hitTest(thread, child, x, y)) {
      return child;
    }
  }
  return NULL;
}

static void
focusChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Object* child
  )
{
  if (self->focusedChild == child) return;
  if (self->focusedChild && hasInterface(thread, self->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Focusable_getType(thread))) {
    Arcadia_Engine_UI_Focusable_blur(thread, self->focusedChild);
  }
  self->focusedChild = child;
  if (self->focusedChild && hasInterface(thread, self->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Focusable_getType(thread))) {
    Arcadia_Engine_UI_Focusable_focus(thread, self->focusedChild);
  }
}

static void
focusNextChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self
  )
{
  Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children);
  if (!n) return;
  Arcadia_SizeValue start = 0;
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    if (Arcadia_List_getObjectReferenceValueAt(thread, self->children, i) == self->focusedChild) {
      start = i + 1;
      break;
    }
  }
  for (Arcadia_SizeValue k = 0; k < n; ++k) {
    Arcadia_SizeValue i = (start + k) % n;
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i);
    if (hasInterface(thread, child, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Focusable_getType(thread))) {
      focusChild(thread, self, child);
      return;
    }
  }
}

static void
setHovered
  (
    Arcadia_Thread* thread,
    Arcadia_Object* child,
    Arcadia_BooleanValue hovered
  )
{
  if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_ButtonNode_getType(thread))) {
    Arcadia_Engine_UI_ButtonNode_setHovered(thread, (Arcadia_Engine_UI_ButtonNode*)child, hovered);
  } else if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_SegmentedChoiceNode_getType(thread))) {
    Arcadia_Engine_UI_SegmentedChoiceNode_setHovered(thread, (Arcadia_Engine_UI_SegmentedChoiceNode*)child, hovered);
  } else if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_SliderNode_getType(thread))) {
    Arcadia_Engine_UI_SliderNode_setHovered(thread, (Arcadia_Engine_UI_SliderNode*)child, hovered);
  } else if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_ToggleNode_getType(thread))) {
    Arcadia_Engine_UI_ToggleNode_setHovered(thread, (Arcadia_Engine_UI_ToggleNode*)child, hovered);
  } else if (!hovered && child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_LayoutPanelNode_getType(thread))) {
    Arcadia_Engine_UI_LayoutPanelNode_clearHover(thread, (Arcadia_Engine_UI_LayoutPanelNode*)child);
  }
}

static Arcadia_Natural64Value
Arcadia_Engine_UI_CanvasNode_getChildCountImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ return Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_UI_CanvasNode*)self)->children); }

static Arcadia_Object*
Arcadia_Engine_UI_CanvasNode_getChildAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value index
  )
{ return Arcadia_List_getObjectReferenceValueAt(thread, ((Arcadia_Engine_UI_CanvasNode*)self)->children, index); }

static void
Arcadia_Engine_UI_CanvasNode_addChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  )
{ Arcadia_List_insertBackObjectReferenceValue(thread, ((Arcadia_Engine_UI_CanvasNode*)self)->children, child); }

static void
Arcadia_Engine_UI_CanvasNode_removeChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  )
{
  Arcadia_Engine_UI_CanvasNode* canvas = (Arcadia_Engine_UI_CanvasNode*)self;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)canvas->children); i < n; ++i) {
    if (Arcadia_List_getObjectReferenceValueAt(thread, canvas->children, i) == child) {
      Arcadia_List_removeAt(thread, canvas->children, i, 1);
      if (canvas->hoveredChild == child) canvas->hoveredChild = NULL;
      if (canvas->pressedChild == child) canvas->pressedChild = NULL;
      if (canvas->focusedChild == child) canvas->focusedChild = NULL;
      return;
    }
  }
}

static Arcadia_BooleanValue
Arcadia_Engine_UI_CanvasNode_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  Arcadia_Engine_UI_CanvasNode* canvas = (Arcadia_Engine_UI_CanvasNode*)self;
  if (canvas->suppressCaptureKeyRelease &&
      Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
      Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == canvas->suppressedCaptureKey) {
    canvas->suppressCaptureKeyRelease = Arcadia_BooleanValue_False;
    return Arcadia_BooleanValue_True;
  }
  if (canvas->captureOwner) {
    canvas->captureKeyboardHandler(thread, canvas->captureOwner, event);
    return Arcadia_BooleanValue_True;
  }
  if (canvas->popup) {
    if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released) {
      Arcadia_Engine_Input_KeyboardKey key = Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event);
      if (key == Arcadia_Engine_Input_KeyboardKey_Escape) {
        Arcadia_Engine_UI_CanvasNode_hidePopup(thread, canvas);
        return Arcadia_BooleanValue_True;
      }
      if (key == Arcadia_Engine_Input_KeyboardKey_Tabulator) {
        Arcadia_Engine_UI_CanvasNode_hidePopup(thread, canvas);
        focusNextChild(thread, canvas);
        return Arcadia_BooleanValue_True;
      }
    }
    if (canvas->popupKeyboardHandler && canvas->popupKeyboardOwner &&
        canvas->popupKeyboardHandler(thread, canvas->popupKeyboardOwner, event))
      return Arcadia_BooleanValue_True;
    // A keyboard handler may have closed the popup reentrantly.
    if (canvas->popup)
      Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, canvas->popup, event);
    return Arcadia_BooleanValue_True;
  }
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
      Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Tabulator) {
    // A focused layout panel gets the first chance to tab between its children.
    // Other canvas children retain the existing top-level focus traversal.
    if (canvas->focusedChild &&
        Arcadia_Object_isInstanceOf(thread, canvas->focusedChild, _Arcadia_Engine_UI_LayoutPanelNode_getType(thread)) &&
        hasInterface(thread, canvas->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_KeyboardInput_getType(thread)) &&
        Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, canvas->focusedChild, event)) return Arcadia_BooleanValue_True;
    focusNextChild(thread, canvas);
    return Arcadia_BooleanValue_True;
  }
  if (canvas->focusedChild && hasInterface(thread, canvas->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_KeyboardInput_getType(thread))) {
    return
    Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent
      (
        thread,
        canvas->focusedChild,
        event
      );
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
Arcadia_Engine_UI_CanvasNode_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  Arcadia_Engine_UI_CanvasNode* canvas = (Arcadia_Engine_UI_CanvasNode*)self;
  if (canvas->captureOwner) return Arcadia_BooleanValue_True;
  if (canvas->popup) {
    Arcadia_Object* popup = canvas->popup;
    if (Arcadia_Engine_UI_HitTestable_hitTest(thread, popup, event->x, canvas->visualsCanvasHeight - event->y))
      Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, popup, event);
    // A modal popup consumes wheel/motion outside its bounds as well.
    return Arcadia_BooleanValue_True;
  }
  Arcadia_Object* child = hitTest(thread, canvas, event->x, canvas->visualsCanvasHeight - event->y);
  if (event->action == Arcadia_Engine_Input_MousePointerAction_Scrolled) {
    if (child && hasInterface(thread, child, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread)))
      return Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, child, event);
    return Arcadia_BooleanValue_False;
  }
  if (canvas->hoveredChild != child) {
    setHovered(thread, canvas->hoveredChild, Arcadia_BooleanValue_False);
    setHovered(thread, child, Arcadia_BooleanValue_True);
  }
  canvas->hoveredChild = child;
  Arcadia_Object* receiver = canvas->pressedChild ? canvas->pressedChild : child;
  if (receiver && hasInterface(thread, receiver, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread))) {
    return
    Arcadia_Engine_UI_PointerInput_handleMousePointerEvent
      (
        thread,
        receiver,
        event
      );
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
Arcadia_Engine_UI_CanvasNode_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  Arcadia_Engine_UI_CanvasNode* canvas = (Arcadia_Engine_UI_CanvasNode*)self;
  if (canvas->suppressCaptureButtonRelease &&
      event->action == Arcadia_Engine_Input_MouseButtonAction_Released &&
      event->button == canvas->suppressedCaptureButton) {
    canvas->suppressCaptureButtonRelease = Arcadia_BooleanValue_False;
    return Arcadia_BooleanValue_True;
  }
  if (canvas->captureOwner) {
    canvas->captureMouseHandler(thread, canvas->captureOwner, event);
    return Arcadia_BooleanValue_True;
  }
  if (canvas->consumePopupRelease) {
    if (event->action == Arcadia_Engine_Input_MouseButtonAction_Released)
      canvas->consumePopupRelease = Arcadia_BooleanValue_False;
    return Arcadia_BooleanValue_True;
  }
  if (canvas->popup) {
    Arcadia_Object* popup = canvas->popup;
    Arcadia_ValueStack_pushObjectReferenceValue(thread, popup);
    Arcadia_BooleanValue inside = Arcadia_Engine_UI_HitTestable_hitTest(thread, popup,
      event->x, canvas->visualsCanvasHeight - event->y);
    if (!inside && event->action == Arcadia_Engine_Input_MouseButtonAction_Pressed) {
      // Dismiss without click-through. The release of this press is consumed too.
      canvas->consumePopupRelease = Arcadia_BooleanValue_True;
      Arcadia_Engine_UI_CanvasNode_hidePopup(thread, canvas);
    } else if (inside) {
      Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, popup, event);
    }
    Arcadia_ValueStack_popValues(thread, 1);
    return Arcadia_BooleanValue_True;
  }
  Arcadia_Object* child = hitTest(thread, canvas, event->x, canvas->visualsCanvasHeight - event->y);
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Pressed) {
    canvas->pressedChild = child;
    if (child) focusChild(thread, canvas, child);
  }
  Arcadia_Object* receiver = canvas->pressedChild ? canvas->pressedChild : child;
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Released) {
    canvas->pressedChild = NULL;
  }
  if (receiver && hasInterface(thread, receiver, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread))) {
    return
    Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent
      (
        thread,
        receiver,
        event
      );
  }
  return Arcadia_BooleanValue_False;
}

Arcadia_Engine_UI_CanvasNode*
Arcadia_Engine_UI_CanvasNode_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_CanvasNode);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_UI_CanvasNode);
}

Arcadia_Engine_UI_CanvasNode*
Arcadia_Engine_UI_CanvasNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_CanvasDefinition* source
  )
{
  Arcadia_Engine_UI_CanvasNode* node = Arcadia_Engine_UI_CanvasNode_create(thread);
  Arcadia_Engine_UI_WidgetNode_setName(thread, (Arcadia_Engine_UI_WidgetNode*)node, ((Arcadia_ADL_Definition*)source)->name);
  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, node, source->width, source->height);
  Arcadia_Engine_UI_populateContainerFromDefinition(thread, backendContext, (Arcadia_Object*)node, (Arcadia_Object*)source);
  return node;
}

void
Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  self->visualsCanvasWidth = width;
  self->visualsCanvasHeight = height;
}

void
Arcadia_Engine_UI_CanvasNode_updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNode, self->visualsCanvasWidth, self->visualsCanvasHeight);
  Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangle(thread, self->viewportNode, 0.f, 0.f, 1.f, 1.f);

  Arcadia_Engine_Visuals_ViewportNode_setClearColorBuffer(thread, self->viewportNode, Arcadia_BooleanValue_False);
  Arcadia_Engine_Visuals_ViewportNode_setClearDepthBuffer(thread, self->viewportNode, Arcadia_BooleanValue_False);
  Arcadia_Engine_Visuals_ViewportNode_setDepthTestEnabled(thread, self->viewportNode, Arcadia_BooleanValue_False);

  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix, 0.f, (Arcadia_Real32Value)self->visualsCanvasWidth, 0.f, (Arcadia_Real32Value)self->visualsCanvasHeight, -1.f, +1.f);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);

  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, enterPassNode, self->cameraNode);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)enterPassNode, (Arcadia_Engine_Visuals_EnterPassNode*)enterPassNode);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children); i < n; ++i) {
    Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, enterPassNode, self->viewportNode);
    Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, enterPassNode, self->cameraNode);
    Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)enterPassNode, (Arcadia_Engine_Visuals_EnterPassNode*)enterPassNode);
    Arcadia_Object* object = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i);
    Arcadia_Engine_UI_RenderableDispatch* renderableDispatch =
      (Arcadia_Engine_UI_RenderableDispatch*)Arcadia_ObjectType_getInterfaceDispatch
        (
          thread,
          (Arcadia_ObjectType*)Arcadia_Object_getType(thread, object),
          (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Renderable_getType(thread)
        );
    if (renderableDispatch && renderableDispatch->render) {
      Arcadia_Engine_UI_Renderable_render(thread, object, enterPassNode);
    }
  }
  if (self->popup) {
    // The popup is not a normal child; render it above all regular children.
    Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, enterPassNode, self->viewportNode);
    Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, enterPassNode, self->cameraNode);
    Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)enterPassNode, enterPassNode);
    Arcadia_Engine_UI_Renderable_render(thread, self->popup, enterPassNode);
  }
}
