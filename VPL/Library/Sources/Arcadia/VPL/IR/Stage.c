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
#include "Arcadia/VPL/IR/Stage.h"
#include "Arcadia/VPL/IR/ObjectArguments.h"

static void
Arcadia_VPL_IR_Stage_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Stage* self
  );

static void
Arcadia_VPL_IR_Stage_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StageDispatch* self
  );

static void
Arcadia_VPL_IR_Stage_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Stage* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_Stage_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_Stage_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_Stage_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.Stage", Arcadia_VPL_IR_Stage,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_VPL_IR_Stage_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Stage* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_Stage);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->kind = Arcadia_ValueStack_getNatural8Value(thread, 4);
  self->interface = (Arcadia_List*)Arcadia_VPL_IR_getRequiredObjectArgument(thread, 3, _Arcadia_List_getType(thread));
  self->functions = (Arcadia_List*)Arcadia_VPL_IR_getRequiredObjectArgument(thread, 2, _Arcadia_List_getType(thread));
  self->entryFunction = (Arcadia_VPL_IR_Function*)Arcadia_ValueStack_getObjectReferenceValue(thread, 1);
  if (self->entryFunction) {
    self->entryFunction = (Arcadia_VPL_IR_Function*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_IR_Function_getType(thread));
  }
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_Stage);
}

static void
Arcadia_VPL_IR_Stage_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StageDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_Stage_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Stage* self
  )
{
  if (self->interface) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->interface);
  }
  if (self->functions) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->functions);
  }
  if (self->entryFunction) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->entryFunction);
  }
}

Arcadia_VPL_IR_Stage*
Arcadia_VPL_IR_Stage_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value kind,
    Arcadia_List* interface,
    Arcadia_List* functions
  )
{
  return Arcadia_VPL_IR_Stage_createWithEntryFunction(thread, kind, interface, functions, NULL);
}

Arcadia_VPL_IR_Stage*
Arcadia_VPL_IR_Stage_createWithEntryFunction
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value kind,
    Arcadia_List* interface,
    Arcadia_List* functions,
    Arcadia_VPL_IR_Function* entryFunction
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_Stage);
  Arcadia_ValueStack_pushNatural8Value(thread, kind);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)interface);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)functions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)entryFunction);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_VPL_IR_Stage);
}
