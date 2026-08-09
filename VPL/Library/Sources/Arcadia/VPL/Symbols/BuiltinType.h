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

#if !defined(ARCADIA_VPL_SYMBOLS_BUILTINTYPE_H_INCLUDED)
#define ARCADIA_VPL_SYMBOLS_BUILTINTYPE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Symbols/Symbol.h"

// A member of a constant record.
Arcadia_declareObjectType(u8"Arcadia.VPL.Symbols.BuiltinType", Arcadia_VPL_Symbols_BuiltinType,
                          u8"Arcadia.VPL.Symbols.Symbol");

struct Arcadia_VPL_Symbols_BuiltinTypeDispatch {
  Arcadia_VPL_Symbols_SymbolDispatch _parent;
};

struct Arcadia_VPL_Symbols_BuiltinType {
  Arcadia_VPL_Symbols_Symbol _parent;
  Arcadia_String* name;
  Arcadia_String* type;
  // The scope of this builtin type.
  Arcadia_Languages_Scope* scope;
};

Arcadia_VPL_Symbols_BuiltinType*
Arcadia_VPL_Symbols_BuiltinType_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  );

#endif // ARCADIA_VPL_SYMBOLS_BUILTINTYPE_H_INCLUDED
