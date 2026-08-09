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

#if !defined (ARCADIA_RING1_UNICODE_UTF8ARRAYITERATOR_MODULE_H_INCLUDED)
#define ARCADIA_RING1_UNICODE_UTF8ARRAYITERATOR_MODULE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Unicode/UTF8.h"
#include "Arcadia/Ring1/Implementation/Natural32.h"
#include "Arcadia/Ring1/Implementation/Status.h"
#include "Arcadia/Ring1/Implementation/Thread.h"

#define CodePoint_Start (Arcadia_Unicode_CodePoint_Last + 1)
#define CodePoint_End (Arcadia_Unicode_CodePoint_Last + 2)
#define CodePoint_Error (Arcadia_Unicode_CodePoint_Last + 3)

/// @brief Error detecting UTF8 decoder using a struct to be allocated on the stack and as part of other objects.
typedef struct _Arcadia_UTF8ArrayIterator {
  /// @brief Pointer to the array of Bytes.
  const Arcadia_Natural8Value* p;
  /// @brief The number of Bytes in the array pointed to by @a p.
  Arcadia_SizeValue n;

  /// The index of the Byte at which the current code point starts at.
  /// @a 0 if codePoint is CodePoint_Start.
  Arcadia_SizeValue byteIndex;
  /// @brief The index of the code point at which the current code point starts at.
  /// @a 0 if codePoint is CodePoint_Start.
  Arcadia_SizeValue codePointIndex;
  /// The number of code points visisted so far.
  Arcadia_SizeValue numberOfCodePoints;
  
  /// The length, in Bytes, of the current code point.
  /// @a 0 if codePoint is CodePoint_Start or CodePoint_Error. Not @a 0 otherwise.
  Arcadia_SizeValue codePointLength;
  /// The current code point.
  /// If this is CodePoint_Error, then numberOfBytes is 0.
  Arcadia_Natural32Value codePoint;

} _Arcadia_UTF8ArrayIterator;

static inline void
_Arcadia_UTF8ArrayIterator_initialize
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self,
    const Arcadia_Natural8Value* p,
    const Arcadia_SizeValue n
  );

static inline void
_Arcadia_UTF8ArrayIterator_uninitialize
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  );

static inline void
_Arcadia_UTF8ArrayIterator_next
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  );
  
static inline Arcadia_BooleanValue
_Arcadia_UTF8ArrayIterator_hasError
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  );
  
static inline Arcadia_BooleanValue
_Arcadia_UTF8ArrayIterator_hasCodePoint
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  );

static inline Arcadia_SizeValue
_Arcadia_UTF8ArrayIterator_getCodePointLength
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  );

static inline Arcadia_Natural32Value
_Arcadia_UTF8ArrayIterator_getCodePoint
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  );

static inline void
_Arcadia_UTF8ArrayIterator_initialize
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self,
    const Arcadia_Natural8Value* p,
    const Arcadia_SizeValue n
  )
{
  if (!self || !p || Arcadia_SizeValue_Maximum == n) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->p = p;
  self->n = n;
  self->numberOfCodePoints = 0;
  self->codePointIndex = 0;
  self->byteIndex = 0;
  self->codePointLength = 0;
  self->codePoint = CodePoint_Start;
}

