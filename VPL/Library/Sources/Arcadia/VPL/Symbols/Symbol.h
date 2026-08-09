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

#if !defined(ARCADIA_VPL_SYMBOLS_SYMBOL_H_INCLUDED)
#define ARCADIA_VPL_SYMBOLS_SYMBOL_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Languages/Include.h"
#include "Arcadia/VPL/Configure.h"

// A constant. Can be either a constant record or a constant scalar.
Arcadia_declareObjectType(u8"Arcadia.VPL.Symbols.Symbol", Arcadia_VPL_Symbols_Symbol,
                          u8"Arcadia.Object");

struct Arcadia_VPL_Symbols_SymbolDispatch {
  Arcadia_ObjectDispatch _parent;
  Arcadia_String* (*getName)(Arcadia_Thread*, Arcadia_VPL_Symbols_Symbol*);
};

struct Arcadia_VPL_Symbols_Symbol {
  Arcadia_Object _parent;
};

/// @brief Get the name of this symbol.
/// @param thread A pointer to this thread.
/// @param self A pointer to this symbol.
/// @return The name of this symbol.
Arcadia_String*
Arcadia_VPL_Symbols_Symbol_getName
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Symbol* self
  );

#endif // ARCADIA_VPL_SYMBOLS_SYMBOL_H_INCLUDED
