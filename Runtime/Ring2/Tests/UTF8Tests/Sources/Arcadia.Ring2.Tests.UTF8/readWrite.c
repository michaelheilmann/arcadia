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

#include "Arcadia.Ring2.Tests.UTF8/readWrite.h"

#include "Arcadia/Ring2/Include.h"

/// @brief
/// - create a Byte buffer holding the sequence (source)
/// - create an empty Byte buffer (target)
/// - read its unicode code points using UTF8ByteBufferReader from source
/// - write each unicode code point into target using UTF8ByteBufferWriter
/// - compare the contents of both Byte buffers
/// - success if thes contents are equal
/// - failure otherwise
static void
onReadWriteTestFixture
  (
    Arcadia_Thread* thread,
    char const* p,
    size_t n
  )
{
  Arcadia_ByteArrayBuilder* sourceByteArrayBuilder = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, sourceByteArrayBuilder, p, n);
  Arcadia_ByteArrayBuilder* targetByteArrayBuilder = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArrayBuilder_ByteReader_create(thread, sourceByteArrayBuilder));

  Arcadia_Unicode_Encoder* writer = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    Arcadia_Unicode_Encoder_encodeCodePoints(thread, writer, &codePoint, 1, targetByteArrayBuilder);
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  }
  if (!Arcadia_ByteArrayBuilder_isEqualTo(thread, sourceByteArrayBuilder, targetByteArrayBuilder)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_Tests_UTF8_readWrite1
  (
    Arcadia_Thread* thread
  )
{
  onReadWriteTestFixture(thread, u8"abc", sizeof(u8"abc") - 1);
  onReadWriteTestFixture(thread, u8"xyz", sizeof(u8"xyz") - 1);
}
