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
#include "Arcadia/Engine/Audials/Nodes/StreamingSoundSourceNode.h"
#include "Arcadia/Engine/Audials/BackendContext.h"
#include "Arcadia/Engine/Audials/Resources/StreamingSoundSourceResource.h"
#include <math.h>

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNodeDispatch* self
  );

static void
renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

static void
setBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self,
    Arcadia_Engine_Audials_BackendContext* backendContext
  );

static Arcadia_BooleanValue
isPlayingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

static void
playImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

static void
pauseImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

static void
stopImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

static void
setVolumeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self,
    Arcadia_Real32Value volume
  );

static const Arcadia_ObjectType_Operations objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.StreamingSoundSourceNode", Arcadia_Engine_Audials_StreamingSoundSourceNode,
                         u8"Arcadia.Engine.Audials.Node", Arcadia_Engine_Audials_Node, &typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_StreamingSoundSourceNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->provider = (Arcadia_Engine_Audials_AudioStreamProvider*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_Audials_AudioStreamProvider_getType(thread));
  Arcadia_Object_lock(thread, (Arcadia_Object*)self->provider);
  if (Arcadia_ValueStack_isVoidValue(thread, 2)) self->backendContext = NULL;
  else {
    self->backendContext = (Arcadia_Engine_Audials_BackendContext*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Audials_BackendContext_getType(thread));
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }
  self->resource = NULL;
  self->volume = 1.f;
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_StreamingSoundSourceNode);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  if (self->provider) Arcadia_Object_visit(thread, (Arcadia_Object*)self->provider);
  if (self->backendContext) Arcadia_Object_visit(thread, (Arcadia_Object*)self->backendContext);
  if (self->resource) Arcadia_Object_visit(thread, (Arcadia_Object*)self->resource);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  if (self->resource) Arcadia_Engine_Audials_Implementation_Resource_unref(thread, (Arcadia_Engine_Audials_Implementation_Resource*)self->resource);
  if (self->backendContext) Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
  if (self->provider) Arcadia_Object_unlock(thread, (Arcadia_Object*)self->provider);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNodeDispatch* self
  )
{
  ((Arcadia_Engine_Audials_NodeDispatch*)self)->render = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Node*))&renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setAudialsBackendContext = (void(*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Audials_BackendContext*))&setBackendContextImpl;
  self->isPlaying = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Engine_Audials_StreamingSoundSourceNode*))&isPlayingImpl;
  self->play = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_StreamingSoundSourceNode*))&playImpl;
  self->pause = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_StreamingSoundSourceNode*))&pauseImpl;
  self->stop = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_StreamingSoundSourceNode*))&stopImpl;
  self->setVolume = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_StreamingSoundSourceNode*, Arcadia_Real32Value))&setVolumeImpl;
}

static void
renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  if (!self->backendContext) return;
  if (!self->resource) {
    self->resource = Arcadia_Engine_Audials_BackendContext_createStreamingSoundSourceResource(thread, self->backendContext, self->provider);
    Arcadia_Engine_Audials_Implementation_Resource_ref(thread, (Arcadia_Engine_Audials_Implementation_Resource*)self->resource);
  }
  Arcadia_Engine_Audials_Implementation_Resource_render(thread, (Arcadia_Engine_Audials_Implementation_Resource*)self->resource);
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_setVolume(thread, self->resource, self->volume);
}

static void
setBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self,
    Arcadia_Engine_Audials_BackendContext* backendContext
  )
{
  if (self->backendContext == backendContext) return;
  if (backendContext) Arcadia_Object_lock(thread, (Arcadia_Object*)backendContext);
  if (self->resource) {
    Arcadia_Engine_Audials_Implementation_Resource_unref(thread, (Arcadia_Engine_Audials_Implementation_Resource*)self->resource);
    self->resource = NULL;
  }
  if (self->backendContext) Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
  self->backendContext = backendContext;
}

static Arcadia_BooleanValue
isPlayingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  if (!self->resource) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_isPlaying(thread, self->resource);
}

static void
playImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  if (!self->resource) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_setVolume(thread, self->resource, self->volume);
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_play(thread, self->resource);
}

static void
pauseImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  if (!self->resource) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_pause(thread, self->resource);
}

static void
stopImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  if (!self->resource) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_stop(thread, self->resource);
}

static void
setVolumeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self,
    Arcadia_Real32Value volume
  )
{
  if (isnan(volume)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (volume < 0.f) volume = 0.f;
  if (volume > 1.f) volume = 1.f;
  self->volume = volume;
  if (self->resource) Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_setVolume(thread, self->resource, volume);
}

Arcadia_Engine_Audials_StreamingSoundSourceNode*
Arcadia_Engine_Audials_StreamingSoundSourceNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_Engine_Audials_AudioStreamProvider* provider
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Audials_StreamingSoundSourceNode);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)provider);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Audials_StreamingSoundSourceNode);
}

Arcadia_BooleanValue
Arcadia_Engine_Audials_StreamingSoundSourceNode_isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  Arcadia_VirtualCallWithReturn(Arcadia_Engine_Audials_StreamingSoundSourceNode, isPlaying, self);
}

void
Arcadia_Engine_Audials_StreamingSoundSourceNode_play
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  Arcadia_VirtualCall(Arcadia_Engine_Audials_StreamingSoundSourceNode, play, self);
}

void
Arcadia_Engine_Audials_StreamingSoundSourceNode_pause
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  Arcadia_VirtualCall(Arcadia_Engine_Audials_StreamingSoundSourceNode, pause, self);
}

void
Arcadia_Engine_Audials_StreamingSoundSourceNode_stop
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  )
{
  Arcadia_VirtualCall(Arcadia_Engine_Audials_StreamingSoundSourceNode, stop, self);
}

void
Arcadia_Engine_Audials_StreamingSoundSourceNode_setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self,
    Arcadia_Real32Value volume
  )
{
  Arcadia_VirtualCall(Arcadia_Engine_Audials_StreamingSoundSourceNode, setVolume, self, volume);
}
