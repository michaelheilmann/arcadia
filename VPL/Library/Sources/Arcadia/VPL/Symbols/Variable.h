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

#if !defined(ARCADIA_VPL_SYMBOLS_VARIABLE_H_INCLUDED)
#define ARCADIA_VPL_SYMBOLS_VARIABLE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Symbols/Symbol.h"
#include "Arcadia/VPL/Tree/Include.h"
typedef struct Arcadia_VPL_Symbols_Program Arcadia_VPL_Symbols_Program;

/// @brief The representation of variables:
/// - local variables in procedures
/// - fields in constant records
/// - parameters of procedures
Arcadia_declareObjectType(u8"Arcadia.VPL.Symbols.Variable", Arcadia_VPL_Symbols_Variable,
                          u8"Arcadia.VPL.Symbols.Symbol");

struct Arcadia_VPL_Symbols_VariableDispatch {
  Arcadia_VPL_Symbols_SymbolDispatch _parent;
};

struct Arcadia_VPL_Symbols_Variable {
  Arcadia_VPL_Symbols_Symbol _parent;
  /// The enclosing symbol.
  Arcadia_VPL_Symbols_Symbol* enclosing;
  /// The name of the variable.
  Arcadia_String* name;
  /// The type of the variable.
  Arcadia_VPL_Symbols_Symbol* type;
  // The tree node.
  Arcadia_VPL_Tree_Node* node;
};

/// @brief Create a variable scalar.
/// @param thread A pointer to this thread.
/// @param name The name of the variable scalar.
/// @param type The type of the variable scalar.
Arcadia_VPL_Symbols_Variable*
Arcadia_VPL_Symbols_Variable_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_VPL_Symbols_Symbol* type,
    Arcadia_VPL_Symbols_Symbol* enclosing,
    Arcadia_VPL_Tree_Node* node
  );

#endif // ARCADIA_VPL_SYMBOLS_VARIABLE_H_INCLUDED
