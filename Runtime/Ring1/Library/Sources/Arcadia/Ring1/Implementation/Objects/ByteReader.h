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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEREADER_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEREADER_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Object.h"

Arcadia_declareObjectType(u8"Arcadia.ByteReader", Arcadia_ByteReader,
                          u8"Arcadia.Object");

struct Arcadia_ByteReaderDispatch {
  Arcadia_ObjectDispatch _parent;
  Arcadia_BooleanValue (*hasValue)(Arcadia_Thread*, Arcadia_ByteReader*);
  Arcadia_BooleanValue (*hasError)(Arcadia_Thread*, Arcadia_ByteReader*);
  Arcadia_Natural8Value (*getValue)(Arcadia_Thread*, Arcadia_ByteReader*);
  void (*nextValue)(Arcadia_Thread*, Arcadia_ByteReader*);
};

struct Arcadia_ByteReader {
  Arcadia_Object _parent;
};

Arcadia_BooleanValue
Arcadia_ByteReader_hasValue
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  );

Arcadia_BooleanValue
Arcadia_ByteReader_hasError
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  );

Arcadia_Natural8Value
Arcadia_ByteReader_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  );

void
Arcadia_ByteReader_nextValue
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEREADER_H_INCLUDED
