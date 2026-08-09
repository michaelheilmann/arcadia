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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEARRAYBUILDER_BYTEREADER_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEARRAYBUILDER_BYTEREADER_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Objects/ByteReader.h"
typedef struct Arcadia_ByteArrayBuilder Arcadia_ByteArrayBuilder;

Arcadia_declareObjectType(u8"Arcadia.ByteArrayBuilder.ByteReader", Arcadia_ByteArrayBuilder_ByteReader,
                          u8"Arcadia.ByteReader");

struct Arcadia_ByteArrayBuilder_ByteReaderDispatch {
  Arcadia_ByteReaderDispatch _parent;
};

struct Arcadia_ByteArrayBuilder_ByteReader {
  Arcadia_ByteReader _parent;
  Arcadia_ByteArrayBuilder* source;
  Arcadia_SizeValue index;
};

Arcadia_ByteArrayBuilder_ByteReader*
Arcadia_ByteArrayBuilder_ByteReader_create
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* source
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEARRAYBUILDER_BYTEREADER_H_INCLUDED
