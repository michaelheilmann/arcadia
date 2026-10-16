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

#if !defined(ARCADIA_VPL_IR_TERMINATOR_H_INCLUDED)
#define ARCADIA_VPL_IR_TERMINATOR_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

// The kinds of terminators.
#define Arcadia_VPL_IR_TerminatorKind_Branch (0)
#define Arcadia_VPL_IR_TerminatorKind_ConditionalBranch (1)
#define Arcadia_VPL_IR_TerminatorKind_Return (2)
#define Arcadia_VPL_IR_TerminatorKind_Discard (3)

// The base type of all terminators of a basic block.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.Terminator", Arcadia_VPL_IR_Terminator,
                          u8"Arcadia.Object");

struct Arcadia_VPL_IR_TerminatorDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_IR_Terminator {
  Arcadia_Object _parent;
  /// The kind of this terminator, one of Arcadia_VPL_IR_TerminatorKind_*.
  Arcadia_Natural8Value kind;
};

#endif // ARCADIA_VPL_IR_TERMINATOR_H_INCLUDED
