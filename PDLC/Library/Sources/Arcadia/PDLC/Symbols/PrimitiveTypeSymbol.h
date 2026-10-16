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

#if !defined(ARCADIA_PDLC_SYMBOLS_PRIMITIVETYPESYMBOL_H_INCLUDED)
#define ARCADIA_PDLC_SYMBOLS_PRIMITIVETYPESYMBOL_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/PDLC/Symbols/Symbol.h"
#include "Arcadia/PDLC/AST/Include.h"

/// @code
/// class Arcadia.PDLC.PrimitiveTypeSymbol extends Arcadia.PDLC.Symbol {
///
///   constructor(name:Arcadia.String)
///
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.PrimitiveTypeSymbol", Arcadia_PDLC_PrimitiveTypeSymbol,
                          u8"Arcadia.PDLC.Symbol");

struct Arcadia_PDLC_PrimitiveTypeSymbolDispatch {
  Arcadia_PDLC_SymbolDispatch _parent;
};

struct Arcadia_PDLC_PrimitiveTypeSymbol {
  Arcadia_PDLC_Symbol _parent;
};

Arcadia_PDLC_PrimitiveTypeSymbol*
Arcadia_PDLC_PrimitiveTypeSymbol_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  );

#endif // ARCADIA_PDLC_SYMBOLS_PRIMITIVETYPESYMBOL_H_INCLUDED
