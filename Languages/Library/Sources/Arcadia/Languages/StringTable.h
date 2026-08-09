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

#if !defined(ARCADIA_LANGUAGES_STRINGTABLE_H_INCLUDED)
#define ARCADIA_LANGUAGES_STRINGTABLE_H_INCLUDED

#if !defined(ARCADIA_LANGUAGES_MODULE)
  #error("do not include directly, include `Arcadia/Languages/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"

// Unmanaged.
typedef struct Arcadia_Languages_StringTable_Node Arcadia_Languages_StringTable_Node;

Arcadia_declareObjectType(u8"Arcadia.Languages.StringTable", Arcadia_Languages_StringTable,
                          u8"Arcadia.Object");

struct Arcadia_Languages_StringTableDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Languages_StringTable {
  Arcadia_Object _parent;
  Arcadia_Languages_StringTable_Node** buckets;
  Arcadia_SizeValue size;
  Arcadia_SizeValue capacity;
};

Arcadia_Languages_StringTable*
Arcadia_Languages_StringTable_getOrCreate
  (
    Arcadia_Thread* thread
  );

Arcadia_String*
Arcadia_Languages_StringTable_getOrCreateString
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self,
    Arcadia_StringBuilder* stringBuffer
  );

Arcadia_String*
Arcadia_Languages_StringTable_getOrCreateStringFromBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self,
    Arcadia_Natural8Value const *bytes,
    Arcadia_SizeValue numberOfBytes
  );

Arcadia_String*
Arcadia_Languages_StringTable_getOrCreateStringFromCxxString
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self,
    const char *string
  );

#endif // ARCADIA_LANGUAGES_STRINGTABLE_H_INCLUDED
