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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEREADER_UNICODECODEPOINTREADER_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEREADER_UNICODECODEPOINTREADER_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Objects/UnicodeCodePointReader.h"
typedef struct Arcadia_ByteReader Arcadia_ByteReader;

/// @code
/// class Arcadia.ByteReader.UnicodeCodePointReader extends Arcadia.UnicodeCodePointReader {
///   construct(source:Arcadia.ByteReader)
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.ByteReader.UnicodeCodePointReader", Arcadia_ByteReader_UnicodeCodePointReader,
                          u8"Arcadia.UnicodeCodePointReader");

struct Arcadia_ByteReader_UnicodeCodePointReaderDispatch {
  Arcadia_UnicodeCodePointReaderDispatch _parent;
};

struct Arcadia_ByteReader_UnicodeCodePointReader {
  Arcadia_UnicodeCodePointReader parent;

  Arcadia_Natural8Value flags;

  Arcadia_Natural32Value codePoint;
  Arcadia_SizeValue codePointByteLength;
  Arcadia_SizeValue codePointByteIndex;

  Arcadia_ByteReader* source;

  Arcadia_SizeValue numberOfBytes;
  Arcadia_Natural8Value bytes[4];
};

Arcadia_ByteReader_UnicodeCodePointReader*
Arcadia_ByteReader_UnicodeCodePointReader_create
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* source
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEREADER_UNICODECODEPOINTREADER_H_INCLUDED
