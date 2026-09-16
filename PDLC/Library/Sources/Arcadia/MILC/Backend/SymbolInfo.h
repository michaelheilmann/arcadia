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

#if !defined(ARCADIA_MILC_BACKEND_SYMBOLINFO_H_INCLUDED)
#define ARCADIA_MILC_BACKEND_SYMBOLINFO_H_INCLUDED

#include "Arcadia/MILC/Context.h"
#include "Arcadia/MILC/Symbols/Include.h"

/// @brief Additional information for a symbol.
/// Used by the C backend.
Arcadia_declareObjectType(u8"Arcadia.MILC.Backend.SymbolInfo", Arcadia_MILC_Backend_SymbolInfo,
                          u8"Arcadia.Object");

struct Arcadia_MILC_Backend_SymbolInfoDispatch {
  Arcadia_ObjectDispatch _parent;
  void (*dump)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*, Arcadia_SizeValue, Arcadia_StringBuilder*);
  Arcadia_MILC_Symbol* (*getSymbol)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*);
  Arcadia_String* (*getCxxName)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*);
  Arcadia_String* (*getCxxNameUpperCase)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*);

  Arcadia_MILC_Backend_SymbolInfo* (*getModule)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*);
  Arcadia_FilePath* (*getCxxHeaderFilePath)(Arcadia_Thread* thread, Arcadia_MILC_Backend_SymbolInfo* self);
  Arcadia_FilePath* (*getCxxHeaderFileAbsolutePath)(Arcadia_Thread* thread, Arcadia_MILC_Backend_SymbolInfo* self);
  Arcadia_FilePath* (*getCxxSourceFilePath)(Arcadia_Thread* thread, Arcadia_MILC_Backend_SymbolInfo* self);
  Arcadia_FilePath* (*getCxxSourceFileAbsolutePath)(Arcadia_Thread* thread, Arcadia_MILC_Backend_SymbolInfo* self);

};

struct Arcadia_MILC_Backend_SymbolInfo {
  Arcadia_Object _parent;
  /// @brief The symbol.
  Arcadia_MILC_Symbol* symbol;
  /// @brief The context.
  Arcadia_MILC_Context* context;
};

Arcadia_MILC_Backend_SymbolInfo*
Arcadia_MILC_Backend_SymbolInfo_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  );

void
Arcadia_MILC_Backend_SymbolInfo_dump
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_SymbolInfo* self,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* target
  );

/// The symbol.
Arcadia_MILC_Symbol*
Arcadia_MILC_Backend_SymbolInfo_getSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_SymbolInfo* self
  );

/// The Cxx uppercase name, for example, Arcadia_Engine.
Arcadia_String*
Arcadia_MILC_Backend_SymbolInfo_getCxxName
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_SymbolInfo* self
  );

/// The Cxx uppercase name, for example, ARCADIA_ENGINE.
Arcadia_String*
Arcadia_MILC_Backend_SymbolInfo_getCxxNameUpperCase
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_SymbolInfo* self
  );

/// Return the module symbol of this symbol.
/// For a module symbol, this returns the module symbol itself.
Arcadia_MILC_Backend_SymbolInfo*
Arcadia_MILC_Backend_SymbolInfo_getModule
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_SymbolInfo* self
  );

/// For classes, enumerations, procedures, this path is relative to the module source directory.
/// that is `<module source directory>/<relative type path>` path.
/// For module types, this is the `<module source directory>/<module name as path>/Include.c` path.
Arcadia_FilePath*
Arcadia_MILC_Backend_SymbolInfo_getCxxHeaderFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_SymbolInfo* self
  );

Arcadia_FilePath*
Arcadia_MILC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_SymbolInfo* self
  );

/// For classes, enumerations, procedures, this path is relative to the module source directory.
/// that is `<module source directory>/<relative type path>` path.
/// For module types, this is the `<module source directory>//<module name as path>/Include.c` path.
Arcadia_FilePath*
Arcadia_MILC_Backend_SymbolInfo_getCxxSourceFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_SymbolInfo* self
  );

Arcadia_FilePath*
Arcadia_MILC_Backend_SymbolInfo_getCxxSourceFileAbsolutePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_SymbolInfo* self
  );

/// @brief Get if this is a built-in symbol.
/// @param thread A pointer to this thread.
/// @param self A pointer to this symbol info.
/// @return #Arcadia_BooleanValue_True if this symbol is a built-in symbol. #Arcadia_BooleanValue_False otherwise.
/// @remarks Built-in symbols do not have a module.
Arcadia_BooleanValue
Arcadia_MILC_Backend_SymbolInfo_isBuiltIn
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_SymbolInfo* self
  );

#endif // ARCADIA_MILC_BACKEND_SYMBOLINFO_H_INCLUDED
