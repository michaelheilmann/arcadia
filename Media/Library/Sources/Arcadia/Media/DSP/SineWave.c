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

#define ARCADIA_MEDIA_MODULE (1)
#include "Arcadia/Media/DSP/SineWave.h"

#include "Arcadia/Media/Quantization.h"

#if defined(Arcadia_Media_Configuration_DSP_withOutputPins) && 1 == Arcadia_Media_Configuration_DSP_withOutputPins

static Arcadia_SizeValue
getNumberOfOutputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  );

#endif

#if defined(Arcadia_Media_Configuration_DSP_withInputPins) && 1 == Arcadia_Media_Configuration_DSP_withInputPins

static Arcadia_SizeValue
getNumberOfOutputPins 
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  );

#endif

static void
Arcadia_Media_DSP_SineWave_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  );

static void
Arcadia_Media_DSP_SineWave_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWaveDispatch* self
  );

static void
Arcadia_Media_DSP_SineWave_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  );

static void
Arcadia_Media_DSP_SineWave_generate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self,
    Arcadia_Natural32Value sampleRate,
    Arcadia_Natural32Value numberOfSamples,
    Arcadia_ByteArrayBuilder* target
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_DSP_SineWave_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_DSP_SineWave_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_DSP_SineWave_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.DSP.SineWave", Arcadia_Media_DSP_SineWave,
                         u8"Arcadia.Media.DSP", Arcadia_Media_DSP,
                         &_typeOperations);

#if defined(Arcadia_Media_Configuration_DSP_withOutputPins) && 1 == Arcadia_Media_Configuration_DSP_withOutputPins

static Arcadia_SizeValue
getNumberOfOutputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  )
{ return Arcadia_SizeValue_Literal(0); }

#endif

#if defined(Arcadia_Media_Configuration_DSP_withInputPins) && 1 == Arcadia_Media_Configuration_DSP_withInputPins

static Arcadia_SizeValue
getNumberOfOutputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  )
{ return Arcadia_SizeValue_Literal(1); }

#endif

static void
Arcadia_Media_DSP_SineWave_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_DSP_SineWave);
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
  Arcadia_LeaveConstructor(Arcadia_Media_DSP_SineWave);
}

static void
Arcadia_Media_DSP_SineWave_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWaveDispatch* self
  )
{
  ((Arcadia_Media_DSPDispatch*)self)->generate = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Natural32Value, Arcadia_Natural32Value, Arcadia_ByteArrayBuilder*)) & Arcadia_Media_DSP_SineWave_generate;
}

static void
Arcadia_Media_DSP_SineWave_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Media_DSP_SineWave_generate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self,
    Arcadia_Natural32Value sampleRate,
    Arcadia_Natural32Value numberOfSamples,
    Arcadia_ByteArrayBuilder* target
  )
{
  static const Arcadia_Real32Value PI = 3.14159265358979323846;
  const Arcadia_Real32Value c = 2.f * PI * (Arcadia_Real32Value)self->frequency / sampleRate;
  for (Arcadia_SizeValue i = 0, n = numberOfSamples; i < n; ++i) {
    Arcadia_Real32Value v = sinf(c * (Arcadia_Real32Value)i);
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, target, &v, sizeof(v));
  }
}

Arcadia_Media_DSP_SineWave*
Arcadia_Media_DSP_SineWave_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value frequency
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_DSP_SineWave);
  Arcadia_ValueStack_pushInteger32Value(thread, frequency);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Media_DSP_SineWave);
}
