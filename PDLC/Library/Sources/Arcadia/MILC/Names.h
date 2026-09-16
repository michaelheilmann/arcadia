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

#if !defined(ARCADIA_MILC_NAMES_H_INCLUDED)
#define ARCADIA_MILC_NAMES_H_INCLUDED

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Collections/Include.h"

static inline Arcadia_String*
Arcadia_MILC_Names_toTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* names
  )
{ 
  Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
  Arcadia_String* name = (Arcadia_String*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)names, 0, _Arcadia_String_getType(thread));
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, name);
  for (Arcadia_SizeValue i = 1, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)names); i < n; ++i) {
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8".");
    name = (Arcadia_String*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)names, i, _Arcadia_String_getType(thread));
    Arcadia_StringBuilder_insertBackString(thread, stringBuilder, name);
  }
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder));
}

#endif // ARCADIA_MILC_NAMES_H_INCLUDED
