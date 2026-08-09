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
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/toReal.h"

#include "Arcadia/Ring1/Implementation/ImmutableUTF8String.h"
#include "Arcadia/Ring1/Implementation/StringToReal/toReal64.h"

Arcadia_Real32Value
_toReal32
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  )
{
  return (Arcadia_Real32Value)Arcadia_toReal64(thread, Arcadia_RuntimeUTF8String_getBytes(thread, immutableUTF8StringValue), Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, immutableUTF8StringValue));
}

Arcadia_Real64Value
_toReal64
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  )
{
  return Arcadia_toReal64(thread, Arcadia_RuntimeUTF8String_getBytes(thread, immutableUTF8StringValue), Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, immutableUTF8StringValue));

}
