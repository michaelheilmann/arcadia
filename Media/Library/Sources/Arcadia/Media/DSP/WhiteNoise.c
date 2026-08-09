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
#include "Arcadia/Media/DSP/WhiteNoise.h"

#include "Arcadia/Media/Quantization.h"

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
Arcadia_Media_DSP_WhiteNoise_generate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self,
    Arcadia_Natural32Value sampleRate,
    Arcadia_Natural32Value numberOfSamples,
    Arcadia_ByteArrayBuilder* target
  );

/// @return The random number [-1,+1].
/// @todo We must replace rand().
static Arcadia_Real32Value
Arcadia_getRandomReal32Value
  (
    Arcadia_Thread* thread
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
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Media_DSP_WhiteNoise);
}

static void
Arcadia_Media_DSP_WhiteNoise_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoiseDispatch* self
  )
{
  ((Arcadia_Media_DSPDispatch*)self)->generate = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Natural32Value, Arcadia_Natural32Value, Arcadia_ByteArrayBuilder*)) & Arcadia_Media_DSP_WhiteNoise_generate;
}

static void
Arcadia_Media_DSP_WhiteNoise_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Media_DSP_WhiteNoise_generate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_WhiteNoise* self,
    Arcadia_Natural32Value sampleRate,
    Arcadia_Natural32Value numberOfSamples,
    Arcadia_ByteArrayBuilder* target
  )
{ 
  for (Arcadia_SizeValue i = 0, n = numberOfSamples; i < n; ++i) {
    Arcadia_Real32Value v = Arcadia_getRandomReal32Value(thread);
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, target, &v, sizeof(v));
  }
}

static Arcadia_Real32Value
Arcadia_getRandomReal32Value
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Integer32Value i = rand(); // [0,RAND_MAX]
  Arcadia_Real32Value f = ((Arcadia_Real32Value)i) / ((Arcadia_Real32Value)RAND_MAX); // [0,1]
  f *= 2.f; // [0,2]
  f -= 1.f; // [-1,+1]
  return f;
}

Arcadia_Media_DSP_WhiteNoise*
Arcadia_Media_DSP_WhiteNoise_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_DSP_WhiteNoise);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Media_DSP_WhiteNoise);
}
