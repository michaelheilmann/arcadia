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

#define ARCADIA_MEDIA_PRIVATE (1)
#include "Arcadia/Media/DSP/LowPass.h"

#include <math.h>

static Arcadia_SizeValue
getNumberOfOutputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self
  );

static Arcadia_SizeValue
getNumberOfInputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self
  );

static void
Arcadia_Media_DSP_LowPass_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self
  );

static void
Arcadia_Media_DSP_LowPass_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPassDispatch* self
  );

static void
Arcadia_Media_DSP_LowPass_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self
  );

static void
Arcadia_Media_DSP_LowPass_render
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self,
    Arcadia_Media_DSP_Buffer* target
  );

static void
Arcadia_Media_DSP_LowPass_reset
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self
  );

static void
Arcadia_Media_DSP_LowPass_reseed
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self,
    Arcadia_Natural32Value seed
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_DSP_LowPass_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_DSP_LowPass_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_DSP_LowPass_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.DSP.LowPass", Arcadia_Media_DSP_LowPass,
                         u8"Arcadia.Media.DSP", Arcadia_Media_DSP,
                         &_typeOperations);

static Arcadia_SizeValue
getNumberOfOutputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self
  )
{ return Arcadia_SizeValue_Literal(1); }

static Arcadia_SizeValue
getNumberOfInputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self
  )
{ return Arcadia_SizeValue_Literal(1); }

static void
Arcadia_Media_DSP_LowPass_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_DSP_LowPass);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->input = (Arcadia_Media_DSP*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Media_DSP_getType(thread));
  self->cutoff = Arcadia_ValueStack_getReal32Value(thread, 1);
  if (self->cutoff <= 0.0f) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->state = 0.0f;
  Arcadia_LeaveConstructor(Arcadia_Media_DSP_LowPass);
}

static void
Arcadia_Media_DSP_LowPass_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPassDispatch* self
  )
{
  ((Arcadia_Media_DSPDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Media_DSP_Buffer*)) & Arcadia_Media_DSP_LowPass_render;
  ((Arcadia_Media_DSPDispatch*)self)->reset = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & Arcadia_Media_DSP_LowPass_reset;
  ((Arcadia_Media_DSPDispatch*)self)->reseed = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Natural32Value)) & Arcadia_Media_DSP_LowPass_reseed;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfOutputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfOutputPins;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfInputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfInputPins;
}

static void
Arcadia_Media_DSP_LowPass_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self
  )
{
  if (self->input) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->input);
  }
}

static void
Arcadia_Media_DSP_LowPass_render
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self,
    Arcadia_Media_DSP_Buffer* target
  )
{
  static const Arcadia_Real32Value pi = 3.14159265358979323846f;
  Arcadia_Natural32Value sampleRate = Arcadia_Media_DSP_Buffer_getSampleRate(thread, target);
  Arcadia_SizeValue numberOfSamples = Arcadia_Media_DSP_Buffer_getNumberOfSamples(thread, target);
  Arcadia_Media_DSP_Buffer* inputBuffer = Arcadia_Media_DSP_Buffer_create(thread, sampleRate, numberOfSamples);
  Arcadia_Object_lock(thread, (Arcadia_Object*)inputBuffer);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Media_DSP_render(thread, self->input, inputBuffer);
    Arcadia_Real32Value const* inputSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, inputBuffer);
    Arcadia_Real32Value* outputSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, target);
    Arcadia_Real32Value alpha = 1.0f - expf(-2.0f * pi * self->cutoff / (Arcadia_Real32Value)sampleRate);
    Arcadia_Real32Value state = self->state;
    for (Arcadia_SizeValue i = 0, n = numberOfSamples; i < n; ++i) {
      state = state + alpha * (inputSamples[i] - state);
      outputSamples[i] = state;
    }
    self->state = state;
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)inputBuffer);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)inputBuffer);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_Media_DSP_LowPass_reset
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self
  )
{ self->state = 0.0f; }

static void
Arcadia_Media_DSP_LowPass_reseed
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_LowPass* self,
    Arcadia_Natural32Value seed
  )
{/*Intentionally empty.*/}

Arcadia_Media_DSP_LowPass*
Arcadia_Media_DSP_LowPass_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* input,
    Arcadia_Real32Value cutoff
  )
{
  if (!input || cutoff <= 0.0f) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  _Arcadia_BeginCreate(Arcadia_Media_DSP_LowPass);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)input);
  Arcadia_ValueStack_pushReal32Value(thread, cutoff);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Media_DSP_LowPass);
}
