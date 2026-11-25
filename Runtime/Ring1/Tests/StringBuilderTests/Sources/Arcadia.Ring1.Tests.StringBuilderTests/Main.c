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
#include "Arcadia/Ring1/Include.h"
#include <stdlib.h>

static Arcadia_Value
textValue
  (
    Arcadia_Thread* thread,
    char const* bytes,
    Arcadia_SizeValue size
  )
{
  return Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, (Arcadia_Natural8Value const*)bytes, size));
}

static void
check
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* builder,
    char const* bytes,
    Arcadia_SizeValue size,
    Arcadia_SizeValue count
  )
{
  Arcadia_Tests_assertEqualBytes(thread, bytes, size, Arcadia_StringBuilder_getBytes(thread, builder),
                                Arcadia_StringBuilder_getNumberOfBytes(thread, builder));
  Arcadia_Tests_assertEqualSize(thread, count, Arcadia_StringBuilder_getNumberOfCodePoints(thread, builder));
}

static void
testInsertion
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builder);
  Arcadia_StringBuilder_insertBack(thread, builder, textValue(thread, "ac", 2));
  Arcadia_StringBuilder_insertAt(thread, builder, 1, textValue(thread, "\xC3\xA9", 2));
  Arcadia_StringBuilder_insertFront(thread, builder, textValue(thread, "^", 1));
  Arcadia_StringBuilder_insertBack(thread, builder, textValue(thread, "$", 1));
  check(thread, builder, "^a\xC3\xA9" "c$", 6, 5);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builder);
}

static void
testSelfInsertion
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builder);
  Arcadia_Value self = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)builder);
  Arcadia_StringBuilder_insertBack(thread, builder, textValue(thread, "abc", 3));
  Arcadia_StringBuilder_insertAt(thread, builder, 1, self);
  check(thread, builder, "aabcbc", 6, 6);
  Arcadia_StringBuilder_clear(thread, builder);
  Arcadia_StringBuilder_insertBack(thread, builder, textValue(thread, "abc", 3));
  Arcadia_StringBuilder_insertFront(thread, builder, self);
  check(thread, builder, "abcabc", 6, 6);
  Arcadia_StringBuilder_insertBack(thread, builder, self);
  check(thread, builder, "abcabcabcabc", 12, 12);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builder);
}

static void
testRemovalAndComparison
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builder);
  Arcadia_StringBuilder_insertBack(thread, builder, textValue(thread, "a\xC3\xA9\xE2\x82\xAC\xF0\x9F\x98\x80" "z", 11));
  Arcadia_StringBuilder_removeCodePointsAt(thread, builder, 1, 2);
  check(thread, builder, "a\xF0\x9F\x98\x80" "z", 6, 3);
  Arcadia_StringBuilder_removeCodePointsAt(thread, builder, 3, 0);
  check(thread, builder, "a\xF0\x9F\x98\x80" "z", 6, 3);
  Arcadia_StringBuilder_removeCodePointsBack(thread, builder, 2);
  check(thread, builder, "a", 1, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_StringBuilder_compareTo(thread, builder, textValue(thread, "ab", 2)) < 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_StringBuilder_compareTo(thread, builder, textValue(thread, "", 0)) > 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_StringBuilder_compareTo(thread, builder, textValue(thread, "a", 1)) == 0);
  Arcadia_StringBuilder_removeCodePointsAt(thread, builder, 0, 1);
  check(thread, builder, "", 0, 0);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builder);
}

