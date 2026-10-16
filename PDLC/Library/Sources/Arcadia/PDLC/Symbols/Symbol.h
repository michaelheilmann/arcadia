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

#if !defined(ARCADIA_PDLC_SYMBOLS_SYMBOL_H_INCLUDED)
#define ARCADIA_PDLC_SYMBOLS_SYMBOL_H_INCLUDED

#include "Arcadia/PDLC/Symbols/SymbolKind.h"
typedef struct Arcadia_PDLC_Completer Arcadia_PDLC_Completer;

/// @code
/// class Arcadia.PDLC.Symbol {
///
///   field kind:Arcadia.PDLC.SymbolKind
///
///   field name:Arcadia.String
///
///   constructor(kind:Arcadia.PDLC.SymbolKind, name:Arcadia.String)
///
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.Symbol", Arcadia_PDLC_Symbol,
                          u8"Arcadia.Object");

struct Arcadia_PDLC_SymbolDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_PDLC_Symbol {
  Arcadia_Object _parent;
  /// The name of this symbol.
  Arcadia_String* name;
  /// The kind of this symbol.
  Arcadia_PDLC_SymbolKind kind;
  /// The enclosing symbol.
  Arcadia_PDLC_Symbol* enclosing;
  /// The completer for this symbol or null.
  Arcadia_PDLC_Completer* completer;
};

Arcadia_PDLC_Symbol*
Arcadia_PDLC_Symbol_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_SymbolKind kind,
    Arcadia_String* name
  );

#endif // ARCADIA_PDLC_SYMBOLS_SYMBOL_H_INCLUDED
