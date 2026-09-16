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

#if !defined(ARCADIA_MILC_SYMBOLS_SYMBOLS_H_INCLUDED)
#define ARCADIA_MILC_SYMBOLS_SYMBOLS_H_INCLUDED

#include "Arcadia/Ring1/Include.h"
typedef struct Arcadia_MILC_Context Arcadia_MILC_Context;
typedef struct Arcadia_MILC_Symbol Arcadia_MILC_Symbol;
typedef struct Arcadia_MILC_Environment Arcadia_MILC_Environment;

/// @brief Singleton for predefined symbols - in particular, primitive type symbols.
Arcadia_declareObjectType(u8"Arcadia.MILC.Symbols", Arcadia_MILC_Symbols,
                          u8"Arcadia.Object");

struct Arcadia_MILC_SymbolsDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_MILC_Symbols {
  Arcadia_Object _parent;

  /// @brief The context.
  Arcadia_MILC_Context* context;
  

  /// @code
  /// Arcadia.Integer16
  /// @endcode
  Arcadia_MILC_Symbol* integer16Symbol;
  /// @code
  /// Arcadia.Integer32
  /// @endcode
  Arcadia_MILC_Symbol* integer32Symbol;
  /// @code
  /// Arcadia.Integer64
  /// @endcode
  Arcadia_MILC_Symbol* integer64Symbol;
  /// @code
  /// Arcadia.Integer8
  /// @endcode
  Arcadia_MILC_Symbol* integer8Symbol;
  

  /// @code
  /// Arcadia.Natural16
  /// @endcode
  Arcadia_MILC_Symbol* natural16Symbol;
  /// @code
  /// Arcadia.Natural32
  /// @endcode
  Arcadia_MILC_Symbol* natural32Symbol;
  /// @code
  /// Arcadia.Natural64
  /// @endcode
  Arcadia_MILC_Symbol* natural64Symbol;
  /// @code
  /// Arcadia.Natural8
  /// @endcode
  Arcadia_MILC_Symbol* natural8Symbol;

  /// @code
  /// Arcadia.Boolean
  /// @endcode
  Arcadia_MILC_Symbol* booleanSymbol;


  /// @code
  /// Arcadia.Object
  /// @endcode
  Arcadia_MILC_Symbol* objectSymbol;

  /// @code
  /// Arcadia.Void
  /// @endcode
  Arcadia_MILC_Symbol* voidSymbol;
};

Arcadia_MILC_Symbols*
Arcadia_MILC_Symbols_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

Arcadia_MILC_Symbols*
Arcadia_MILC_Symbols_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

Arcadia_MILC_Environment*
Arcadia_MILC_Symbols_getEnvironment
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbols* self,
    Arcadia_MILC_Symbol* symbol
  );

/// @brief Get if a symbol is a built-in symbol.
/// @param thread A pointer to this thread.
/// @param self A pointer to this symbols.
/// @return #Arcadia_BooleanValue_True if this symbol is a built-in symbol. #Arcadia_BooleanValue_False otherwise.
/// @remarks Built-in symbols do not have a module.
Arcadia_BooleanValue
Arcadia_MILC_Symbols_isBuiltIn
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbols* self,
    Arcadia_MILC_Symbol* symbol
  );

#endif // ARCADIA_MILC_SYMBOLS_SYMBOLS_H_INCLUDED
