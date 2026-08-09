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
#include "Arcadia/VPL/Symbols/Procedure.h"

#include "Arcadia/VPL/Symbols/Stage.h"
#include "Arcadia/VPL/Tree/Include.h"

static void
Arcadia_VPL_Symbols_Procedure_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Procedure* self
  );

static void
Arcadia_VPL_Symbols_Procedure_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ProcedureDispatch* self
  );

static void
Arcadia_VPL_Symbols_Procedure_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Procedure* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_Symbols_Procedure_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_Symbols_Procedure_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_Symbols_Procedure_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Symbols.Procedure", Arcadia_VPL_Symbols_Procedure,
                         u8"Arcadia.VPL.Symbols.Symbol", Arcadia_VPL_Symbols_Symbol,
                         &_typeOperations);

static void
Arcadia_VPL_Symbols_Procedure_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Procedure* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Symbols_Procedure);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->name = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_String_getType(thread));
  self->enclosing = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_VPL_Symbols_Symbol_getType(thread));
  self->node = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_Tree_Node_getType(thread));
  self->scope = NULL;
  self->parameters = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->returnValueType = NULL;
  Arcadia_LeaveConstructor(Arcadia_VPL_Symbols_Procedure);
}

static void
Arcadia_VPL_Symbols_Procedure_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ProcedureDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_VPL_Symbols_Procedure_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Procedure* self
  )
{
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
  if (self->enclosing) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enclosing);
  }
  if (self->node) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->node);
  }
  if (self->scope) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->scope);
  }
  if (self->parameters) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->parameters);
  }
  if (self->returnValueType) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->returnValueType);
  }
}

Arcadia_VPL_Symbols_Procedure*
Arcadia_VPL_Symbols_Procedure_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value flags,
    Arcadia_String* name,
    Arcadia_VPL_Symbols_Symbol* enclosing,
    Arcadia_VPL_Tree_Node* node
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Symbols_Procedure);
  if (name) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (enclosing) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)enclosing);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (node) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)node);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_VPL_Symbols_Procedure);
}
