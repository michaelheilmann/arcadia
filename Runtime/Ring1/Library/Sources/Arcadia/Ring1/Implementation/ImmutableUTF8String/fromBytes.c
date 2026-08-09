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
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/fromBytes.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/hash.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/type.h"

Arcadia_RuntimeUTF8String*
_createFromBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (!bytes) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_SizeValue_Maximum - sizeof(Arcadia_RuntimeUTF8String) < numberOfBytes) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_Unicode_isUTF8(thread, bytes, numberOfBytes, NULL)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
    Arcadia_Thread_jump(thread);
  }
  _ensureTypeRegistered(thread);
  Arcadia_RuntimeUTF8String* string = NULL;
  Arcadia_Process_allocate(Arcadia_Thread_getProcess(thread), (void**)&string, TypeName, sizeof(TypeName) - 1, sizeof(Arcadia_RuntimeUTF8String) + numberOfBytes);
  Arcadia_Memory_copy(thread, string->bytes, bytes, numberOfBytes);
  string->numberOfBytes = numberOfBytes;
  string->hash = _hashUTF8(thread, string->bytes, string->numberOfBytes);
  return string;
}
