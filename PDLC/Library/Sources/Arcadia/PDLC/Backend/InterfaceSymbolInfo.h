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

#if !defined(ARCADIA_PDLC_BACKEND_INTERFACESYMBOLINFO_H_INCLUDED)
#define ARCADIA_PDLC_BACKEND_INTERFACESYMBOLINFO_H_INCLUDED

#include "Arcadia/PDLC/Backend/SymbolInfo.h"

/// @brief Additional information for an interface symbol.
/// Used by the C backend.
Arcadia_declareObjectType(u8"Arcadia.PDLC.Backend.InterfaceSymbolInfo", Arcadia_PDLC_Backend_InterfaceSymbolInfo,
                          u8"Arcadia.PDLC.Backend.SymbolInfo");

struct Arcadia_PDLC_Backend_InterfaceSymbolInfoDispatch {
  Arcadia_PDLC_Backend_SymbolInfoDispatch _parent;
};

struct Arcadia_PDLC_Backend_InterfaceSymbolInfo {
  Arcadia_PDLC_Backend_SymbolInfo _parent;
  /// @brief The symbol.
  Arcadia_PDLC_Symbol* symbol;

  /// @brief The Cxx name. For example, `Arcadia_String` or `Arcadia_Engine_Input_MouseButtonEvent`.
  Arcadia_String* cxxName;
  /// @brief The uppercase Cxx name. For example, `ARCADIA_STRING` or `ARCADIA_ENGINE_INPUT_MOUSEBUTTONEVENT`.
  Arcadia_String* cxxNameUpperCase;

  /// @brief The path to the Cxx source file.
  /// This is usually `<interface name as path>.c`.
  Arcadia_FilePath* cxxSourceFilePath;
  /// @brief The absolute path to the Cxx source file.
  /// This is `<module directory>/Library/Sources/<cxx source file path>`.
  Arcadia_FilePath* cxxSourceFileAbsolutePath;

  /// @brief The path to the Cxx header file.
  /// This is usually `<interface name as path>.h`.
  Arcadia_FilePath* cxxHeaderFilePath;
  /// @brief The absolute path to the Cxx header file.
  /// This is `<module directory>/Library/Sources/<cxx header file path>`.
  Arcadia_FilePath* cxxHeaderFileAbsolutePath;
};

Arcadia_PDLC_Backend_InterfaceSymbolInfo*
Arcadia_PDLC_Backend_InterfaceSymbolInfo_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  );

#endif // ARCADIA_PDLC_BACKEND_INTERFACESYMBOLINFO_H_INCLUDED
