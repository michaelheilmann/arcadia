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

#if !defined(ARCADIA_VPL_SYMBOLS_CONSTANT_H_INCLUDED)
#define ARCADIA_VPL_SYMBOLS_CONSTANT_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Symbols/Symbol.h"
#include "Arcadia/VPL/Symbols/ConstantKind.h"

// A constant. Can be either a constant record or a constant scalar.
Arcadia_declareObjectType(u8"Arcadia.VPL.Symbols.Constant", Arcadia_VPL_Symbols_Constant,
                          u8"Arcadia.VPL.Symbols.Symbol");

struct Arcadia_VPL_Symbols_ConstantDispatch {
  Arcadia_VPL_Symbols_SymbolDispatch _parent;
  Arcadia_VPL_ConstantKind (*getKind)(Arcadia_Thread*, Arcadia_VPL_Symbols_Constant*);
};

struct Arcadia_VPL_Symbols_Constant {
  Arcadia_VPL_Symbols_Symbol _parent;
};

/// @brief Get the kind of this constant.
/// @param thread A pointer to this thread.
/// @param self A pointer to this constant.
/// @return The kind of this constant.
Arcadia_VPL_ConstantKind
Arcadia_VPL_Symbols_Constant_getKind
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Constant* self
  );

#endif // ARCADIA_VPL_SYMBOLS_CONSTANT_H_INCLUDED
