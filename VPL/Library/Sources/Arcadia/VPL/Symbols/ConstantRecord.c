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
#include "Arcadia/VPL/Symbols/ConstantRecord.h"

#include "Arcadia/VPL/Symbols/Program.h"
#include <string.h>

static void
Arcadia_VPL_Symbols_ConstantRecord_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecord* self
  );

static void
Arcadia_VPL_Symbols_ConstantRecord_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecordDispatch* self
  );

static void
Arcadia_VPL_Symbols_ConstantRecord_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecord* self
  );

static Arcadia_VPL_ConstantKind
Arcadia_VPL_Symbols_ConstantRecord_getKindImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecord* self
  );

static Arcadia_String*
Arcadia_VPL_Symbols_ConstantRecord_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecord* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_Symbols_ConstantRecord_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_Symbols_ConstantRecord_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_Symbols_ConstantRecord_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Symbols.ConstantRecord", Arcadia_VPL_Symbols_ConstantRecord,
                         u8"Arcadia.VPL.Symbols.Constant", Arcadia_VPL_Symbols_Constant,
                         &_typeOperations);

static void
Arcadia_VPL_Symbols_ConstantRecord_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecord* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Symbols_ConstantRecord);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->name = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  self->program = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_Symbols_Program_getType(thread));
  self->node = NULL;
  self->fields = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->mappedName = NULL;
  self->scope = NULL;
  Arcadia_LeaveConstructor(Arcadia_VPL_Symbols_ConstantRecord);
}

static void
Arcadia_VPL_Symbols_ConstantRecord_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecordDispatch* self
  )
{
  ((Arcadia_VPL_Symbols_ConstantDispatch*)self)->getKind = (Arcadia_VPL_ConstantKind (*)(Arcadia_Thread*, Arcadia_VPL_Symbols_Constant*))&Arcadia_VPL_Symbols_ConstantRecord_getKindImpl;
  ((Arcadia_VPL_Symbols_SymbolDispatch*)self)->getName = (Arcadia_String*(*)(Arcadia_Thread*, Arcadia_VPL_Symbols_Symbol*)) & Arcadia_VPL_Symbols_ConstantRecord_getNameImpl;
}

static void
Arcadia_VPL_Symbols_ConstantRecord_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecord* self
  )
{
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
  if (self->program) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->program);
  }
  if (self->node) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->node);
  }

  if (self->fields) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->fields);
  }

  if (self->mappedName) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->mappedName);
  }
  if (self->scope) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->fields);
  }
}

static Arcadia_VPL_ConstantKind
Arcadia_VPL_Symbols_ConstantRecord_getKindImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecord* self
  )
{ return Arcadia_VPL_ConstantKind_Record; }

static Arcadia_String*
Arcadia_VPL_Symbols_ConstantRecord_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecord* self
  )
{ return self->name; }

Arcadia_VPL_Symbols_ConstantRecord*
Arcadia_VPL_Symbols_ConstantRecord_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_VPL_Tree_Node* node
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Symbols_ConstantRecord);
  if (name) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (program) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)program);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_VPL_Symbols_ConstantRecord);
}

Arcadia_List*
Arcadia_VPL_Symbols_ConstantRecord_getFields
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecord* self
  )
{ return self->fields; }
