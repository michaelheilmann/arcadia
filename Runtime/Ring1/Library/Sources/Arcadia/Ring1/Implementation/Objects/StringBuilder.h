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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_OBJECTS_STRINGBUILDER_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_OBJECTS_STRINGBUILDER_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring2/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Object.h"

/// @code
/// class Arcadia.StringBuilder {
///   constructor()
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.StringBuilder", Arcadia_StringBuilder,
                          u8"Arcadia.Object");

struct Arcadia_StringBuilderDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_StringBuilder {
  Arcadia_Object _parent;
  Arcadia_Natural8Value* elements;
  /// @brief The size, in Bytes, of this string buffer.
  Arcadia_SizeValue size;
  /// @brief The capacity, in Bytes, of this string buffer.
  Arcadia_SizeValue capacity;
};

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_StringBuilder_create
Arcadia_StringBuilder*
Arcadia_StringBuilder_create
  (
    Arcadia_Thread* thread
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_StringBuilder_insertAt
void
Arcadia_StringBuilder_insertAt
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue codePointIndex,
    Arcadia_Value value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_StringBuilder_insertBack
void
Arcadia_StringBuilder_insertBack
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Value value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_StringBuilder_insertFront
void
Arcadia_StringBuilder_insertFront
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Value value
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_StringBuilder_compareTo
Arcadia_Integer32Value
Arcadia_StringBuilder_compareTo
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Value other
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_StringBuilder_clear
void
Arcadia_StringBuilder_clear
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_StringBuilder_getNumberOfBytes
Arcadia_SizeValue
Arcadia_StringBuilder_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder const* self
  );

/// @brief Get the number of Unicode code points.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string buffer.
/// @return The number of Unicode code points.
Arcadia_SizeValue
Arcadia_StringBuilder_getNumberOfCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self
  );

/// @brief Convert occurrences of ASCII character a-z to upper case.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string buffer.
void
Arcadia_StringBuilder_toUpperASCII
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue* index,
    Arcadia_SizeValue* length
  );

/// @brief Convert occurrences of ASCII characters A-Z to lower case.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string buffer.
void
Arcadia_StringBuilder_toLowerASCII
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue* index,
    Arcadia_SizeValue* length
  );

/// @brief Remove code points from the front of this string buffer.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string buffer.
/// @param numberOfCodePoints The number of code points to remove.
/// @error Arcadia_Status_ArgumentValueInvalid @a numberOfCodePoints is greater than the number of code points in this string buffer
/// @todo Add to official API. Add Arcadia.StringBuilder.removeCodePointsAt.
void
Arcadia_StringBuilder_removeCodePointsFront
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue numberOfCodePoints
  );

/// @brief Remove code points from the back of this string buffer.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string buffer.
/// @param numberOfCodePoints The number of code points to remove.
/// @error Arcadia_Status_ArgumentValueInvalid @a numberOfCodePoints is greater than the number of code points in this string buffer
/// @todo Add to official API. Add Arcadia.StringBuilder.removeCodePointsAt.
void
Arcadia_StringBuilder_removeCodePointsBack
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue numberOfCodePoints
  );

/// @brief Get if a sequence of Bytes is a suffix of this string buffer.
/// @param self A pointer to ths string buffer.
/// @param bytes A pointer to an array of @a numberOfBytes Bytes.
/// @param numberOfBytes The number of Bytes in the array pointed to by @a bytes.
/// @return
/// Arcadia_BooleanValue_True if the sequence of Bytes is a suffix of the string buffer.
/// Arcadia_BooleanValue_False otherwise.
Arcadia_BooleanValue
Arcadia_StringBuilder_endsWith_pn
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  );

/// @brief Get if the specified sequence of Bytes is a prefix of this string buffer.
/// @param self A pointer to this string buffer.
/// @param bytes A pointer to an array of @a numberOfBytes Bytes.
/// @param numberOfBytes The number of Bytes in the array pointed to by @a bytes.
/// @return
/// Arcadia_BooleanValue_True if the sequence of Bytes is a prefix of the string buffer.
/// Arcadia_BooleanValue_False otherwise.
Arcadia_BooleanValue
Arcadia_StringBuilder_startsWith_pn
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  );

void
Arcadia_StringBuilder_insertBackCodePoint
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value codePoint
  );

void
Arcadia_StringBuilder_insertBackCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  );

void
Arcadia_StringBuilder_insertFrontCodePoint
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value codePoint
  );

void
Arcadia_StringBuilder_insertFrontCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  );

Arcadia_Natural8Value const*
Arcadia_StringBuilder_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder const* self
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_OBJECTS_STRINGBUILDER_H_INCLUDED
