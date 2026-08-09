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

#if !defined(ARCADIA_MILC_BACKEND_ENUMERATIONCONSTANTSYMBOLINFO_H_INCLUDED)
#define ARCADIA_MILC_BACKEND_ENUMERATIONCONSTANTSYMBOLINFO_H_INCLUDED

#include "Arcadia/MILC/Context.h"
#include "Arcadia/MILC/Backend/SymbolInfo.h"

/// @brief Additional information for an enumeration constant symbol.
/// Used by the C backend.
Arcadia_declareObjectType(u8"Arcadia.MILC.Backend.EnumerationConstantSymbolInfo", Arcadia_MILC_Backend_EnumerationConstantSymbolInfo,
                          u8"Arcadia.MILC.Backend.SymbolInfo");

struct Arcadia_MILC_Backend_EnumerationConstantSymbolInfoDispatch {
  Arcadia_MILC_Backend_SymbolInfoDispatch _parent;
};

struct Arcadia_MILC_Backend_EnumerationConstantSymbolInfo {
  Arcadia_MILC_Backend_SymbolInfo _parent;
  /// @brief The symbol.
  Arcadia_MILC_Symbol* symbol;
  /// @brief The Cxx name. For example, `Arcadia_Engine_Visuals_CullMode_Back` or `Arcadia_Engine_Input_MouseButtonAction_Pressed`.
  Arcadia_String* cxxName;
  /// @brief The uppercase Cxx name. For example, `ARCADIA_ENGINE_VISUALS_CULLMODE_BACK` or `ARCADIA_ENGINE_INPUT_MOUSEBUTTONACTION_PRESSED`.
  Arcadia_String* cxxNameUpperCase;
};

Arcadia_MILC_Backend_EnumerationConstantSymbolInfo*
Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  );

#endif // ARCADIA_MILC_BACKEND_ENUMERATIONCONSTANTSYMBOLINFO_H_INCLUDED
