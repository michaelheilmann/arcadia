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

#if !defined(ARCADIA_LANGUAGES_LINEMAP_H_INCLUDED)
#define ARCADIA_LANGUAGES_LINEMAP_H_INCLUDED

#if !defined(ARCADIA_LANGUAGES_MODULE)
  #error("do not include directly, include `Arcadia/Languages/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Languages.LineMap", Arcadia_Languages_LineMap,
                          u8"Arcadia.Object");

struct Arcadia_Languages_LineMapDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Languages_LineMap {
  Arcadia_Object _parent;
  // Contains the start position of each line.
  // A line includes its line terminators if any.
  Arcadia_List* lineMap;
};

Arcadia_Languages_LineMap*
Arcadia_Languages_LineMap_create
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* contents
  );

Arcadia_SizeValue
Arcadia_Languages_LineMap_getLine
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_LineMap* self,
    Arcadia_SizeValue offset
  );

#endif // ARCADIA_LANGUAGES_LINEMAP_H_INCLUDED
