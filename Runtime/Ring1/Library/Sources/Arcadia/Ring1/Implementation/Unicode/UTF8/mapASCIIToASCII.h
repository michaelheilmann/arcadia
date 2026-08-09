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


#if !defined(ARCADIA_RING1_IMPLEMENTATION_UNICODE_UTF8_MAPASCIITOASCII_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_UNICODE_UTF8_MAPASCIITOASCII_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Natural8.h"
#include "Arcadia/Ring1/Implementation/Size.h"
#include "Arcadia/Ring1/Implementation/Status.h"
#include "Arcadia/Ring1/Implementation/Thread.h"

#include <assert.h>

static inline void
Arcadia_Unicode_UTF8_mapASCIIToASCII
  (
    Arcadia_Thread* thread,
    void* bytes,
    Arcadia_SizeValue numberOfBytes,
    void (*callback)(Arcadia_Thread*, Arcadia_Natural8Value source, Arcadia_Natural8Value* target),
    Arcadia_SizeValue* index,
    Arcadia_SizeValue* length
  )
{
  Arcadia_Natural8Value* START = bytes;
  Arcadia_Natural8Value* END = ((Arcadia_Natural8Value*)bytes) + numberOfBytes;

  Arcadia_Natural8Value* start;
  // Adjust "start" if "index" is specified.
  if (index) {
    Arcadia_SizeValue i = 0;
    Arcadia_Natural8Value* p = START;
    while (i != *index && p != END) {
      if ((*p & 0x80) == 0x00) {
        // To determine if the first Byte is in the range 0xxx xxxx,
        // mask the Byte with 1000 0000 / 0x80. If the result is 0,
        // then the first Byte is in the range 0xxx xxxx.
        p += 1;
      } else if ((*p & 0xE0) == 0xC0) {
        // To determine if the first Byte is in the range 110x xxxx,
        // mask the Byte with 11100000 / 0xE0. If the result is 1100 0000 / 0xC0,
        // then the first Byte is in the range 110x xxxx.
        p += 2;
      } else if ((*p & 0xF0) == 0xE0) {
        // To determine if the first Byte is in the range 1110 xxxx,
        // mask the Byte with 1111 0000 / 0xF0. If the result is 1110 0000 / 0xE0,
        // then the first Byte is in the range 1110 xxxx.
        p += 3;
      } else /*if ((*current & 0xF8) == 0xF0)*/ {
        // To determine if the first Byte is in the range 1111 0xxx,
        // mask the Byte with 1111 1000 / 0xF8. If the result is 1111 0000 / 0xF0,
        // then the first Byte is in th range 1111 0xxx.
        p += 4;
      }
      i += 1;
    }
    if (i != *index) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    start = p;
  } else {
    start = START;
  }

  // Adjust "end" if "length" is specified.
  uint8_t* end;
  if (length) {
    Arcadia_SizeValue i = 0;
    Arcadia_Natural8Value* p = start;
    while (i != *length && p != END) {
      if ((*p & 0x80) == 0x00) {
        // To determine if the first Byte is in the range 0xxx xxxx,
        // mask the Byte with 1000 0000 / 0x80. If the result is 0,
        // then the first Byte is in the range 0xxx xxxx.
        p += 1;
      } else if ((*p & 0xE0) == 0xC0) {
        // To determine if the first Byte is in the range 110x xxxx,
        // mask the Byte with 11100000 / 0xE0. If the result is 1100 0000 / 0xC0,
        // then the first Byte is in the range 110x xxxx.
        p += 2;
      } else if ((*p & 0xF0) == 0xE0) {
        // To determine if the first Byte is in the range 1110 xxxx,
        // mask the Byte with 1111 0000 / 0xF0. If the result is 1110 0000 / 0xE0,
        // then the first Byte is in the range 1110 xxxx.
        p += 3;
      } else /*if ((*p & 0xF8) == 0xF0)*/ {
        // To determine if the first Byte is in the range 1111 0xxx,
        // mask the Byte with 1111 1000 / 0xF8. If the result is 1111 0000 / 0xF0,
        // then the first Byte is in th range 1111 0xxx.
        p += 4;
      }
      i += 1;
    }
    if (i != *length) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    end = p;
  } else {
    end = END;
  }

  Arcadia_Natural8Value* current = start;
  while (current != end) {
    if ((*current & 0x80) == 0x00) {
      // To determine if the first Byte is in the range 0xxx xxxx,
      // mask the Byte with 1000 0000 / 0x80. If the result is 0,
      // then the first Byte is in the range 0xxx xxxx.
      (*callback)(thread, *current, current);
      current += 1;
    } else if ((*current & 0xE0) == 0xC0) {
      // To determine if the first Byte is in the range 110x xxxx,
      // mask the Byte with 11100000 / 0xE0. If the result is 1100 0000 / 0xC0,
      // then the first Byte is in the range 110x xxxx.
      current += 2;
    } else if ((*current & 0xF0) == 0xE0) {
      // To determine if the first Byte is in the range 1110 xxxx,
      // mask the Byte with 1111 0000 / 0xF0. If the result is 1110 0000 / 0xE0,
      // then the first Byte is in the range 1110 xxxx.
      current += 3;
    } else /*if ((*current & 0xF8) == 0xF0)*/ {
      // To determine if the first Byte is in the range 1111 0xxx,
      // mask the Byte with 1111 1000 / 0xF8. If the result is 1111 0000 / 0xF0,
      // then the first Byte is in th range 1111 0xxx.
      current += 4;
    }
  }  
}

#endif // ARCADIA_RING1_IMPLEMENTATION_UNICODE_UTF8_MAPASCIITOASCII_H_INCLUDED
