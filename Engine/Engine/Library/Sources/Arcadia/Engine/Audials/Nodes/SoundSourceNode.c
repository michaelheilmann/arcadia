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
#include "Arcadia/Engine/Audials/Nodes/SoundSourceNode.h"

#include "Arcadia/Engine/Include.h"

#define IsLoopingDirty (1)
#define VolumeDirty (2)
#define AllDirty (IsLoopingDirty|VolumeDirty)

static void
Arcadia_Engine_Audials_SoundSourceNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

static void
Arcadia_Engine_Audials_SoundSourceNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

static void
Arcadia_Engine_Audials_SoundSourceNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNodeDispatch* self
  );

static void
Arcadia_Engine_Audials_SoundSourceNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );
  
static Arcadia_Media_DSP*
makeDSP
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* reference
  );

static void
Arcadia_Engine_Audials_SoundSourceNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

static void
Arcadia_Engine_Audials_SoundSourceNode_setAudialsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self,
    Arcadia_Engine_Audials_BackendContext* backendContext
  );

static Arcadia_BooleanValue
Arcadia_Engine_Audials_SoundSourceNode_isPlayingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

static void
Arcadia_Engine_Audials_SoundSourceNode_playImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

static void
Arcadia_Engine_Audials_SoundSourceNode_stopImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

static void
Arcadia_Engine_Audials_SoundSourceNode_setIsLoopingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self,
    Arcadia_BooleanValue isLooping
  );

static Arcadia_BooleanValue
Arcadia_Engine_Audials_SoundSourceNode_getIsLoopingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

static void
Arcadia_Engine_Audials_SoundSourceNode_setVolumeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self,
    Arcadia_Real32Value volume
  );

static Arcadia_Real32Value
Arcadia_Engine_Audials_SoundSourceNode_getVolumeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Audials_SoundSourceNode_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Audials_SoundSourceNode_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Audials_SoundSourceNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Audials_SoundSourceNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.SoundSourceNode", Arcadia_Engine_Audials_SoundSourceNode,
                         u8"Arcadia.Engine.Audials.Node", Arcadia_Engine_Audials_Node,
                         &_typeOperations);

static void
Arcadia_Engine_Audials_SoundSourceNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_SoundSourceNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->source = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ADL_SampleBufferDefinition_getType(thread));
  //
  if (Arcadia_ValueStack_isVoidValue(thread, 2)) {
    self->backendContext = NULL;
  } else {
    self->backendContext = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Audials_BackendContext_getType(thread));
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }
  //
  self->soundSourceResource = NULL;
  //
  self->dirtyBits = AllDirty;
  //
  self->isLooping = Arcadia_BooleanValue_False;
  self->volume = 1.f;
  //  
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_SoundSourceNode);
}

static void
Arcadia_Engine_Audials_SoundSourceNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{
  if (self->backendContext) {
    if (self->soundSourceResource) {
      Arcadia_Engine_Audials_Implementation_Resource_unref(thread, (Arcadia_Engine_Audials_Implementation_Resource*)self->soundSourceResource);
      self->soundSourceResource = NULL;
    }
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    self->backendContext = NULL;
  }
}

