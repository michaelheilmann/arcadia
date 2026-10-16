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

#include "Arcadia/Audials/Implementation/OpenAL/Resources/StreamingSoundSourceResource.h"
#include <assert.h>
#include <stdlib.h>

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResourceDispatch* self
  );

static void
loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static void
renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static void
unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static void
unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static Arcadia_BooleanValue
isPlayingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static void
playImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static void
pauseImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static void
stopImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static void
setVolumeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self,
    Arcadia_Real32Value volume
  );

static void
updateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  );

static Arcadia_BooleanValue
fillBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self,
    ALuint bufferID
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

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.Implementation.OpenAL.StreamingSoundSourceResource", Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource,
                         u8"Arcadia.Engine.Audials.Implementation.StreamingSoundSourceResource", Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource, &typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource);
  {
    Arcadia_Value context = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread, &context);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->provider = (Arcadia_Engine_Audials_AudioStreamProvider*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_Audials_AudioStreamProvider_getType(thread));
  Arcadia_Object_lock(thread, (Arcadia_Object*)self->provider);
  self->sampleRate = Arcadia_Engine_Audials_AudioStreamProvider_getSampleRate(thread, self->provider);
  if (!self->sampleRate) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->renderBuffer = Arcadia_Media_DSP_Buffer_create(thread, self->sampleRate, ARCADIA_OPENAL_STREAM_CHUNK_FRAMES);
  Arcadia_Object_lock(thread, (Arcadia_Object*)self->renderBuffer);
  self->pcmBuffer = (Arcadia_Integer16Value*)malloc(sizeof(Arcadia_Integer16Value) * ARCADIA_OPENAL_STREAM_CHUNK_FRAMES);
  if (!self->pcmBuffer) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  self->endOfStream = Arcadia_BooleanValue_False;
  self->stopped = Arcadia_BooleanValue_False;
  self->volume = 1.f;
  self->alSourceID = 0;
  for (Arcadia_SizeValue i = 0; i < ARCADIA_OPENAL_STREAM_BUFFER_COUNT; ++i) self->alBufferIDs[i] = 0;
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  )
{
  assert(!self->alSourceID);
  for (Arcadia_SizeValue i = 0; i < ARCADIA_OPENAL_STREAM_BUFFER_COUNT; ++i) assert(!self->alBufferIDs[i]);
  if (self->renderBuffer) Arcadia_Object_unlock(thread, (Arcadia_Object*)self->renderBuffer);
  if (self->provider) Arcadia_Object_unlock(thread, (Arcadia_Object*)self->provider);
  free(self->pcmBuffer);
  self->pcmBuffer = NULL;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  )
{
  if (self->provider) Arcadia_Object_visit(thread, (Arcadia_Object*)self->provider);
  if (self->renderBuffer) Arcadia_Object_visit(thread, (Arcadia_Object*)self->renderBuffer);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResourceDispatch* self
  )
{
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch* streamDispatch = (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)self;
  streamDispatch->isPlaying = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*))&isPlayingImpl;
  streamDispatch->play = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*))&playImpl;
  streamDispatch->pause = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*))&pauseImpl;
  streamDispatch->stop = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*))&stopImpl;
  streamDispatch->setVolume = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*, Arcadia_Real32Value))&setVolumeImpl;
  streamDispatch->update = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*))&updateImpl;
  Arcadia_Engine_Audials_Implementation_ResourceDispatch* resourceDispatch = (Arcadia_Engine_Audials_Implementation_ResourceDispatch*)self;
  resourceDispatch->load = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_Resource*))&loadImpl;
  resourceDispatch->render = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_Resource*))&renderImpl;
  resourceDispatch->unload = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_Resource*))&unloadImpl;
  resourceDispatch->unlink = (void(*)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_Resource*))&unlinkImpl;
}

static void
loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  )
{
  if (!self->alSourceID) {
    alGenSources(1, &self->alSourceID);
    if (alGetError()) goto alFailure;
    alSourcei(self->alSourceID, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcef(self->alSourceID, AL_GAIN, self->volume);
    alGenBuffers(ARCADIA_OPENAL_STREAM_BUFFER_COUNT, self->alBufferIDs);
    if (alGetError()) goto alFailure;
    for (Arcadia_SizeValue i = 0; i < ARCADIA_OPENAL_STREAM_BUFFER_COUNT; ++i) {
      if (!fillBufferImpl(thread, self, self->alBufferIDs[i])) break;
    }
  }
  return;
alFailure:
  Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
  Arcadia_Thread_jump(thread);
}

static void
renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  )
{
  loadImpl(thread, self);
}

