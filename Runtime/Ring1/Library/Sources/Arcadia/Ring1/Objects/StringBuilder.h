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

#if !defined(ARCADIA_RING1_OBJECTS_STRINGBUILDER_H_INCLUDED)
#define ARCADIA_RING1_OBJECTS_STRINGBUILDER_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Object.h"

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

/// @brief Create an empty mutable UTF-8 string.
// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_StringBuilder_create
Arcadia_StringBuilder*
Arcadia_StringBuilder_create
  (
    Arcadia_Thread* thread
  );

/// @brief Insert at a zero-based codepoint boundary, including the end.
/// @remarks Accepts RuntimeUTF8String, RuntimeByteArray, ByteArrayBuilder, String and StringBuilder values.
/// Byte-array inputs must be valid UTF-8. Self insertion uses the original contents.
/// Invalid input or allocation failure leaves contents unchanged.
// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_StringBuilder_insertAt
void
Arcadia_StringBuilder_insertAt
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue codePointIndex,
    Arcadia_Value value
  );

/// @brief Append a value accepted by insertAt, with the same failure guarantees.
// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_StringBuilder_insertBack
void
Arcadia_StringBuilder_insertBack
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Value value
  );

/// @brief Prepend a value accepted by insertAt, with the same failure guarantees.
// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_StringBuilder_insertFront
void
Arcadia_StringBuilder_insertFront
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Value value
  );

/// @brief Compare unsigned UTF-8 bytes lexicographically; a proper prefix sorts first.
/// @remarks Accepts RuntimeUTF8String, String and StringBuilder; other types raise ArgumentTypeInvalid.
// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_StringBuilder_compareTo
Arcadia_Integer32Value
Arcadia_StringBuilder_compareTo
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Value other
  );

/// @brief Remove all contents while retaining capacity.
// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_StringBuilder_clear
void
Arcadia_StringBuilder_clear
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self
  );

/// @brief Return the number of stored bytes, including embedded NUL bytes.
// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_StringBuilder_getNumberOfBytes
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
/// @param index Optional codepoint start; null means zero.
/// @param length Optional codepoint count; null means through the end.
/// @remarks Invalid ranges raise ArgumentValueInvalid before modification; non-ASCII bytes are unchanged.
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
/// @param index Optional codepoint start; null means zero.
/// @param length Optional codepoint count; null means through the end.
/// @remarks Invalid ranges raise ArgumentValueInvalid before modification; non-ASCII bytes are unchanged.
void
Arcadia_StringBuilder_toLowerASCII
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue* index,
    Arcadia_SizeValue* length
  );

/// @brief Remove a range of Unicode code points without changing the remaining bytes.
/// @param codePointIndex The zero-based start index; the end index is valid for an empty range.
/// @param numberOfCodePoints The number of code points to remove.
/// @error Arcadia_Status_ArgumentValueInvalid The range is outside the string.
/// @remarks An invalid range leaves the string unchanged.
void
Arcadia_StringBuilder_removeCodePointsAt
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue codePointIndex,
    Arcadia_SizeValue numberOfCodePoints
  );

/// @brief Atomically replace a codepoint range with a value accepted by insertAt.
/// @remarks The range is half-open; an empty range at the end is valid.
/// Self replacement uses the original contents. An empty value removes the range.
/// Invalid input or allocation failure leaves the string unchanged.
void
Arcadia_StringBuilder_replaceCodePointsAt
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue codePointIndex,
    Arcadia_SizeValue numberOfCodePoints,
    Arcadia_Value value
  );

/// @brief Remove code points from the front of this string buffer.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string buffer.
/// @param numberOfCodePoints The number of code points to remove.
/// @error Arcadia_Status_ArgumentValueInvalid @a numberOfCodePoints is greater than the number of code points in this string buffer
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
void
Arcadia_StringBuilder_removeCodePointsBack
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue numberOfCodePoints
  );

/// @brief Get if a sequence of Bytes is a suffix of this string buffer.
/// @param self A pointer to ths string buffer.
/// @param bytes A pointer to an array of @a numberOfBytes Bytes; null is allowed only for zero bytes.
/// @error Arcadia_Status_ArgumentValueInvalid A nonempty byte sequence has a null pointer.
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
/// @param bytes A pointer to an array of @a numberOfBytes Bytes; null is allowed only for zero bytes.
/// @error Arcadia_Status_ArgumentValueInvalid A nonempty byte sequence has a null pointer.
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

/// @brief Insert one Unicode scalar value at the back.
/// @error Arcadia_Status_EncodingInvalid The value is not a Unicode scalar value.
void
Arcadia_StringBuilder_insertBackCodePoint
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value codePoint
  );

/// @brief Insert Unicode scalar values in array order at the back.
/// @remarks A zero count is a no-op (including a null array); otherwise the array must contain count readable values.
/// Embedded U+0000 is permitted. Invalid input or allocation failure leaves the string unchanged.
/// @error Arcadia_Status_EncodingInvalid An input value is not a Unicode scalar value.
/// @error Arcadia_Status_ArgumentValueInvalid A nonempty array is null or its size overflows.
void
Arcadia_StringBuilder_insertBackCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  );

/// @brief Insert one Unicode scalar value at the front.
/// @error Arcadia_Status_EncodingInvalid The value is not a Unicode scalar value.
void
Arcadia_StringBuilder_insertFrontCodePoint
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value codePoint
  );

/// @brief Insert Unicode scalar values in array order at the front.
/// @remarks A zero count is a no-op (including a null array); otherwise the array must contain count readable values.
/// Embedded U+0000 is permitted. Invalid input or allocation failure leaves the string unchanged.
/// @error Arcadia_Status_EncodingInvalid An input value is not a Unicode scalar value.
/// @error Arcadia_Status_ArgumentValueInvalid A nonempty array is null or its size overflows.
void
Arcadia_StringBuilder_insertFrontCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  );

/// @brief Borrow the stored bytes; no implicit NUL terminator is supplied.
/// @remarks Valid until mutation or destruction. Keep the builder rooted while using the pointer.
/// For an independent immutable snapshot use Arcadia_String_create with the builder as an object reference value.
Arcadia_Natural8Value const*
Arcadia_StringBuilder_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder const* self
  );

#endif // ARCADIA_RING1_OBJECTS_STRINGBUILDER_H_INCLUDED
