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

#if !defined(ARCADIA_ADL_GRAPHS_MODULE_H_INCLUDED)
#define ARCADIA_ADL_GRAPHS_MODULE_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Languages/Include.h"
#include "Arcadia/ADL/Reader.module.h"

// The graph front end interprets the reserved name segment `__graph`.
// User-authored definitions, graph template names, and graph instance names
// must not claim a name containing this segment.
#define ARCADIA_ADL_GRAPHS_RESERVED_SEGMENT (u8"__graph")

// Compare the specified Arcadia string and the specified static C string by value.
static inline Arcadia_BooleanValue
Arcadia_ADL_Graphs_stringEquals
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    const char* cxxString
  )
{
  if (NULL == string) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Value t = Arcadia_Value_makeObjectReferenceValue(string);
  return Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, cxxString), &t);
}

// Compare two Arcadia strings by value.
static inline Arcadia_BooleanValue
Arcadia_ADL_Graphs_stringEqualsString
  (
    Arcadia_Thread* thread,
    Arcadia_String* a,
    Arcadia_String* b
  )
{
  Arcadia_Value t = Arcadia_Value_makeObjectReferenceValue(b);
  return Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)a, &t);
}

// Get if the specified Arcadia string has zero bytes.
static inline Arcadia_BooleanValue
Arcadia_ADL_Graphs_stringIsEmpty
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
{
  return 0 == Arcadia_String_getNumberOfBytes(thread, string) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}

// Get if any dot-separated segment of the specified name equals the reserved segment.
static inline Arcadia_BooleanValue
Arcadia_ADL_Graphs_hasReservedSegment
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  )
{
  static const char* reserved = "__graph";
  Arcadia_SizeValue numberOfBytes = Arcadia_String_getNumberOfBytes(thread, name);
  Arcadia_SizeValue segmentStart = 0;
  for (Arcadia_SizeValue i = 0; i <= numberOfBytes; ++i) {
    if (i == numberOfBytes || '.' == Arcadia_String_getByteAt(thread, name, i)) {
      if (8 == i - segmentStart) {
        Arcadia_BooleanValue matches = Arcadia_BooleanValue_True;
        for (Arcadia_SizeValue j = 0; j < 8; ++j) {
          if (Arcadia_String_getByteAt(thread, name, segmentStart + j) != (Arcadia_Natural8Value)reserved[j]) {
            matches = Arcadia_BooleanValue_False;
            break;
          }
        }
        if (matches) {
          return Arcadia_BooleanValue_True;
        }
      }
      segmentStart = i + 1;
    }
  }
  return Arcadia_BooleanValue_False;
}

// Concatenate the specified prefix, the specified separator, and the specified suffix into a fresh string.
static inline Arcadia_String*
Arcadia_ADL_Graphs_concat
  (
    Arcadia_Thread* thread,
    Arcadia_String* prefix,
    const char* separator,
    Arcadia_String* suffix
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackString(thread, builder, prefix);
  Arcadia_StringBuilder_insertBackCxxString(thread, builder, separator);
  Arcadia_StringBuilder_insertBackString(thread, builder, suffix);
  Arcadia_SizeValue numberOfBytes = Arcadia_StringBuilder_getNumberOfBytes(thread, builder);
  Arcadia_Natural8Value const* bytes = Arcadia_StringBuilder_getBytes(thread, builder);
  return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, bytes, numberOfBytes)));
}

// Raise an error with the specified diagnostic message.
// The message is composed of the specified prefix, the specified name (may be null), and the specified suffix.
static inline void
Arcadia_ADL_Graphs_raise
  (
    Arcadia_Thread* thread,
    Arcadia_Status status,
    const char* prefix,
    Arcadia_String* name,
    const char* suffix
  )
{
  Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, message, prefix);
  if (name) {
    Arcadia_StringBuilder_insertBackString(thread, message, name);
  }
  if (suffix) {
    Arcadia_StringBuilder_insertBackCxxString(thread, message, suffix);
  }
  Arcadia_Languages_DiagnosticsOld_emit(thread, message);
  Arcadia_Thread_setStatus(thread, status);
  Arcadia_Thread_jump(thread);
}

#endif // ARCADIA_ADL_GRAPHS_MODULE_H_INCLUDED
