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
#include "Arcadia/VPL/IR/MatrixType.h"

static void
Arcadia_VPL_IR_MatrixType_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_MatrixType* self
  );

static void
Arcadia_VPL_IR_MatrixType_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_MatrixTypeDispatch* self
  );

static void
Arcadia_VPL_IR_MatrixType_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_MatrixType* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_MatrixType_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_MatrixType_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_MatrixType_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.MatrixType", Arcadia_VPL_IR_MatrixType,
                         u8"Arcadia.VPL.IR.Type", Arcadia_VPL_IR_Type,
                         &_typeOperations);

static void
Arcadia_VPL_IR_MatrixType_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_MatrixType* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_MatrixType);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_TypeKind_Matrix);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->elementKind = Arcadia_ValueStack_getNatural8Value(thread, 3);
  self->rows = Arcadia_ValueStack_getNatural8Value(thread, 2);
  self->columns = Arcadia_ValueStack_getNatural8Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_MatrixType);
}

static void
Arcadia_VPL_IR_MatrixType_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_MatrixTypeDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_MatrixType_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_MatrixType* self
  )
{/*Intentionally empty.*/}

Arcadia_VPL_IR_MatrixType*
Arcadia_VPL_IR_MatrixType_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value elementKind,
    Arcadia_Natural8Value rows,
    Arcadia_Natural8Value columns
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_MatrixType);
  Arcadia_ValueStack_pushNatural8Value(thread, elementKind);
  Arcadia_ValueStack_pushNatural8Value(thread, rows);
  Arcadia_ValueStack_pushNatural8Value(thread, columns);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_VPL_IR_MatrixType);
}