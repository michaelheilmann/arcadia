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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Implementation/Unicode.h"

#include "Arcadia/Ring1/Implementation/Unicode/UTF8ArrayIterator.h"
#include "Arcadia/Ring1/Include.h"

/// @brief Classify the first Byte of an UTF8 sequence to determine the length of the sequence.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param x The Byte.
/// @success <code>*RETURN</code> was assigned the length of the sequence.
/// @error Arcadia_Status_InvalidEncoding @a x can not be classified.
static Arcadia_SizeValue
classifyFirstByte
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value x
  ) {
#pragma push_macro("Version")
#undef Version
#define Version (2)
#if Version == 2
  if ((x & 0x80) == 0x00) {
    // To determine if the first Byte is in the range 0xxx xxxx,
    // mask the Byte with 1000 0000 / 0x80. If the result is 0,
    // then the first Byte is in the range 0xxx xxxx.
    return 1;
  } else if ((x & 0xE0) == 0xC0) {
    // To determine if the first Byte is in the range 110x xxxx,
    // mask the Byte with 11100000 / 0xE0. If the result is 1100 0000 / 0xC0,
    // then the first Byte is in the range 110x xxxx.
    return 2;
  } else if ((x & 0xF0) == 0xE0) {
    // To determine if the first Byte is in the range 1110 xxxx,
    // mask the Byte with 1111 0000 / 0xF0. If the result is 1110 0000 / 0xE0,
    // then the first Byte is in the range 1110 xxxx.
    return 3;
  } else if ((x & 0xF8) == 0xF0) {
    // To determine if the first Byte is in the range 1111 0xxx,
    // mask the Byte with 1111 1000 / 0xF8. If the result is 1111 0000 / 0xF0,
    // then the first Byte is in th range 1111 0xxx.
    return 4;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
    Arcadia_Thread_jump(thread);
  }
#elif Version == 1
  if (x <= 0x7F) {
    // To determine if the first Byte is in the range 0xxx xxxx, we must ensure that the first Bit is 0.
    // If x is smaller than or equal to 0111 1111 / 0x7f then the first Bit is 0.
    return 1;
  } else if(x <= 0xDF) {
    // To determine if the first Byte is in the range 110x xxxx, we must ensure that the first three Bit are 110.
    // If x is smaller than or equal to 1101 1111 / 0xDF then the first three Bit are 110.
    return 2;
  } else if (x <= 0xEF) {
    // To determine if the first Byte is in the range 1110 xxxx, we must ensure that the first four Bit are 1110.
    // If x is smaller than or equal to 1110 1111 / 0xEF the the first four Bit are 1110.
    return 3;
  } else if (x <= 0xF7) {
    // To determine if the first Byte is in the range 1111 0xxx, we must ensure that the first five Bit are 1111 0.
    // If x is smaller than or equal to 1111 0111 / 0xF7, then the first five Bit are 1111 0.
    return 4;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
    Arcadia_Thread_jump(thread);
  }
#endif
#undef Version
#pragma pop_macro("Version")
}

