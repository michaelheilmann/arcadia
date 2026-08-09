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

#include "Arcadia/Engine/UI/Nodes/PanelNode.h"

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

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.CanvasNode", Arcadia_Engine_UI_CanvasNode,
                         u8"Arcadia.Engine.UI.WidgetNode", Arcadia_Engine_UI_WidgetNode,
                         &_typeOperations);

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
  self->rectangles = (Arcadia_List*)Arcadia_ArrayList_create(thread);
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
  if (self->rectangles) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->rectangles);
  }
}

static void
Arcadia_Engine_UI_CanvasNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNodeDispatch* self
  )
{/*Intentionally empty.*/}

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
  Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix, -1, +1, -1, +1, -1, +1);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);

  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, enterPassNode, self->cameraNode);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)enterPassNode, (Arcadia_Engine_Visuals_EnterPassNode*)enterPassNode);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->rectangles); i < n; ++i) {
    Arcadia_Object* object = Arcadia_List_getObjectReferenceValueAt(thread, self->rectangles, i);
    if (Arcadia_Object_isInstanceOf(thread, object, _Arcadia_Engine_UI_PanelNode_getType(thread))) {
      Arcadia_Engine_UI_WidgetNode_render(thread, (Arcadia_Engine_UI_WidgetNode*)object, enterPassNode);
    } else {
    }
  }
}
