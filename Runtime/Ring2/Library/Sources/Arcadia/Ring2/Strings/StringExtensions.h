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

#if !defined(ARCADIA_RING2_STRINGS_STRINGEXTENSIONS_H_INCLUDED)
#define ARCADIA_RING2_STRINGS_STRINGEXTENSIONS_H_INCLUDED

#include "Arcadia/Ring1/Include.h"

Arcadia_String*
Arcadia_String_createFromCxxInt
  (
    Arcadia_Thread* thread,
    int x
  );

int
Arcadia_String_toCxxInt
  (
    Arcadia_Thread* thread,
    Arcadia_String* self
  );

Arcadia_String*
Arcadia_String_createFromCxxString
  (
    Arcadia_Thread* thread,
    const char *x
  );

void
Arcadia_StringBuilder_insertBackCxxInt
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    int x
  );

void
Arcadia_StringBuilder_insertBackCxxString
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    const char *x
  );

void
Arcadia_StringBuilder_insertFrontString
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_String* x
  );

void
Arcadia_StringBuilder_insertBackString
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_String* x
  );

/* @todo Precursor to the iterator API. */
void
Arcadia_String_insertByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_SizeValue index,
    Arcadia_ByteArrayBuilder* target
  );

/* @todo Precursor to the iterator API. */
void
Arcadia_String_insertFrontByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_ByteArrayBuilder* target
  );

/* @todo Precursor to the iterator API. */
void
Arcadia_String_insertBackByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_ByteArrayBuilder* target
  );

#endif // ARCADIA_RING2_STRINGS_STRINGEXTENSIONS_H_INCLUDED
