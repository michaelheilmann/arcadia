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
#include "Arcadia/ADL/Definitions/UI/LabelDefinition.h"

static void
Arcadia_ADL_UI_LabelDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_LabelDefinition* self
  );

static void
Arcadia_ADL_UI_LabelDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_LabelDefinitionDispatch* self
  );

static void
Arcadia_ADL_UI_LabelDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_LabelDefinition* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_UI_LabelDefinition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_UI_LabelDefinition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_UI_LabelDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.UI.LabelDefinition", Arcadia_ADL_UI_LabelDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &_typeOperations);

static void
Arcadia_ADL_UI_LabelDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_LabelDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_UI_LabelDefinition);
  {
    Arcadia_Value definitions = Arcadia_ValueStack_getValue(thread, 14);
    Arcadia_Value name = Arcadia_ValueStack_getValue(thread, 13);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (14 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->left = Arcadia_ValueStack_getInteger32Value(thread, 12);
  self->bottom = Arcadia_ValueStack_getInteger32Value(thread, 11);
  self->width = Arcadia_ValueStack_getNatural32Value(thread, 10);
  self->height = Arcadia_ValueStack_getNatural32Value(thread, 9);
  self->text = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 8, _Arcadia_String_getType(thread));
  self->horizontalAlignment = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 7, _Arcadia_String_getType(thread));
  self->verticalAlignment = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 6, _Arcadia_String_getType(thread));
  self->red = Arcadia_ValueStack_getNatural8Value(thread, 5);
  self->green = Arcadia_ValueStack_getNatural8Value(thread, 4);
  self->blue = Arcadia_ValueStack_getNatural8Value(thread, 3);
  self->alpha = Arcadia_ValueStack_getNatural8Value(thread, 2);
  self->pixelSize = Arcadia_ValueStack_getNatural32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_ADL_UI_LabelDefinition);
}

static void
Arcadia_ADL_UI_LabelDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_LabelDefinition* self
  )
{
  if (self->text) Arcadia_Object_visit(thread, (Arcadia_Object*)self->text);
  if (self->horizontalAlignment) Arcadia_Object_visit(thread, (Arcadia_Object*)self->horizontalAlignment);
  if (self->verticalAlignment) Arcadia_Object_visit(thread, (Arcadia_Object*)self->verticalAlignment);
}

static void
Arcadia_ADL_UI_LabelDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_LabelDefinitionDispatch* self
  )
{ }

Arcadia_ADL_UI_LabelDefinition*
Arcadia_ADL_UI_LabelDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height,
    Arcadia_String* text,
    Arcadia_String* horizontalAlignment,
    Arcadia_String* verticalAlignment,
    Arcadia_Natural8Value red,
    Arcadia_Natural8Value green,
    Arcadia_Natural8Value blue,
    Arcadia_Natural8Value alpha,
    Arcadia_Natural32Value pixelSize
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_UI_LabelDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushNatural32Value(thread, width);
  Arcadia_ValueStack_pushNatural32Value(thread, height);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)horizontalAlignment);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)verticalAlignment);
  Arcadia_ValueStack_pushNatural8Value(thread, red);
  Arcadia_ValueStack_pushNatural8Value(thread, green);
  Arcadia_ValueStack_pushNatural8Value(thread, blue);
  Arcadia_ValueStack_pushNatural8Value(thread, alpha);
  Arcadia_ValueStack_pushNatural32Value(thread, pixelSize);
  Arcadia_ValueStack_pushNatural8Value(thread, 14);
  _Arcadia_EndCreate(Arcadia_ADL_UI_LabelDefinition);
}