static Arcadia_BooleanValue
fillBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self,
    ALuint bufferID
  )
{
  if (self->endOfStream) return Arcadia_BooleanValue_False;
  Arcadia_SizeValue framesWritten = 0;
  if (!Arcadia_Engine_Audials_AudioStreamProvider_read(thread, self->provider, self->renderBuffer, &framesWritten)) {
    self->endOfStream = Arcadia_BooleanValue_True;
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Real32Value* source = Arcadia_Media_DSP_Buffer_getSamples(thread, self->renderBuffer);
  for (Arcadia_SizeValue i = 0; i < framesWritten; ++i) {
    Arcadia_Real32Value sample = source[i];
    if (sample < -1.f) sample = -1.f;
    if (sample > 1.f) sample = 1.f;
    self->pcmBuffer[i] = (Arcadia_Integer16Value)(sample < 0.f ? sample * 32768.f : sample * 32767.f);
  }
  alBufferData(bufferID, AL_FORMAT_MONO16, self->pcmBuffer, (ALsizei)(framesWritten * sizeof(Arcadia_Integer16Value)), (ALsizei)self->sampleRate);
  if (alGetError()) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  alSourceQueueBuffers(self->alSourceID, 1, &bufferID);
  if (alGetError()) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_BooleanValue_True;
}

static void
updateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  )
{
  if (!self->alSourceID || self->stopped) return;
  ALint sourceState = 0;
  alGetSourcei(self->alSourceID, AL_SOURCE_STATE, &sourceState);
  if (alGetError()) goto alFailure;
  if (sourceState == AL_PAUSED) return;
  ALint processed = 0;
  alGetSourcei(self->alSourceID, AL_BUFFERS_PROCESSED, &processed);
  if (alGetError()) goto alFailure;
  while (processed-- > 0) {
    ALuint bufferID = 0;
    alSourceUnqueueBuffers(self->alSourceID, 1, &bufferID);
    if (alGetError()) goto alFailure;
    fillBufferImpl(thread, self, bufferID);
  }
  ALint queued = 0;
  ALint state = 0;
  alGetSourcei(self->alSourceID, AL_BUFFERS_QUEUED, &queued);
  alGetSourcei(self->alSourceID, AL_SOURCE_STATE, &state);
  if (alGetError()) goto alFailure;
  if (queued > 0 && state != AL_PLAYING && state != AL_PAUSED) {
    alSourcePlay(self->alSourceID);
    if (alGetError()) goto alFailure;
  }
  return;
alFailure:
  Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
  Arcadia_Thread_jump(thread);
}

static Arcadia_BooleanValue
isPlayingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  )
{
  if (!self->alSourceID) return Arcadia_BooleanValue_False;
  ALint state = 0;
  alGetSourcei(self->alSourceID, AL_SOURCE_STATE, &state);
  if (alGetError()) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  return state == AL_PLAYING;
}

static void
playImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  )
{
  if (!self->alSourceID || self->stopped) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  alSourcePlay(self->alSourceID);
  if (alGetError()) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
pauseImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
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
stopImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
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
  ALint queued = 0;
  alGetSourcei(self->alSourceID, AL_BUFFERS_QUEUED, &queued);
  if (alGetError()) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  while (queued-- > 0) {
    ALuint ignored = 0;
    alSourceUnqueueBuffers(self->alSourceID, 1, &ignored);
    if (alGetError()) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }
  self->stopped = Arcadia_BooleanValue_True;
}

static void
setVolumeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self,
    Arcadia_Real32Value volume
  )
{
  self->volume = volume;
  if (self->alSourceID) {
    alSourcef(self->alSourceID, AL_GAIN, volume);
    if (alGetError()) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }
}

static void
unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  )
{
  if (self->alSourceID) {
    alDeleteSources(1, &self->alSourceID);
    self->alSourceID = 0;
  }
  for (Arcadia_SizeValue i = 0; i < ARCADIA_OPENAL_STREAM_BUFFER_COUNT; ++i) {
    if (self->alBufferIDs[i]) {
      alDeleteBuffers(1, &self->alBufferIDs[i]);
      self->alBufferIDs[i] = 0;
    }
  }
}

static void
unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource* self
  )
{
  ((Arcadia_Engine_Audials_Implementation_Resource*)self)->context = NULL;
}

Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource*
Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* backendContext,
    Arcadia_Engine_Audials_AudioStreamProvider* provider
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)backendContext);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)provider);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource);
}