static void
testInvalidInputs
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builder);
  Arcadia_StringBuilder_insertBack(thread, builder, textValue(thread, "abc", 3));
  static const Arcadia_Natural8Value invalid[][4] = {
    { 0x80 }, { 0xc2 }, { 0xc0, 0x80 }, { 0xed, 0xa0, 0x80 }, { 0xf4, 0x90, 0x80, 0x80 },
  };
  static const Arcadia_SizeValue lengths[] = { 1, 1, 2, 3, 4 };
  for (Arcadia_SizeValue i = 0; i < sizeof(lengths) / sizeof(lengths[0]); ++i) {
    for (Arcadia_SizeValue position = 0; position < 3; ++position) {
      Arcadia_Value value = Arcadia_Value_makeRuntimeByteArrayValue(
        Arcadia_RuntimeByteArray_create(thread, invalid[i], lengths[i]));
      Arcadia_JumpTarget jt;
      Arcadia_Thread_pushJumpTarget(thread, &jt);
      if (Arcadia_JumpTarget_save(&jt)) {
        if (position == 0) {
          Arcadia_StringBuilder_insertFront(thread, builder, value);
        } else if (position == 1) {
          Arcadia_StringBuilder_insertAt(thread, builder, 1, value);
        } else {
          Arcadia_StringBuilder_insertBack(thread, builder, value);
        }
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_EncodingInvalid);
        Arcadia_Thread_setStatus(thread, 0);
      }
      check(thread, builder, "abc", 3, 3);
    }
  }
  static const Arcadia_SizeValue ranges[][2] = { { 4, 0 }, { 3, 1 }, { 1, SIZE_MAX }, { 0, 4 } };
  for (Arcadia_SizeValue i = 0; i < sizeof(ranges) / sizeof(ranges[0]); ++i) {
    Arcadia_JumpTarget jt;
    Arcadia_Thread_pushJumpTarget(thread, &jt);
    if (Arcadia_JumpTarget_save(&jt)) {
      Arcadia_StringBuilder_removeCodePointsAt(thread, builder, ranges[i][0], ranges[i][1]);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_setStatus(thread, 0);
    }
    check(thread, builder, "abc", 3, 3);
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builder);
}

static void
testCodePointInsertion
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builder);
  static const Arcadia_Natural32Value points[] = { 0, 0xe9, 0x20ac, 0x1f600 };
  Arcadia_StringBuilder_insertBackCodePoint(thread, builder, 'z');
  Arcadia_StringBuilder_insertFrontCodePoints(thread, builder, points, 4);
  Arcadia_StringBuilder_insertFrontCodePoint(thread, builder, 'a');
  Arcadia_StringBuilder_insertBackCodePoints(thread, builder, points, 4);
  static const char expected[] = "a\0\xc3\xa9\xe2\x82\xac\xf0\x9f\x98\x80z\0\xc3\xa9\xe2\x82\xac\xf0\x9f\x98\x80";
  check(thread, builder, expected, sizeof(expected) - 1, 10);
  Arcadia_StringBuilder_insertFrontCodePoints(thread, builder, NULL, 0);
  Arcadia_StringBuilder_insertBackCodePoints(thread, builder, NULL, 0);
  static const Arcadia_Natural32Value invalid[][2] = { { 'x', 0x110000 }, { 'x', 0xd800 }, { 'x', 0xdfff } };
  for (Arcadia_SizeValue i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
    for (Arcadia_SizeValue front = 0; front < 2; ++front) {
      Arcadia_JumpTarget jt;
      Arcadia_Thread_pushJumpTarget(thread, &jt);
      if (Arcadia_JumpTarget_save(&jt)) {
        if (front) {
          Arcadia_StringBuilder_insertFrontCodePoints(thread, builder, invalid[i], 2);
        } else {
          Arcadia_StringBuilder_insertBackCodePoints(thread, builder, invalid[i], 2);
        }
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_EncodingInvalid);
        Arcadia_Thread_setStatus(thread, 0);
      }
      check(thread, builder, expected, sizeof(expected) - 1, 10);
    }
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builder);
}

