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

#if !defined(ARCADIA_MILC_BACKEND_PRIMITIVETYPESYMBOLINFO_H_INCLUDED)
#define ARCADIA_MILC_BACKEND_PRIMITIVETYPESYMBOLINFO_H_INCLUDED

#include "Arcadia/MILC/Backend/SymbolInfo.h"

/// @brief Additional information for a primitive type symbol.
/// Used by the C backend.
Arcadia_declareObjectType(u8"Arcadia.MILC.Backend.PrimitiveTypeSymbolInfo", Arcadia_MILC_Backend_PrimitiveTypeSymbolInfo,
                          u8"Arcadia.MILC.Backend.SymbolInfo");

struct Arcadia_MILC_Backend_PrimitiveTypeSymbolInfoDispatch {
  Arcadia_MILC_Backend_SymbolInfoDispatch _parent;
};

struct Arcadia_MILC_Backend_PrimitiveTypeSymbolInfo {
  Arcadia_MILC_Backend_SymbolInfo _parent;
  /// @brief The symbol.
  Arcadia_MILC_Symbol* symbol;
  /// @brief The Cxx name. For example, `Arcadia_BooleanValue` or `Arcadia_VoidValue`.
  Arcadia_String* cxxName;
  /// @brief The uppercase Cxx name. For example, `ARCADIA_BOOLEANVALUE` or `ARCADIA_VOIDVALUE`.
  Arcadia_String* cxxNameUpperCase;
};

Arcadia_MILC_Backend_PrimitiveTypeSymbolInfo*
Arcadia_MILC_Backend_PrimitiveTypeSymbolInfo_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  );

#endif // ARCADIA_MILC_BACKEND_PRIMITIVETYPESYMBOLINFO_H_INCLUDED
