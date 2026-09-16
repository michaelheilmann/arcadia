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
#include "Arcadia/Media/DSP/WhiteNoise.h"

static Arcadia_SizeValue
getNumberOfOutputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self
  );

static Arcadia_SizeValue
getNumberOfInputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self
  );

static void
Arcadia_Media_DSP_WhiteNoise_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self
  );

static void
Arcadia_Media_DSP_WhiteNoise_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoiseDispatch* self
  );

static void
Arcadia_Media_DSP_WhiteNoise_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self
  );

static void
Arcadia_Media_DSP_WhiteNoise_render
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self,
    Arcadia_Media_DSP_Buffer* target
  );

static void
Arcadia_Media_DSP_WhiteNoise_reset
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self
  );

static void
Arcadia_Media_DSP_WhiteNoise_reseed
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self,
    Arcadia_Natural32Value seed
  );

static Arcadia_Natural32Value
Arcadia_Media_DSP_WhiteNoise_next
  (
    Arcadia_Media_DSP_WhiteNoise* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_DSP_WhiteNoise_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_DSP_WhiteNoise_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_DSP_WhiteNoise_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.DSP.WhiteNoise", Arcadia_Media_DSP_WhiteNoise,
                         u8"Arcadia.Media.DSP", Arcadia_Media_DSP,
                         &_typeOperations);

static Arcadia_SizeValue
getNumberOfOutputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self
  )
{ return Arcadia_SizeValue_Literal(1); }

static Arcadia_SizeValue
getNumberOfInputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self
  )
{ return Arcadia_SizeValue_Literal(0); }

static void
Arcadia_Media_DSP_WhiteNoise_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_DSP_WhiteNoise);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->initialSeed = Arcadia_ValueStack_getNatural32Value(thread, 2);
  if (!self->initialSeed) {
    self->initialSeed = Arcadia_Natural32Value_Literal(0x6d2b79f5);
  }
  self->randomState = self->initialSeed;
  self->amplitude = Arcadia_ValueStack_getReal32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_Media_DSP_WhiteNoise);
}

static void
Arcadia_Media_DSP_WhiteNoise_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoiseDispatch* self
  )
{
  ((Arcadia_Media_DSPDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Media_DSP_Buffer*)) & Arcadia_Media_DSP_WhiteNoise_render;
  ((Arcadia_Media_DSPDispatch*)self)->reset = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & Arcadia_Media_DSP_WhiteNoise_reset;
  ((Arcadia_Media_DSPDispatch*)self)->reseed = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Natural32Value)) & Arcadia_Media_DSP_WhiteNoise_reseed;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfOutputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfOutputPins;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfInputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfInputPins;
}

static void
Arcadia_Media_DSP_WhiteNoise_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Media_DSP_WhiteNoise_render
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self,
    Arcadia_Media_DSP_Buffer* target
  )
{ 
  Arcadia_SizeValue numberOfSamples = Arcadia_Media_DSP_Buffer_getNumberOfSamples(thread, target);
  Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, target);
  for (Arcadia_SizeValue i = 0, n = numberOfSamples; i < n; ++i) {
    Arcadia_Real32Value normalized = ((Arcadia_Real32Value)(Arcadia_Media_DSP_WhiteNoise_next(self) & 0x00ffffffu) / 8388607.5f) - 1.0f;
    samples[i] = normalized * self->amplitude;
  }
}

static Arcadia_Natural32Value
Arcadia_Media_DSP_WhiteNoise_next
  (
    Arcadia_Media_DSP_WhiteNoise* self
  )
{
  Arcadia_Natural32Value value = self->randomState;
  value ^= value << 13;
  value ^= value >> 17;
  value ^= value << 5;
  self->randomState = value;
  return value;
}

static void
Arcadia_Media_DSP_WhiteNoise_reset
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self
  )
{ self->randomState = self->initialSeed; }

static void
Arcadia_Media_DSP_WhiteNoise_reseed
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self,
    Arcadia_Natural32Value seed
  )
{
  self->initialSeed = seed ? seed : Arcadia_Natural32Value_Literal(0x6d2b79f5);
  self->randomState = self->initialSeed;
}

Arcadia_Media_DSP_WhiteNoise*
Arcadia_Media_DSP_WhiteNoise_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value seed,
    Arcadia_Real32Value amplitude
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_DSP_WhiteNoise);
  Arcadia_ValueStack_pushNatural32Value(thread, seed);
  Arcadia_ValueStack_pushReal32Value(thread, amplitude);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Media_DSP_WhiteNoise);
}
