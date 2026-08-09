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

#if !defined(ARCADIA_MILC_BACKEND_ENUMERATIONSYMBOLINFO_H_INCLUDED)
#define ARCADIA_MILC_BACKEND_ENUMERATIONSYMBOLINFO_H_INCLUDED

#include "Arcadia/MILC/Backend/SymbolInfo.h"

/// @brief Additional information for an enumeration symbol.
/// Used by the C backend.
Arcadia_declareObjectType(u8"Arcadia.MILC.Backend.EnumerationSymbolInfo", Arcadia_MILC_Backend_EnumerationSymbolInfo,
                          u8"Arcadia.MILC.Backend.SymbolInfo");

struct Arcadia_MILC_Backend_EnumerationSymbolInfoDispatch {
  Arcadia_MILC_Backend_SymbolInfoDispatch _parent;
};

struct Arcadia_MILC_Backend_EnumerationSymbolInfo {
  Arcadia_MILC_Backend_SymbolInfo _parent;
  /// @brief The symbol.
  Arcadia_MILC_Symbol* symbol;
  /// @brief The Cxx name. For example, `Arcadia_Engine_Visuals_CullMode` or `Arcadia_Engine_Input_MouseButtonAction`.
  Arcadia_String* cxxName;
  /// @brief The uppercase Cxx name. For example, `ARCADIA_ENGINE_VISUALS_CULLMODE` or `ARCADIA_ENGINE_INPUT_MOUSEBUTTONACTION`.
  Arcadia_String* cxxNameUpperCase;

  /// @brief The path to the Cxx source file.
  /// This is usually `<enumeration name as path>.c`.
  Arcadia_FilePath* cxxSourceFilePath;
  /// @brief The absolute path to the Cxx source file.
  /// This is `<module directory>/Library/Sources/<cxx source file path>`.
  Arcadia_FilePath* cxxSourceFileAbsolutePath;

  /// @brief The path to the Cxx header file.
  /// This is usually `<enmumeration name as path>.h`.
  Arcadia_FilePath* cxxHeaderFilePath;
  /// @brief The absolute path to the Cxx header file.
  /// This is `<module directory>/Library/Sources/<cxx header file path>`.
  Arcadia_FilePath* cxxHeaderFileAbsolutePath;
};

Arcadia_MILC_Backend_EnumerationSymbolInfo*
Arcadia_MILC_Backend_EnumerationSymbolInfo_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  );

#endif // ARCADIA_MILC_BACKEND_ENUMERATIONSYMBOLINFO_H_INCLUDED
