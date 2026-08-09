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

#if !defined(ARCADIA_VPL_SYMBOLS_STAGE_H_INCLUDED)
#define ARCADIA_VPL_SYMBOLS_STAGE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Symbols/Symbol.h"
typedef struct Arcadia_VPL_Symbols_Program Arcadia_VPL_Symbols_Program;
typedef struct Arcadia_VPL_Symbols_Procedure Arcadia_VPL_Symbols_Procedure;

/// Symbolic constant for a flag of Arcadia_VPL_Symbols_Stage.
#define Arcadia_VPL_Symbols_StageFlags_Vertex (1)

/// Symbolic constant for a flag of Arcadia_VPL_Symbols_Stage.
#define Arcadia_VPL_Symbols_StageFlags_Fragment (2)

/// Represents the vertex stage or the fragment stage.
/// There must be exactly two stages in a program, a vertex stage and a fragment stage.
Arcadia_declareObjectType(u8"Arcadia.VPL.Symbols.Stage", Arcadia_VPL_Symbols_Stage,
                          u8"Arcadia.VPL.Symbols.Symbol");

struct Arcadia_VPL_Symbols_StageDispatch {
  Arcadia_VPL_Symbols_SymbolDispatch _parent;
};

struct Arcadia_VPL_Symbols_Stage {
  Arcadia_VPL_Symbols_Symbol _parent;
  Arcadia_VPL_Symbols_Program* program;
  Arcadia_Natural8Value flags;

  // The list of variable scalars of this program.
  // This list is initially empty.
  Arcadia_List* variableScalars;

  /// The stage scope.
  Arcadia_Languages_Scope* scope;
};

/// @param program The program owning the stage.
/// @param flags Either Arcadia_VPL_Symbols_StageFlags_Vertex or Arcadia_VPL_Symbols_StageFlags_Fragment.
Arcadia_VPL_Symbols_Stage*
Arcadia_VPL_Symbols_Stage_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_Natural8Value flags
  );

#endif  // ARCADIA_VPL_SYMBOLS_STAGE_H_INCLUDED
