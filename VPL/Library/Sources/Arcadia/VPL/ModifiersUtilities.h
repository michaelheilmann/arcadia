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

#if !defined(ARCADIA_VPL_MODIFIERSUTILITIES_H_INCLUDED)
#define ARCADIA_VPL_MODIFIERSUTILITIES_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
#error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Tree/Node.h"
#include "Arcadia/VPL/Symbols/Include.h"

#define Arcadia_Symbols_Modifiers_Constant (1 << 1)
#define Arcadia_Symbols_Modifiers_Fragment (1 << 2)
#define Arcadia_Symbols_Modifiers_Pixel (1 << 3)
#define Arcadia_Symbols_Modifiers_Variable (1 << 4)
#define Arcadia_Symbols_Modifiers_Vertex (1 << 5)

Arcadia_declareObjectType(u8"Arcadia.VPL.ModifierUtilities", Arcadia_VPL_ModifiersUtilities,
                          u8"Arcadia.Object");

struct Arcadia_VPL_ModifiersUtilitiesDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_ModifiersUtilities {
  Arcadia_Object _parent;

  /// @brief A set for asserting that a modifier list does not contain the same modifier twice.
  Arcadia_Set* temporary;

  /// @brief Set of all modifiers.
  Arcadia_Set* ALL;
  // The `builtin` modifier.
  Arcadia_String* BUILTIN;
  // The `constant` modifier.
  Arcadia_String* CONSTANT;
  // The `fragment``modifier.
  Arcadia_String* FRAGMENT;
  // The `pixel``modifier.
  Arcadia_String* PIXEL;
  // The `variable` modifier`.
  Arcadia_String* VARIABLE;
  // The `vertex` modifier.
  Arcadia_String* VERTEX;
  // The `position` modifier.
  Arcadia_String* POSITION;
};

Arcadia_VPL_ModifiersUtilities*
Arcadia_VPL_ModifiersUtilities_create
  (
    Arcadia_Thread* thread
  );

/// @brief
/// Validate a modifier list for a variable scalar (list of Arcadia.String) objects.
/// If not valid, raise a semantical error.
/// Otherwise return a bitmask of Arcadia_Symbols_VariableScalarFlags_* constants.
Arcadia_Natural8Value
Arcadia_VPL_ModifiersUtilities_variableScalarModifiers
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ModifiersUtilities* self,
    Arcadia_List* modifiers
  );

Arcadia_Natural8Value
Arcadia_VPL_ModifiersUtilities_procedureModifiers
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ModifiersUtilities* self,
    Arcadia_List* modifiers
  );

#endif // ARCADIA_VPL_MODIFIERSUTILITIES_H_INCLUDED
