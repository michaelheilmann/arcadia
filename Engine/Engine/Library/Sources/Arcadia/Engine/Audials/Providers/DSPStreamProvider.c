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
#include "Arcadia/Engine/Audials/Providers/DSPStreamProvider.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_DSPStreamProvider* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_DSPStreamProvider* self
  );

static Arcadia_BooleanValue
readImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_DSPStreamProvider* self,
    Arcadia_Media_DSP_Buffer* target,
    Arcadia_SizeValue* numberOfFramesWritten
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_DSPStreamProviderDispatch* self
  );

static const Arcadia_ObjectType_Operations objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.DSPStreamProvider", Arcadia_Engine_Audials_DSPStreamProvider,
                         u8"Arcadia.Engine.Audials.AudioStreamProvider", Arcadia_Engine_Audials_AudioStreamProvider,
                         &typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_DSPStreamProvider* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_DSPStreamProvider);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->dsp = (Arcadia_Media_DSP*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_Media_DSP_getType(thread));
  self->sampleRate = Arcadia_ValueStack_getNatural32Value(thread, 2);
  self->parent.sampleRate = self->sampleRate;
  self->framesRemaining = Arcadia_ValueStack_getSizeValue(thread, 1);
  self->isInfinite = Arcadia_BooleanValue_False;
  if (!self->sampleRate) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_DSPStreamProvider);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_DSPStreamProvider* self
  )
{
  Arcadia_Object_visit(thread, (Arcadia_Object*)self->dsp);
}

static Arcadia_BooleanValue
readImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_DSPStreamProvider* self,
    Arcadia_Media_DSP_Buffer* target,
    Arcadia_SizeValue* numberOfFramesWritten
  )
{
  Arcadia_SizeValue requested = Arcadia_Media_DSP_Buffer_getNumberOfSamples(thread, target);
  if (Arcadia_Media_DSP_Buffer_getSampleRate(thread, target) != self->sampleRate) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  *numberOfFramesWritten = 0;
  if (!self->isInfinite && !self->framesRemaining) return Arcadia_BooleanValue_False;
  Arcadia_SizeValue frames = self->isInfinite || requested < self->framesRemaining ? requested : self->framesRemaining;
  if (!frames) return Arcadia_BooleanValue_False;
  if (frames != requested) {
    Arcadia_Media_DSP_Buffer* partial = Arcadia_Media_DSP_Buffer_create(thread, self->sampleRate, frames);
    Arcadia_Object_lock(thread, (Arcadia_Object*)partial);
    Arcadia_Media_DSP_render(thread, self->dsp, partial);
    Arcadia_Real32Value* source = Arcadia_Media_DSP_Buffer_getSamples(thread, partial);
    Arcadia_Real32Value* destination = Arcadia_Media_DSP_Buffer_getSamples(thread, target);
    for (Arcadia_SizeValue i = 0; i < frames; ++i) destination[i] = source[i];
    Arcadia_Object_unlock(thread, (Arcadia_Object*)partial);
  } else {
    Arcadia_Media_DSP_render(thread, self->dsp, target);
  }
  if (!self->isInfinite) self->framesRemaining -= frames;
  *numberOfFramesWritten = frames;
  return Arcadia_BooleanValue_True;
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_DSPStreamProviderDispatch* self
  )
{
  ((Arcadia_Engine_Audials_AudioStreamProviderDispatch*)self)->read = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Engine_Audials_AudioStreamProvider*, Arcadia_Media_DSP_Buffer*, Arcadia_SizeValue*))&readImpl;
}

Arcadia_Engine_Audials_DSPStreamProvider*
Arcadia_Engine_Audials_DSPStreamProvider_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* dsp,
    Arcadia_Natural32Value sampleRate,
    Arcadia_SizeValue numberOfFrames
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Audials_DSPStreamProvider);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, dsp);
  Arcadia_ValueStack_pushNatural32Value(thread, sampleRate);
  Arcadia_ValueStack_pushSizeValue(thread, numberOfFrames);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_Engine_Audials_DSPStreamProvider);
}

Arcadia_Engine_Audials_DSPStreamProvider*
Arcadia_Engine_Audials_DSPStreamProvider_createInfinite
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* dsp,
    Arcadia_Natural32Value sampleRate
  )
{
  Arcadia_Engine_Audials_DSPStreamProvider* self =
    Arcadia_Engine_Audials_DSPStreamProvider_create(thread, dsp, sampleRate, 0);
  self->isInfinite = Arcadia_BooleanValue_True;
  return self;
}
