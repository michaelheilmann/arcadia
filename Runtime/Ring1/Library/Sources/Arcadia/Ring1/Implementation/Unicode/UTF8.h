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


#if !defined(ARCADIA_RING1_IMPLEMENTATION_UNICODE_UTF8_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_UNICODE_UTF8_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif
#include "Arcadia/Ring1/Implementation/Natural8.h"
#include "Arcadia/Ring1/Implementation/Size.h"
#include "Arcadia/Ring1/Implementation/Unicode/Unicode.h"

#define Arcadia_Configuration_Unicode_UTF8_ClassifyFirstByteOptimization (1)

/// @brief
/// The first Byte of an code point encoded in UTF-8 encoding denotes the number of subsequent Bytes.
/// If the value of the given Byte is valid for  code point encoded in UTF-8 encoding, this function returns 1, 2, 3, or 4.
/// Otherwise it returns Arcadia_SizeValue_Maximum.
/// @param value
/// The Byte.
/// @return
/// 1, 2, 3, or 4 or Arcadia_SizeValue_Maximum.
static inline Arcadia_SizeValue
Arcadia_Unicode_UTF8_classifyFirstByte
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value value
  );

static Arcadia_SizeValue
Arcadia_Unicode_UTF8_classifyFirstByte
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value value
  )
{
#if Arcadia_Configuration_Unicode_UTF8_ClassifyFirstByteOptimization == 1
  if ((value & 0x80) == 0x00) {
    // To determine if the first Byte is in the range 0xxx xxxx,
    // mask the Byte with 1000 0000 / 0x80. If the result is 0,
    // then the first Byte is in the range 0xxx xxxx.
    return 1;
  } else if ((value & 0xE0) == 0xC0) {
    // To determine if the first Byte is in the range 110x xxxx,
    // mask the Byte with 11100000 / 0xE0. If the result is 1100 0000 / 0xC0,
    // then the first Byte is in the range 110x xxxx.
    return 2;
  } else if ((value & 0xF0) == 0xE0) {
    // To determine if the first Byte is in the range 1110 xxxx,
    // mask the Byte with 1111 0000 / 0xF0. If the result is 1110 0000 / 0xE0,
    // then the first Byte is in the range 1110 xxxx.
    return 3;
  } else if ((value & 0xF8) == 0xF0) {
    // To determine if the first Byte is in the range 1111 0xxx,
    // mask the Byte with 1111 1000 / 0xF8. If the result is 1111 0000 / 0xF0,
    // then the first Byte is in th range 1111 0xxx.
    return 4;
  } else {
    return Arcadia_SizeValue_Maximum;
  }
#elif Arcadia_Configuration_Unicode_UTF8_ClassifyFirstByteOptimization == 0
  if (value <= 0x7F) { /* 0b01111111 */
    // To determine if the first Byte is in the range 0xxx xxxx, we must ensure that the first Bit is 0.
    // If x is smaller than or equal to 0111 1111 / 0x7f then the first Bit is 0.
    return 1;
  } else if (value <= 0xDF) { /* 0b11011111 */
    // To determine if the first Byte is in the range 110x xxxx, we must ensure that the first three Bit are 110.
    // If x is smaller than or equal to 1101 1111 / 0xDF then the first three Bit are 110.
    return 2;
  } else if (value <= 0xEF) { /* 0b11101111 */
    // To determine if the first Byte is in the range 1110 xxxx, we must ensure that the first four Bit are 1110.
    // If x is smaller than or equal to 1110 1111 / 0xEF the the first four Bit are 1110.
    return 3;
  } else if (value <= 0xF7) { /* 0b11110111 */
    // To determine if the first Byte is in the range 1111 0xxx, we must ensure that the first five Bit are 1111 0.
    // If x is smaller than or equal to 1111 0111 / 0xF7, then the first five Bit are 1111 0.
    return 4;
  } else {
    return Arcadia_SizeValue_Maximum;
  }
#else
  #error("unknown/unsupported optimization level")
#endif
}

#endif // ARCADIA_RING1_IMPLEMENTATION_UNICODE_UTF8_H_INCLUDED
