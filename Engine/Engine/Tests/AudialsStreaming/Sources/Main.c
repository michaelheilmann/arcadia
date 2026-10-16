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

#include <math.h>
#include <stdlib.h>

#include "Arcadia/Engine/Include.h"

static void
assertReal32Near
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value received,
    Arcadia_Real32Value expected
  )
{
  Arcadia_Tests_assertTrue(thread, fabsf(received - expected) < 0.0001f);
}

static Arcadia_Media_DSP_Buffer*
createLockedBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value sampleRate,
    Arcadia_SizeValue numberOfFrames
  )
{
  Arcadia_Media_DSP_Buffer* buffer = Arcadia_Media_DSP_Buffer_create(thread, sampleRate, numberOfFrames);
  Arcadia_Object_lock(thread, (Arcadia_Object*)buffer);
  return buffer;
}

static void
testPartialFinalChunkAndContinuity
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SineWave* reference = Arcadia_Media_DSP_SineWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)reference);
  Arcadia_Media_DSP_SineWave* streamed = Arcadia_Media_DSP_SineWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)streamed);
  Arcadia_Engine_Audials_DSPStreamProvider* provider =
    Arcadia_Engine_Audials_DSPStreamProvider_create(thread, (Arcadia_Media_DSP*)streamed, 8, 10);
  Arcadia_Object_lock(thread, (Arcadia_Object*)provider);
  Arcadia_Media_DSP_Buffer* referenceBuffer = createLockedBuffer(thread, 8, 10);
  Arcadia_Media_DSP_Buffer* chunk = createLockedBuffer(thread, 8, 4);

  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)reference, referenceBuffer);
  Arcadia_Real32Value* referenceSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, referenceBuffer);
  Arcadia_SizeValue expectedChunkSizes[] = { 4, 4, 2 };
  Arcadia_SizeValue offset = 0;
  for (Arcadia_SizeValue chunkIndex = 0; chunkIndex < 3; ++chunkIndex) {
    Arcadia_SizeValue framesWritten = 0;
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
    Arcadia_Tests_assertTrue(thread, framesWritten == expectedChunkSizes[chunkIndex]);
    Arcadia_Real32Value* chunkSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, chunk);
    for (Arcadia_SizeValue i = 0; i < framesWritten; ++i) {
      assertReal32Near(thread, chunkSamples[i], referenceSamples[offset + i]);
    }
    offset += framesWritten;
  }
  Arcadia_Tests_assertTrue(thread, offset == 10);

  Arcadia_SizeValue framesWritten = 100;
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
  Arcadia_Tests_assertTrue(thread, framesWritten == 0);
  framesWritten = 100;
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
  Arcadia_Tests_assertTrue(thread, framesWritten == 0);

  Arcadia_Object_unlock(thread, (Arcadia_Object*)chunk);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)referenceBuffer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)provider);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)streamed);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)reference);
}

static void
testExactChunkBoundary
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SineWave* sine = Arcadia_Media_DSP_SineWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)sine);
  Arcadia_Engine_Audials_DSPStreamProvider* provider =
    Arcadia_Engine_Audials_DSPStreamProvider_create(thread, (Arcadia_Media_DSP*)sine, 8, 8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)provider);
  Arcadia_Media_DSP_Buffer* chunk = createLockedBuffer(thread, 8, 4);

  for (Arcadia_SizeValue i = 0; i < 2; ++i) {
    Arcadia_SizeValue framesWritten = 0;
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
    Arcadia_Tests_assertTrue(thread, framesWritten == 4);
  }
  Arcadia_SizeValue framesWritten = 1;
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
  Arcadia_Tests_assertTrue(thread, framesWritten == 0);

  Arcadia_Object_unlock(thread, (Arcadia_Object*)chunk);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)provider);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)sine);
}

static void
testInfiniteProvider
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SineWave* sine = Arcadia_Media_DSP_SineWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)sine);
  Arcadia_Engine_Audials_DSPStreamProvider* provider =
    Arcadia_Engine_Audials_DSPStreamProvider_createInfinite(thread, (Arcadia_Media_DSP*)sine, 8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)provider);
  Arcadia_Media_DSP_Buffer* chunk = createLockedBuffer(thread, 8, 4);

  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    Arcadia_SizeValue framesWritten = 0;
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
    Arcadia_Tests_assertTrue(thread, framesWritten == 4);
  }

  Arcadia_Object_unlock(thread, (Arcadia_Object*)chunk);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)provider);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)sine);
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  testPartialFinalChunkAndContinuity(thread);
  testExactChunkBoundary(thread);
  testInfiniteProvider(thread);
}

int
main
  (
    int argc,
    char** argv
  )
{
  (void)argc;
  (void)argv;
  return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
