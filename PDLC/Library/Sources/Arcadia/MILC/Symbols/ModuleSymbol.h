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

#if !defined(ARCADIA_MILC_SYMBOLS_MODULESYMBOL_H_INCLUDED)
#define ARCADIA_MILC_SYMBOLS_MODULESYMBOL_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/MILC/Symbols/Symbol.h"
typedef struct Arcadia_MILC_AST_ModuleDefinitionNode Arcadia_MILC_AST_ModuleDefinitionNode;

/// @code
/// class Arcadia.MILC.ModuleSymbol extends Arcadia.MILC.Symbol {
///
///   constructor(name:Arcadia.String)
///
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.ModuleSymbol", Arcadia_MILC_ModuleSymbol,
                          u8"Arcadia.MILC.Symbol");

struct Arcadia_MILC_ModuleSymbolDispatch {
  Arcadia_MILC_SymbolDispatch _parent;
};

struct Arcadia_MILC_ModuleSymbol {
  Arcadia_MILC_Symbol _parent;
  /// @brief The scope of this module.
  Arcadia_Languages_Scope* scope;
  /// @brief The module definition node of this module symbol.
  Arcadia_MILC_AST_ModuleDefinitionNode* moduleDefinitionNode;
};

Arcadia_MILC_ModuleSymbol*
Arcadia_MILC_ModuleSymbol_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  );

#endif // ARCADIA_MILC_SYMBOLS_MODULESYMBOL_H_INCLUDED
