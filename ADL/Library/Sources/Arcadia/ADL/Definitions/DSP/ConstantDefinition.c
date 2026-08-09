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

#define ARCADIA_ADL_MODULE (1)
#include "Arcadia/ADL/Definitions/DSP/ConstantDefinition.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/DSP/Include.h"

static void
Arcadia_ADL_ConstantDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ConstantDefinition* self
  );

static void
Arcadia_ADL_ConstantDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ConstantDefinition* self
  );

static void
Arcadia_ADL_ConstantDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ConstantDefinition* self
  );

static void
Arcadia_ADL_ConstantDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ConstantDefinitionDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_ConstantDefinition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_ConstantDefinition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_ConstantDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.ConstantDefinition", Arcadia_ADL_ConstantDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &_typeOperations);

static void
Arcadia_ADL_ConstantDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ConstantDefinition* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_ADL_ConstantDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ConstantDefinition* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_ADL_ConstantDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ConstantDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_ConstantDefinition);
  {
    Arcadia_Value definitions, name;
    definitions = Arcadia_ValueStack_getValue(thread, 3);
    name = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->value = Arcadia_ValueStack_getReal32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_ADL_ConstantDefinition);
}

static void
Arcadia_ADL_ConstantDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ConstantDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = (void (*)(Arcadia_Thread*, Arcadia_ADL_Definition*)) & Arcadia_ADL_ConstantDefinition_linkImpl;
}

Arcadia_ADL_ConstantDefinition*
Arcadia_ADL_ConstantDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Real32Value value
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_ConstantDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushReal32Value(thread, value);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_ADL_ConstantDefinition);
}
