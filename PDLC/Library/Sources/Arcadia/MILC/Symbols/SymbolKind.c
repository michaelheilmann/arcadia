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

#define ARCADIA_MILC_PRIVATE (1)
#include "Arcadia/MILC/Symbols/SymbolKind.h"

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
};

Arcadia_defineEnumerationType(u8"Arcadia.MILC.SymbolKind", Arcadia_MILC_SymbolKind,
                              &_typeOperations);

Arcadia_String*
Arcadia_MILC_SymbolKind_toString
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_SymbolKind kind
  )
{
  switch (kind) {
    case Arcadia_MILC_SymbolKind_Class: {
      return Arcadia_String_createFromCxxString(thread, u8"class");
    } break;
    case Arcadia_MILC_SymbolKind_Constructor: {
      return Arcadia_String_createFromCxxString(thread, u8"constructor");
    } break;
    case Arcadia_MILC_SymbolKind_Enumeration: {
      return Arcadia_String_createFromCxxString(thread, u8"enumeration");
    } break;
    case Arcadia_MILC_SymbolKind_EnumerationConstant: {
      return Arcadia_String_createFromCxxString(thread, u8"enumeration constant");
    } break;
    case Arcadia_MILC_SymbolKind_Method: {
      return Arcadia_String_createFromCxxString(thread, u8"method");
    } break;
    case Arcadia_MILC_SymbolKind_Module: {
      return Arcadia_String_createFromCxxString(thread, u8"module");
    } break;
    case Arcadia_MILC_SymbolKind_PrimitiveType: {
      return Arcadia_String_createFromCxxString(thread, u8"primitive type");
    } break;
    case Arcadia_MILC_SymbolKind_Procedure: {
      return Arcadia_String_createFromCxxString(thread, u8"procedure");
    } break;
    case Arcadia_MILC_SymbolKind_Variable: {
      return Arcadia_String_createFromCxxString(thread, u8"field");
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}