static int
runInvalidInputs
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testInvalidInputs) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runCodePointInsertion
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testCodePointInsertion) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testReplacement
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builder);
  Arcadia_StringBuilder_insertBack(thread, builder, textValue(thread, "a\xc3\xa9z", 4));
  Arcadia_StringBuilder_replaceCodePointsAt(thread, builder, 1, 1, textValue(thread, "XYZ", 3));
  check(thread, builder, "aXYZz", 5, 5);
  Arcadia_StringBuilder_replaceCodePointsAt(thread, builder, 1, 3, textValue(thread, "\xc3\xa9", 2));
  check(thread, builder, "a\xc3\xa9z", 4, 3);
  Arcadia_StringBuilder_replaceCodePointsAt(thread, builder, 3, 0, textValue(thread, "!", 1));
  check(thread, builder, "a\xc3\xa9z!", 5, 4);
  Arcadia_StringBuilder_replaceCodePointsAt(thread, builder, 1, 1,
    Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)builder));
  check(thread, builder, "aa\xc3\xa9z!z!", 8, 7);
  Arcadia_StringBuilder_replaceCodePointsAt(thread, builder, 0, 7, textValue(thread, "", 0));
  check(thread, builder, "", 0, 0);
  Arcadia_StringBuilder_insertBack(thread, builder, textValue(thread, "abc", 3));
  static const Arcadia_Natural8Value invalid[] = { 0xc2 };
  Arcadia_Value bad = Arcadia_Value_makeRuntimeByteArrayValue(Arcadia_RuntimeByteArray_create(thread, invalid, sizeof(invalid)));
  Arcadia_JumpTarget jt;
  Arcadia_Thread_pushJumpTarget(thread, &jt);
  if (Arcadia_JumpTarget_save(&jt)) {
    Arcadia_StringBuilder_replaceCodePointsAt(thread, builder, 1, 1, bad);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_EncodingInvalid);
    Arcadia_Thread_setStatus(thread, 0);
  }
  check(thread, builder, "abc", 3, 3);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builder);
}

static int
runReplacement
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testReplacement) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testQueriesAndCase
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builder);
  check(thread, builder, "", 0, 0);
  Arcadia_StringBuilder_insertBack(thread, builder, textValue(thread, "a\xc3\xa9" "bC", 5));
  Arcadia_Tests_assertTrue(thread, Arcadia_StringBuilder_startsWith_pn(thread, builder, "a\xc3\xa9", 3));
  Arcadia_Tests_assertTrue(thread, Arcadia_StringBuilder_endsWith_pn(thread, builder, "bC", 2));
  Arcadia_Tests_assertFalse(thread, Arcadia_StringBuilder_startsWith_pn(thread, builder, "A", 1));
  Arcadia_Tests_assertFalse(thread, Arcadia_StringBuilder_endsWith_pn(thread, builder, "longer", 6));
  Arcadia_Tests_assertTrue(thread, Arcadia_StringBuilder_startsWith_pn(thread, builder, "", 0));
  Arcadia_Tests_assertTrue(thread, Arcadia_StringBuilder_endsWith_pn(thread, builder, "", 0));
  Arcadia_SizeValue index = 1, length = 2;
  Arcadia_StringBuilder_toUpperASCII(thread, builder, &index, &length);
  check(thread, builder, "a\xc3\xa9" "BC", 5, 4);
  Arcadia_StringBuilder_toLowerASCII(thread, builder, NULL, NULL);
  check(thread, builder, "a\xc3\xa9" "bc", 5, 4);
  Arcadia_StringBuilder_toUpperASCII(thread, builder, NULL, NULL);
  check(thread, builder, "A\xc3\xa9" "BC", 5, 4);
  Arcadia_StringBuilder_removeCodePointsFront(thread, builder, 2);
  check(thread, builder, "BC", 2, 2);
  Arcadia_StringBuilder_removeCodePointsBack(thread, builder, 0);
  check(thread, builder, "BC", 2, 2);
  Arcadia_StringBuilder_clear(thread, builder);
  check(thread, builder, "", 0, 0);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builder);
}

