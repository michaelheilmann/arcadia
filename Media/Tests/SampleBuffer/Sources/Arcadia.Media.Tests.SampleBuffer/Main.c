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
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
