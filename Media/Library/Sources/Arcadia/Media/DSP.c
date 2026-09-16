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
#include "Arcadia/Media/DSP.h"

static void
Arcadia_Media_DSP_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self
  );

static void
Arcadia_Media_DSP_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSPDispatch* self
  );

static void
Arcadia_Media_DSP_resetImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self
  );

static void
Arcadia_Media_DSP_reseedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self,
    Arcadia_Natural32Value seed
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*) & Arcadia_Media_DSP_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_DSP_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.DSP", Arcadia_Media_DSP,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Media_DSP_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_DSP);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->ownerGraph = NULL;
  Arcadia_LeaveConstructor(Arcadia_Media_DSP);
}

static void
Arcadia_Media_DSP_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSPDispatch* self
  )
{
  self->reset = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & Arcadia_Media_DSP_resetImpl;
  self->reseed = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Natural32Value)) & Arcadia_Media_DSP_reseedImpl;
}

static void
Arcadia_Media_DSP_resetImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Media_DSP_reseedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self,
    Arcadia_Natural32Value seed
  )
{/*Intentionally empty.*/}

void
Arcadia_Media_DSP_render
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self,
    Arcadia_Media_DSP_Buffer* target
  )
{ Arcadia_VirtualCall(Arcadia_Media_DSP, render, self, target); }

void
Arcadia_Media_DSP_generate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self,
    Arcadia_Natural32Value sampleRate,
    Arcadia_Natural32Value numberOfSamples,
    Arcadia_ByteArrayBuilder* target
  )
{
  Arcadia_Media_DSP_Buffer* buffer = Arcadia_Media_DSP_Buffer_create(thread, sampleRate, numberOfSamples);
  Arcadia_Object_lock(thread, (Arcadia_Object*)buffer);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Media_DSP_render(thread, self, buffer);
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, target, Arcadia_Media_DSP_Buffer_getSamples(thread, buffer), numberOfSamples * sizeof(Arcadia_Real32Value));
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
    Arcadia_Thread_jump(thread);
  }
}

Arcadia_SizeValue
Arcadia_Media_DSP_getNumberOfOutputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Media_DSP, getNumberOfOutputPins, self); }

Arcadia_SizeValue
Arcadia_Media_DSP_getNumberOfInputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Media_DSP, getNumberOfInputPins, self); }

void
_Arcadia_Media_DSP_reset
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self
  )
{ Arcadia_VirtualCall(Arcadia_Media_DSP, reset, self); }

void
_Arcadia_Media_DSP_reseed
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self,
    Arcadia_Natural32Value seed
  )
{ Arcadia_VirtualCall(Arcadia_Media_DSP, reseed, self, seed); }
