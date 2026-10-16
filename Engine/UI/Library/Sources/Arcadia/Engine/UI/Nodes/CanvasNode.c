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
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
      Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Tabulator) {
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
  Arcadia_Object* child = hitTest(thread, canvas, event->x, canvas->visualsCanvasHeight - event->y);
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
}
