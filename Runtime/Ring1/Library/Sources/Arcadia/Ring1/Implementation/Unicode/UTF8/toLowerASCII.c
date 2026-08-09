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
#include "Arcadia/Ring1/Implementation/Unicode/UTF8/toLowerASCII.h"

#include "Arcadia/Ring1/Implementation/Unicode/UTF8/mapASCIIToASCII.h"

static inline void
f
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value source,
    Arcadia_Natural8Value* target
  )
{
  if ('A' <= source && source <= 'Z') {
    *target = source + 32;
  } else {
    *target = source;
  }
}

void
Arcadia_Unicode_UTF8_toLowerASCII
  (
    Arcadia_Thread* thread,
    void* bytes,
    Arcadia_SizeValue numberOfBytes,
    Arcadia_SizeValue* index,
    Arcadia_SizeValue* length
  )
{ Arcadia_Unicode_UTF8_mapASCIIToASCII(thread, bytes, numberOfBytes, &f, index, length); }
