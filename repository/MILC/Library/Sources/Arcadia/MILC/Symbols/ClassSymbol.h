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

#if !defined(ARCADIA_MILC_SYMBOLS_CLASSSYMBOL_H_INCLUDED)
#define ARCADIA_MILC_SYMBOLS_CLASSSYMBOL_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/MILC/Symbols/Symbol.h"
#include "Arcadia/MILC/AST/Include.h"

/// @code
/// class Arcadia.MILC.ClassSymbol extends Arcadia.MILC.Symbol {
///
///   constructor(name:Arcadia.String)
///
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.ClassSymbol", Arcadia_MILC_ClassSymbol,
                          u8"Arcadia.MILC.Symbol");

struct Arcadia_MILC_ClassSymbolDispatch {
  Arcadia_MILC_SymbolDispatch _parent;
};

struct Arcadia_MILC_ClassSymbol {
  Arcadia_MILC_Symbol _parent;
  /// The abstract syntax tree defining this class symbol if any. A null pointer otherwise.
  Arcadia_MILC_AST_ClassDefinitionNode* ast;
  /// The symbols of the class members of this class in order of appearance.
  Arcadia_List* members;
  /// The scope of this class.
  Arcadia_Languages_Scope* scope;
  /// The resolved parent class symbol if any. A null pointer otherwise.
  Arcadia_MILC_ClassSymbol* parentClassSymbol;
};

Arcadia_MILC_ClassSymbol*
Arcadia_MILC_ClassSymbol_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  );

#endif // ARCADIA_MILC_SYMBOLS_CLASSSYMBOL_H_INCLUDED
