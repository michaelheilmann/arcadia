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
#include "Arcadia/Media/DSP/SawtoothWave.h"

#include "Arcadia/Media/Quantization.h"

static void
Arcadia_Media_DSP_SawtoothWave_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SawtoothWave* self
  );

static void
Arcadia_Media_DSP_SawtoothWave_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SawtoothWaveDispatch* self
  );

static void
Arcadia_Media_DSP_SawtoothWave_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SawtoothWave* self
  );

static void
Arcadia_Media_DSP_SawtoothWave_generate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SawtoothWave* self,
    Arcadia_Natural32Value sampleRate,
    Arcadia_Natural32Value numberOfSamples,
    Arcadia_ByteBuffer* target
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_DSP_SawtoothWave_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_DSP_SawtoothWave_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_DSP_SawtoothWave_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.DSP.SawtoothWave", Arcadia_Media_DSP_SawtoothWave,
                         u8"Arcadia.Media.DSP", &_typeOperations);

static void
Arcadia_Media_DSP_SawtoothWave_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SawtoothWave* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_DSP_SawtoothWave);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->frequency = Arcadia_ValueStack_getInteger32Value(thread, 1);
  if (self->frequency < 1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Media_DSP_SawtoothWave);
}

static void
Arcadia_Media_DSP_SawtoothWave_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SawtoothWaveDispatch* self
  )
{
  ((Arcadia_Media_DSPDispatch*)self)->generate = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Natural32Value, Arcadia_Natural32Value, Arcadia_ByteBuffer*)) & Arcadia_Media_DSP_SawtoothWave_generate;
}

static void
Arcadia_Media_DSP_SawtoothWave_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SawtoothWave* self
  ){/*Intentionally empty.*/}

static void
Arcadia_Media_DSP_SawtoothWave_generate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SawtoothWave* self,
    Arcadia_Natural32Value sampleRate,
    Arcadia_Natural32Value numberOfSamples,
    Arcadia_ByteBuffer* target
  )
{
  const Arcadia_Real32Value period = (Arcadia_Real32Value)sampleRate / self->frequency;
  for (Arcadia_SizeValue i = 0, n = numberOfSamples; i < n; ++i) {
    Arcadia_Real32Value v = fmod((Arcadia_Real32Value)i, period) / period - 0.5f;
    Arcadia_ByteBuffer_insertBackBytes(thread, target, &v, sizeof(v));
  }
}

Arcadia_Media_DSP_SawtoothWave*
Arcadia_Media_DSP_SawtoothWave_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value frequency
  )
{
  Arcadia_SizeValue oldValueStackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushInteger32Value(thread, frequency);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  ARCADIA_CREATEOBJECT(Arcadia_Media_DSP_SawtoothWave);
}
