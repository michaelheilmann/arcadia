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
#include "Arcadia/ADL/Definitions/UI/ButtonDefinition.h"

#include "Arcadia/ADL/Definitions/Visuals/ColorDefinition.h"
#include "Arcadia/ADL/Reference.h"

static void Arcadia_ADL_UI_ButtonDefinition_linkImpl(Arcadia_Thread* thread, Arcadia_ADL_UI_ButtonDefinition* self);
static void Arcadia_ADL_UI_ButtonDefinition_constructImpl(Arcadia_Thread* thread, Arcadia_ADL_UI_ButtonDefinition* self);
static void Arcadia_ADL_UI_ButtonDefinition_initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_ADL_UI_ButtonDefinitionDispatch* self);
static void Arcadia_ADL_UI_ButtonDefinition_visitImpl(Arcadia_Thread* thread, Arcadia_ADL_UI_ButtonDefinition* self);

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_UI_ButtonDefinition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_UI_ButtonDefinition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_UI_ButtonDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.UI.ButtonDefinition", Arcadia_ADL_UI_ButtonDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &_typeOperations);

static void
linkColorReference
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* reference
  )
{
  if (!reference) return;
  Arcadia_ADL_Reference_resolve(thread, reference);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)reference->definition, _Arcadia_ADL_ColorDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_ADL_UI_ButtonDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonDefinition* self
  )
{
  linkColorReference(thread, self->backgroundColor);
  linkColorReference(thread, self->hoveredBackgroundColor);
  linkColorReference(thread, self->pressedBackgroundColor);
  linkColorReference(thread, self->disabledBackgroundColor);
  linkColorReference(thread, self->focusedBackgroundColor);
}

static void
Arcadia_ADL_UI_ButtonDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_UI_ButtonDefinition);
  {
   Arcadia_Value definitions = Arcadia_ValueStack_getValue(thread, 22);
     Arcadia_Value name = Arcadia_ValueStack_getValue(thread, 21);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (22 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->left = Arcadia_ValueStack_getInteger32Value(thread, 20);
  self->bottom = Arcadia_ValueStack_getInteger32Value(thread, 19);
  self->width = Arcadia_ValueStack_getNatural32Value(thread, 18);
  self->height = Arcadia_ValueStack_getNatural32Value(thread, 17);
  self->text = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 16, _Arcadia_String_getType(thread));
  self->horizontalAlignment = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 15, _Arcadia_String_getType(thread));
  self->verticalAlignment = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 14, _Arcadia_String_getType(thread));
  self->backgroundColor = Arcadia_ValueStack_isVoidValue(thread, 13) ? NULL : Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions, (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 13, _Arcadia_String_getType(thread)));
  self->hoveredBackgroundColor = Arcadia_ValueStack_isVoidValue(thread, 12) ? NULL : Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions, (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 12, _Arcadia_String_getType(thread)));
  self->pressedBackgroundColor = Arcadia_ValueStack_isVoidValue(thread, 11) ? NULL : Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions, (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 11, _Arcadia_String_getType(thread)));
  self->disabledBackgroundColor = Arcadia_ValueStack_isVoidValue(thread, 10) ? NULL : Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions, (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 10, _Arcadia_String_getType(thread)));
  self->focusedBackgroundColor = Arcadia_ValueStack_isVoidValue(thread, 9) ? NULL : Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions, (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 9, _Arcadia_String_getType(thread)));
  self->backgroundRed = Arcadia_ValueStack_getNatural8Value(thread, 8);
  self->backgroundGreen = Arcadia_ValueStack_getNatural8Value(thread, 7);
  self->backgroundBlue = Arcadia_ValueStack_getNatural8Value(thread, 6);
  self->pixelSize = Arcadia_ValueStack_getNatural32Value(thread, 5);
  self->borderWidth = Arcadia_ValueStack_getNatural32Value(thread, 4);
  self->borderRed = Arcadia_ValueStack_getNatural8Value(thread, 3);
  self->borderGreen = Arcadia_ValueStack_getNatural8Value(thread, 2);
  self->borderBlue = Arcadia_ValueStack_getNatural8Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_ADL_UI_ButtonDefinition);
}

static void
Arcadia_ADL_UI_ButtonDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonDefinition* self
  )
{
  if (self->text) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->text);
  }
  if (self->horizontalAlignment) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->horizontalAlignment);
  }
  if (self->verticalAlignment) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->verticalAlignment);
  }
  if (self->backgroundColor) Arcadia_Object_visit(thread, (Arcadia_Object*)self->backgroundColor);
  if (self->hoveredBackgroundColor) Arcadia_Object_visit(thread, (Arcadia_Object*)self->hoveredBackgroundColor);
  if (self->pressedBackgroundColor) Arcadia_Object_visit(thread, (Arcadia_Object*)self->pressedBackgroundColor);
  if (self->disabledBackgroundColor) Arcadia_Object_visit(thread, (Arcadia_Object*)self->disabledBackgroundColor);
  if (self->focusedBackgroundColor) Arcadia_Object_visit(thread, (Arcadia_Object*)self->focusedBackgroundColor);
}

static void
Arcadia_ADL_UI_ButtonDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = (void (*)(Arcadia_Thread*, Arcadia_ADL_Definition*))&Arcadia_ADL_UI_ButtonDefinition_linkImpl;
}

Arcadia_ADL_UI_ButtonDefinition*
Arcadia_ADL_UI_ButtonDefinition_create
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
    Arcadia_String* backgroundColor,
    Arcadia_String* hoveredBackgroundColor,
    Arcadia_String* pressedBackgroundColor,
    Arcadia_String* disabledBackgroundColor,
    Arcadia_String* focusedBackgroundColor,
    Arcadia_Natural8Value backgroundRed,
    Arcadia_Natural8Value backgroundGreen,
    Arcadia_Natural8Value backgroundBlue,
    Arcadia_Natural32Value pixelSize,
    Arcadia_Natural32Value borderWidth,
    Arcadia_Natural8Value borderRed,
    Arcadia_Natural8Value borderGreen,
    Arcadia_Natural8Value borderBlue
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_UI_ButtonDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushNatural32Value(thread, width);
  Arcadia_ValueStack_pushNatural32Value(thread, height);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)horizontalAlignment);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)verticalAlignment);
  if (backgroundColor) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)backgroundColor); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (hoveredBackgroundColor) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)hoveredBackgroundColor); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (pressedBackgroundColor) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)pressedBackgroundColor); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (disabledBackgroundColor) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)disabledBackgroundColor); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (focusedBackgroundColor) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)focusedBackgroundColor); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, backgroundRed);
  Arcadia_ValueStack_pushNatural8Value(thread, backgroundGreen);
  Arcadia_ValueStack_pushNatural8Value(thread, backgroundBlue);
  Arcadia_ValueStack_pushNatural32Value(thread, pixelSize);
  Arcadia_ValueStack_pushNatural32Value(thread, borderWidth);
  Arcadia_ValueStack_pushNatural8Value(thread, borderRed);
  Arcadia_ValueStack_pushNatural8Value(thread, borderGreen);
  Arcadia_ValueStack_pushNatural8Value(thread, borderBlue);
  Arcadia_ValueStack_pushNatural8Value(thread, 22);
  _Arcadia_EndCreate(Arcadia_ADL_UI_ButtonDefinition);
}
