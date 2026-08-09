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

#if !defined(ARCADIA_RING2_TESTS_UTF8_READWRITE_H_INCLUDED)
#define ARCADIA_RING2_TESTS_UTF8_READWRITE_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

/// @summary
/// Let x := [x[1], ..., x[n]] be a sequence such that each x[i] is an UTF-8 Byte sequence.
/// This test then performs for each x[i] the followin test:
/// - create a Byte buffer holding x[i]
/// - create an empty Byte buffer (target)
/// - read its unicode code points using UTF8ByteBufferReader from source
/// - write each unicode code point into target using UTF8ByteBufferWriter
/// - compare the contents of both Byte buffers
/// - success if their contents are equal
/// - failure otherwise
void
Arcadia_Tests_UTF8_readWrite1
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_RING2_TESTS_UTF8_READWRITE_H_INCLUDED

