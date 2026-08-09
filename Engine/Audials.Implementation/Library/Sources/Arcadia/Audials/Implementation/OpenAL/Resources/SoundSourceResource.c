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

#include "Arcadia/Audials/Implementation/OpenAL/Resources/SoundSourceResource.h"

#include "Arcadia/Audials/Implementation/OpenAL/BackendContext.h"
#include "Arcadia/Media/Include.h"
#include <assert.h>

#define DataDirty (1)
#define IsLoopingDirty (2)
#define VolumeDirty (4)
#define AllDirty (DataDirty|IsLoopingDirty|VolumeDirty)

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResourceDispatch* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static Arcadia_BooleanValue
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_pause
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_play
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_stop
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_setIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self,
    Arcadia_BooleanValue isLooping
  );

static Arcadia_BooleanValue
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_getIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self,
    Arcadia_Real32Value volume
  );

static Arcadia_Real32Value
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_getVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.Implementation.OpenAL.SoundSourceResource", Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource,
                         u8"Arcadia.Engine.Audials.Implementation.SoundSourceResource", Arcadia_Engine_Audials_Implementation_SoundSourceResource,
                         &_typeOperations);

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource);
  {
    Arcadia_Value t;
    t = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread, &t);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->sampleBuffer = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Media_SampleBuffer_getType(thread));
  //
  self->isLooping = Arcadia_BooleanValue_False;
  self->volume = 1.f;
  //
  self->dirtyBits = AllDirty;
  self->byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  self->alBufferID = 0;
  self->alSourceID = 0;
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource);
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResourceDispatch* self
  )
{
  //
  ((Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch*)self)->isPlaying = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_isPlaying;
  ((Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch*)self)->play = (void (*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_play;
  ((Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch*)self)->pause = (void (*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_pause;
  ((Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch*)self)->stop = (void (*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_stop;
  ((Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch*)self)->setIsLooping = (void (*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*, Arcadia_BooleanValue)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_setIsLooping;
  ((Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch*)self)->getIsLooping = (Arcadia_BooleanValue (*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_getIsLooping;
  ((Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch*)self)->setVolume = (void (*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*, Arcadia_Real32Value)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_setVolume;
  ((Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch*)self)->getVolume = (Arcadia_Real32Value(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_getVolume;
  //
  ((Arcadia_Engine_Audials_Implementation_ResourceDispatch*)self)->load = (void (*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_Resource*)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_loadImpl;
  ((Arcadia_Engine_Audials_Implementation_ResourceDispatch*)self)->unlink = (void (*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_Resource*)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_unlinkImpl;
  ((Arcadia_Engine_Audials_Implementation_ResourceDispatch*)self)->unload = (void (*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_Resource*)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_unloadImpl;
  ((Arcadia_Engine_Audials_Implementation_ResourceDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_Resource*)) & Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_renderImpl;
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  assert(0 == self->alBufferID);
  assert(0 == self->alSourceID);
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  if (self->sampleBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->sampleBuffer);
  }
  if (self->byteBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->byteBuffer);
  }
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  if (0 == self->alBufferID) {
    alGenBuffers(1, &self->alBufferID);
    if (alGetError()) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    self->dirtyBits |= AllDirty;
  }
  if (0 == self->alSourceID) {
    alGenSources(1, &self->alSourceID);
    if (alGetError()) {
      alDeleteBuffers(1, &self->alBufferID);
      self->alBufferID = 0;
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    alSourcei(self->alSourceID, AL_LOOPING, AL_FALSE);
    alSourcei(self->alSourceID, AL_SOURCE_RELATIVE, AL_TRUE);
    self->dirtyBits |= AllDirty;
  }
  // Volumne.
  if (self->dirtyBits & VolumeDirty) {
    alSourcef(self->alSourceID, AL_GAIN, self->volume);
    if (alGetError()) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    self->dirtyBits &= ~VolumeDirty;
  }
  // Looping.
  if (self->dirtyBits & IsLoopingDirty) {
    alSourcei(self->alSourceID, AL_LOOPING, self->isLooping ? AL_TRUE : AL_FALSE);
    if (alGetError()) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    self->dirtyBits &= ~IsLoopingDirty;
  }
  if (self->dirtyBits & DataDirty) {
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      if (self->sampleBuffer->sampleFormat != Arcadia_Media_SampleFormat_Integer16) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
      alBufferData(self->alBufferID, AL_FORMAT_MONO16, self->sampleBuffer->bytes,
                                                       self->sampleBuffer->length * self->sampleBuffer->sampleRate * Arcadia_Media_SampleFormat_getNumberOfBytes(thread, self->sampleBuffer->sampleFormat),
                                                       self->sampleBuffer->sampleRate);
      alSourcei(self->alSourceID, AL_BUFFER, self->alBufferID);
      if (alGetError()) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Thread_popJumpTarget(thread);
      self->dirtyBits &= ~DataDirty;
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_jump(thread);
    }
  }
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_loadImpl(thread, self);
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  assert(0 == self->alBufferID);
  assert(0 == self->alSourceID);
  ((Arcadia_Engine_Audials_Implementation_Resource*)self)->context = NULL;
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  if (self->alSourceID) {
    alDeleteSources(1, &self->alSourceID);
    self->alSourceID = 0;
  }
  if (self->alBufferID) {
    alDeleteBuffers(1, &self->alBufferID);
    self->alBufferID = 0;
  }
}

static Arcadia_BooleanValue
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  if (!self->alSourceID) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  ALint alSourceState;
  alGetSourcei(self->alSourceID, AL_SOURCE_STATE, &alSourceState);
  if (alGetError()) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  return AL_PLAYING == alSourceState;
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_pause
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  if (!self->alSourceID) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  alSourcePause(self->alSourceID);
  if (alGetError()) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_play
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  if (!self->alSourceID) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_isPlaying(thread, self)) {
    alSourcePlay(self->alSourceID);
    if (alGetError()) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_stop
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  if (!self->alSourceID) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  alSourceStop(self->alSourceID);
  if (alGetError()) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_setIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self,
    Arcadia_BooleanValue isLooping
  )
{
  self->dirtyBits |= self->isLooping != isLooping ? IsLoopingDirty : 0;
  self->isLooping = isLooping;
}

static Arcadia_BooleanValue
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_getIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{
  return self->isLooping;
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self,
    Arcadia_Real32Value volume
  )
{
  if (!self->alSourceID || !Arcadia_Real32Value_isFinite(thread, volume)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  ALfloat minimumGain, maximumGain;
  alGetSourcef(self->alSourceID, AL_MIN_GAIN, &minimumGain);
  alGetSourcef(self->alSourceID, AL_MAX_GAIN, &maximumGain);
  if (alGetError()) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  if (volume < minimumGain) volume = minimumGain;
  if (volume > maximumGain) volume = maximumGain;
  alSourcef(self->alSourceID, AL_GAIN, volume);
  if (alGetError()) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  self->volume = volume;
}

static Arcadia_Real32Value
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_getVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* self
  )
{ return self->volume; }

Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource*
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* backendContext,
    Arcadia_Media_SampleBuffer* sampleBuffer
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (sampleBuffer) Arcadia_ValueStack_pushObjectReferenceValue(thread, sampleBuffer); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource);
}
