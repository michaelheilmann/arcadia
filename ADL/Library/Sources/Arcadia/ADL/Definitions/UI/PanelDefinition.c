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
#include "Arcadia/ADL/Definitions/UI/PanelDefinition.h"
#include "Arcadia/ADL/Definitions/UI/ContainerDefinition.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_PanelDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_UI_PanelDefinition);
  if (6 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value definitions = Arcadia_ValueStack_getValue(thread, 6);
  Arcadia_Value name = Arcadia_ValueStack_getValue(thread, 5);
  Arcadia_ValueStack_pushValue(thread, &definitions);
  Arcadia_ValueStack_pushValue(thread, &name);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->left = Arcadia_ValueStack_getInteger32Value(thread, 4);
  self->bottom = Arcadia_ValueStack_getInteger32Value(thread, 3);
  self->width = Arcadia_ValueStack_getNatural32Value(thread, 2);
  self->height = Arcadia_ValueStack_getNatural32Value(thread, 1);
  self->layoutType = NULL;
  self->vertical = Arcadia_BooleanValue_True;
  self->gap = 0;
  self->paddingLeft = self->paddingBottom = 0;
  self->paddingRight = self->paddingTop = 0;
  self->mainAlignment = NULL;
  self->crossAlignment = NULL;
  self->childLayouts = NULL;
  self->childLayoutCount = 0;
  self->childReferences = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_ADL_UI_PanelDefinition);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_PanelDefinition* self
  )
{
  if (self->childLayouts) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->childLayouts);
    self->childLayouts = NULL;
  }
  self->childLayoutCount = 0;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_PanelDefinition* self
  )
{
  if (self->layoutType) Arcadia_Object_visit(thread, (Arcadia_Object*)self->layoutType);
  if (self->mainAlignment) Arcadia_Object_visit(thread, (Arcadia_Object*)self->mainAlignment);
  if (self->crossAlignment) Arcadia_Object_visit(thread, (Arcadia_Object*)self->crossAlignment);
  if (self->childReferences) Arcadia_Object_visit(thread, (Arcadia_Object*)self->childReferences);
}

static Arcadia_List*
getChildReferencesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ return ((Arcadia_ADL_UI_PanelDefinition*)self)->childReferences; }

static void
initializeContainerDefinitionDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ContainerDefinitionDispatch* self
  )
{ self->getChildReferences = &getChildReferencesImpl; }

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.ADL.UI.PanelDefinition", Arcadia_ADL_UI_PanelDefinition,
    u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition, &typeOperations,
    { .interfaceType = _Arcadia_ADL_UI_ContainerDefinition_getType(thread),
      .dispatchSize = sizeof(Arcadia_ADL_UI_ContainerDefinitionDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeContainerDefinitionDispatchImpl }
  )

Arcadia_ADL_UI_PanelDefinition*
Arcadia_ADL_UI_PanelDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_UI_PanelDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushNatural32Value(thread, width);
  Arcadia_ValueStack_pushNatural32Value(thread, height);
  Arcadia_ValueStack_pushNatural8Value(thread, 6);
  _Arcadia_EndCreate(Arcadia_ADL_UI_PanelDefinition);
}