static void
Arcadia_Engine_Audials_SoundSourceNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNodeDispatch* self
  )
{
  ((Arcadia_Engine_Audials_NodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Audials_Node*)) & Arcadia_Engine_Audials_SoundSourceNode_renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setAudialsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Audials_BackendContext*)) & Arcadia_Engine_Audials_SoundSourceNode_setAudialsBackendContextImpl;
  //
  ((Arcadia_Engine_Audials_SoundSourceNodeDispatch*)self)->isPlaying = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*)) & Arcadia_Engine_Audials_SoundSourceNode_isPlayingImpl;
  ((Arcadia_Engine_Audials_SoundSourceNodeDispatch*)self)->play = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*)) & Arcadia_Engine_Audials_SoundSourceNode_playImpl;
  ((Arcadia_Engine_Audials_SoundSourceNodeDispatch*)self)->stop = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*)) & Arcadia_Engine_Audials_SoundSourceNode_stopImpl;
  ((Arcadia_Engine_Audials_SoundSourceNodeDispatch*)self)->setIsLooping = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*, Arcadia_BooleanValue)) & Arcadia_Engine_Audials_SoundSourceNode_setIsLoopingImpl;
  ((Arcadia_Engine_Audials_SoundSourceNodeDispatch*)self)->getIsLooping = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*)) & Arcadia_Engine_Audials_SoundSourceNode_getIsLoopingImpl;
  ((Arcadia_Engine_Audials_SoundSourceNodeDispatch*)self)->setVolume = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*, Arcadia_Real32Value)) & Arcadia_Engine_Audials_SoundSourceNode_setVolumeImpl;
  ((Arcadia_Engine_Audials_SoundSourceNodeDispatch*)self)->getVolume = (Arcadia_Real32Value(*)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*)) & Arcadia_Engine_Audials_SoundSourceNode_getVolumeImpl;  
}

static void
Arcadia_Engine_Audials_SoundSourceNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{ 
  if (self->source) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->source);
  }
  if (self->soundSourceResource) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->soundSourceResource);
  }
  if (self->backendContext) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->backendContext);
  }
}

static Arcadia_Media_DSP*
makeDSP
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* reference
  )
{
  Arcadia_ADL_Definition* definition = reference->definition;
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_SineWaveDefinition_getType(thread))) {
    Arcadia_ADL_SineWaveDefinition* sineWaveDefinition = (Arcadia_ADL_SineWaveDefinition*)definition;
    return (Arcadia_Media_DSP*)Arcadia_Media_DSP_SineWave_create(thread, sineWaveDefinition->frequency); // TODO: Unchecked cast from Natural32 to Integer32.
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_WhiteNoiseDefinition_getType(thread))) {
    return (Arcadia_Media_DSP*)Arcadia_Media_DSP_WhiteNoise_create(thread, 1u, 1.0f);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_Engine_Audials_SoundSourceNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{
  if (self->backendContext) {
    if (!self->soundSourceResource) {
      Arcadia_Engine_Audials_BackendContext* backendContext = self->backendContext;

      // (1) sound wave
      Arcadia_Integer32Value sampleRate = 44100;
      Arcadia_Media_SampleBuffer* sampleBuffer = Arcadia_Media_SampleBuffer_create(thread, 2, sampleRate, Arcadia_Media_SampleFormat_Integer16);
      Arcadia_ADL_SampleBufferDefinition* sampleBufferSource = ((Arcadia_Engine_Audials_SoundSourceNode*)self)->source;
      Arcadia_Media_DSP* dsp = makeDSP(thread, sampleBufferSource->dsp);
      Arcadia_Media_SampleBuffer_fill(thread, sampleBuffer, dsp);

      // (2) sound source
      self->soundSourceResource =
        Arcadia_Engine_Audials_BackendContext_createSoundSourceResource
          (
            thread,
            (Arcadia_Engine_Audials_BackendContext*)backendContext,
            sampleBuffer
          );
      Arcadia_Engine_Audials_Implementation_Resource_ref(thread, (Arcadia_Engine_Audials_Implementation_Resource*)self->soundSourceResource);
    }
    Arcadia_Engine_Audials_Implementation_Resource_render(thread, (Arcadia_Engine_Audials_Implementation_Resource*)self->soundSourceResource);
    if (self->dirtyBits & VolumeDirty) {
      Arcadia_Engine_Audials_Implementation_SoundSourceResource_setVolume(thread, self->soundSourceResource, self->volume);
      self->dirtyBits &= ~VolumeDirty;
    }
    if (self->dirtyBits & IsLoopingDirty) {
      Arcadia_Engine_Audials_Implementation_SoundSourceResource_setIsLooping(thread, self->soundSourceResource, self->isLooping);
      self->dirtyBits &= ~IsLoopingDirty;
    }
  }
}

static void
Arcadia_Engine_Audials_SoundSourceNode_setAudialsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self,
    Arcadia_Engine_Audials_BackendContext* backendContext
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
    if (self->soundSourceResource) {
      Arcadia_Engine_Audials_Implementation_Resource_unref(thread, (Arcadia_Engine_Audials_Implementation_Resource*)self->soundSourceResource);
      self->soundSourceResource = NULL;
    }
  }
  self->backendContext = backendContext;
}

