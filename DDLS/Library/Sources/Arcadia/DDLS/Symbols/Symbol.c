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

#include "Arcadia/DDLS/Symbols/Symbol.h"

#include "Arcadia/DDLS/Symbols/Scope.h"

static void
Arcadia_DDLS_Symbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Symbol* self
  );

static void
Arcadia_DDLS_Symbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolDispatch* self
  );

static void
Arcadia_DDLS_Symbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Symbol* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_Symbol_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_Symbol_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_Symbol_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDLS_Symbol_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_Symbol_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_Symbol_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.Symbol", Arcadia_DDLS_Symbol,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_DDLS_Symbol_typeOperations);

static void
Arcadia_DDLS_Symbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Symbol* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_Symbol);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->kind = Arcadia_ValueStack_getInteger32Value(thread, 1);
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_Symbol);
}

static void
Arcadia_DDLS_Symbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDLS_Symbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Symbol* self
  )
{/*Intentionally empty.*/}
