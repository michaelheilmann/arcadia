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

#if !defined(ARCADIA_DDLS_SYMBOLS_SYMBOL_H_INCLUDED)
#define ARCADIA_DDLS_SYMBOLS_SYMBOL_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
typedef struct Arcadia_DDLS_Scope Arcadia_DDLS_Scope;

/// A "schema" symbol.
#define Arcadia_DDLS_SymbolKind_Schema (0)

/// A "schema reference" symbol.
#define Arcadia_DDLS_SymbolKind_SchemaReference (1)

/// A "map" symbol.
#define Arcadia_DDLS_SymbolKind_Map (2)

/// A "map entry" symbol.
#define Arcadia_DDLS_SymbolKind_MapEntry (3)

/// A "scalar" symbol.
#define Arcadia_DDLS_SymbolKind_Scalar (4)

/// A "list" symbol.
#define Arcadia_DDLS_SymbolKind_List (5)

/// An "any" symbol.
#define Arcadia_DDLS_SymbolKind_Any (6)

/// A "choice" symbol.
#define Arcadia_DDLS_SymbolKind_Choice (7)

Arcadia_declareObjectType(u8"Arcadia.DDLS.Symbol", Arcadia_DDLS_Symbol,
                          u8"Arcadia.Object");

struct Arcadia_DDLS_SymbolDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_DDLS_Symbol {
  Arcadia_Object parent;
  Arcadia_Natural8Value kind;
};

#endif // ARCADIA_DDLS_SYMBOLS_SYMBOL_H_INCLUDED
