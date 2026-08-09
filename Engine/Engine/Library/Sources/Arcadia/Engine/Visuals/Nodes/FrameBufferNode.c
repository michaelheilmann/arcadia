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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/Visuals/Nodes/FrameBufferNode.h"

#include "Arcadia/Engine/Include.h"

static void
Arcadia_Engine_Visuals_FrameBufferNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self
  );

static void
Arcadia_Engine_Visuals_FrameBufferNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self
  );

static void
Arcadia_Engine_Visuals_FrameBufferNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self
  );

static void
Arcadia_Engine_Visuals_FrameBufferNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self
  );

static void
Arcadia_Engine_Visuals_FrameBufferNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_Visuals_FrameBufferNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static void
Arcadia_Engine_Visuals_FrameBufferNode_setSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Visuals_FrameBufferNode_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_FrameBufferNode_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_FrameBufferNode_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_FrameBufferNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_FrameBufferNode_visitImpl
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.FrameBufferNode", Arcadia_Engine_Visuals_FrameBufferNode,
                         u8"Arcadia.Engine.Visuals.Node", Arcadia_Engine_Visuals_Node,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_FrameBufferNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_FrameBufferNode);
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (Arcadia_ValueStack_isVoidValue(thread, 1)) {
    self->backendContext = NULL;
  } else {
    self->backendContext = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }
  self->width = 320;
  self->height = 240;
  self->frameBufferResource = NULL;
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_FrameBufferNode);
}

static void
Arcadia_Engine_Visuals_FrameBufferNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self
  )
{
  ((Arcadia_Engine_Visuals_NodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Node*, Arcadia_Engine_Visuals_EnterPassNode*)) & Arcadia_Engine_Visuals_FrameBufferNode_renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_FrameBufferNode_setVisualsBackendContextImpl;
  ((Arcadia_Engine_Visuals_FrameBufferNodeDispatch*)self)->setSize = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_FrameBufferNode*, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Visuals_FrameBufferNode_setSizeImpl;
  ((Arcadia_Engine_Visuals_FrameBufferNodeDispatch*)self)->getSize = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_FrameBufferNode*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & Arcadia_Engine_Visuals_FrameBufferNode_getSizeImpl;
}

static void
Arcadia_Engine_Visuals_FrameBufferNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self
  )
{
  if (self->backendContext) {
    if (self->frameBufferResource) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->frameBufferResource);
      self->frameBufferResource = NULL;
    }
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    self->backendContext = NULL;
  }
}

static void
Arcadia_Engine_Visuals_FrameBufferNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_FrameBufferNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self, (Arcadia_Engine_Visuals_BackendContext*)enterPassNode->backendContext);
  if (self->backendContext) {
    if (!self->frameBufferResource) {
      Arcadia_Engine_Visuals_BackendContext* backendContext = self->backendContext;
      self->frameBufferResource =
        Arcadia_Engine_Visuals_BackendContext_createFrameBufferResource
        (
          thread,
          (Arcadia_Engine_Visuals_BackendContext*)backendContext
        );
      Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)self->frameBufferResource);
    }
    Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)self->frameBufferResource);
    Arcadia_Engine_Visuals_FrameBufferResource_setSize(thread, self->frameBufferResource, self->width, self->height);
  }
}

static void
Arcadia_Engine_Visuals_FrameBufferNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  if (backendContext == self->backendContext) {
    // Only change something if the backend context changes.
    return;
  }
  if (backendContext) {
    Arcadia_Object_lock(thread, (Arcadia_Object*)backendContext);
  }
  if (self->backendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
  }
  if (self->backendContext) {
    if (self->frameBufferResource) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->frameBufferResource);
      self->frameBufferResource = NULL;
    }
  }
  self->backendContext = backendContext;
}

static void
Arcadia_Engine_Visuals_FrameBufferNode_setSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  self->width = width;
  self->height = height;
}

static void
Arcadia_Engine_Visuals_FrameBufferNode_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  *width = self->width;
  *height = self->height;
}

Arcadia_Engine_Visuals_FrameBufferNode*
Arcadia_Engine_Visuals_FrameBufferNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_FrameBufferNode);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_FrameBufferNode);
}


void
Arcadia_Engine_Visuals_FrameBufferNode_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_FrameBufferNode, getSize, self, width, height); }

void
Arcadia_Engine_Visuals_FrameBufferNode_setSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferNode* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_FrameBufferNode, setSize, self, width, height); }
