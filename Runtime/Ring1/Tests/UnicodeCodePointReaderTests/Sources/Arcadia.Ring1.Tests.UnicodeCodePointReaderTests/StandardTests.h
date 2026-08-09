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

#if !defined(ARCADIA_RING1_TESTS_UNICODECODEPOINTREADERTESTS_STANDARDTESTS_H_INCLUDED)
#define ARCADIA_RING1_TESTS_UNICODECODEPOINTREADERTESTS_STANDARDTESTS_H_INCLUDED

#include "Arcadia/Ring1/Include.h"

typedef struct Result {
  Arcadia_Natural32Value codePoint;
  Arcadia_SizeValue byteIndex;
  Arcadia_SizeValue codePointLength;
  Arcadia_BooleanValue error;
  Arcadia_BooleanValue hasValue;
} Result;

void
Arcadia_Ring1_Tests_UnicodeCodePointReaderTests_textFixture
  (
    Arcadia_Thread* thread,
    const Arcadia_Natural8Value* bytes,
    Arcadia_SizeValue numberOfBytes,
    const Result* results,
    Arcadia_SizeValue numberOfResults
  );

void
Arcadia_Ring1_Tests_UnicodeCodePointReader_test1
  (
    Arcadia_Thread* thread
  );

void
Arcadia_Ring1_Tests_UnicodeCodePointReader_test2
  (
    Arcadia_Thread* thread
  );

void
Arcadia_Ring1_Tests_UnicodeCodePointReader_test3
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_RING1_TESTS_UNICODECODEPOINTREADERTESTS_STANDARDTESTS_H_INCLUDED
