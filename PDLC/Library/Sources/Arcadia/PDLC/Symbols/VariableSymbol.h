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

#if !defined(ARCADIA_PDLC_SYMBOLS_VariableSymbol_H_INCLUDED)
#define ARCADIA_PDLC_SYMBOLS_VariableSymbol_H_INCLUDED

#include "Arcadia/PDLC/Symbols/Symbol.h"
#include "Arcadia/PDLC/AST/Include.h"

/// @code
/// class Arcadia.PDLC.VariableSymbol extends Arcadia.PDLC.Symbol {
///
///   constructor(name:Arcadia.String)
///
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.VariableSymbol", Arcadia_PDLC_VariableSymbol,
                          u8"Arcadia.PDLC.Symbol");

struct Arcadia_PDLC_VariableSymbolDispatch {
  Arcadia_PDLC_SymbolDispatch _parent;
};

struct Arcadia_PDLC_VariableSymbol {
  Arcadia_PDLC_Symbol _parent;
  /// The abstract syntax tree defining this enumeration constant symbol if any. A null pointer otherwise.
  Arcadia_PDLC_AST_FieldDefinitionNode* ast;
  /// The type of this field.
  Arcadia_PDLC_Symbol* type;
  Arcadia_BooleanValue isReference;
  Arcadia_BooleanValue isReadOnly;
  Arcadia_BooleanValue isOutput;
  Arcadia_Natural8Value ownership;
  Arcadia_List* recordFields;
  Arcadia_BooleanValue isConstant;
  Arcadia_Integer32Value constantValue;
};

Arcadia_PDLC_VariableSymbol*
Arcadia_PDLC_VariableSymbol_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  );

#endif // ARCADIA_PDLC_SYMBOLS_VariableSymbol_H_INCLUDED
