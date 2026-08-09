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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_REALTOSTRING_REAL64STRING_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_REALTOSTRING_REAL64STRING_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Natural8.h"
#include "Arcadia/Ring1/Implementation/Real64.h"
#include "Arcadia/Ring1/Implementation/Size.h"

// @brief Convert a Real64 value to an UTF-8 string.
// @param function A pointer to a function receiving the string.
// This function is called if a call to this function successfull converted the specified Real32 value @a value to an UTF-8 string.
// It receives as its first argument and second argument @a thread and @a context passed to the call.
// The third argument is a pointer to a constant array of Bytes. The fourth argument is the number of Bytes in that array.
// The Bytes are the conversion of the Real32 value @a value to an UTF-8 string.
void
Arcadia_Real64Value_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value value,
    void* context,
    void (*function)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_REALTOSTRING_REAL64STRING_H_INCLUDED
