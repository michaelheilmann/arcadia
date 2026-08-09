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

#if !defined(ARCADIA_DDLS_SYMBOLS_MAPENTRYSYMBOL_H_INCLUDED)
#define ARCADIA_DDLS_SYMBOLS_MAPENTRYSYMBOL_H_INCLUDED

#include "Arcadia/DDLS/Symbols/Symbol.h"

Arcadia_declareObjectType(u8"Arcadia.DDLS.MapEntrySymbol", Arcadia_DDLS_MapEntrySymbol,
                          u8"Arcadia.DDLS.Symbol");

struct Arcadia_DDLS_MapEntrySymbolDispatch {
  Arcadia_DDLS_SymbolDispatch parent;
};

struct Arcadia_DDLS_MapEntrySymbol {
  Arcadia_DDLS_Symbol parent;
  /// #Arcadia_BooleanValue_True if the entry is optional, #Arcadia_BooleanValue_False otherwise.
  /// Default is #Arcadia_BooleanValue_False.
  Arcadia_BooleanValue optional;
  Arcadia_String* entryName;
  Arcadia_DDLS_Symbol* entrySymbol;
};

Arcadia_DDLS_MapEntrySymbol*
Arcadia_DDLS_MapEntrySymbol_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_DDLS_SYMBOLS_MAPENTRYSYMBOL_H_INCLUDED