void
Arcadia_Unicode_encodeCodePointsUTF8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints,
    Arcadia_Unicode_EncodeCodePointCallbackContext* context,
    Arcadia_Unicode_EncodeCodePointCallbackFunction* function
  )
{
  if (!codePoints || !function) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!numberOfCodePoints) {
    return;
  }
  for (Arcadia_SizeValue i = 0, n = numberOfCodePoints; i < n; ++i) {
    Arcadia_Natural32Value codePoint = codePoints[i];
    if (codePoint <= 0x7f) {
      Arcadia_Natural8Value x[1];
      x[0] = (codePoint & 0b1111111);
      (*function)(thread, context, &x[0], Arcadia_SizeValue_Literal(1));
    } else if (/*0x80 <=*/ codePoint <= 0x7ff) {
      // This CP uses the lower 11 bit of the 32 Bit number.
      // The first 5 of these bits are stored in the first Byte.
      // The last 6 of these bits are stored in the second Byte.
      Arcadia_Natural8Value x[2];
      x[0] = (codePoint & 0b11111000000) >> 6;
      x[1] = (codePoint & 0b00000111111) >> 0;
      // OR in 1100.0000.
      x[0] |= 0b11000000;
      // OR in 1000.0000.
      x[1] |= 0b10000000;
      (*function)(thread, context, &x[0], Arcadia_SizeValue_Literal(2));
    } else if (/*0x800 <=*/ codePoint <= 0xffff) {
      // This CP uses the lower 16 bit of the 32 Bit number.
      // The first 4 of these bits are stored in the first Byte.
      // The next 6 of these bits are stored in the second Byte.
      // The last 6 of these bits are stored in teh third Byte.
      Arcadia_Natural8Value x[3];
      x[0] = (codePoint & 0b1111000000000000) >> 12;
      x[1] = (codePoint & 0b0000111111000000) >> 6;
      x[2] = (codePoint & 0b0000000000111111) >> 0;
      // OR in 1110.0000.
      x[0] |= 0b11100000;
      // OR in 1000.0000.
      x[1] |= 0b10000000;
      x[2] |= 0b10000000;
      (*function)(thread, context, &x[0], Arcadia_SizeValue_Literal(3));
    } else if (/*0x10000 <=*/ codePoint <= 0x10FFFF) {
      // This CP uses the lower 21 Bit of the code point.
      // The first 3 are stored in the first Byte.
      // The next 6 are stored in the second Byte.
      // The next 6 are stored in the third Byte.
      // The last 6 are stored in the fourth Byte.
      Arcadia_Natural8Value x[4];
      x[0] = (codePoint & 0b111000000000000000000) >> 18;
      x[1] = (codePoint & 0b000111111000000000000) >> 12;
      x[2] = (codePoint & 0b000000000111111000000) >> 6;
      x[3] = (codePoint & 0b000000000000000111111) >> 0;
      // OR in 1111.0000.
      x[0] |= 0b1110000;
      // OR in 1000.0000.
      x[1] |= 0b10000000;
      x[2] |= 0b10000000;
      x[3] |= 0b10000000;
      (*function)(thread, context, &x[0], Arcadia_SizeValue_Literal(4));
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
}

static inline Arcadia_Natural8Value Arcadia_swapEndianessNatural8Value(Arcadia_Thread* thread, Arcadia_Natural8Value x) {
  return x;
}


static inline Arcadia_Natural16Value Arcadia_swapEndianessNatural16Value(Arcadia_Thread* thread, Arcadia_Natural16Value x) {
  return
    ((x & 0xff00) >> 8) |
    ((x & 0x00ff) << 8)
    ;
}

static inline Arcadia_Natural32Value Arcadia_swapEndianessNatural32Value(Arcadia_Thread* thread, Arcadia_Natural32Value x) {
  return
    ((x & 0xff000000) >> 24) |
    ((x & 0x000000ff) << 24) |
    ((x & 0x00ff0000) >>  8) |
    ((x & 0x0000ff00) <<  8)
    ;
}

static inline Arcadia_Natural64Value Arcadia_swapEndianessNatural64Value(Arcadia_Thread* thread, Arcadia_Natural64Value x) {
  return
    ((x & 0xff00000000000000) >> 56) |
    ((x & 0x00000000000000ff) << 56) |
    ((x & 0x00ff000000000000) >> 32) |
    ((x & 0x000000000000ff00) << 32) |
    ((x & 0x0000ff0000000000) >> 16) |
    ((x & 0x0000000000ff0000) << 16) |
    ((x & 0x000000ff00000000) >>  8) |
    ((x & 0x00000000ff000000) <<  8)
    ;
}

void
Arcadia_Unicode_encodeCodePointsUtf32Be
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints,
    Arcadia_Unicode_EncodeCodePointCallbackContext* context,
    Arcadia_Unicode_EncodeCodePointCallbackFunction* function
  )
{
  if (!codePoints || !function) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!numberOfCodePoints) {
    return;
  }
  for (Arcadia_SizeValue i = 0, n = numberOfCodePoints; i < n; ++i) {
    Arcadia_Natural32Value codePoint = codePoints[i];
    if (codePoint <= 0x10FFFF) {
    #if Arcadia_Configuration_ByteOrder_LittleEndian == Arcadia_Configuration_ByteOrder
      codePoint = Arcadia_swapEndianessNatural32Value(thread, codePoint);
    #elif Arcadia_Configuration_ByteOrder_BigEndian == Arcadia_Configuration_ByteOrder
      // Nothing to do here.
    #else
      #error("environment not (yet) supported")
    #endif
      (*function)(thread, context, (Arcadia_Natural8Value*)&codePoint, Arcadia_SizeValue_Literal(4));
   } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
}

