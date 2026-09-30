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

#define ARCADIA_ENGINE_TEXT_PRIVATE (1)
#include "Arcadia/Engine/Text/MarkupTextRun.h"

static void constructImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupTextRun* self);
static void initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupTextRunDispatch* self);
static void visitImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupTextRun* self);

static const Arcadia_ObjectType_Operations objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations typeOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objectTypeOperations };

Arcadia_defineObjectType(u8"Arcadia.Engine.Text.MarkupTextRun", Arcadia_Engine_Text_MarkupTextRun,
                         u8"Arcadia.Object", Arcadia_Object,
                         &typeOperations);

static void constructImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupTextRun* self) {
  Arcadia_EnterConstructor(Arcadia_Engine_Text_MarkupTextRun);
  { Arcadia_ValueStack_pushNatural8Value(thread, 0); Arcadia_superTypeConstructor(thread, _type, self); }
  if (7 != _numberOfArguments) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->text = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 7, _Arcadia_String_getType(thread));
  self->glyphSource = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 6, _Arcadia_String_getType(thread));
  self->dynamicToken = NULL;
  if (!Arcadia_ValueStack_isVoidValue(thread, 5)) {
    self->dynamicToken = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 5, _Arcadia_String_getType(thread));
  }
  self->pixelSize = Arcadia_ValueStack_getNatural32Value(thread, 4);
  self->colorName = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_String_getType(thread));
  self->alignment = (Arcadia_Engine_Text_TextAlignment)Arcadia_ValueStack_getInteger32Value(thread, 2);
  self->fixedPlacement = Arcadia_ValueStack_getBooleanValue(thread, 1);
  self->hasRectangle = Arcadia_BooleanValue_False;
  self->rectangleLeft = 0;
  self->rectangleBottom = 0;
  self->rectangleWidth = 0;
  self->rectangleHeight = 0;
  Arcadia_LeaveConstructor(Arcadia_Engine_Text_MarkupTextRun);
}

static void initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupTextRunDispatch* self) {/*Intentionally empty.*/}

static void visitImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupTextRun* self) {
  if (self->text) Arcadia_Object_visit(thread, (Arcadia_Object*)self->text);
  if (self->glyphSource) Arcadia_Object_visit(thread, (Arcadia_Object*)self->glyphSource);
  if (self->dynamicToken) Arcadia_Object_visit(thread, (Arcadia_Object*)self->dynamicToken);
  if (self->colorName) Arcadia_Object_visit(thread, (Arcadia_Object*)self->colorName);
}

Arcadia_Engine_Text_MarkupTextRun* Arcadia_Engine_Text_MarkupTextRun_create(Arcadia_Thread* thread, Arcadia_String* text, Arcadia_String* glyphSource, Arcadia_String* dynamicToken, Arcadia_Natural32Value pixelSize, Arcadia_String* colorName, Arcadia_Engine_Text_TextAlignment alignment, Arcadia_BooleanValue fixedPlacement) {
  _Arcadia_BeginCreate(Arcadia_Engine_Text_MarkupTextRun);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)glyphSource);
  if (dynamicToken) { Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)dynamicToken); } else { Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void); }
  Arcadia_ValueStack_pushNatural32Value(thread, pixelSize);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)colorName);
  Arcadia_ValueStack_pushInteger32Value(thread, (Arcadia_Integer32Value)alignment);
  Arcadia_ValueStack_pushBooleanValue(thread, fixedPlacement);
  Arcadia_ValueStack_pushNatural8Value(thread, 7);
  _Arcadia_EndCreate(Arcadia_Engine_Text_MarkupTextRun);
}
