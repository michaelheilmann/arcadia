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

#if !defined(ARCADIA_MILC_SYMBOLS_CONSTRUCTORSYMBOL_H_INCLUDED)
#define ARCADIA_MILC_SYMBOLS_CONSTRUCTORSYMBOL_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/MILC/Symbols/Symbol.h"
#include "Arcadia/MILC/AST/Include.h"

/// @code
/// class Arcadia.MILC.ConstructorSymbol extends Arcadia.MILC.Symbol {
///
///   constructor(name:Arcadia.String)
///
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.ConstructorSymbol", Arcadia_MILC_ConstructorSymbol,
                          u8"Arcadia.MILC.Symbol");

struct Arcadia_MILC_ConstructorSymbolDispatch {
  Arcadia_MILC_SymbolDispatch _parent;
};

struct Arcadia_MILC_ConstructorSymbol {
  Arcadia_MILC_Symbol _parent;
  /// The abstract syntax tree defining this constructor symbol if any. A null pointer otherwise.
  Arcadia_MILC_AST_ConstructorDefinitionNode* ast;

  /// The symbols of the parameters of this constructor in order of appearance.
  Arcadia_List* parameters;

  /// The scope for the parameters.
  Arcadia_Languages_Scope* scope;
};

Arcadia_MILC_ConstructorSymbol*
Arcadia_MILC_ConstructorSymbol_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  );

#endif // ARCADIA_MILC_SYMBOLS_CONSTRUCTORSYMBOL_H_INCLUDED