void
Arcadia_Unicode_encodeCodePointsUtf32Le
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints,
    Arcadia_Unicode_EncodeCodePointCallbackContext* context,
    Arcadia_Unicode_EncodeCodePointCallbackFunction* function
  )
{
  if (!codePoints || !function) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!numberOfCodePoints) {
    return;
  }
  for (Arcadia_SizeValue i = 0, n = numberOfCodePoints; i < n; ++i) {
    Arcadia_Natural32Value codePoint = codePoints[i];
    if (codePoint <= 0x10FFFF) {
    #if Arcadia_Configuration_ByteOrder_BigEndian == Arcadia_Configuration_ByteOrder
      codePoint = Arcadia_swapEndianessNatural32Value(thread, codePoint);
    #elif Arcadia_Configuration_ByteOrder_LittleEndian == Arcadia_Configuration_ByteOrder
      // Nothing to do here.
    #else
      #error("environment not (yet) supported")
    #endif
      (*function)(thread, context, (Arcadia_Natural8Value*)&codePoint, Arcadia_SizeValue_Literal(4));
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
}

typedef struct Arcadia_Unicode_UTF8EncoderState {
  Arcadia_Natural8Value const* bytes;
  Arcadia_SizeValue numberOfBytes;
  Arcadia_Unicode_EncodeCodePointCallbackContext* context;
  Arcadia_Unicode_EncodeCodePointCallbackFunction* function;
} Arcadia_Unicode_UTF8EncoderState;

static void
Arcadia_Unicode_encodeBytesUTF8Internal
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8EncoderState* state
  )
{ 
  if (!state->numberOfBytes) {
    return;
  }
  for (Arcadia_Natural8Value const* current = (Arcadia_Natural8Value const*)state->bytes, *end = (Arcadia_Natural8Value const*)state->bytes + state->numberOfBytes; current != end; ) {
    Arcadia_Natural8Value x = *current;
    if (x <= 0x7f) {
      (*state->function)(thread, state->context, current, Arcadia_SizeValue_Literal(1));
      current++;
    } else if (x <= 0x7ff) {
      (*state->function)(thread, state->context, current, Arcadia_SizeValue_Literal(1));
      if (end - current < 2) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
        Arcadia_Thread_jump(thread);
      }
      current++;
      for (size_t i = 1; i < 2; ++i) {
        if (0x80 != (*current) & 0xc0) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
          Arcadia_Thread_jump(thread);
        }
        (*state->function)(thread, state->context, current, Arcadia_SizeValue_Literal(1));
        current++;
      }
    } else if (x <= 0xffff) {
      (*state->function)(thread, state->context, current, Arcadia_SizeValue_Literal(1));
      if (end - current < 3) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
        Arcadia_Thread_jump(thread);
      }
      current++;
      for (size_t i = 1; i < 3; ++i) {
        if (0x80 != (*current) & 0xc0) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
          Arcadia_Thread_jump(thread);
        }
        (*state->function)(thread, state->context, current, Arcadia_SizeValue_Literal(1));
        current++;
      }
    } else if (x <= 0x10ffff) {
      (*state->function)(thread, state->context, current, Arcadia_SizeValue_Literal(1));
      if (end - current < 4) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
        Arcadia_Thread_jump(thread);
      }
      current++;
      for (size_t i = 1; i < 4; ++i) {
        if (0x80 != (*current) & 0xc0) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
          Arcadia_Thread_jump(thread);
        }
        (*state->function)(thread, state->context, current, Arcadia_SizeValue_Literal(1));
        current++;
      }
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
}

void
Arcadia_Unicode_encodeBytesUTF8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes,
    Arcadia_Unicode_EncodeCodePointCallbackContext* context,
    Arcadia_Unicode_EncodeCodePointCallbackFunction* function
  )
{
  Arcadia_Unicode_UTF8EncoderState s;
  s.bytes = bytes;
  s.numberOfBytes = numberOfBytes;
  s.context = context;
  s.function = function;
  if (!numberOfBytes) {
    return;
  }
  for (Arcadia_Natural8Value const* current = (Arcadia_Natural8Value const*)bytes, *end = (Arcadia_Natural8Value const*)bytes + numberOfBytes; current != end; ) {
    Arcadia_Natural8Value x = *current;
    if (x <= 0x7f) {
      (*s.function)(thread, s.context, current, Arcadia_SizeValue_Literal(1));
      current++;
    } else if (x <= 0x7ff) {
      (*function)(thread, s.context, current, Arcadia_SizeValue_Literal(1));
      if (end - current < 2) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
        Arcadia_Thread_jump(thread);
      }
      current++;
      for (size_t i = 1; i < 2; ++i) {
        if (0x80 != (*current) & 0xc0) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
          Arcadia_Thread_jump(thread);
        }
        (*s.function)(thread, s.context, current, Arcadia_SizeValue_Literal(1));
        current++;
      }
    } else if (x <= 0xffff) {
      (*s.function)(thread, s.context, current, Arcadia_SizeValue_Literal(1));
      if (end - current < 3) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
        Arcadia_Thread_jump(thread);
      }
      current++;
      for (size_t i = 1; i < 3; ++i) {
        if (0x80 != (*current) & 0xc0) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
          Arcadia_Thread_jump(thread);
        }
        (*s.function)(thread, s.context, current, Arcadia_SizeValue_Literal(1));
        current++;
      }
    } else if (x <= 0x10ffff) {
      (*s.function)(thread, s.context, current, Arcadia_SizeValue_Literal(1));
      if (end - current < 4) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
        Arcadia_Thread_jump(thread);
      }
      current++;
      for (size_t i = 1; i < 4; ++i) {
        if (0x80 != (*current) & 0xc0) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
          Arcadia_Thread_jump(thread);
        }
        (*s.function)(thread, s.context, current, Arcadia_SizeValue_Literal(1));
        current++;
      }
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
}
