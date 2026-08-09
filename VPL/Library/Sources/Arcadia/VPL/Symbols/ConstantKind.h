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

#if !defined(ARCADIA_VPL_SYMBOLS_CONSTANTKIND_H_INCLUDED)
#define ARCADIA_VPL_SYMBOLS_CONSTANTKIND_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

// An enumeration of kinds of constants.
Arcadia_declareEnumerationType(u8"Arcadia.VPL.Symbols.ConstantKind", Arcadia_VPL_ConstantKind);

enum Arcadia_VPL_ConstantKind {
  Arcadia_VPL_ConstantKind_Record,
  Arcadia_VPL_ConstantKind_Scalar,
};

#endif // ARCADIA_VPL_SYMBOLS_CONSTANTKIND_H_INCLUDED
