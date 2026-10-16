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

#if !defined(ARCADIA_VPL_IR_BLOCK_H_INCLUDED)
#define ARCADIA_VPL_IR_BLOCK_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"

// Forward declaration, the full definition lives in "Arcadia/VPL/IR/Terminator.h".
typedef struct Arcadia_VPL_IR_Terminator Arcadia_VPL_IR_Terminator;

// A basic block of the IR.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.Block", Arcadia_VPL_IR_Block,
                          u8"Arcadia.Object");

struct Arcadia_VPL_IR_BlockDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_IR_Block {
  Arcadia_Object _parent;
  /// The instructions of this block, a list of Arcadia.VPL.IR.Instruction objects.
  Arcadia_List* instructions;
  /// The terminator of this block, or the null pointer if unset.
  Arcadia_VPL_IR_Terminator* terminator;
};

Arcadia_VPL_IR_Block*
Arcadia_VPL_IR_Block_create
  (
    Arcadia_Thread* thread,
    Arcadia_List* instructions,
    Arcadia_VPL_IR_Terminator* terminator
  );

#endif // ARCADIA_VPL_IR_BLOCK_H_INCLUDED