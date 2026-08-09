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
#include "Arcadia/VPL/Symbols/ConstantScalar.h"

#include "Arcadia/VPL/Symbols/Program.h"

#include <string.h>

static void
Arcadia_VPL_Symbols_ConstantScalar_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantScalar* self
  );

static void
Arcadia_VPL_Symbols_ConstantScalar_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantScalarDispatch* self
  );

static void
Arcadia_VPL_Symbols_ConstantScalar_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantScalar* self
  );

static Arcadia_VPL_ConstantKind
Arcadia_VPL_Symbols_ConstantScalar_getKindImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantScalar* self
  );

static Arcadia_String*
Arcadia_VPL_Symbols_ConstantScalar_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantScalar* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_Symbols_ConstantScalar_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_Symbols_ConstantScalar_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_Symbols_ConstantScalar_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Symbols.ConstantScalar", Arcadia_VPL_Symbols_ConstantScalar,
                         u8"Arcadia.VPL.Symbols.Constant", Arcadia_VPL_Symbols_Constant,
                         &_typeOperations);

static void
Arcadia_VPL_Symbols_ConstantScalar_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantScalar* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Symbols_ConstantScalar);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->name = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_String_getType(thread));
  self->type = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_VPL_Symbols_Symbol_getType(thread));
  self->program = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_Symbols_Program_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_Symbols_ConstantScalar);
}

static void
Arcadia_VPL_Symbols_ConstantScalar_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantScalarDispatch* self
  )
{
  ((Arcadia_VPL_Symbols_ConstantDispatch*)self)->getKind = (Arcadia_VPL_ConstantKind (*)(Arcadia_Thread*, Arcadia_VPL_Symbols_Constant*))&Arcadia_VPL_Symbols_ConstantScalar_getKindImpl;
  ((Arcadia_VPL_Symbols_SymbolDispatch*)self)->getName = (Arcadia_String*(*)(Arcadia_Thread*, Arcadia_VPL_Symbols_Symbol*)) & Arcadia_VPL_Symbols_ConstantScalar_getNameImpl;
}

static void
Arcadia_VPL_Symbols_ConstantScalar_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantScalar* self
  )
{
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
  if (self->type) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->type);
  }
  if (self->program) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->program);
  }
}

static Arcadia_VPL_ConstantKind
Arcadia_VPL_Symbols_ConstantScalar_getKindImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantScalar* self
  )
{ return Arcadia_VPL_ConstantKind_Scalar; }

static Arcadia_String*
Arcadia_VPL_Symbols_ConstantScalar_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantScalar* self
  )
{ return self->name; }

Arcadia_VPL_Symbols_ConstantScalar*
Arcadia_VPL_Symbols_ConstantScalar_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_VPL_Symbols_Symbol* type,
    Arcadia_VPL_Symbols_Program* program
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Symbols_ConstantScalar);
  if (name) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (type) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)type);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (program) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)program);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_VPL_Symbols_ConstantScalar);
}
