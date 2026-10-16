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
#include "Arcadia/VPL/IR/Block.h"
#include "Arcadia/VPL/IR/Terminator.h"

static void
Arcadia_VPL_IR_Block_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Block* self
  );

static void
Arcadia_VPL_IR_Block_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_BlockDispatch* self
  );

static void
Arcadia_VPL_IR_Block_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Block* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_Block_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_Block_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_Block_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.Block", Arcadia_VPL_IR_Block,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_VPL_IR_Block_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Block* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_Block);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->instructions = (Arcadia_List*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_List_getType(thread));
  if (!Arcadia_ValueStack_isObjectReferenceValue(thread, 1)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (NULL == Arcadia_ValueStack_getObjectReferenceValue(thread, 1)) {
    self->terminator = NULL;
  } else {
    self->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_IR_Terminator_getType(thread));
  }
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_Block);
}

static void
Arcadia_VPL_IR_Block_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_BlockDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_Block_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Block* self
  )
{
  if (self->instructions) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->instructions);
  }
  if (self->terminator) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->terminator);
  }
}

Arcadia_VPL_IR_Block*
Arcadia_VPL_IR_Block_create
  (
    Arcadia_Thread* thread,
    Arcadia_List* instructions,
    Arcadia_VPL_IR_Terminator* terminator
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_Block);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)instructions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)terminator);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_VPL_IR_Block);
}