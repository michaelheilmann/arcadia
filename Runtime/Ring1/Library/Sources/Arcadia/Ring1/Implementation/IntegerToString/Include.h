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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_INTEGERTOSTRING_INCLUDE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_INTEGERTOSTRING_INCLUDE_H_INCLUDED

#include "Arcadia/Ring1/Implementation/Integer16.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
#include "Arcadia/Ring1/Implementation/Integer64.h"
#include "Arcadia/Ring1/Implementation/Integer8.h"

#include "Arcadia/Ring1/Implementation/Natural8.h"
#include "Arcadia/Ring1/Implementation/Size.h"

void
Arcadia_Integer16Value_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value integer16Value,
    void* context,
    void (*function)(Arcadia_Thread* thread, void* context, const Arcadia_Natural8Value* bytes, Arcadia_SizeValue numberOfBytes)
  );

void
Arcadia_Integer32Value_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value integer32Value,
    void* context,
    void (*function)(Arcadia_Thread* thread, void* context, const Arcadia_Natural8Value* bytes, Arcadia_SizeValue numberOfBytes)
  );

void
Arcadia_Integer64Value_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value integer64Value,
    void* context,
    void (*function)(Arcadia_Thread* thread, void* context, const Arcadia_Natural8Value* bytes, Arcadia_SizeValue numberOfBytes)
  );

void
Arcadia_Integer8Value_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value integer8Value,
    void* context,
    void (*function)(Arcadia_Thread* thread, void* context, const Arcadia_Natural8Value* bytes, Arcadia_SizeValue numberOfBytes)
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_INTEGERTOSTRING_INCLUDE_H_INCLUDED
