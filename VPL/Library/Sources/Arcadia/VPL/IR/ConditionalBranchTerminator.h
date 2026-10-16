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

#if !defined(ARCADIA_VPL_IR_CONDITIONALBRANCHTERMINATOR_H_INCLUDED)
#define ARCADIA_VPL_IR_CONDITIONALBRANCHTERMINATOR_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/IR/Terminator.h"
#include "Arcadia/VPL/IR/Block.h"
#include "Arcadia/VPL/IR/Value.h"

// Terminates a block by branching to one of two blocks depending on a condition.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.ConditionalBranchTerminator", Arcadia_VPL_IR_ConditionalBranchTerminator,
                          u8"Arcadia.VPL.IR.Terminator");

struct Arcadia_VPL_IR_ConditionalBranchTerminatorDispatch {
  Arcadia_VPL_IR_TerminatorDispatch _parent;
};

struct Arcadia_VPL_IR_ConditionalBranchTerminator {
  Arcadia_VPL_IR_Terminator _parent;
  /// The condition.
  Arcadia_VPL_IR_Value* condition;
  /// The block to branch to if the condition is true.
  Arcadia_VPL_IR_Block* trueTarget;
  /// The block to branch to if the condition is false.
  Arcadia_VPL_IR_Block* falseTarget;
};

Arcadia_VPL_IR_ConditionalBranchTerminator*
Arcadia_VPL_IR_ConditionalBranchTerminator_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Value* condition,
    Arcadia_VPL_IR_Block* trueTarget,
    Arcadia_VPL_IR_Block* falseTarget
  );

#endif // ARCADIA_VPL_IR_CONDITIONALBRANCHTERMINATOR_H_INCLUDED