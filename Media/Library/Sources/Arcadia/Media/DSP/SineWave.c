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
#include "Arcadia/Media/DSP/SineWave.h"

#include "Arcadia/Media/Quantization.h"

static Arcadia_SizeValue
getNumberOfOutputPins 
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  );

static Arcadia_SizeValue
getNumberOfInputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  );

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
Arcadia_Media_DSP_SineWave_render
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self,
    Arcadia_Media_DSP_Buffer* target
  );

static void
Arcadia_Media_DSP_SineWave_reset
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  );

static void
Arcadia_Media_DSP_SineWave_reseed
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self,
    Arcadia_Natural32Value seed
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

static Arcadia_SizeValue
getNumberOfOutputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  )
{ return Arcadia_SizeValue_Literal(1); }

static Arcadia_SizeValue
getNumberOfInputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  )
{ return Arcadia_SizeValue_Literal(0); }

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
  self->frequency = Arcadia_ValueStack_getReal32Value(thread, 1);
  if (self->frequency <= 0.0f) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->phase = 0.0f;
  Arcadia_LeaveConstructor(Arcadia_Media_DSP_SineWave);
}

static void
Arcadia_Media_DSP_SineWave_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWaveDispatch* self
  )
{
  ((Arcadia_Media_DSPDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Media_DSP_Buffer*)) & Arcadia_Media_DSP_SineWave_render;
  ((Arcadia_Media_DSPDispatch*)self)->reset = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & Arcadia_Media_DSP_SineWave_reset;
  ((Arcadia_Media_DSPDispatch*)self)->reseed = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Natural32Value)) & Arcadia_Media_DSP_SineWave_reseed;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfOutputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfOutputPins;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfInputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfInputPins;
}

static void
Arcadia_Media_DSP_SineWave_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Media_DSP_SineWave_render
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self,
    Arcadia_Media_DSP_Buffer* target
  )
{
  static const Arcadia_Real32Value PI = 3.14159265358979323846;
  Arcadia_Natural32Value sampleRate = Arcadia_Media_DSP_Buffer_getSampleRate(thread, target);
  Arcadia_SizeValue numberOfSamples = Arcadia_Media_DSP_Buffer_getNumberOfSamples(thread, target);
  Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, target);
  Arcadia_Real32Value phase = self->phase;
  Arcadia_Real32Value phaseIncrement = self->frequency / (Arcadia_Real32Value)sampleRate;
  for (Arcadia_SizeValue i = 0, n = numberOfSamples; i < n; ++i) {
    samples[i] = sinf(2.0f * PI * phase);
    phase += phaseIncrement;
    if (phase >= 1.0f) {
      phase = fmodf(phase, 1.0f);
    }
  }
  self->phase = phase;
}

static void
Arcadia_Media_DSP_SineWave_reset
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self
  )
{ self->phase = 0.0f; }

static void
Arcadia_Media_DSP_SineWave_reseed
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_SineWave* self,
    Arcadia_Natural32Value seed
  )
{/*Intentionally empty.*/}

Arcadia_Media_DSP_SineWave*
Arcadia_Media_DSP_SineWave_create
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value frequency
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_DSP_SineWave);
  Arcadia_ValueStack_pushReal32Value(thread, frequency);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Media_DSP_SineWave);
}
