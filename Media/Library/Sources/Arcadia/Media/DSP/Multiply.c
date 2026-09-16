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
#include "Arcadia/Media/DSP/Multiply.h"

static Arcadia_SizeValue
getNumberOfOutputPins(Arcadia_Thread* thread, Arcadia_Media_DSP_Multiply* self);

static Arcadia_SizeValue
getNumberOfInputPins(Arcadia_Thread* thread, Arcadia_Media_DSP_Multiply* self);

static void
Arcadia_Media_DSP_Multiply_constructImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_Multiply* self);

static void
Arcadia_Media_DSP_Multiply_visitImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_Multiply* self);

static void
Arcadia_Media_DSP_Multiply_render(Arcadia_Thread* thread, Arcadia_Media_DSP_Multiply* self, Arcadia_Media_DSP_Buffer* target);

static void
Arcadia_Media_DSP_Multiply_reset(Arcadia_Thread* thread, Arcadia_Media_DSP_Multiply* self);

static void
Arcadia_Media_DSP_Multiply_reseed(Arcadia_Thread* thread, Arcadia_Media_DSP_Multiply* self, Arcadia_Natural32Value seed);

static void
Arcadia_Media_DSP_Multiply_initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_MultiplyDispatch* self);

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_DSP_Multiply_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_DSP_Multiply_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_DSP_Multiply_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.DSP.Multiply", Arcadia_Media_DSP_Multiply,
                         u8"Arcadia.Media.DSP", Arcadia_Media_DSP,
                         &_typeOperations);

static Arcadia_SizeValue
getNumberOfOutputPins(Arcadia_Thread* thread, Arcadia_Media_DSP_Multiply* self)
{ return Arcadia_SizeValue_Literal(1); }

static Arcadia_SizeValue
getNumberOfInputPins(Arcadia_Thread* thread, Arcadia_Media_DSP_Multiply* self)
{ return Arcadia_SizeValue_Literal(2); }

static void
Arcadia_Media_DSP_Multiply_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Multiply* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_DSP_Multiply);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->left = (Arcadia_Media_DSP*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Media_DSP_getType(thread));
  self->right = (Arcadia_Media_DSP*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Media_DSP_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_Media_DSP_Multiply);
}

static void
Arcadia_Media_DSP_Multiply_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Multiply* self
  )
{
  if (self->left) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->left);
  }
  if (self->right) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->right);
  }
}

static void
Arcadia_Media_DSP_Multiply_render
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Multiply* self,
    Arcadia_Media_DSP_Buffer* target
  )
{
  Arcadia_Natural32Value sampleRate = Arcadia_Media_DSP_Buffer_getSampleRate(thread, target);
  Arcadia_SizeValue numberOfSamples = Arcadia_Media_DSP_Buffer_getNumberOfSamples(thread, target);
  Arcadia_Media_DSP_Buffer* leftBuffer = NULL;
  Arcadia_Media_DSP_Buffer* rightBuffer = NULL;
  Arcadia_BooleanValue leftBufferLocked = Arcadia_BooleanValue_False;
  Arcadia_BooleanValue rightBufferLocked = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    leftBuffer = Arcadia_Media_DSP_Buffer_create(thread, sampleRate, numberOfSamples);
    Arcadia_Object_lock(thread, (Arcadia_Object*)leftBuffer);
    leftBufferLocked = Arcadia_BooleanValue_True;
    rightBuffer = Arcadia_Media_DSP_Buffer_create(thread, sampleRate, numberOfSamples);
    Arcadia_Object_lock(thread, (Arcadia_Object*)rightBuffer);
    rightBufferLocked = Arcadia_BooleanValue_True;
    Arcadia_Media_DSP_render(thread, self->left, leftBuffer);
    Arcadia_Media_DSP_render(thread, self->right, rightBuffer);
    Arcadia_Real32Value const* leftSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, leftBuffer);
    Arcadia_Real32Value const* rightSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, rightBuffer);
    Arcadia_Real32Value* outputSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, target);
    for (Arcadia_SizeValue i = 0, n = numberOfSamples; i < n; ++i) {
      outputSamples[i] = leftSamples[i] * rightSamples[i];
    }
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (rightBufferLocked) Arcadia_Object_unlock(thread, (Arcadia_Object*)rightBuffer);
    if (leftBufferLocked) Arcadia_Object_unlock(thread, (Arcadia_Object*)leftBuffer);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)rightBuffer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)leftBuffer);
}

static void
Arcadia_Media_DSP_Multiply_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_MultiplyDispatch* self
  )
{
  ((Arcadia_Media_DSPDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Media_DSP_Buffer*)) & Arcadia_Media_DSP_Multiply_render;
  ((Arcadia_Media_DSPDispatch*)self)->reset = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & Arcadia_Media_DSP_Multiply_reset;
  ((Arcadia_Media_DSPDispatch*)self)->reseed = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Natural32Value)) & Arcadia_Media_DSP_Multiply_reseed;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfOutputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfOutputPins;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfInputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfInputPins;
}

static void
Arcadia_Media_DSP_Multiply_reset(Arcadia_Thread* thread, Arcadia_Media_DSP_Multiply* self)
{/*Intentionally empty.*/}

static void
Arcadia_Media_DSP_Multiply_reseed(Arcadia_Thread* thread, Arcadia_Media_DSP_Multiply* self, Arcadia_Natural32Value seed)
{/*Intentionally empty.*/}

Arcadia_Media_DSP_Multiply*
Arcadia_Media_DSP_Multiply_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* left,
    Arcadia_Media_DSP* right
  )
{
  if (!left || !right) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  _Arcadia_BeginCreate(Arcadia_Media_DSP_Multiply);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)left);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)right);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Media_DSP_Multiply);
}
