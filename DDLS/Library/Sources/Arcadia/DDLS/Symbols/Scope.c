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

#include "Arcadia/DDLS/Symbols/Scope.h"

static void
Arcadia_DDLS_Scope_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Scope* self
  );

static void
Arcadia_DDLS_Scope_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ScopeDispatch* self
  );

static void
Arcadia_DDLS_Scope_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Scope* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_Scope_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_Scope_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_Scope_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDLS_Scope_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_Scope_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_Scope_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.Scope", Arcadia_DDLS_Scope,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_DDLS_Scope_typeOperations);

static void
Arcadia_DDLS_Scope_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Scope* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_Scope);
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
  Arcadia_Value v = Arcadia_ValueStack_getValue(thread, 1);
  if (Arcadia_Value_isInstanceOf(thread, &v, _Arcadia_DDLS_Scope_getType(thread))) {
    self->enclosing = (Arcadia_DDLS_Scope*)Arcadia_Value_getObjectReferenceValue(&v);
  } else if (Arcadia_Value_isInstanceOf(thread, &v, _Arcadia_VoidValue_getType(thread))) {
    self->enclosing = NULL;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->symbols = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_Scope);
}

static void
Arcadia_DDLS_Scope_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ScopeDispatch* self
  )
{ }

static void
Arcadia_DDLS_Scope_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Scope* self
  )
{
  if (self->symbols) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->symbols);
  }
  if (self->enclosing) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enclosing);
  }
}

Arcadia_DDLS_Scope*
Arcadia_DDLS_Scope_create
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Scope* enclosing
  )
{
  _Arcadia_BeginCreate(Arcadia_DDLS_Scope);
  if (enclosing) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)enclosing);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDLS_Scope);
}