static void
testValueSourcesAndSnapshot
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builder);
  Arcadia_ByteArrayBuilder* bytes = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)bytes);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, bytes, "b", 1);
  Arcadia_StringBuilder_insertBack(thread, builder, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)bytes));
  static const Arcadia_Natural8Value a[] = { 'a' };
  Arcadia_StringBuilder_insertFront(thread, builder, Arcadia_Value_makeRuntimeByteArrayValue(Arcadia_RuntimeByteArray_create(thread, a, 1)));
  Arcadia_String* snapshot = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)builder));
  Arcadia_Object_lock(thread, (Arcadia_Object*)snapshot);
  Arcadia_StringBuilder_insertBack(thread, builder, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)snapshot));
  check(thread, builder, "abab", 4, 4);
  Arcadia_Tests_assertTrue(thread, Arcadia_StringBuilder_compareTo(thread, builder, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)snapshot)) > 0);
  Arcadia_StringBuilder_clear(thread, builder);
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Tests_assertEqualBytes(thread, "ab", 2, Arcadia_String_getBytes(thread, snapshot), Arcadia_String_getNumberOfBytes(thread, snapshot));
  Arcadia_StringBuilder_insertBack(thread, builder, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)snapshot));
  check(thread, builder, "ab", 2, 2);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)snapshot);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)bytes);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builder);
}

static int
runQueriesAndCase
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testQueriesAndCase) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runValueSourcesAndSnapshot
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testValueSourcesAndSnapshot) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runInsertion
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testInsertion) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runSelfInsertion
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testSelfInsertion) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRemovalAndComparison
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRemovalAndComparison) ? EXIT_SUCCESS : EXIT_FAILURE;
}


static void
testBoundaryMatrix
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)builder);
  Arcadia_StringBuilder_insertBack(thread, builder, textValue(thread, "a\xc3\xa9" "Z", 4));
  Arcadia_Tests_assertTrue(thread, Arcadia_StringBuilder_startsWith_pn(thread, builder, NULL, 0));
  Arcadia_Tests_assertTrue(thread, Arcadia_StringBuilder_endsWith_pn(thread, builder, NULL, 0));
  Arcadia_ByteArrayBuilder* bytes = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)bytes);
  static const Arcadia_Natural8Value malformed[] = { 0x61, 0xc2 };
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, bytes, malformed, sizeof(malformed));
  Arcadia_Value invalidBytes = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)bytes);
  Arcadia_Value invalidType = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  Arcadia_Value empty = textValue(thread, "", 0);
  Arcadia_ValueStack_pushValue(thread, &empty);
  static const Arcadia_Status statuses[] = {
    Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_ArgumentValueInvalid,
    Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_ArgumentValueInvalid,
    Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_ArgumentValueInvalid,
    Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_ArgumentValueInvalid,
    Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_ArgumentTypeInvalid,
    Arcadia_Status_ArgumentTypeInvalid, Arcadia_Status_ArgumentTypeInvalid,
    Arcadia_Status_ArgumentTypeInvalid, Arcadia_Status_ArgumentTypeInvalid,
    Arcadia_Status_EncodingInvalid, Arcadia_Status_EncodingInvalid,
    Arcadia_Status_EncodingInvalid, Arcadia_Status_EncodingInvalid,
    Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_ArgumentValueInvalid,
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(statuses) / sizeof(statuses[0]); ++i) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_SizeValue index = 2, length = SIZE_MAX;
    Arcadia_JumpTarget jt;
    Arcadia_Thread_pushJumpTarget(thread, &jt);
    if (Arcadia_JumpTarget_save(&jt)) {
      switch (i) {
        case 0: Arcadia_StringBuilder_insertAt(thread, builder, 4, empty); break;
        case 1: Arcadia_StringBuilder_removeCodePointsFront(thread, builder, 4); break;
        case 2: Arcadia_StringBuilder_removeCodePointsBack(thread, builder, 4); break;
        case 3: Arcadia_StringBuilder_replaceCodePointsAt(thread, builder, 3, 1, empty); break;
        case 4: Arcadia_StringBuilder_replaceCodePointsAt(thread, builder, 2, SIZE_MAX, empty); break;
        case 5: Arcadia_StringBuilder_toUpperASCII(thread, builder, &index, &length); break;
        case 6: Arcadia_StringBuilder_toLowerASCII(thread, builder, &index, &length); break;
        case 7: Arcadia_StringBuilder_insertBackCodePoints(thread, builder, NULL, 1); break;
        case 8: Arcadia_StringBuilder_insertFrontCodePoints(thread, builder, NULL, 1); break;
        case 9: Arcadia_StringBuilder_insertFront(thread, builder, invalidType); break;
        case 10: Arcadia_StringBuilder_insertAt(thread, builder, 1, invalidType); break;
        case 11: Arcadia_StringBuilder_insertBack(thread, builder, invalidType); break;
        case 12: Arcadia_StringBuilder_replaceCodePointsAt(thread, builder, 1, 1, invalidType); break;
        case 13: Arcadia_StringBuilder_compareTo(thread, builder, invalidType); break;
        case 14: Arcadia_StringBuilder_insertFront(thread, builder, invalidBytes); break;
        case 15: Arcadia_StringBuilder_insertAt(thread, builder, 1, invalidBytes); break;
        case 16: Arcadia_StringBuilder_insertBack(thread, builder, invalidBytes); break;
        case 17: Arcadia_StringBuilder_replaceCodePointsAt(thread, builder, 1, 1, invalidBytes); break;
        case 18: Arcadia_StringBuilder_startsWith_pn(thread, builder, NULL, 1); break;
        case 19: Arcadia_StringBuilder_endsWith_pn(thread, builder, NULL, 1); break;
      }
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, status == statuses[i]);
      Arcadia_Tests_assertEqualSize(thread, stackSize, Arcadia_ValueStack_getSize(thread));
    }
    check(thread, builder, "a\xc3\xa9" "Z", 4, 3);
  }
  Arcadia_ValueStack_popValues(thread, 3);
}

