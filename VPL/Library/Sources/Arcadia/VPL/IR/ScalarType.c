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
#include "Arcadia/VPL/IR/ScalarType.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ScalarType* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.ScalarType", Arcadia_VPL_IR_ScalarType,
                         u8"Arcadia.VPL.IR.Type", Arcadia_VPL_IR_Type,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ScalarType* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_ScalarType);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_TypeKind_Scalar);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->elementKind = Arcadia_ValueStack_getNatural8Value(thread, 1);
  if (self->elementKind > Arcadia_VPL_IR_ScalarElementKind_Floating) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_ScalarType);
}

Arcadia_VPL_IR_ScalarType*
Arcadia_VPL_IR_ScalarType_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value elementKind
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_ScalarType);
  Arcadia_ValueStack_pushNatural8Value(thread, elementKind);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_VPL_IR_ScalarType);
}