static Arcadia_BooleanValue
Arcadia_Engine_Audials_SoundSourceNode_isPlayingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{
  if (!self->soundSourceResource) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Engine_Audials_Implementation_SoundSourceResource_isPlaying(thread, self->soundSourceResource);
}

static void
Arcadia_Engine_Audials_SoundSourceNode_playImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{
  if (!self->soundSourceResource) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (self->dirtyBits & VolumeDirty) {
    Arcadia_Engine_Audials_Implementation_SoundSourceResource_setVolume(thread, self->soundSourceResource, self->volume);
    self->dirtyBits &= ~VolumeDirty;
  }
  Arcadia_Engine_Audials_Implementation_SoundSourceResource_play(thread, self->soundSourceResource);
}

static void
Arcadia_Engine_Audials_SoundSourceNode_stopImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{
  if (!self->soundSourceResource) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Audials_Implementation_SoundSourceResource_stop(thread, self->soundSourceResource);
}

static void
Arcadia_Engine_Audials_SoundSourceNode_setIsLoopingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self,
    Arcadia_BooleanValue isLooping
  )
{
  self->dirtyBits |= self->isLooping != isLooping ? IsLoopingDirty : 0;
  self->isLooping = isLooping;
}

static Arcadia_BooleanValue
Arcadia_Engine_Audials_SoundSourceNode_getIsLoopingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{ return self->isLooping; }

static void
Arcadia_Engine_Audials_SoundSourceNode_setVolumeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self,
    Arcadia_Real32Value volume
  )
{
  if (isnan(volume)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (volume < 0.f) volume = 0.f;
  else if (volume > 1.f) volume = 1.f;
  self->dirtyBits |= self->volume != volume ? VolumeDirty : 0;
  self->volume = volume;
}

static Arcadia_Real32Value
Arcadia_Engine_Audials_SoundSourceNode_getVolumeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{ return self->volume; }

Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_SoundSourceNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_SampleBufferDefinition* source
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Audials_SoundSourceNode);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, source); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Audials_SoundSourceNode);
}

Arcadia_BooleanValue
Arcadia_Engine_Audials_SoundSourceNode_isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Audials_SoundSourceNode, isPlaying, self); }

void
Arcadia_Engine_Audials_SoundSourceNode_pause
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_SoundSourceNode, pause, self); }

void
Arcadia_Engine_Audials_SoundSourceNode_play
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_SoundSourceNode, play, self); }

void
Arcadia_Engine_Audials_SoundSourceNode_stop
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_SoundSourceNode, stop, self); }

void
Arcadia_Engine_Audials_SoundSourceNode_setIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self,
    Arcadia_BooleanValue isLooping
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_SoundSourceNode, setIsLooping, self, isLooping); }

Arcadia_BooleanValue
Arcadia_Engine_Audials_SoundSourceNode_getIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Audials_SoundSourceNode, getIsLooping, self); }

void
Arcadia_Engine_Audials_SoundSourceNode_setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self,
    Arcadia_Real32Value volume
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_SoundSourceNode, setVolume, self, volume); }

Arcadia_Real32Value
Arcadia_Engine_Audials_SoundSourceNode_getVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Audials_SoundSourceNode, getVolume, self); }
