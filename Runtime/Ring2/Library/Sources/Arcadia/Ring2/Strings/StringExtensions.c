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

#define ARCADIA_RING2_MODULE (1)
#include "Arcadia/Ring2/Strings/StringExtensions.h"

#include <string.h>
#include <limits.h> // INT_MAX, INT_MIN

Arcadia_String*
Arcadia_String_createFromCxxInt
  (
    Arcadia_Thread* thread,
    int x
  )
{
  #if !defined(INT_MAX)
    #error("INT_MAX not defined")
  #endif
  #if !defined(INT_MIN)
    #error("INT_MIN not defined")
  #endif
  #if INT_MAX == Arcadia_Integer64Value_Maximum && INT_MIN == Arcadia_Integer64Value_Minimum
    return Arcadia_String_createFromInteger32(thread, x);
  #elif INT_MAX == Arcadia_Integer32Value_Maximum && INT_MIN == Arcadia_Integer32Value_Minimum
    return Arcadia_String_createFromInteger32(thread, x);
  #else
    #error("environment not (yet) supported");
  #endif
}

Arcadia_String*
Arcadia_String_createFromCxxString
  (
    Arcadia_Thread* thread,
    const char *x
  )
{
  if (!x) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, x, strlen(x))));
}

int
Arcadia_String_toCxxInt
  (
    Arcadia_Thread* thread,
    Arcadia_String* self
  )
{
  Arcadia_Integer64Value target = Arcadia_String_toInteger64(thread, self);
  if (target < INT_MIN || target > INT_MAX) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  return (int)target;
}

void
Arcadia_StringBuilder_insertBackCxxInt
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    int x
  )
{
  Arcadia_String* string = Arcadia_String_createFromCxxInt(thread, x);
  Arcadia_StringBuilder_insertBack(thread, self, Arcadia_Value_makeObjectReferenceValue(string));
}

void
Arcadia_StringBuilder_insertBackCxxString
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    const char *x
  )
{
  Arcadia_String* y = Arcadia_String_createFromCxxString(thread, x);
  Arcadia_StringBuilder_insertBack(thread, self, Arcadia_Value_makeObjectReferenceValue(y));
}

void
Arcadia_StringBuilder_insertFrontString
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_String* x
  )
{
  Arcadia_StringBuilder_insertFront(thread, self, Arcadia_Value_makeObjectReferenceValue(x));
}

void
Arcadia_StringBuilder_insertBackString
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_String* x
  )
{
  Arcadia_StringBuilder_insertBack(thread, self, Arcadia_Value_makeObjectReferenceValue(x));
}

void
Arcadia_String_insertByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_SizeValue index,
    Arcadia_ByteArrayBuilder* target
  )
{ Arcadia_ByteArrayBuilder_insertBytes(thread, target, index, Arcadia_String_getBytes(thread, self), Arcadia_String_getNumberOfBytes(thread,self)); }

void
Arcadia_String_insertFrontByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_ByteArrayBuilder* target
  )
{ Arcadia_ByteArrayBuilder_insertFrontBytes(thread, target, Arcadia_String_getBytes(thread, self), Arcadia_String_getNumberOfBytes(thread, self)); }

void
Arcadia_String_insertBackByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_ByteArrayBuilder* target
  )
{ Arcadia_ByteArrayBuilder_insertBackBytes(thread, target, Arcadia_String_getBytes(thread, self), Arcadia_String_getNumberOfBytes(thread, self)); }