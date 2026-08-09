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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_IMMUTABLEUTF8STRING_TOINTEGER_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_IMMUTABLEUTF8STRING_TOINTEGER_H_INCLUDED

#include "Arcadia/Ring1/Implementation/Integer16.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
#include "Arcadia/Ring1/Implementation/Integer64.h"
#include "Arcadia/Ring1/Implementation/Integer8.h"
typedef struct Arcadia_RuntimeUTF8String Arcadia_RuntimeUTF8String;

Arcadia_Integer16Value
_toInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  );

Arcadia_Integer32Value
_toInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  );

Arcadia_Integer64Value
_toInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  );

Arcadia_Integer8Value
_toInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_IMMUTABLEUTF8STRING_TOINTEGER_H_INCLUDED
