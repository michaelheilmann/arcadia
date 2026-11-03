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

#include "Arcadia/Testing/Include.h"

#include <stdlib.h>
#include <string.h>

#include "Arcadia/Media/Include.h"
#include "Formats.h"

// If defined and 1, then omit slow tests.
#define Arcadia_Media_Tests_SampleBuffer_OmitSlowTests (1)

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_List* sampleRates = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, sampleRates);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 8000);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 11025);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 16000);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 22050);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 32000);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 37800);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 44100);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 47250);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 48000);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 50000);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 50400);

  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 64000);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 88200);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 96000);
#if !(defined(Arcadia_Media_Tests_SampleBuffer_OmitSlowTests) && 1 == Arcadia_Media_Tests_SampleBuffer_OmitSlowTests)
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 176400);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 192000);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 352800);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 384000);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 2822400);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 5644800);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 11289600);

  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 11289600);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 22579200);
  Arcadia_List_insertBackInteger32Value(thread, sampleRates, 45158400);
#endif

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)sampleRates); i < n; ++i) {
    Arcadia_Integer32Value sampleRate = Arcadia_List_getInteger32ValueAt(thread, sampleRates, i);
    Arcadia_Media_SampleBuffer_create(thread, 8, sampleRate, Arcadia_Media_SampleFormat_Integer16);
    Arcadia_Media_SampleBuffer_create(thread, 8, sampleRate, Arcadia_Media_SampleFormat_Integer8);
    Arcadia_Media_SampleBuffer_create(thread, 8, sampleRate, Arcadia_Media_SampleFormat_Natural16);
    Arcadia_Media_SampleBuffer_create(thread, 8, sampleRate, Arcadia_Media_SampleFormat_Natural8);
  }
  Arcadia_ValueStack_popValues(thread, 1);
}

static int
runConstruction
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&test1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// Exercise signed 8-bit DSP output, including its final sample, after GC.
static void
testInteger8DspFill(Arcadia_Thread* thread)
{
  Arcadia_Media_SampleBuffer* buffer =
    Arcadia_Media_SampleBuffer_create(thread, 1, 8, Arcadia_Media_SampleFormat_Integer8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)buffer);
  Arcadia_Media_DSP_SineWave* sine = Arcadia_Media_DSP_SineWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)sine);
  Arcadia_Media_SampleBuffer_fill(thread, buffer, (Arcadia_Media_DSP*)sine);
  Arcadia_Integer8Value* pcm = (Arcadia_Integer8Value*)buffer->bytes;
  Arcadia_Tests_assertTrue(thread, pcm[0] == 0);
  Arcadia_Tests_assertTrue(thread, pcm[2] >= 126);
  Arcadia_Tests_assertTrue(thread, pcm[6] <= -126);
  Arcadia_Tests_assertTrue(thread, pcm[7] < 0);
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Tests_assertTrue(thread, pcm[2] >= 126 && pcm[6] <= -126);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)sine);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
}

static int
runInteger8DspFill(void)
{ return Arcadia_Tests_safeExecute(&testInteger8DspFill) ? EXIT_SUCCESS : EXIT_FAILURE; }

static void
expectInvalidDimensions(Arcadia_Thread* thread, Arcadia_Integer32Value length,
                        Arcadia_Integer32Value sampleRate)
{
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Media_SampleBuffer_create(thread, length, sampleRate, Arcadia_Media_SampleFormat_Integer8);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_setStatus(thread, 0);
  }
}

static void
testDimensions(Arcadia_Thread* thread)
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  expectInvalidDimensions(thread, -1, 48000);
  expectInvalidDimensions(thread, 1, 0);
  expectInvalidDimensions(thread, 1, -1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static int
runDimensions(void)
{ return Arcadia_Tests_safeExecute(&testDimensions) ? EXIT_SUCCESS : EXIT_FAILURE; }

// Verify PCM silence values and independent clone storage across all four formats.
static void
testPcmSilenceAndClone(Arcadia_Thread* thread)
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  const Arcadia_Media_SampleFormat formats[] = {
    Arcadia_Media_SampleFormat_Integer16,
    Arcadia_Media_SampleFormat_Integer8,
    Arcadia_Media_SampleFormat_Natural16,
    Arcadia_Media_SampleFormat_Natural8,
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
    Arcadia_Media_SampleBuffer* buffer = Arcadia_Media_SampleBuffer_create(thread, 1, 8, formats[i]);
    Arcadia_Object_lock(thread, (Arcadia_Object*)buffer);
    Arcadia_SizeValue bytesPerSample = Arcadia_Media_SampleFormat_getNumberOfBytes(thread, formats[i]);
    Arcadia_SizeValue byteCount = 8 * bytesPerSample;
    for (Arcadia_SizeValue j = 0; j < 8; ++j) {
      switch (formats[i]) {
        case Arcadia_Media_SampleFormat_Integer16:
          Arcadia_Tests_assertTrue(thread, ((Arcadia_Integer16Value*)buffer->bytes)[j] == 0);
          break;
        case Arcadia_Media_SampleFormat_Integer8:
          Arcadia_Tests_assertTrue(thread, ((Arcadia_Integer8Value*)buffer->bytes)[j] == 0);
          break;
        case Arcadia_Media_SampleFormat_Natural16:
          Arcadia_Tests_assertTrue(thread, ((Arcadia_Natural16Value*)buffer->bytes)[j] == 32768);
          break;
        case Arcadia_Media_SampleFormat_Natural8:
          Arcadia_Tests_assertTrue(thread, ((Arcadia_Natural8Value*)buffer->bytes)[j] == 128);
          break;
        default:
          Arcadia_Tests_assertTrue(thread, false);
          break;
      }
    }
    Arcadia_Media_SampleBuffer* clone = Arcadia_Media_SampleBuffer_createClone(thread, buffer);
    Arcadia_Object_lock(thread, (Arcadia_Object*)clone);
    Arcadia_Tests_assertTrue(thread, clone->bytes != buffer->bytes);
    Arcadia_Tests_assertTrue(thread, !memcmp(clone->bytes, buffer->bytes, byteCount));
    Arcadia_Natural8Value firstByte = clone->bytes[0];
    buffer->bytes[0] ^= 0x5a;
    Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
    Arcadia_Tests_assertTrue(thread, clone->bytes[0] == firstByte);
    Arcadia_Tests_assertTrue(thread, clone->bytes[0] != buffer->bytes[0]);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)clone);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static int
runPcmSilenceAndClone(void)
{ return Arcadia_Tests_safeExecute(&testPcmSilenceAndClone) ? EXIT_SUCCESS : EXIT_FAILURE; }

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Construction", &runConstruction },
    { "Formats", &Arcadia_Media_Tests_SampleBuffer_formats },
    { "Integer8DspFill", &runInteger8DspFill },
    { "Dimensions", &runDimensions },
    { "PcmSilenceAndClone", &runPcmSilenceAndClone },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
