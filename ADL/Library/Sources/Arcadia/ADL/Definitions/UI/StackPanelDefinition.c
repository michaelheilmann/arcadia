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

#define ARCADIA_ADL_PRIVATE (1)
#include "Arcadia/ADL/Definitions/UI/StackPanelDefinition.h"

#include "Arcadia/ADL/Definitions/UI/ContainerDefinition.h"

static void
Arcadia_ADL_UI_StackPanelDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_StackPanelDefinition* self
  );

static void
Arcadia_ADL_UI_StackPanelDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_StackPanelDefinitionDispatch* self
  );

static void
Arcadia_ADL_UI_StackPanelDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_StackPanelDefinition* self
  );

static void
Arcadia_ADL_UI_StackPanelDefinition_initializeContainerDefinitionDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ContainerDefinitionDispatch* self
  );

static Arcadia_List*
Arcadia_ADL_UI_StackPanelDefinition_getChildReferencesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_UI_StackPanelDefinition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_UI_StackPanelDefinition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_UI_StackPanelDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.ADL.UI.StackPanelDefinition",
    Arcadia_ADL_UI_StackPanelDefinition,
    u8"Arcadia.ADL.Definition",
    Arcadia_ADL_Definition,
    &_typeOperations,
    {
      .interfaceType = _Arcadia_ADL_UI_ContainerDefinition_getType(thread),
      .dispatchSize = sizeof(Arcadia_ADL_UI_ContainerDefinitionDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_ADL_UI_StackPanelDefinition_initializeContainerDefinitionDispatchImpl
    }
  )

static void
Arcadia_ADL_UI_StackPanelDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_StackPanelDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_UI_StackPanelDefinition);
  {
    Arcadia_Value definitions = Arcadia_ValueStack_getValue(thread, 16);
    Arcadia_Value name = Arcadia_ValueStack_getValue(thread, 15);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (16 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->left = Arcadia_ValueStack_getInteger32Value(thread, 14);
  self->bottom = Arcadia_ValueStack_getInteger32Value(thread, 13);
  self->width = Arcadia_ValueStack_getNatural32Value(thread, 12);
  self->height = Arcadia_ValueStack_getNatural32Value(thread, 11);
  self->vertical = Arcadia_ValueStack_getBooleanValue(thread, 10);
  self->gap = Arcadia_ValueStack_getInteger32Value(thread, 9);
  self->paddingLeft = Arcadia_ValueStack_getInteger32Value(thread, 8);
  self->paddingBottom = Arcadia_ValueStack_getInteger32Value(thread, 7);
  self->paddingRight = Arcadia_ValueStack_getInteger32Value(thread, 6);
  self->paddingTop = Arcadia_ValueStack_getInteger32Value(thread, 5);
  self->mainAlignment = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _Arcadia_String_getType(thread));
  self->crossAlignment = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_String_getType(thread));
  self->backgroundModelName = Arcadia_ValueStack_isVoidValue(thread, 2) ? NULL :
    Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  self->backgroundBorderWidth = Arcadia_ValueStack_getNatural32Value(thread, 1);
  self->childReferences = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_ADL_UI_StackPanelDefinition);
}

static void
Arcadia_ADL_UI_StackPanelDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_StackPanelDefinition* self
  )
{
  if (self->mainAlignment) Arcadia_Object_visit(thread, (Arcadia_Object*)self->mainAlignment);
  if (self->crossAlignment) Arcadia_Object_visit(thread, (Arcadia_Object*)self->crossAlignment);
  if (self->backgroundModelName) Arcadia_Object_visit(thread, (Arcadia_Object*)self->backgroundModelName);
  if (self->childReferences) Arcadia_Object_visit(thread, (Arcadia_Object*)self->childReferences);
}

static void
Arcadia_ADL_UI_StackPanelDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_StackPanelDefinitionDispatch* self
  )
{ }

static void
Arcadia_ADL_UI_StackPanelDefinition_initializeContainerDefinitionDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ContainerDefinitionDispatch* self
  )
{
  self->getChildReferences = &Arcadia_ADL_UI_StackPanelDefinition_getChildReferencesImpl;
}

static Arcadia_List*
Arcadia_ADL_UI_StackPanelDefinition_getChildReferencesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  return ((Arcadia_ADL_UI_StackPanelDefinition*)self)->childReferences;
}

Arcadia_ADL_UI_StackPanelDefinition*
Arcadia_ADL_UI_StackPanelDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height,
    Arcadia_BooleanValue vertical,
    Arcadia_Integer32Value gap,
    Arcadia_Integer32Value paddingLeft,
    Arcadia_Integer32Value paddingBottom,
    Arcadia_Integer32Value paddingRight,
    Arcadia_Integer32Value paddingTop,
    Arcadia_String* mainAlignment,
    Arcadia_String* crossAlignment,
    Arcadia_String* backgroundModelName,
    Arcadia_Natural32Value backgroundBorderWidth
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_UI_StackPanelDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushNatural32Value(thread, width);
  Arcadia_ValueStack_pushNatural32Value(thread, height);
  Arcadia_ValueStack_pushBooleanValue(thread, vertical);
  Arcadia_ValueStack_pushInteger32Value(thread, gap);
  Arcadia_ValueStack_pushInteger32Value(thread, paddingLeft);
  Arcadia_ValueStack_pushInteger32Value(thread, paddingBottom);
  Arcadia_ValueStack_pushInteger32Value(thread, paddingRight);
  Arcadia_ValueStack_pushInteger32Value(thread, paddingTop);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)mainAlignment);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)crossAlignment);
  if (backgroundModelName) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)backgroundModelName); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural32Value(thread, backgroundBorderWidth);
  Arcadia_ValueStack_pushNatural8Value(thread, 16);
  _Arcadia_EndCreate(Arcadia_ADL_UI_StackPanelDefinition);
}
