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

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Arcadia/Ring1/Include.h"

typedef struct Result {
  Arcadia_Natural32Value codePoint;
  Arcadia_SizeValue byteIndex;
  Arcadia_SizeValue codePointLength;
  bool error;
} Result;

static void
testFixtureUnchecked
  (
    Arcadia_Thread* thread,
    const Arcadia_Natural8Value* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  _Arcadia_UTF8ArrayIterator it;
  _Arcadia_UTF8ArrayIterator_initialize(thread, &it, bytes, numberOfBytes);
  Arcadia_JumpTarget jt;
  Arcadia_Thread_pushJumpTarget(thread, &jt);
  if (Arcadia_JumpTarget_save(&jt)) {
    while (_Arcadia_UTF8ArrayIterator_hasCodePoint(thread, &it)) {
      _Arcadia_UTF8ArrayIterator_next(thread, &it);
    }
    if (_Arcadia_UTF8ArrayIterator_hasError(thread, &it)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Thread_popJumpTarget(thread);
    _Arcadia_UTF8ArrayIterator_uninitialize(thread, &it);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    _Arcadia_UTF8ArrayIterator_uninitialize(thread, &it);
  }
}

static void
testFixture
  (
    Arcadia_Thread* thread,
    const Arcadia_Natural8Value* bytes,
    Arcadia_SizeValue numberOfBytes,
    const Result* results,
    Arcadia_SizeValue numberOfResults
  )
{
  _Arcadia_UTF8ArrayIterator it;
  _Arcadia_UTF8ArrayIterator_initialize(thread, &it, bytes, numberOfBytes);
  Arcadia_JumpTarget jt;
  Arcadia_Thread_pushJumpTarget(thread, &jt);
  if (Arcadia_JumpTarget_save(&jt)) {
    Arcadia_SizeValue resultIndex = 0;
    while (resultIndex < numberOfResults) {
      const Result* result = &(results[resultIndex]);
      if (result->error) {
        if (_Arcadia_UTF8ArrayIterator_hasCodePoint(thread, &it)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
          Arcadia_Thread_jump(thread);
        }
        if (!_Arcadia_UTF8ArrayIterator_hasError(thread, &it)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
          Arcadia_Thread_jump(thread);
        }
      } else {
        if (_Arcadia_UTF8ArrayIterator_hasError(thread, &it)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
          Arcadia_Thread_jump(thread);
        }
        if (!_Arcadia_UTF8ArrayIterator_hasCodePoint(thread, &it)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
          Arcadia_Thread_jump(thread);
        }
        if (result->byteIndex != _Arcadia_UTF8ArrayIterator_getByteIndex(thread, &it)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
          Arcadia_Thread_jump(thread);
        }
        if (result->codePointLength != _Arcadia_UTF8ArrayIterator_getCodePointLength(thread, &it)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
          Arcadia_Thread_jump(thread);
        }
        if (result->codePoint != _Arcadia_UTF8ArrayIterator_getCodePoint(thread, &it)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
          Arcadia_Thread_jump(thread);
        }
        if (resultIndex != _Arcadia_UTF8ArrayIterator_getCodePointIndex(thread, &it)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
          Arcadia_Thread_jump(thread);
        }
        _Arcadia_UTF8ArrayIterator_next(thread, &it);
      }
      resultIndex++;
    }
    if (resultIndex != numberOfResults) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Thread_popJumpTarget(thread);
    _Arcadia_UTF8ArrayIterator_uninitialize(thread, &it);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    _Arcadia_UTF8ArrayIterator_uninitialize(thread, &it);
  }
}

static void
Arcadia_Ring1_Tests_UTF8ArrayIteratorTests1
  (
    Arcadia_Thread* thread
  )
{
  {
    Arcadia_Natural8Value bytes[] = {
      0
    };
    Result results[] = {
      { .byteIndex  = 0, .codePointLength = 0, .codePoint = 0, .error = false, },
    };
    testFixture(thread, &bytes[0], 0, &results[0], 0);
  }
}

static void
Arcadia_Ring1_Tests_UTF8ArrayIteratorTests2
  (
    Arcadia_Thread* thread
  )
{
  {
    Arcadia_Natural8Value bytes[] = {
      'a'
    };
    Result results[] = {
      {.byteIndex = 0, .codePointLength = 1, .codePoint = 'a', .error = false, },
    };
    testFixture(thread, &bytes[0], sizeof(bytes) / sizeof(Arcadia_Natural8Value), &results[0], sizeof(results) / sizeof(Result)); 
  }
  {
    Arcadia_Natural8Value bytes[] = {
      0xCF, 0x80, // "GREEK SMALL LETTER PI"/U+03C0/https://www.compart.com/de/unicode/U+03C0
    };
    Result results[] = {
      {.byteIndex = 0, .codePointLength = 2, .codePoint = 0x03C0, .error = false, },
    };
    testFixture(thread, &bytes[0], sizeof(bytes) / sizeof(Arcadia_Natural8Value), &results[0], sizeof(results) / sizeof(Result));
  }
  {
    Arcadia_Natural8Value bytes[] = {
      0x61, // "a"/https://www.compart.com/de/unicode/U+0061
      0x62, // "b"/https://www.compart.com/de/unicode/U+0062
    };
    Result results[] = {
      { .byteIndex = 0, .codePointLength = 1, .codePoint = 0x0061, .error = false, },
      { .byteIndex = 1, .codePointLength = 1, .codePoint = 0x0062, .error = false, },
    };
    testFixture(thread, &bytes[0], sizeof(bytes) / sizeof(Arcadia_Natural8Value), &results[0], sizeof(results) / sizeof(Result));
  }
  {
    Arcadia_Natural8Value bytes[] = {
      0xE2, 0x80, 0x93, // "EN DASH"/U+2013/https://www.compart.com/de/unicode/U+2013
    };
    Result results[] = {
      {.byteIndex = 0, .codePointLength = 3, .codePoint = 0x2013, .error = false, },
    };
    testFixture(thread, &bytes[0], sizeof(bytes) / sizeof(Arcadia_Natural8Value), &results[0], sizeof(results) / sizeof(Result));
  }
  {
    Arcadia_Natural8Value bytes[] = {
      0x61, // "a"/https://www.compart.com/de/unicode/U+0061
      0x62, // "b"/https://www.compart.com/de/unicode/U+0062
      0x79, // "y"/https://www.compart.com/de/unicode/U+0079
      0x7A, // "z"/https://www.compart.com/de/unicode/U+007A
      0xCF, 0x80, // "GREEK SMALL LETTER PI"/U+03C0/https://www.compart.com/de/unicode/U+03C0
      0xE2, 0x80, 0x93, // "EN DASH"/U+2013/https://www.compart.com/de/unicode/U+2013
    };
    Result results[] = {
      {.byteIndex = 0, .codePointLength = 1, .codePoint = 0x0061, .error = false, },
      {.byteIndex = 1, .codePointLength = 1, .codePoint = 0x0062, .error = false, },
      {.byteIndex = 2, .codePointLength = 1, .codePoint = 0x0079, .error = false, },
      {.byteIndex = 3, .codePointLength = 1, .codePoint = 0x007A, .error = false, },
      {.byteIndex = 4, .codePointLength = 2, .codePoint = 0x03C0, .error = false, },
      {.byteIndex = 6, .codePointLength = 3, .codePoint = 0x2013, .error = false, },
    };
    testFixture(thread, &bytes[0], sizeof(bytes) / sizeof(Arcadia_Natural8Value), &results[0], sizeof(results) / sizeof(Result));
  }
  {
    // Check if the parser rejects overlong encodings.
    // "<DELETE>"/"DEL"/U+007F is in the 1-Byte range and shall be encoded as a 1 Byte sequence
    // 01111111 = 0x7F
    // according to standard.
    // We can encode it as a 2 Byte sequence, however
    // 11000001 10111111 = C1 BF
    // which must be rejected according to standard.
    Arcadia_Natural8Value bytes[] = {
      0xC1,
      0xBF,
    };
    Result results[] = {
      {.byteIndex = 0, .codePointLength = 0, .codePoint = 0, .error = true },
      {.byteIndex = 0, .codePointLength = 0, .codePoint = 0, .error = true },
    };
    testFixture(thread, &bytes[0], sizeof(bytes) / sizeof(Arcadia_Natural8Value), &results[0], sizeof(results) / sizeof(Result));
  }
  {
    // Check if the parser rejects overlong encodings.
    // "<DELETE>"/"DEL"/U+007F is in the 1-Byte range and shall be encoded as a 1 Byte sequence
    // b(01111111) = h(7F)
    // according to standard.
    // We can encode it as a 2 Byte sequence, however
    // b(11100000 10000001 10111111b) = h(E0 81 BF)
    // which must be rejected according to standard.
    Arcadia_Natural8Value bytes[] = {
      0xE0,
      0x81,
      0xBF,
    };
    Result results[] = {
      {.byteIndex = 0, .codePointLength = 0, .codePoint = 0, .error = true },
      {.byteIndex = 0, .codePointLength = 0, .codePoint = 0, .error = true },
      {.byteIndex = 0, .codePointLength = 0, .codePoint = 0, .error = true },
    };
    testFixture(thread, &bytes[0], sizeof(bytes) / sizeof(Arcadia_Natural8Value), &results[0], sizeof(results) / sizeof(Result));
  }
  {
    // Check if the parser rejects overlong encodings.
    // "<DELETE>"/"DEL"/U+007F is in the 1-Byte range and shall be encoded as a 1 Byte sequence
    // 01111111b = 7Fh
    // according to standard.
    // We can encode it as a 2 Byte sequence, however
    // b(11110000 10000000 10000001 10111111) = h(F0 80 81 BF)
    // which must be rejected according to standard.
    Arcadia_Natural8Value bytes[] = {
      0xF0,
      0x80,
      0x81,
      0xBF,
    };
    Result results[] = {
      {.byteIndex = 0, .codePointLength = 0, .codePoint = 0, .error = true },
      {.byteIndex = 0, .codePointLength = 0, .codePoint = 0, .error = true },
      {.byteIndex = 0, .codePointLength = 0, .codePoint = 0, .error = true },
      {.byteIndex = 0, .codePointLength = 0, .codePoint = 0, .error = true },
    };
    testFixture(thread, &bytes[0], sizeof(bytes) / sizeof(Arcadia_Natural8Value), &results[0], sizeof(results) / sizeof(Result));
  }
  {
    // Check if the parser rejects overlong encodings.
    // "a"/"U+0061" is in the 1-Byte range and shall be encoded as 1 Byte sequence
    // b(01100001) = h(61)
    // according to standard.
    // We can encode it as 2 Byte sequence, however
    // b(11000001 10100001) = h(C1 A1)
    // which must be rejected according to standard.
    Arcadia_Natural8Value bytes[] = {
      0xC1,
      0xA1,
    };
    Result results[] = {
      {.byteIndex = 0, .codePointLength = 0, .codePoint = 0, .error = true },
      {.byteIndex = 0, .codePointLength = 0, .codePoint = 0, .error = true },
    };
    testFixture(thread, &bytes[0], sizeof(bytes) / sizeof(Arcadia_Natural8Value), &results[0], sizeof(results) / sizeof(Result));
  }
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_UTF8ArrayIteratorTests1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_UTF8ArrayIteratorTests2)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
