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

#if !defined(ARCADIA_ADL_READER_H_INCLUDED)
#define ARCADIA_ADL_READER_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/DDL/Include.h"
typedef struct Arcadia_ADL_Context Arcadia_ADL_Context;

/* The base of all readers. A reader reads ADL definitions of a type from a DDL node. */
Arcadia_declareObjectType(u8"Arcadia.ADL.Reader", Arcadia_ADL_Reader,
                          u8"Arcadia.Object");

struct Arcadia_ADL_ReaderDispatch {
  Arcadia_ObjectDispatch _parent;
  Arcadia_String* (*getTypeName)(Arcadia_Thread*, Arcadia_ADL_Reader*);
  Arcadia_ADL_Definition* (*read)(Arcadia_Thread*, Arcadia_ADL_Reader*, Arcadia_ADL_Context*, Arcadia_ADL_Definitions*, Arcadia_DDL_Node*);
};

struct Arcadia_ADL_Reader {
  Arcadia_Object _parent;
};

/* Get teh type name supported by this reader. */
Arcadia_String*
Arcadia_ADL_Reader_getTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reader* self
  );

/* Read the ADL definition from a DDL node. */
Arcadia_ADL_Definition*
Arcadia_ADL_Reader_read
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  );

#endif  // ARCADIA_ADL_READER_H_INCLUDED
