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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_OBJECTS_UNICODECODEPOINTREADER_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_OBJECTS_UNICODECODEPOINTREADER_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Object.h"
typedef struct Arcadia_ByteReader Arcadia_ByteReader;

/// @brief
/// Decodes Unicode Bytes sequences to Unicode code points.
/// Reports erroneous Unicode Byte sequences.
/// @code
/// class Arcadia.UnicodeCodePointReader {
///   constructor()
/// }
/// @endcode
/// https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/
Arcadia_declareObjectType(u8"Arcadia.UnicodeCodePointReader", Arcadia_UnicodeCodePointReader,
                          u8"Arcadia.Object");

struct Arcadia_UnicodeCodePointReaderDispatch {
  Arcadia_ObjectDispatch _parent;
  void (*nextValue)(Arcadia_Thread*, Arcadia_UnicodeCodePointReader* self);
  Arcadia_Natural32Value (*getValue)(Arcadia_Thread* thread, Arcadia_UnicodeCodePointReader* self);
  Arcadia_BooleanValue (*hasValue)(Arcadia_Thread* thread, Arcadia_UnicodeCodePointReader* self);
  Arcadia_BooleanValue (*hasError)(Arcadia_Thread* thread, Arcadia_UnicodeCodePointReader* self);
  void (*getByteRange)(Arcadia_Thread* thread, Arcadia_UnicodeCodePointReader* self, Arcadia_SizeValue* start, Arcadia_SizeValue* length);
};

struct Arcadia_UnicodeCodePointReader {
  Arcadia_Object _parent;
};

void
Arcadia_UnicodeCodePointReader_nextValue
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self
  );

/// @brief Get the code point.
/// @return The code point.
/// @error Arcadia_Status_OperationInvalid if Arcadia_UnicodeCodePointReader_hasCodePoint(o) returns #Arcadia_BooleanValue_False.
Arcadia_Natural32Value
Arcadia_UnicodeCodePointReader_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self
  );

/// @brief Get if there is a Unicode code point.
/// @return #Arcadia_BooleanValue_True if there is a Unicode code point. #Arcadia_BooleanValue_False.
Arcadia_BooleanValue
Arcadia_UnicodeCodePointReader_hasValue
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self
  );

/// @brief Get if there is an error.
/// @return #Arcadia_BooleanValue_True if there is an error. #Arcadia_BooleanValue_False.
/// @remarks If Arcadia_UnicodeCodePointReader_hasError(o) returns #Arcadia_BooleanValue_True then Arcadia_UnicodeCodePointReader_hasCodePoint(o) returns #Arcadia_BooleanValue_False.
Arcadia_BooleanValue
Arcadia_UnicodeCodePointReader_hasError
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self
  );

/// @brief Get the range of Bytes of the code point / error in the input.
/// @param [out] *start is assigned the zero-based index of the Byte at which the code point starts at.
/// @param [out] *length is assigned the length, in Bytes, of the code point.
/// This is 0 if  there is no current code point or there is an error.
void
Arcadia_UnicodeCodePointReader_getByteRange
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self,
    Arcadia_SizeValue *start,
    Arcadia_SizeValue* length
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_OBJECTS_UNICODECODEPOINTREADER_H_INCLUDED
