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
#include "Arcadia/VPL/IR/StageIO.h"

static void
Arcadia_VPL_IR_StageIO_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StageIO* self
  );

static void
Arcadia_VPL_IR_StageIO_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StageIODispatch* self
  );

static void
Arcadia_VPL_IR_StageIO_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StageIO* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_StageIO_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_StageIO_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_StageIO_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.StageIO", Arcadia_VPL_IR_StageIO,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_VPL_IR_StageIO_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StageIO* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_StageIO);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (5 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->direction = Arcadia_ValueStack_getNatural8Value(thread, 5);
  self->name = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _Arcadia_String_getType(thread));
  self->type = (Arcadia_VPL_IR_Type*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_VPL_IR_Type_getType(thread));
  self->location = Arcadia_ValueStack_getInteger32Value(thread, 2);
  self->semantics = Arcadia_ValueStack_getNatural8Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_StageIO);
}

static void
Arcadia_VPL_IR_StageIO_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StageIODispatch* self
  )
{ }

static void
Arcadia_VPL_IR_StageIO_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StageIO* self
  )
{
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
  if (self->type) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->type);
  }
}

Arcadia_VPL_IR_StageIO*
Arcadia_VPL_IR_StageIO_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value direction,
    Arcadia_String* name,
    Arcadia_VPL_IR_Type* type,
    Arcadia_Integer32Value location,
    Arcadia_Natural8Value semantics
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_StageIO);
  Arcadia_ValueStack_pushNatural8Value(thread, direction);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)type);
  Arcadia_ValueStack_pushInteger32Value(thread, location);
  Arcadia_ValueStack_pushNatural8Value(thread, semantics);
  Arcadia_ValueStack_pushNatural8Value(thread, 5);
  _Arcadia_EndCreate(Arcadia_VPL_IR_StageIO);
}