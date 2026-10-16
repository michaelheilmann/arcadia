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
#include "Arcadia/ADL/Definitions/UI/ScrollPanelDefinition.h"

#include "Arcadia/ADL/Definitions/UI/ContainerDefinition.h"

static void
Arcadia_ADL_UI_ScrollPanelDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ScrollPanelDefinition* self
  );

static void
Arcadia_ADL_UI_ScrollPanelDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ScrollPanelDefinitionDispatch* self
  );

static void
Arcadia_ADL_UI_ScrollPanelDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ScrollPanelDefinition* self
  );

static void
Arcadia_ADL_UI_ScrollPanelDefinition_initializeContainerDefinitionDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ContainerDefinitionDispatch* self
  );

static Arcadia_List*
Arcadia_ADL_UI_ScrollPanelDefinition_getChildReferencesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_UI_ScrollPanelDefinition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_UI_ScrollPanelDefinition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_UI_ScrollPanelDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.ADL.UI.ScrollPanelDefinition",
    Arcadia_ADL_UI_ScrollPanelDefinition,
    u8"Arcadia.ADL.Definition",
    Arcadia_ADL_Definition,
    &_typeOperations,
    {
      .interfaceType = _Arcadia_ADL_UI_ContainerDefinition_getType(thread),
      .dispatchSize = sizeof(Arcadia_ADL_UI_ContainerDefinitionDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_ADL_UI_ScrollPanelDefinition_initializeContainerDefinitionDispatchImpl
    }
  )

static void
Arcadia_ADL_UI_ScrollPanelDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ScrollPanelDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_UI_ScrollPanelDefinition);
  {
    Arcadia_Value definitions = Arcadia_ValueStack_getValue(thread, 8);
    Arcadia_Value name = Arcadia_ValueStack_getValue(thread, 7);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (8 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->left = Arcadia_ValueStack_getInteger32Value(thread, 6);
  self->bottom = Arcadia_ValueStack_getInteger32Value(thread, 5);
  self->width = Arcadia_ValueStack_getNatural32Value(thread, 4);
  self->height = Arcadia_ValueStack_getNatural32Value(thread, 3);
  self->contentWidth = Arcadia_ValueStack_getNatural32Value(thread, 2);
  self->contentHeight = Arcadia_ValueStack_getNatural32Value(thread, 1);
  self->childReferences = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_ADL_UI_ScrollPanelDefinition);
}

static void
Arcadia_ADL_UI_ScrollPanelDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ScrollPanelDefinition* self
  )
{
  if (self->childReferences) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->childReferences);
  }
}

static void
Arcadia_ADL_UI_ScrollPanelDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ScrollPanelDefinitionDispatch* self
  )
{ }

static void
Arcadia_ADL_UI_ScrollPanelDefinition_initializeContainerDefinitionDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ContainerDefinitionDispatch* self
  )
{
  self->getChildReferences = &Arcadia_ADL_UI_ScrollPanelDefinition_getChildReferencesImpl;
}

static Arcadia_List*
Arcadia_ADL_UI_ScrollPanelDefinition_getChildReferencesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  return ((Arcadia_ADL_UI_ScrollPanelDefinition*)self)->childReferences;
}

Arcadia_ADL_UI_ScrollPanelDefinition*
Arcadia_ADL_UI_ScrollPanelDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height,
    Arcadia_Natural32Value contentWidth,
    Arcadia_Natural32Value contentHeight
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_UI_ScrollPanelDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushNatural32Value(thread, width);
  Arcadia_ValueStack_pushNatural32Value(thread, height);
  Arcadia_ValueStack_pushNatural32Value(thread, contentWidth);
  Arcadia_ValueStack_pushNatural32Value(thread, contentHeight);
  Arcadia_ValueStack_pushNatural8Value(thread, 8);
  _Arcadia_EndCreate(Arcadia_ADL_UI_ScrollPanelDefinition);
}
