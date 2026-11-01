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

#if !defined(ARCADIA_PDLC_SYMBOLS_SYMBOLS_H_INCLUDED)
#define ARCADIA_PDLC_SYMBOLS_SYMBOLS_H_INCLUDED

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Collections/Include.h"

typedef struct Arcadia_PDLC_Context Arcadia_PDLC_Context;
typedef struct Arcadia_PDLC_Symbol Arcadia_PDLC_Symbol;
typedef struct Arcadia_PDLC_Environment Arcadia_PDLC_Environment;

/// @brief Singleton for predefined symbols - in particular, primitive type symbols.
Arcadia_declareObjectType(u8"Arcadia.PDLC.Symbols", Arcadia_PDLC_Symbols,
                          u8"Arcadia.Object");

struct Arcadia_PDLC_SymbolsDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_PDLC_Symbols {
  Arcadia_Object _parent;

  /// @brief The context.
  Arcadia_PDLC_Context* context;

  /// @brief The built-in type symbols, keyed by their fully qualified type name.
  ///
  /// @remarks The C++ name of a built-in type is derived from its fully qualified type name by
  /// `Arcadia_PDLC_Backend_Implementation_computeCxxName`, so the type name is the only
  /// intrinsic property a built-in type needs. Membership of this registry is what makes a symbol
  /// built-in, so the registry cannot drift out of sync with `Arcadia_PDLC_Symbols_isBuiltIn`.
  Arcadia_Map* builtIns;
  /// Native callable targets supplied by the built-in registry, keyed by member symbol.
  Arcadia_Map* nativeTargets;
};

Arcadia_PDLC_Symbols*
Arcadia_PDLC_Symbols_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  );

Arcadia_PDLC_Symbols*
Arcadia_PDLC_Symbols_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  );

Arcadia_PDLC_Environment*
Arcadia_PDLC_Symbols_getEnvironment
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self,
    Arcadia_PDLC_Symbol* symbol
  );

/// @brief Look up a built-in type symbol by its fully qualified type name.
/// @param name A NUL-terminated UTF-8 encoded fully qualified type name, e.g. `Arcadia.Object`.
/// @return A pointer to the built-in type symbol.
/// @remarks Raises `Arcadia_Status_ArgumentValueInvalid` if @c name is not a built-in type name.
/// This cannot happen for the built-in type names the compiler itself relies on, so a raise
/// indicates a defect in the compiler rather than in the compiled program.
Arcadia_PDLC_Symbol*
Arcadia_PDLC_Symbols_lookupBuiltin
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self,
    const char* name
  );

/// @brief Get the values of the built-in registry.
/// @return A pointer to a list of the built-in type symbols.
/// @remarks The order of the list is unspecified and must not be relied upon: a built-in type is
/// looked up by name, never by position.
Arcadia_List*
Arcadia_PDLC_Symbols_getBuiltIns
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self
  );

/// @brief Get if a symbol is a built-in symbol.
/// @param thread A pointer to this thread.
/// @param self A pointer to this symbols.
/// @return #Arcadia_BooleanValue_True if this symbol is a built-in symbol. #Arcadia_BooleanValue_False otherwise.
/// @remarks Built-in symbols do not have a module. A symbol is built-in if, and only if, it is a
/// member of the built-in registry, so this predicate cannot drift out of sync with the set of
/// symbols the enter phase registers.
Arcadia_BooleanValue
Arcadia_PDLC_Symbols_isBuiltIn
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self,
    Arcadia_PDLC_Symbol* symbol
  );

/// Resolve source annotations and registry-supplied native bindings at one boundary.
/// Returns null for a non-native callable. Native target names are verbatim C identifiers.
Arcadia_String*
Arcadia_PDLC_Symbols_getNativeTarget
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self,
    Arcadia_PDLC_Symbol* symbol
  );

void
Arcadia_PDLC_Symbols_setNativeTarget
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self,
    Arcadia_PDLC_Symbol* symbol,
    Arcadia_String* target
  );

#endif // ARCADIA_PDLC_SYMBOLS_SYMBOLS_H_INCLUDED
