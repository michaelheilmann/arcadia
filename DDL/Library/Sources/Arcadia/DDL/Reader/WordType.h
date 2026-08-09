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

#if !defined(ARCADIA_DDL_READER_WORDTYPE_H_INCLUDED)
#define ARCADIA_DDL_READER_WORDTYPE_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

Arcadia_declareEnumerationType("Arcadia.DDL.WordType", Arcadia_DDL_WordType);

enum Arcadia_DDL_WordType {

#define Define(Name, Description) \
  Arcadia_DDL_WordType_##Name,

#include "Arcadia/DDL/Reader/WordType.i"

#undef Define

};

#endif // ARCADIA_DDL_READER_WORDTYPE_H_INCLUDED
