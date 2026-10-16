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

#define ARCADIA_VPL_PRIVATE (1)
#include "Arcadia/VPL/IR/DiscardInstruction.h"

static void
Arcadia_VPL_IR_DiscardInstruction_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_DiscardInstruction* self
  );

static void
Arcadia_VPL_IR_DiscardInstruction_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_DiscardInstructionDispatch* self
  );

static void
Arcadia_VPL_IR_DiscardInstruction_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_DiscardInstruction* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_DiscardInstruction_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_DiscardInstruction_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_DiscardInstruction_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.DiscardInstruction", Arcadia_VPL_IR_DiscardInstruction,
                         u8"Arcadia.VPL.IR.Instruction", Arcadia_VPL_IR_Instruction,
                         &_typeOperations);

static void
Arcadia_VPL_IR_DiscardInstruction_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_DiscardInstruction* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_DiscardInstruction);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_InstructionKind_Discard);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_DiscardInstruction);
}

static void
Arcadia_VPL_IR_DiscardInstruction_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_DiscardInstructionDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_DiscardInstruction_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_DiscardInstruction* self
  )
{ }

Arcadia_VPL_IR_DiscardInstruction*
Arcadia_VPL_IR_DiscardInstruction_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_DiscardInstruction);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_VPL_IR_DiscardInstruction);
}