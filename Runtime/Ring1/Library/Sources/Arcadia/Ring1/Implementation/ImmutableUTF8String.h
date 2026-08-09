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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_IMMUTABLEUTF8STRING_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_IMMUTABLEUTF8STRING_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/ARMS/Include.h"
#include "Arcadia/Ring1/Implementation/Boolean.h"
#include "Arcadia/Ring1/Implementation/Integer16.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
#include "Arcadia/Ring1/Implementation/Integer64.h"
#include "Arcadia/Ring1/Implementation/Integer8.h"
#include "Arcadia/Ring1/Implementation/Natural16.h"
#include "Arcadia/Ring1/Implementation/Natural32.h"
#include "Arcadia/Ring1/Implementation/Natural64.h"
#include "Arcadia/Ring1/Implementation/Natural8.h"
#include "Arcadia/Ring1/Implementation/Real32.h"
#include "Arcadia/Ring1/Implementation/Real64.h"
#include "Arcadia/Ring1/Implementation/Size.h"
#include "Arcadia/Ring1/Implementation/Void.h"

typedef struct Arcadia_RuntimeUTF8String Arcadia_RuntimeUTF8String;

typedef Arcadia_RuntimeUTF8String* Arcadia_RuntimeUTF8StringValue;

struct Arcadia_RuntimeUTF8String {
  Arcadia_SizeValue hash;
  Arcadia_SizeValue numberOfBytes;
  Arcadia_Natural8Value bytes[];
};

/// @brief Create an immutable UTF8 string object from an UTF8 string.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param bytes A pointer to a sequence of @a numberOfBytes Bytes.
/// @param numberOfBytes The number of Bytes in the sequence of Bytes pointed to by @a bytes.
/// @return A pointer to the immmutable UTF8 string object.
/// @error #Arcadia_Status_ArgumentInvalid @a bytes ia null pointer
/// @error #Arcadia_Status_ArgumentInvalid the sequence of Bytes is an UTF8 string
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  );

/// @brief Create an immutable UTF8 string object from Arcadia_BooleanValue value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param booleanValue The Arcadia_BooleanValue value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue booleanValue
  );

/// @brief Create an immutable UTF8 string object from Arcadia_Integer16Value value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param integer16Value The Arcadia_Integer16Value value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value integer16Value
  );

/// @brief Create an immutable UTF8 string object from Arcadia_Integer32Value value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param integer32Value The Arcadia_Integer32Value value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value integer32Value
  );

/// @brief Create an immutable UTF8 string object from Arcadia_Integer64Value value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param integer64Value The Arcadia_Integer64Value value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value integer64Value
  );

/// @brief Create an immutable UTF8 string object from Arcadia_Integer8Value value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param integer8Value The Arcadia_Integer8Value value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value integer8Value
  );

/// @brief Create an immutable UTF8 string object from Arcadia_Natural16Value value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param natural16Value The Arcadia_Natural16Value value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value natural16Value
  );

/// @brief Create an immutable UTF8 string object from Arcadia_Natural32Value value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param natural32Value The Arcadia_Natural32Value value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value natural32Value
  );

/// @brief Create an immutable UTF8 string object from Arcadia_Natural64Value value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param natural64Value The Arcadia_Natural64Value value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value natural64Value
  );

/// @brief Create an immutable UTF8 string object from Arcadia_Natural8Value value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param natural8Value The Arcadia_Natural8Value value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value natural8Value
  );

/// @brief Create an immutable UTF8 string object from Arcadia_Real32Value value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param voidValue The Arcadia_Real32Value value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromReal32
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value real32Value
  );

/// @brief Create an immutable UTF8 string object from Arcadia_Real64Value value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param voidValue The Arcadia_Real64Value value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromReal64
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value real64Value
  );

/// @brief Create an immutable UTF8 string object from Arcadia_SizeValue value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param voidValue The Arcadia_SizeValue value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromSize
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue sizeValue
  );

/// @brief Create an immutable UTF8 string object from Arcadia_VoidValue value.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param voidValue The Arcadia_VoidValue value.
/// @return A pointer to the immmutable UTF8 string object.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromVoid
  (
    Arcadia_Thread* thread,
    Arcadia_VoidValue voidValue
  );

/// @brief
/// Visit this Arcadia_RuntimeUTF8String object.
/// @param thread
/// A pointer to the Arcadia_Thread object.
/// @param self
/// A pointer to this Arcadia_RuntimeUTF8String object.
void
Arcadia_RuntimeUTF8String_visit
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

#if defined(Arcadia_ARMS_Configuration_WithBarriers) && 1 == Arcadia_ARMS_Configuration_WithBarriers

