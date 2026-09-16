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
#include "Arcadia/Media/DSP/Affine.h"

static Arcadia_SizeValue
getNumberOfOutputPins(Arcadia_Thread* thread, Arcadia_Media_DSP_Affine* self);

static Arcadia_SizeValue
getNumberOfInputPins(Arcadia_Thread* thread, Arcadia_Media_DSP_Affine* self);

static void
Arcadia_Media_DSP_Affine_constructImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_Affine* self);

static void
Arcadia_Media_DSP_Affine_visitImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_Affine* self);

static void
Arcadia_Media_DSP_Affine_render(Arcadia_Thread* thread, Arcadia_Media_DSP_Affine* self, Arcadia_Media_DSP_Buffer* target);

static void
Arcadia_Media_DSP_Affine_reset(Arcadia_Thread* thread, Arcadia_Media_DSP_Affine* self);

static void
Arcadia_Media_DSP_Affine_reseed(Arcadia_Thread* thread, Arcadia_Media_DSP_Affine* self, Arcadia_Natural32Value seed);

static void
Arcadia_Media_DSP_Affine_initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_AffineDispatch* self);

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_DSP_Affine_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_DSP_Affine_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_DSP_Affine_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.DSP.Affine", Arcadia_Media_DSP_Affine,
                         u8"Arcadia.Media.DSP", Arcadia_Media_DSP,
                         &_typeOperations);

static Arcadia_SizeValue
getNumberOfOutputPins(Arcadia_Thread* thread, Arcadia_Media_DSP_Affine* self)
{ return Arcadia_SizeValue_Literal(1); }

static Arcadia_SizeValue
getNumberOfInputPins(Arcadia_Thread* thread, Arcadia_Media_DSP_Affine* self)
{ return Arcadia_SizeValue_Literal(1); }

static void
Arcadia_Media_DSP_Affine_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Affine* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_DSP_Affine);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->input = (Arcadia_Media_DSP*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_Media_DSP_getType(thread));
  self->scale = Arcadia_ValueStack_getReal32Value(thread, 2);
  self->bias = Arcadia_ValueStack_getReal32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_Media_DSP_Affine);
}

static void
Arcadia_Media_DSP_Affine_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Affine* self
  )
{
  if (self->input) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->input);
  }
}

static void
Arcadia_Media_DSP_Affine_render
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Affine* self,
    Arcadia_Media_DSP_Buffer* target
  )
{
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
    for (Arcadia_SizeValue i = 0, n = numberOfSamples; i < n; ++i) {
      outputSamples[i] = inputSamples[i] * self->scale + self->bias;
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)inputBuffer);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)inputBuffer);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_Media_DSP_Affine_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_AffineDispatch* self
  )
{
  ((Arcadia_Media_DSPDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Media_DSP_Buffer*)) & Arcadia_Media_DSP_Affine_render;
  ((Arcadia_Media_DSPDispatch*)self)->reset = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & Arcadia_Media_DSP_Affine_reset;
  ((Arcadia_Media_DSPDispatch*)self)->reseed = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Natural32Value)) & Arcadia_Media_DSP_Affine_reseed;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfOutputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfOutputPins;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfInputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfInputPins;
}

static void
Arcadia_Media_DSP_Affine_reset(Arcadia_Thread* thread, Arcadia_Media_DSP_Affine* self)
{/*Intentionally empty.*/}

static void
Arcadia_Media_DSP_Affine_reseed(Arcadia_Thread* thread, Arcadia_Media_DSP_Affine* self, Arcadia_Natural32Value seed)
{/*Intentionally empty.*/}

Arcadia_Media_DSP_Affine*
Arcadia_Media_DSP_Affine_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* input,
    Arcadia_Real32Value scale,
    Arcadia_Real32Value bias
  )
{
  if (!input) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  _Arcadia_BeginCreate(Arcadia_Media_DSP_Affine);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)input);
  Arcadia_ValueStack_pushReal32Value(thread, scale);
  Arcadia_ValueStack_pushReal32Value(thread, bias);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_Media_DSP_Affine);
}
