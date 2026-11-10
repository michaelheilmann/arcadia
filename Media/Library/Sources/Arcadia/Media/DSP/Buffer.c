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
#include "Arcadia/Media/DSP/Buffer.h"
#include <stdint.h>

static void
Arcadia_Media_DSP_Buffer_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Buffer* self
  );

static void
Arcadia_Media_DSP_Buffer_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Buffer* self
  );

static void
Arcadia_Media_DSP_Buffer_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_BufferDispatch* self
  );

static void
Arcadia_Media_DSP_Buffer_visitImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_Buffer* self);

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_DSP_Buffer_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Media_DSP_Buffer_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_DSP_Buffer_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_DSP_Buffer_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.DSP.Buffer", Arcadia_Media_DSP_Buffer,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Media_DSP_Buffer_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Buffer* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_DSP_Buffer);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->samples = NULL;
  self->sampleBytes = NULL;
  self->sampleRate = Arcadia_ValueStack_getNatural32Value(thread, 2);
  self->numberOfSamples = Arcadia_ValueStack_getSizeValue(thread, 1);
  if (!self->sampleRate) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (self->numberOfSamples > SIZE_MAX / sizeof(Arcadia_Real32Value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  // Factory memory is zero-filled and owned by the managed ByteArray.
  self->sampleBytes = Arcadia_ByteArrayFactory_createMutable(thread,
    self->numberOfSamples * sizeof(Arcadia_Real32Value));
  self->samples = (Arcadia_Real32Value*)Arcadia_ByteArray_getMutableBytes(thread, self->sampleBytes);
  Arcadia_LeaveConstructor(Arcadia_Media_DSP_Buffer);
}

static void
Arcadia_Media_DSP_Buffer_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Buffer* self
  )
{
  // Storage is GC-managed; no native free here.
  self->samples = NULL;
  self->sampleBytes = NULL;
}

static void
Arcadia_Media_DSP_Buffer_visitImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_Buffer* self)
{
  if (self->sampleBytes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->sampleBytes);
  }
}

static void
Arcadia_Media_DSP_Buffer_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_BufferDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_Media_DSP_Buffer*
Arcadia_Media_DSP_Buffer_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value sampleRate,
    Arcadia_SizeValue numberOfSamples
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_DSP_Buffer);
  Arcadia_ValueStack_pushNatural32Value(thread, sampleRate);
  Arcadia_ValueStack_pushSizeValue(thread, numberOfSamples);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Media_DSP_Buffer);
}

Arcadia_Real32Value*
Arcadia_Media_DSP_Buffer_getSamples
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Buffer* self
  )
{ return self->samples; }

Arcadia_Natural32Value
Arcadia_Media_DSP_Buffer_getSampleRate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Buffer* self
  )
{ return self->sampleRate; }

Arcadia_SizeValue
Arcadia_Media_DSP_Buffer_getNumberOfSamples
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Buffer* self
  )
{ return self->numberOfSamples; }

Arcadia_ByteArray*
Arcadia_Media_DSP_Buffer_getSampleBytes(Arcadia_Thread* thread, Arcadia_Media_DSP_Buffer* self)
{ return self->sampleBytes; }
