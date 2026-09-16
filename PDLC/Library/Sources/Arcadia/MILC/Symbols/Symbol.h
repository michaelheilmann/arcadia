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

#if !defined(ARCADIA_MILC_SYMBOLS_SYMBOL_H_INCLUDED)
#define ARCADIA_MILC_SYMBOLS_SYMBOL_H_INCLUDED

#include "Arcadia/MILC/Symbols/SymbolKind.h"
typedef struct Arcadia_MILC_Completer Arcadia_MILC_Completer;

/// @code
/// class Arcadia.MILC.Symbol {
///
///   field kind:Arcadia.MILC.SymbolKind
///
///   field name:Arcadia.String
///
///   constructor(kind:Arcadia.MILC.SymbolKind, name:Arcadia.String)
///
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.Symbol", Arcadia_MILC_Symbol,
                          u8"Arcadia.Object");

struct Arcadia_MILC_SymbolDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_MILC_Symbol {
  Arcadia_Object _parent;
  /// The name of this symbol.
  Arcadia_String* name;
  /// The kind of this symbol.
  Arcadia_MILC_SymbolKind kind;
  /// The enclosing symbol.
  Arcadia_MILC_Symbol* enclosing;
  /// The completer for this symbol or null.
  Arcadia_MILC_Completer* completer;
};

Arcadia_MILC_Symbol*
Arcadia_MILC_Symbol_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_SymbolKind kind,
    Arcadia_String* name
  );

#endif // ARCADIA_MILC_SYMBOLS_SYMBOL_H_INCLUDED
