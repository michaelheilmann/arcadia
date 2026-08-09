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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_OBJECTS_STRING_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_OBJECTS_STRING_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Object.h"
typedef struct Arcadia_ByteArray Arcadia_ByteArray;

/// @code
/// class Arcadia.String {
/// 
///   constructor(booleanValue : Arcadia.Boolean)
/// 
///   constructor(integer16Value : Arcadia.Integer16)
///   constructor(integer32Value : Arcadia.Integer32)
///   constructor(integer64Value : Arcadia.Integer64)
///   constructor(integer8Value : Arcadia.Integer8)
/// 
///   constructor(natural16Value : Arcadia.Natural16)
///   constructor(natural32Value : Arcadia.Natural32)
///   constructor(natural64Value : Arcadia.Natural64)
///   constructor(natural8Value : Arcadia.Natural8)
/// 
/// }
/// @endcode
Arcadia_declareObjectType("Arcadia.String", Arcadia_String,
                          "Arcadia.Object");

struct Arcadia_StringDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_String {
  Arcadia_Object _parent;
  Arcadia_RuntimeUTF8String* immutableUTF8String;
};

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_create_pn
Arcadia_String*
Arcadia_String_create_pn
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeByteArray* runtimeByteArray
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_create
Arcadia_String*
Arcadia_String_create
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_endsWith_pn
Arcadia_BooleanValue
Arcadia_String_endsWith_pn
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_startsWith_pn
Arcadia_BooleanValue
Arcadia_String_startsWith_pn
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_getNumberOfBytes
Arcadia_SizeValue
Arcadia_String_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

Arcadia_Natural8Value const*
Arcadia_String_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_getByteAt
Arcadia_Natural8Value
Arcadia_String_getByteAt
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self,
    Arcadia_SizeValue index
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_getNumberOfCodePoints
Arcadia_SizeValue
Arcadia_String_getNumberOfCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_isEqualTo_pn
Arcadia_BooleanValue
Arcadia_String_isEqualTo_pn
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_toBoolean
Arcadia_BooleanValue
Arcadia_String_toBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_toInteger16
Arcadia_Integer16Value
Arcadia_String_toInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_toInteger32
Arcadia_Integer32Value
Arcadia_String_toInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_toInteger64
Arcadia_Integer64Value
Arcadia_String_toInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_toInteger8
Arcadia_Integer8Value
Arcadia_String_toInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_toNatural16
Arcadia_Natural16Value
Arcadia_String_toNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_toNatural32
Arcadia_Natural32Value
Arcadia_String_toNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_toNatural64
Arcadia_Natural64Value
Arcadia_String_toNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_toNatural8
Arcadia_Natural8Value
Arcadia_String_toNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_String_toReal32
Arcadia_Real32Value
Arcadia_String_toReal32
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_toReal64
Arcadia_Real64Value
Arcadia_String_toReal64
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_toVoid
Arcadia_VoidValue
Arcadia_String_toVoid
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromBoolean
Arcadia_String*
Arcadia_String_createFromBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue booleanValue
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromInteger16
Arcadia_String*
Arcadia_String_createFromInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value integer16Value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromInteger32
Arcadia_String*
Arcadia_String_createFromInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value integer32Value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromInteger64
Arcadia_String*
Arcadia_String_createFromInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value integer64Value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromInteger8
Arcadia_String*
Arcadia_String_createFromInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value integer8Value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromNatural16
Arcadia_String*
Arcadia_String_createFromNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value natural16Value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromNatural32
Arcadia_String*
Arcadia_String_createFromNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value natural32Value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromNatural64
Arcadia_String*
Arcadia_String_createFromNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value natural64Value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromNatural8
Arcadia_String*
Arcadia_String_createFromNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value natural8Value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromReal32
Arcadia_String*
Arcadia_String_createFromReal32
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value real32Value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromReal64
Arcadia_String*
Arcadia_String_createFromReal64
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value real64Value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromSize
Arcadia_String*
Arcadia_String_createFromSize
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue sizeValue
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_String_createFromVoid
Arcadia_String*
Arcadia_String_createFromVoid
  (
    Arcadia_Thread* thread,
    Arcadia_VoidValue voidValue
  );

/// @brief Get the first occurence of a code point in a string.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @param codePoint the code point.
/// @return An Arcadia.Size value denoting the zero-based code point index from the start of the string if the code point was found, Arcadia.Void otherwise.
Arcadia_Value
Arcadia_String_findFirstOccurrence
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_Natural32Value codePoint
  );

/// @brief Get the first occurence of a code point in a string.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @param codePoint the code point.
/// @return An Arcadia.Size value denoting the zero-based code point index from the start of the string if the code point was found, Arcadia.Void otherwise.
Arcadia_Value
Arcadia_String_findLastOccurrence
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_Natural32Value codePoint
  );

/// @brief Create an empty string.
/// @param thread A pointer to this thread.
/// @return A pointer to the string.
Arcadia_String*
Arcadia_String_createEmpty
  (
    Arcadia_Thread* thread
  );

/// @brief Create a string that is a substring of the specified string.
/// @param start The code point index of at which the substring starts at.
/// @param length The length, in code points, of the substring as a Arcadia_SizeValue or an Arcadia_Void value.
/// @return A pointer to the substring.
Arcadia_String*
Arcadia_String_substring
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_SizeValue start,
    Arcadia_Value length
  );

Arcadia_ByteArray*
Arcadia_String_toByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_String* self
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_OBJECTS_STRING_H_INCLUDED
