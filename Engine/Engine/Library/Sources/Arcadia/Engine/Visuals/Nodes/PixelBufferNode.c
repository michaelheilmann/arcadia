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
#include "Arcadia/Engine/Visuals/Nodes/PixelBufferNode.h"

#include "Arcadia/Engine/Include.h"

static void
Arcadia_Engine_Visuals_PixelBufferNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNode* self
  );

static void
Arcadia_Engine_Visuals_PixelBufferNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNode* self
  );

static void
Arcadia_Engine_Visuals_PixelBufferNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNodeDispatch* self
  );

static void
Arcadia_Engine_Visuals_PixelBufferNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNode* self
  );

static void
Arcadia_Engine_Visuals_PixelBufferNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_Visuals_PixelBufferNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static Arcadia_Media_PixelBuffer*
makePixelBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferDefinition* source
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_PixelBufferNode_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)Arcadia_Engine_Visuals_PixelBufferNode_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_PixelBufferNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_PixelBufferNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.PixelBufferNode", Arcadia_Engine_Visuals_PixelBufferNode,
                         u8"Arcadia.Engine.Visuals.Node", Arcadia_Engine_Visuals_Node,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_PixelBufferNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_PixelBufferNode);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->source = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ADL_PixelBufferDefinition_getType(thread));
  Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)self->source);
  //
  self->pixelBuffer = makePixelBuffer(thread, (Arcadia_ADL_PixelBufferDefinition*)self->source);
  //
  if (Arcadia_ValueStack_isVoidValue(thread, 2)) {
    self->backendContext = NULL;
  } else {
    self->backendContext = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_PixelBufferNode);
}

static void
Arcadia_Engine_Visuals_PixelBufferNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNode* self
  )
{
  if (self->backendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    self->backendContext = NULL;
  }
}

static void
Arcadia_Engine_Visuals_PixelBufferNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNodeDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_NodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Node*, Arcadia_Engine_Visuals_EnterPassNode*)) & Arcadia_Engine_Visuals_PixelBufferNode_renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_PixelBufferNode_setVisualsBackendContextImpl;
}

static void
Arcadia_Engine_Visuals_PixelBufferNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNode* self
  )
{
  if (self->pixelBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->pixelBuffer);
  }
  if (self->source) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->source);
  }
}

static Arcadia_Media_PixelBuffer*
makePixelBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferDefinition* source
  )
{
  Arcadia_Media_PixelBuffer* target = Arcadia_Media_PixelBuffer_create(thread, 0, source->width, source->height, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)source->operations); i < n; ++i) {
    Arcadia_ADL_Reference* e = (Arcadia_ADL_Reference*)Arcadia_List_getObjectReferenceValueAt(thread, source->operations, i);
    Arcadia_Media_PixelBufferOperation* operation;
    if (!e->definition) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)e->definition, _Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_getType(thread))) {
      operation = (Arcadia_Media_PixelBufferOperation*)Arcadia_Media_PixelBufferOperations_CheckerboardFill_create(thread, (Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition*)e->definition);
    } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)e->definition, _Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_getType(thread))) {
      operation = (Arcadia_Media_PixelBufferOperation*)Arcadia_Media_PixelBufferOperations_Fill_create(thread, (Arcadia_ADL_PixelBufferOperations_FillOperationDefinition*)e->definition);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Media_PixelBufferOperation_apply(thread, operation, target);
  }

  return target;
}

static void
Arcadia_Engine_Visuals_PixelBufferNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self, (Arcadia_Engine_Visuals_BackendContext*)enterPassNode->backendContext);
}

static void
Arcadia_Engine_Visuals_PixelBufferNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_PixelBufferNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  Arcadia_BooleanValue backendContextChanged = backendContext != self->backendContext;
  if (backendContextChanged) {
    if (backendContext) {
      Arcadia_Object_lock(thread, (Arcadia_Object*)backendContext);
    }
    if (self->backendContext) {
      Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    }
    self->backendContext = backendContext;
  }
}

Arcadia_Engine_Visuals_PixelBufferNode*
Arcadia_Engine_Visuals_PixelBufferNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_PixelBufferDefinition* source
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_PixelBufferNode);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (source) Arcadia_ValueStack_pushObjectReferenceValue(thread, source); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_PixelBufferNode);
}
