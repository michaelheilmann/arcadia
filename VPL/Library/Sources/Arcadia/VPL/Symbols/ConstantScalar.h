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

#if !defined(ARCADIA_VPL_SYMBOLS_CONSTANTSCALAR_H_INCLUDED)
#define ARCADIA_VPL_SYMBOLS_CONSTANTSCALAR_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Symbols/Constant.h"
typedef struct Arcadia_VPL_Symbols_Program Arcadia_VPL_Symbols_Program;

// A constant scalar.
// The "constant scalar" maps to an "uniform block" of the scalar's name containing a single field called "value".
//
// ```
// 'constant' 'scalar' <name> ':' <type> ';'
// ```
Arcadia_declareObjectType(u8"Arcadia.VPL.Symbols.ConstantScalar", Arcadia_VPL_Symbols_ConstantScalar,
                          u8"Arcadia.VPL.Symbols.Constant");

struct Arcadia_VPL_Symbols_ConstantScalarDispatch {
  Arcadia_VPL_Symbols_ConstantDispatch _parent;
};

struct Arcadia_VPL_Symbols_ConstantScalar {
  Arcadia_VPL_Symbols_Constant _parent;
  Arcadia_String* name;
  Arcadia_VPL_Symbols_Symbol* type;
  /// The program owning this constant scalar.
  Arcadia_VPL_Symbols_Program* program;
};

/// @brief Create a constant scalar.
/// @param thread A pointer to this thread.
/// @param name The name.
/// @param type The type.
/// @param program The program.
Arcadia_VPL_Symbols_ConstantScalar*
Arcadia_VPL_Symbols_ConstantScalar_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_VPL_Symbols_Symbol* type,
    Arcadia_VPL_Symbols_Program* program
  );

#endif // ARCADIA_VPL_SYMBOLS_CONSTANTSCALAR_H_INCLUDED