static void
testSelfReplacementMatrix
  (
    Arcadia_Thread* thread
  )
{
  static const char source[] = "a\xc3\xa9\xe2\x82\xac\xf0\x9f\x98\x80" "z";
  static const Arcadia_SizeValue offsets[] = { 0, 1, 3, 6, 10, 11 };
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)builder);
  Arcadia_Value self = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)builder);
  for (Arcadia_SizeValue first = 0; first <= 5; ++first) {
    for (Arcadia_SizeValue last = first; last <= 5; ++last) {
      Arcadia_StringBuilder_clear(thread, builder);
      Arcadia_StringBuilder_insertBack(thread, builder, textValue(thread, source, sizeof(source) - 1));
      char expected[2 * (sizeof(source) - 1)];
      Arcadia_SizeValue prefix = offsets[first], suffix = sizeof(source) - 1 - offsets[last];
      if (prefix) {
        Arcadia_Memory_copy(thread, expected, source, prefix);
      }
      Arcadia_Memory_copy(thread, expected + prefix, source, sizeof(source) - 1);
      if (suffix) {
        Arcadia_Memory_copy(thread, expected + prefix + sizeof(source) - 1, source + offsets[last], suffix);
      }
      Arcadia_StringBuilder_replaceCodePointsAt(thread, builder, first, last - first, self);
      check(thread, builder, expected, prefix + sizeof(source) - 1 + suffix, 10 - (last - first));
    }
  }
  Arcadia_ValueStack_popValues(thread, 1);
}

static int
runBoundaryMatrix
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testBoundaryMatrix) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runSelfReplacementMatrix
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testSelfReplacementMatrix) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Insertion", &runInsertion },
    { "SelfInsertion", &runSelfInsertion },
    { "RemovalAndComparison", &runRemovalAndComparison },
    { "InvalidInputs", &runInvalidInputs },
    { "CodePointInsertion", &runCodePointInsertion },
    { "Replacement", &runReplacement },
    { "QueriesAndCase", &runQueriesAndCase },
    { "ValueSourcesAndSnapshot", &runValueSourcesAndSnapshot },
    { "BoundaryMatrix", &runBoundaryMatrix },
    { "SelfReplacementMatrix", &runSelfReplacementMatrix },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
