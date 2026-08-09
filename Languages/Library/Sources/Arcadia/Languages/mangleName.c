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

#include "Arcadia/Languages/mangleName.h"

static Arcadia_String*
on
  (
    Arcadia_Thread* thread,
    const Arcadia_Natural8Value* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  static const Arcadia_Natural32Value base16DigitCodePoints[] =
  {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'a', // 10
    'b',
    'c',
    'd',
    'e',
    'f', // 15
  };
  Arcadia_StringBuilder* target = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCodePoint(thread, target, 'x');
  const Arcadia_Natural8Value* start = bytes,
                             * end = bytes + numberOfBytes;
  const Arcadia_Natural8Value* current = start;
  while (current != end) {
    Arcadia_Natural8Value value = *current;
    // Get the lower digit.
    Arcadia_Natural8Value lower = value % 16;
    // Get the upper digit.
    Arcadia_Natural8Value upper = value / 16;

    Arcadia_StringBuilder_insertBackCodePoint(thread, target, base16DigitCodePoints[upper]);
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, base16DigitCodePoints[lower]);

    current++;
  }
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(target));
}

static Arcadia_String*
onString
  (
    Arcadia_Thread* thread,
    Arcadia_String* source
  )
{ return on(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source)); }

static Arcadia_String*
onStringBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* source
  )
{ return on(thread, Arcadia_StringBuilder_getBytes(thread, source), Arcadia_StringBuilder_getNumberOfBytes(thread, source)); }

Arcadia_String*
Arcadia_Languages_mangleName
  (
    Arcadia_Thread* thread,
    Arcadia_Value source
  )
{
  if (!Arcadia_Value_isObjectReferenceValue(&source)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Object* object = Arcadia_Value_getObjectReferenceValue(&source);
  if (Arcadia_Object_isInstanceOf(thread, object, _Arcadia_String_getType(thread))) {
    return onString(thread, (Arcadia_String*)object);
  } else if (Arcadia_Object_isInstanceOf(thread, object, _Arcadia_StringBuilder_getType(thread))) {
    return onStringBuffer(thread, (Arcadia_StringBuilder*)object);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}
