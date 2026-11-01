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
#include "Arcadia/ADL/Definitions/UI/SegmentedChoiceDefinition.h"

static void
Arcadia_ADL_UI_SegmentedChoiceDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_SegmentedChoiceDefinition* self
  );

static void
Arcadia_ADL_UI_SegmentedChoiceDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_SegmentedChoiceDefinitionDispatch* self
  );

static void
Arcadia_ADL_UI_SegmentedChoiceDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_SegmentedChoiceDefinition* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_UI_SegmentedChoiceDefinition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_UI_SegmentedChoiceDefinition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_UI_SegmentedChoiceDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.UI.SegmentedChoiceDefinition", Arcadia_ADL_UI_SegmentedChoiceDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &_typeOperations);

static void
Arcadia_ADL_UI_SegmentedChoiceDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_SegmentedChoiceDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_UI_SegmentedChoiceDefinition);
  {
     Arcadia_Value definitions = Arcadia_ValueStack_getValue(thread, 13);
     Arcadia_Value name = Arcadia_ValueStack_getValue(thread, 12);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (13 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->left = Arcadia_ValueStack_getInteger32Value(thread, 11);
  self->bottom = Arcadia_ValueStack_getInteger32Value(thread, 10);
  self->width = Arcadia_ValueStack_getNatural32Value(thread, 9);
  self->height = Arcadia_ValueStack_getNatural32Value(thread, 8);
  self->segmentLabels = (Arcadia_List*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 7, _Arcadia_List_getType(thread));
  self->selectedIndex = Arcadia_ValueStack_getNatural32Value(thread, 6);
  self->pixelSize = Arcadia_ValueStack_getNatural32Value(thread, 5);
  self->borderWidth = Arcadia_ValueStack_getNatural32Value(thread, 4);
  self->borderRed = Arcadia_ValueStack_getNatural8Value(thread, 3);
  self->borderGreen = Arcadia_ValueStack_getNatural8Value(thread, 2);
  self->borderBlue = Arcadia_ValueStack_getNatural8Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_ADL_UI_SegmentedChoiceDefinition);
}

static void
Arcadia_ADL_UI_SegmentedChoiceDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_SegmentedChoiceDefinition* self
  )
{
  if (self->segmentLabels) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->segmentLabels);
  }
}

static void
Arcadia_ADL_UI_SegmentedChoiceDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_SegmentedChoiceDefinitionDispatch* self
  )
{ }

Arcadia_ADL_UI_SegmentedChoiceDefinition*
Arcadia_ADL_UI_SegmentedChoiceDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height,
    Arcadia_List* segmentLabels,
    Arcadia_Natural32Value selectedIndex,
    Arcadia_Natural32Value pixelSize,
    Arcadia_Natural32Value borderWidth,
    Arcadia_Natural8Value borderRed,
    Arcadia_Natural8Value borderGreen,
    Arcadia_Natural8Value borderBlue
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_UI_SegmentedChoiceDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushNatural32Value(thread, width);
  Arcadia_ValueStack_pushNatural32Value(thread, height);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)segmentLabels);
  Arcadia_ValueStack_pushNatural32Value(thread, selectedIndex);
  Arcadia_ValueStack_pushNatural32Value(thread, pixelSize);
  Arcadia_ValueStack_pushNatural32Value(thread, borderWidth);
  Arcadia_ValueStack_pushNatural8Value(thread, borderRed);
  Arcadia_ValueStack_pushNatural8Value(thread, borderGreen);
  Arcadia_ValueStack_pushNatural8Value(thread, borderBlue);
  Arcadia_ValueStack_pushNatural8Value(thread, 13);
  _Arcadia_EndCreate(Arcadia_ADL_UI_SegmentedChoiceDefinition);
}
