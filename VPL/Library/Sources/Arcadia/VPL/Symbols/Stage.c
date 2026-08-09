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
#include "Arcadia/VPL/Symbols/Stage.h"

#include "Arcadia/VPL/Symbols/Program.h"

static void
Arcadia_VPL_Symbols_Stage_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Stage* self
  );

static void
Arcadia_VPL_Symbols_Stage_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_StageDispatch* self
  );

static void
Arcadia_VPL_Symbols_Stage_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Stage* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_Symbols_Stage_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_Symbols_Stage_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_Symbols_Stage_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Symbols.Stage", Arcadia_VPL_Symbols_Stage,
                         u8"Arcadia.VPL.Symbols.Symbol", Arcadia_VPL_Symbols_Symbol,
                         &_typeOperations);

static void
Arcadia_VPL_Symbols_Stage_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Stage* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Symbols_Stage);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->program = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_VPL_Symbols_Program_getType(thread));
  self->flags = Arcadia_ValueStack_getNatural8Value(thread, 1);
  self->variableScalars = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->scope = NULL;
  Arcadia_LeaveConstructor(Arcadia_VPL_Symbols_Stage);
}

static void
Arcadia_VPL_Symbols_Stage_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_StageDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_VPL_Symbols_Stage_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Stage* self
  )
{
  if (self->program) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->program);
  }
  if (self->variableScalars) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->variableScalars);
  }
  if (self->scope) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->scope);
  }
}

Arcadia_VPL_Symbols_Stage*
Arcadia_VPL_Symbols_Stage_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_Natural8Value flags
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Symbols_Stage);
  if (program) Arcadia_ValueStack_pushObjectReferenceValue(thread, program); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, flags);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_VPL_Symbols_Stage);
}
