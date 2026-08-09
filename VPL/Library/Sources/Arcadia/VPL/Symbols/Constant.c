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
#include "Arcadia/VPL/Symbols/Constant.h"

#include <string.h>

static void
Arcadia_VPL_Symbols_Constant_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Constant* self
  );

static void
Arcadia_VPL_Symbols_Constant_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantDispatch* self
  );

static void
Arcadia_VPL_Symbols_Constant_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Constant* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_Symbols_Constant_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_Symbols_Constant_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_Symbols_Constant_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Symbols.Constant", Arcadia_VPL_Symbols_Constant,
                         u8"Arcadia.VPL.Symbols.Symbol", Arcadia_VPL_Symbols_Symbol,
                         &_typeOperations);

static void
Arcadia_VPL_Symbols_Constant_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Constant* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Symbols_Constant);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_VPL_Symbols_Constant);
}

static void
Arcadia_VPL_Symbols_Constant_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantDispatch* self
  )
{ }

static void
Arcadia_VPL_Symbols_Constant_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Constant* self
  )
{/*Intentionally empty.*/}

Arcadia_VPL_ConstantKind
Arcadia_VPL_Symbols_Constant_getKind
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Constant* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_VPL_Symbols_Constant, getKind, self); }
