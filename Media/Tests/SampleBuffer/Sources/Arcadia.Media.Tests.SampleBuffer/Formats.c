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

#include "Formats.h"
#include "Arcadia/Media/Include.h"
#include <string.h>
#include <stdio.h>

_Static_assert(Arcadia_Media_SampleFormat_Integer16 == 0, "Integer16 value");
_Static_assert(Arcadia_Media_SampleFormat_Integer8 == 1, "Integer8 value");
_Static_assert(Arcadia_Media_SampleFormat_Natural16 == 2, "Natural16 value");
_Static_assert(Arcadia_Media_SampleFormat_Natural8 == 3, "Natural8 value");
_Static_assert(Arcadia_Media_SampleFormat_Real32 == 4, "Real32 remains declared");
_Static_assert(sizeof(Arcadia_Media_SampleFormat) == sizeof(Arcadia_Integer32Value), "enum size");
_Static_assert(_Alignof(Arcadia_Media_SampleFormat) == _Alignof(Arcadia_Integer32Value), "enum alignment");
_Static_assert(_Generic(&Arcadia_Media_SampleFormat_getNumberOfBytes, Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_SampleFormat): 1, default: 0), "byte query signature");
_Static_assert(_Generic(&_Arcadia_Media_SampleFormat_getType, Arcadia_TypeValue (*)(Arcadia_Thread*): 1, default: 0), "descriptor signature");

static void
testMetadata
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_TypeValue type = _Arcadia_Media_SampleFormat_getType(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_getKind(thread, type) == Arcadia_TypeKind_Enumeration);
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  const char expected[] = "Arcadia.Media.SampleFormat";
  Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, name) == sizeof(expected) - 1 && !memcmp(Arcadia_Name_getBytes(thread, name), expected, sizeof(expected) - 1));
  Arcadia_Tests_assertTrue(thread, Arcadia_getType(thread, name) == type);
  for (Arcadia_Integer32Value i = 0; i < 5; ++i) {
    Arcadia_Value value = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(type, i));
    Arcadia_ValueStack_pushValue(thread, &value);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, _Arcadia_Media_SampleFormat_getType(thread) == type);
  for (Arcadia_Integer32Value i = 0; i < 5; ++i) {
    Arcadia_Value value = Arcadia_ValueStack_getValue(thread, 4 - i);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEnumerationValue(&value));
    Arcadia_EnumerationValue enumeration = Arcadia_Value_getEnumerationValue(&value);
    Arcadia_Tests_assertTrue(thread, enumeration.type == type && enumeration.value == i);
  }
  Arcadia_ValueStack_popValues(thread, 5);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
expectUnsupported
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value format
  )
{
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Media_SampleFormat_getNumberOfBytes(thread, (Arcadia_Media_SampleFormat)format);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
  }
}

static void
testQuery
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  const Arcadia_SizeValue expected[] = { 2, 1, 2, 1 };
  for (Arcadia_Integer32Value i = -8; i <= 12; ++i) {
    if (i >= 0 && i < 4) {
      Arcadia_Tests_assertTrue(thread, Arcadia_Media_SampleFormat_getNumberOfBytes(thread, (Arcadia_Media_SampleFormat)i) == expected[i]);
    } else expectUnsupported(thread, i);
  }
  expectUnsupported(thread, INT32_MIN);
  expectUnsupported(thread, INT32_MAX);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

static void
testConsumers
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_Integer32Value i = 0; i < 4; ++i) {
    Arcadia_Media_SampleFormat format = (Arcadia_Media_SampleFormat)i;
    Arcadia_Media_SampleBuffer* buffer = Arcadia_Media_SampleBuffer_create(thread, 1, 3, format);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, buffer);
    Arcadia_SizeValue bytes = 3 * Arcadia_Media_SampleFormat_getNumberOfBytes(thread, format);
    for (Arcadia_SizeValue j = 0; j < bytes; ++j) buffer->bytes[j] = (Arcadia_Natural8Value)(17 + j);
    Arcadia_Media_SampleBuffer* clone = Arcadia_Media_SampleBuffer_createClone(thread, buffer);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, clone);
    Arcadia_Tests_assertTrue(thread, clone != buffer && clone->bytes != buffer->bytes);
    Arcadia_Tests_assertTrue(thread, clone->sampleFormat == format && clone->length == 1 && clone->sampleRate == 3);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, !memcmp(buffer->bytes, clone->bytes, bytes));
    Arcadia_ValueStack_popValues(thread, 2);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Media_Tests_SampleBuffer_formats
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testMetadata)) { fprintf(stderr, "sample format metadata failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testQuery)) { fprintf(stderr, "sample format query failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testConsumers)) { fprintf(stderr, "sample format consumers failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
