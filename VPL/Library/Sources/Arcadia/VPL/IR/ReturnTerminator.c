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
#include "Arcadia/VPL/IR/ReturnTerminator.h"

static void
Arcadia_VPL_IR_ReturnTerminator_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReturnTerminator* self
  );

static void
Arcadia_VPL_IR_ReturnTerminator_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReturnTerminatorDispatch* self
  );

static void
Arcadia_VPL_IR_ReturnTerminator_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReturnTerminator* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_ReturnTerminator_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_ReturnTerminator_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_ReturnTerminator_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.ReturnTerminator", Arcadia_VPL_IR_ReturnTerminator,
                         u8"Arcadia.VPL.IR.Terminator", Arcadia_VPL_IR_Terminator,
                         &_typeOperations);

static void
Arcadia_VPL_IR_ReturnTerminator_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReturnTerminator* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_ReturnTerminator);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_TerminatorKind_Return);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_ValueStack_isObjectReferenceValue(thread, 1)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (NULL == Arcadia_ValueStack_getObjectReferenceValue(thread, 1)) {
    self->value = NULL;
  } else {
    self->value = (Arcadia_VPL_IR_Value*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_IR_Value_getType(thread));
  }
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_ReturnTerminator);
}

static void
Arcadia_VPL_IR_ReturnTerminator_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReturnTerminatorDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_ReturnTerminator_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReturnTerminator* self
  )
{
  if (self->value) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->value);
  }
}

Arcadia_VPL_IR_ReturnTerminator*
Arcadia_VPL_IR_ReturnTerminator_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Value* value
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_ReturnTerminator);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_VPL_IR_ReturnTerminator);
}