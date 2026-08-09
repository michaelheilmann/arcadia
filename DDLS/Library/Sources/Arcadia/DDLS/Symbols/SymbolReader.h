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

#if !defined(ARCADIA_DDLS_SYMBOLS_SYMBOLREADER_H_INCLUDED)
#define ARCADIA_DDLS_SYMBOLS_SYMBOLREADER_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/DDL/Include.h"
#include "Arcadia/DDLS/Symbols/Symbol.h"

Arcadia_declareObjectType(u8"Arcadia.DDLS.SymbolReader", Arcadia_DDLS_SymbolReader,
                          u8"Arcadia.Object");

struct Arcadia_DDLS_SymbolReaderDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_DDLS_SymbolReader {
  Arcadia_Object parent;

  Arcadia_DDLS_Scope* scope;

  Arcadia_Value ANY;
  Arcadia_Value CHOICE;
  Arcadia_Value CHOICES;
  Arcadia_Value BOOLEAN;
  Arcadia_Value LIST;
  Arcadia_Value MAP;
  Arcadia_Value MAPENTRY;
  Arcadia_Value NUMBER;
  Arcadia_Value SCHEMA;
  Arcadia_Value SCHEMAREFERENCE;
  Arcadia_Value STRING;
  Arcadia_Value VOID;

  Arcadia_Value DEFINITION;
  Arcadia_Value NAME;
  Arcadia_Value TYPE;
  Arcadia_Value ELEMENT;
  Arcadia_Value ENTRY;
  Arcadia_Value ENTRIES;
  Arcadia_Value KIND;

  Arcadia_Languages_StringTable* stringTable;
};

Arcadia_DDLS_SymbolReader*
Arcadia_DDLS_SymbolReader_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* stringTable
  );

Arcadia_DDLS_Symbol*
Arcadia_DDLS_SymbolReader_run
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_Node* source
  );

#endif // ARCADIA_DDLS_SYMBOLS_SYMBOLREADER_H_INCLUDED