static inline void
_Arcadia_UTF8ArrayIterator_uninitialize
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static inline void
_Arcadia_UTF8ArrayIterator_next
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  )
{
  if (!self || self->codePoint == CodePoint_End || self->codePoint == CodePoint_Error) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (self->byteIndex + self->codePointLength == self->n) {
    self->codePoint = CodePoint_End;
    return;
  }
  Arcadia_SizeValue nextByteIndex,
                    nextCodePointIndex;
  if (self->codePoint == CodePoint_Start) {
    nextByteIndex = self->byteIndex;
    nextCodePointIndex = self->codePointIndex;
  } else {
    nextByteIndex = self->byteIndex + self->codePointLength;
    nextCodePointIndex = self->codePointIndex + 1;
  }
  Arcadia_Natural8Value byte = self->p[nextByteIndex];
  Arcadia_SizeValue nextCodePointLength = 0;
  Arcadia_Natural32Value nextCodePoint = 0;
  if ((byte & 0x80) == 0x00) {
    // To determine if the first Byte is in the range 0xxx xxxx,
    // mask the Byte with 1000 0000 / 0x80. If the result is 0,
    // then the first Byte is in the range 0xxx xxxx.
    nextCodePointLength = 1;
  } else if ((byte & 0xE0) == 0xC0) {
    // To determine if the first Byte is in the range 110x xxxx,
    // mask the Byte with 11100000 / 0xE0. If the result is 1100 0000 / 0xC0,
    // then the first Byte is in the range 110x xxxx.
    nextCodePointLength = 2;
  } else if ((byte & 0xF0) == 0xE0) {
    // To determine if the first Byte is in the range 1110 xxxx,
    // mask the Byte with 1111 0000 / 0xF0. If the result is 1110 0000 / 0xE0,
    // then the first Byte is in the range 1110 xxxx.
    nextCodePointLength = 3;
  } else if ((byte & 0xF8) == 0xF0) {
    // To determine if the first Byte is in the range 1111 0xxx,
    // mask the Byte with 1111 1000 / 0xF8. If the result is 1111 0000 / 0xF0,
    // then the first Byte is in th range 1111 0xxx.
    nextCodePointLength = 4;
  } else {
    nextCodePoint = CodePoint_Error; // (*)
    nextCodePointLength = 0;
  }
  if (self->n - nextByteIndex < nextCodePointLength) {
    nextCodePoint = CodePoint_Error; // (*)
    nextCodePointLength = 0;
  }

  // The masks to remove the prefix bits from the first Byte.
  static const Arcadia_Natural8Value mask[] = {
    0b11111111,
    0b00011111,
    0b00001111,
    0b00000111,
  };

  if (nextCodePoint != CodePoint_Error) {
    nextCodePoint = byte & mask[nextCodePointLength - 1]; // Warning that nextNumberOfBytes - 1 might yield -1 can be ignored because of (*). 
    for (Arcadia_SizeValue i = 1; i < nextCodePointLength; ++i) {
      byte = self->p[nextByteIndex + i]; // We need to mask with 0011 1111
      if (0x80 != (byte & 0xC0)) {
        nextCodePoint = CodePoint_Error;
        nextCodePointLength = 0;
        break;
      }
      byte &= 0x3F;
      nextCodePoint <<= 6;
      nextCodePoint |= byte;
    }
  }
  if (nextCodePointLength == 1 && nextCodePoint <= 0x7f) {
  } else if (nextCodePointLength == 2 && 0x80 <= nextCodePoint && nextCodePoint <= 0x7ff) {
  } else if (nextCodePointLength == 3 && 0x800 <= nextCodePoint && nextCodePoint <= 0xffff) {
  } else if (nextCodePointLength == 4 && 0x10000 <= nextCodePoint && nextCodePoint > 0x10ffff) {
  } else {
    nextCodePoint = CodePoint_Error;
    nextCodePointLength = 0;
  }
  
  self->byteIndex = nextByteIndex;
  self->codePointLength = nextCodePointLength;
  self->numberOfCodePoints++;
  self->codePointIndex = nextCodePointIndex;
  self->codePoint = nextCodePoint;
}

static inline Arcadia_SizeValue
_Arcadia_UTF8ArrayIterator_getCodePointIndex
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->codePointIndex;  
}

static inline Arcadia_SizeValue
_Arcadia_UTF8ArrayIterator_getNumberOfCodePoints
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->numberOfCodePoints;
}

static inline Arcadia_SizeValue
_Arcadia_UTF8ArrayIterator_getByteIndex
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->byteIndex;
}

static inline Arcadia_BooleanValue
_Arcadia_UTF8ArrayIterator_hasCodePoint
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (self->codePoint == CodePoint_Start) {
    _Arcadia_UTF8ArrayIterator_next(thread, self);
  }
  return CodePoint_End != self->codePoint && CodePoint_Error != self->codePoint;
}

static inline Arcadia_BooleanValue
_Arcadia_UTF8ArrayIterator_hasError
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  )
{ 
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (self->codePoint == CodePoint_Start) {
    _Arcadia_UTF8ArrayIterator_next(thread, self);
  }
  return CodePoint_Error == self->codePoint;
}

static inline Arcadia_SizeValue
_Arcadia_UTF8ArrayIterator_getCodePointLength
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->codePointLength;
}

static inline Arcadia_Natural32Value
_Arcadia_UTF8ArrayIterator_getCodePoint
  (
    Arcadia_Thread* thread,
    _Arcadia_UTF8ArrayIterator* self
  )
{
  if (self->codePoint == CodePoint_Error || self->codePoint == CodePoint_Start || self->codePoint == CodePoint_End) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->codePoint;
}

#endif // ARCADIA_RING1_UNICODE_UTF8ARRAYITERATOR_MODULE_H_INCLUDED
