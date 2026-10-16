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
#include "Arcadia/ADL/Definitions/UI/ToggleDefinition.h"

static void
Arcadia_ADL_UI_ToggleDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ToggleDefinition* self
  );

static void
Arcadia_ADL_UI_ToggleDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ToggleDefinitionDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_UI_ToggleDefinition_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_UI_ToggleDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.UI.ToggleDefinition", Arcadia_ADL_UI_ToggleDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &_typeOperations);

static void
Arcadia_ADL_UI_ToggleDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ToggleDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_UI_ToggleDefinition);
  {
    Arcadia_Value definitions = Arcadia_ValueStack_getValue(thread, 7);
    Arcadia_Value name = Arcadia_ValueStack_getValue(thread, 6);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (7 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->left = Arcadia_ValueStack_getInteger32Value(thread, 5);
  self->bottom = Arcadia_ValueStack_getInteger32Value(thread, 4);
  self->width = Arcadia_ValueStack_getNatural32Value(thread, 3);
  self->height = Arcadia_ValueStack_getNatural32Value(thread, 2);
  self->checked = Arcadia_ValueStack_getBooleanValue(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_ADL_UI_ToggleDefinition);
}

static void
Arcadia_ADL_UI_ToggleDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ToggleDefinitionDispatch* self
  )
{ }

Arcadia_ADL_UI_ToggleDefinition*
Arcadia_ADL_UI_ToggleDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height,
    Arcadia_BooleanValue checked
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_UI_ToggleDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushNatural32Value(thread, width);
  Arcadia_ValueStack_pushNatural32Value(thread, height);
  Arcadia_ValueStack_pushBooleanValue(thread, checked);
  Arcadia_ValueStack_pushNatural8Value(thread, 7);
  _Arcadia_EndCreate(Arcadia_ADL_UI_ToggleDefinition);
}
