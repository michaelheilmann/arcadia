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

#if !defined(ARCADIA_VPL_SYMBOLS_PROCEDURE_H_INCLUDED)
#define ARCADIA_VPL_SYMBOLS_PROCEDURE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Symbols/Symbol.h"
#include "Arcadia/VPL/Tree/Include.h"
typedef struct Arcadia_VPL_Symbols_Stage Arcadia_VPL_Symbols_Stage;
typedef struct Arcadia_VPL_Symbols_BuiltinType Arcadia_VPL_Symbols_BuiltinType;

// @brief Marks a built-in procedure.
#define Arcadia_VPL_Symbols_ProcedureFlags_Builtin (1 << 1)

// The procedure is in vertex stage scope.
// Can be combined with fragment stage scope and pixel stage scope.
// Cannot be specified if Arcadia_VPL_Symbols_ProcedureFlags_Builtin is specified.
#define Arcadia_VPL_Symbols_ProcedureFlags_VertexStage (1 << 2)

// The procedure is in fragment stage scope.
// Can be combined with vertex stage scope and pixel stage scope.
// Cannot be specified if Arcadia_VPL_Symbols_ProcedureFlags_Builtin is specified.
#define Arcadia_VPL_Symbols_ProcedureFlags_FragmentStage (1 << 3)

// The procedure is in pixel stage scope.
// Can be combined with vertex stage scope and fragment stage scope.
// Cannot be specified if Arcadia_VPL_Symbols_ProcedureFlags_Builtin is specified.
#define Arcadia_VPL_Symbols_ProcedureFlags_PixelStage (1 << 4)

// A procedure.
Arcadia_declareObjectType(u8"Arcadia.VPL.Symbols.Procedure", Arcadia_VPL_Symbols_Procedure,
                          u8"Arcadia.VPL.Symbols.Symbol");

struct Arcadia_VPL_Symbols_ProcedureDispatch {
  Arcadia_VPL_Symbols_SymbolDispatch _parent;
};

struct Arcadia_VPL_Symbols_Procedure {
  Arcadia_VPL_Symbols_Symbol _parent;
  /// The flags.
  Arcadia_Natural8Value flags;
  Arcadia_String* name;
  Arcadia_VPL_Symbols_Symbol* enclosing;
  Arcadia_VPL_Tree_Node* node;
  // The scope.
  Arcadia_Languages_Scope* scope;
  // List of Arcadia.Symbols.Variable representing the parameter list.
  Arcadia_List* parameters;
  // Arcadia.Symbols.BuiltinType representing the return value type.
  Arcadia_VPL_Symbols_BuiltinType* returnValueType;
};

/// @param flags The flags of this procedure.
/// @param name The name of this procedure.
/// @param enclosing The enclosing symbol. Must be a program symbol or a stage symbol.
/// @param node The tree node of the definition of this procedure.
Arcadia_VPL_Symbols_Procedure*
Arcadia_VPL_Symbols_Procedure_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value flags,
    Arcadia_String* name,
    Arcadia_VPL_Symbols_Symbol* enclosing,
    Arcadia_VPL_Tree_Node* node
  );

#endif // ARCADIA_VPL_SYMBOLS_PROCEDURE_H_INCLUDED