/// @brief
/// Ensure this Arcadia_RuntimeUTF8String object is gray.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
void
Arcadia_RuntimeUTF8String_ensureGray
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

#endif

/// @brief Get if this string starts with another string.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @param other A pointer to the other string.
/// @return
/// #Arcadia_BooleanValue_True if this string starts with another string.
/// #Arcadia_BooleanValue_False otherwise.
Arcadia_BooleanValue
Arcadia_RuntimeUTF8String_startsWith
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self,
    Arcadia_RuntimeUTF8StringValue other
  );

/// @brief Get if this string ends with another string.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @param other A pointer to the other string.
/// @return
/// #Arcadia_BooleanValue_True if this string ends with another string.
/// #Arcadia_BooleanValue_False otherwise.
Arcadia_BooleanValue
Arcadia_RuntimeUTF8String_endsWith
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self,
    Arcadia_RuntimeUTF8StringValue other
  );

/// @brief Get a pointer to the Bytes of this string.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string object.
/// @return A pointer to the Bytes of this string.
/// @warning The Bytes of the array must not be modified.
Arcadia_Natural8Value const*
Arcadia_RuntimeUTF8String_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Get the number of Bytes of the UTF8 Byte sequence of this string.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The number of Bytes of the UTF8 Byte sequence of this string.
Arcadia_SizeValue
Arcadia_RuntimeUTF8String_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Get the hash of this string.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The hash of this string.
Arcadia_SizeValue
Arcadia_RuntimeUTF8String_getHash
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Get the number of code points of this immutable UTF8 string object.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The number of code points of this string.
Arcadia_SizeValue
Arcadia_RuntimeUTF8String_getNumberOfCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into a Boolean value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Boolean value.
/// @error #Arcadia_Status_ConversionFailed the string is not a Boolean literal
Arcadia_BooleanValue
Arcadia_RuntimeUTF8String_toBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into an Integer16 value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Integer16 value.
/// @error #Arcadia_Status_ConversionFailed the string is not an Integer16 literal
Arcadia_Integer16Value
Arcadia_RuntimeUTF8String_toInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into an Integer32 value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Integer32 value.
/// @error #Arcadia_Status_ConversionFailed the string is not an Integer32 literal
Arcadia_Integer32Value
Arcadia_RuntimeUTF8String_toInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into an Integer64 value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Integer64 value.
/// @error #Arcadia_Status_ConversionFailed the string is not an Integer64 literal
Arcadia_Integer64Value
Arcadia_RuntimeUTF8String_toInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into an Integer8 value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Integer8 value.
/// @error #Arcadia_Status_ConversionFailed the string is not an Integer8 literal
Arcadia_Integer8Value
Arcadia_RuntimeUTF8String_toInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into an Natural16 value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Natural16 value.
/// @error #Arcadia_Status_ConversionFailed the string is not an Natural16 literal
Arcadia_Natural16Value
Arcadia_RuntimeUTF8String_toNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into an Natural32 value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Natural32 value.
/// @error #Arcadia_Status_ConversionFailed the string is not an Natural32 literal
Arcadia_Natural32Value
Arcadia_RuntimeUTF8String_toNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into an Natural64 value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Natural64 value.
/// @error #Arcadia_Status_ConversionFailed the string is not an Natural64 literal
Arcadia_Natural64Value
Arcadia_RuntimeUTF8String_toNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into an Natural8 value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Natural8 value.
/// @error #Arcadia_Status_ConversionFailed the string is not an Natural8 literal
Arcadia_Natural8Value
Arcadia_RuntimeUTF8String_toNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into an Real32 value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Real32 value.
/// @error #Arcadia_Status_ConversionFailed the string is not an Real32 literal
Arcadia_Real32Value
Arcadia_RuntimeUTF8String_toReal32
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into an Real64 value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Real64 value.
/// @error #Arcadia_Status_ConversionFailed the string is not an Real64 literal
Arcadia_Real64Value
Arcadia_RuntimeUTF8String_toReal64
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into an Size value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Size value.
/// @error #Arcadia_Status_ConversionFailed the string is not an Size literal
Arcadia_SizeValue
Arcadia_RuntimeUTF8String_toSize
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @brief Convert this immutable UTF8 string object into a Void value.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @return The Void value.
/// @error #Arcadia_Status_ConversionFailed the string is not a Void literal
Arcadia_VoidValue
Arcadia_RuntimeUTF8String_toVoid
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  );

/// @return A pointer to an "foreign value" type of name "Arcadia.RuntimeUTF8String".
Arcadia_TypeValue
_Arcadia_RuntimeUTF8StringValue_getType
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_IMMUTABLEUTF8STRING_H_INCLUDED

