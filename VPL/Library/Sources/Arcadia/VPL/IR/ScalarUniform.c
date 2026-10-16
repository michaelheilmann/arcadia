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
#include "Arcadia/VPL/IR/ScalarUniform.h"

static void
Arcadia_VPL_IR_ScalarUniform_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ScalarUniform* self
  );

static void
Arcadia_VPL_IR_ScalarUniform_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ScalarUniformDispatch* self
  );

static void
Arcadia_VPL_IR_ScalarUniform_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ScalarUniform* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_ScalarUniform_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_ScalarUniform_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_ScalarUniform_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.ScalarUniform", Arcadia_VPL_IR_ScalarUniform,
                         u8"Arcadia.VPL.IR.Uniform", Arcadia_VPL_IR_Uniform,
                         &_typeOperations);

static void
Arcadia_VPL_IR_ScalarUniform_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ScalarUniform* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_ScalarUniform);
  {
    Arcadia_Object* name = Arcadia_ValueStack_getObjectReferenceValue(thread, 2);
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_UniformKind_Scalar);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->type = (Arcadia_VPL_IR_Type*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_IR_Type_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_ScalarUniform);
}

static void
Arcadia_VPL_IR_ScalarUniform_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ScalarUniformDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_ScalarUniform_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ScalarUniform* self
  )
{
  if (self->type) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->type);
  }
}

Arcadia_VPL_IR_ScalarUniform*
Arcadia_VPL_IR_ScalarUniform_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_VPL_IR_Type* type
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_ScalarUniform);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)type);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_VPL_IR_ScalarUniform);
}