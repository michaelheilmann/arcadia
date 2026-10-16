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
#include "Arcadia/ADL/Definitions/UI/ButtonReader.module.h"

#include "Arcadia/ADL/Definitions/UI/Include.h"
#include "Arcadia/ADL/Reader.module.h"
#include <string.h>

static const char* SCHEMA =
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"UI.Button\",\n"
  "  definition : {\n"
  "    kind : \"Map\",\n"
  "    entries : [\n"
  "      { kind : \"MapEntry\", name : \"type\", type : { kind : \"String\" } },\n"
  "      { kind : \"MapEntry\", name : \"name\", type : { kind : \"String\" } },\n"
  "      { kind : \"MapEntry\", name : \"left\", type : { kind : \"Number\" } },\n"
      "      { kind : \"MapEntry\", name : \"bottom\", type : { kind : \"Number\" } },\n"
      "      { kind : \"MapEntry\", name : \"width\", type : { kind : \"Number\" } },\n"
      "      { kind : \"MapEntry\", name : \"height\", type : { kind : \"Number\" } },\n"
      "      { kind : \"MapEntry\", name : \"text\", type : { kind : \"String\" } },\n"
      "      { kind : \"MapEntry\", name : \"horizontalAlignment\", type : { kind : \"String\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"verticalAlignment\", type : { kind : \"String\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"backgroundColor\", type : { kind : \"String\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"hoveredBackgroundColor\", type : { kind : \"String\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"pressedBackgroundColor\", type : { kind : \"String\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"disabledBackgroundColor\", type : { kind : \"String\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"focusedBackgroundColor\", type : { kind : \"String\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"backgroundRed\", type : { kind : \"Number\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"backgroundGreen\", type : { kind : \"Number\" }, optional : \"true\" },\n"
       "      { kind : \"MapEntry\", name : \"backgroundBlue\", type : { kind : \"Number\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"pixelSize\", type : { kind : \"Number\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"borderWidth\", type : { kind : \"Number\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"borderRed\", type : { kind : \"Number\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"borderGreen\", type : { kind : \"Number\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"borderBlue\", type : { kind : \"Number\" }, optional : \"true\" },\n"
  "    ],\n"
  "  },\n"
  "}\n";

static Arcadia_String* Arcadia_ADL_UI_ButtonReader_getTypeName(Arcadia_Thread* thread, Arcadia_ADL_UI_ButtonReader* self);
static Arcadia_ADL_UI_ButtonDefinition* Arcadia_ADL_UI_ButtonReader_read(Arcadia_Thread* thread, Arcadia_ADL_UI_ButtonReader* self, Arcadia_ADL_Context* context, Arcadia_ADL_Definitions* definitions, Arcadia_DDL_Node* input);
static void Arcadia_ADL_UI_ButtonReader_constructImpl(Arcadia_Thread* thread, Arcadia_ADL_UI_ButtonReader* self);
static void Arcadia_ADL_UI_ButtonReader_initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_ADL_UI_ButtonReaderDispatch* self);
static void Arcadia_ADL_UI_ButtonReader_visitImpl(Arcadia_Thread* thread, Arcadia_ADL_UI_ButtonReader* self);

static Arcadia_Natural8Value getNatural8ValueOptional(Arcadia_Thread* thread, Arcadia_DDL_MapNode* mapNode, Arcadia_String* key, Arcadia_Natural8Value defaultValue);
static Arcadia_Natural32Value getNatural32ValueOptional(Arcadia_Thread* thread, Arcadia_DDL_MapNode* mapNode, Arcadia_String* key, Arcadia_Natural32Value defaultValue);

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_UI_ButtonReader_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_UI_ButtonReader_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_UI_ButtonReader_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.UI.ButtonReader", Arcadia_ADL_UI_ButtonReader,
                         u8"Arcadia.ADL.Reader", Arcadia_ADL_Reader,
                         &_typeOperations);

static Arcadia_String*
Arcadia_ADL_UI_ButtonReader_getTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonReader* self
  )
{ return self->TYPENAME; }

static Arcadia_ADL_UI_ButtonDefinition*
Arcadia_ADL_UI_ButtonReader_read
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  )
{
  Arcadia_DDLS_ValidationContext_run(thread, self->validationContext, self->SCHEMANAME, input);

  Arcadia_String* type = Arcadia_ADL_Reader_getStringValue(thread, (Arcadia_DDL_MapNode*)input, self->TYPE);
  Arcadia_Value t = Arcadia_Value_makeObjectReferenceValue(self->TYPENAME);
  if (!Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &t)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_DDL_MapNode* mapNode = (Arcadia_DDL_MapNode*)input;
  Arcadia_Natural8Value backgroundRed = getNatural8ValueOptional(thread, mapNode, self->BACKGROUND_RED, 56);
  Arcadia_Natural8Value backgroundGreen = getNatural8ValueOptional(thread, mapNode, self->BACKGROUND_GREEN, 120);
  Arcadia_Natural8Value backgroundBlue = getNatural8ValueOptional(thread, mapNode, self->BACKGROUND_BLUE, 240);
  Arcadia_Natural32Value pixelSize = getNatural32ValueOptional(thread, mapNode, Arcadia_String_createFromCxxString(thread, u8"pixelSize"), 24);
  Arcadia_Natural32Value borderWidth = getNatural32ValueOptional(thread, mapNode, Arcadia_String_createFromCxxString(thread, u8"borderWidth"), 0);
  Arcadia_Natural8Value borderRed = getNatural8ValueOptional(thread, mapNode, Arcadia_String_createFromCxxString(thread, u8"borderRed"), 87);
  Arcadia_Natural8Value borderGreen = getNatural8ValueOptional(thread, mapNode, Arcadia_String_createFromCxxString(thread, u8"borderGreen"), 104);
  Arcadia_Natural8Value borderBlue = getNatural8ValueOptional(thread, mapNode, Arcadia_String_createFromCxxString(thread, u8"borderBlue"), 118);
  Arcadia_String* horizontalAlignment = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, self->HORIZONTAL_ALIGNMENT);
  Arcadia_String* verticalAlignment = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, self->VERTICAL_ALIGNMENT);
  Arcadia_String* backgroundColor = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, self->BACKGROUND_COLOR);
  Arcadia_String* hoveredBackgroundColor = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, self->HOVERED_BACKGROUND_COLOR);
  Arcadia_String* pressedBackgroundColor = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, self->PRESSED_BACKGROUND_COLOR);
  Arcadia_String* disabledBackgroundColor = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, self->DISABLED_BACKGROUND_COLOR);
  Arcadia_String* focusedBackgroundColor = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, self->FOCUSED_BACKGROUND_COLOR);
  return Arcadia_ADL_UI_ButtonDefinition_create(thread, definitions,
                                                Arcadia_ADL_Reader_getStringValue(thread, mapNode, self->NAME),
                                                Arcadia_ADL_Reader_getInteger32Value(thread, mapNode, self->LEFT),
                                                Arcadia_ADL_Reader_getInteger32Value(thread, mapNode, self->BOTTOM),
                                                Arcadia_ADL_Reader_getNatural32Value(thread, mapNode, self->WIDTH),
                                                Arcadia_ADL_Reader_getNatural32Value(thread, mapNode, self->HEIGHT),
                                                Arcadia_ADL_Reader_getStringValue(thread, mapNode, self->TEXT),
                                                horizontalAlignment ? horizontalAlignment : self->CENTER,
                                                verticalAlignment ? verticalAlignment : self->CENTER,
                                                backgroundColor,
                                                hoveredBackgroundColor ? hoveredBackgroundColor : backgroundColor,
                                                pressedBackgroundColor ? pressedBackgroundColor : backgroundColor,
                                                disabledBackgroundColor ? disabledBackgroundColor : backgroundColor,
                                                focusedBackgroundColor ? focusedBackgroundColor : backgroundColor,
                                                 backgroundRed,
                                                 backgroundGreen,
                                                 backgroundBlue,
                                                 pixelSize,
                                                 borderWidth,
                                                 borderRed,
                                                 borderGreen,
                                                 borderBlue);
}

static Arcadia_Natural8Value
getNatural8ValueOptional
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key,
    Arcadia_Natural8Value defaultValue
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)mapNode->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* mapEntryNode =
      (Arcadia_DDL_MapEntryNode*)
      Arcadia_List_getObjectReferenceValueCheckedAt
        (
          thread,
          (Arcadia_List*)mapNode->entries,
          i,
          _Arcadia_DDL_MapEntryNode_getType(thread)
        );
    Arcadia_Value t = Arcadia_Value_makeObjectReferenceValue(key);
    if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)mapEntryNode->key->value, &t)) {
      if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)mapEntryNode->value, _Arcadia_DDL_NumberNode_getType(thread))) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      return Arcadia_String_toNatural8(thread, ((Arcadia_DDL_NumberNode*)mapEntryNode->value)->value);
    }
  }
  return defaultValue;
}

static Arcadia_Natural32Value
getNatural32ValueOptional
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key,
    Arcadia_Natural32Value defaultValue
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)mapNode->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* mapEntryNode = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread,
      (Arcadia_List*)mapNode->entries, i, _Arcadia_DDL_MapEntryNode_getType(thread));
    Arcadia_Value keyValue = Arcadia_Value_makeObjectReferenceValue(key);
    if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)mapEntryNode->key->value, &keyValue)) {
      if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)mapEntryNode->value, _Arcadia_DDL_NumberNode_getType(thread))) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      return Arcadia_String_toNatural32(thread, ((Arcadia_DDL_NumberNode*)mapEntryNode->value)->value);
    }
  }
  return defaultValue;
}

static void
Arcadia_ADL_UI_ButtonReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_UI_ButtonReader);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDLS_DefaultReader* reader = (Arcadia_DDLS_DefaultReader*)Arcadia_DDLS_DefaultReader_create(thread);
  Arcadia_DDLS_Node* node = Arcadia_DDLS_DefaultReader_run(thread, reader, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, SCHEMA, strlen(SCHEMA))));
  Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
  Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)node);
  self->validationContext = validationContext;
  self->TYPE = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"type");
  self->NAME = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"name");
  self->LEFT = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"left");
  self->BOTTOM = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"bottom");
  self->WIDTH = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"width");
  self->HEIGHT = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"height");
  self->TEXT = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"text");
  self->HORIZONTAL_ALIGNMENT = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"horizontalAlignment");
  self->VERTICAL_ALIGNMENT = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"verticalAlignment");
  self->BACKGROUND_COLOR = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"backgroundColor");
  self->HOVERED_BACKGROUND_COLOR = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"hoveredBackgroundColor");
  self->PRESSED_BACKGROUND_COLOR = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"pressedBackgroundColor");
  self->DISABLED_BACKGROUND_COLOR = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"disabledBackgroundColor");
  self->FOCUSED_BACKGROUND_COLOR = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"focusedBackgroundColor");
  self->BACKGROUND_RED = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"backgroundRed");
  self->BACKGROUND_GREEN = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"backgroundGreen");
  self->BACKGROUND_BLUE = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"backgroundBlue");
  self->SCHEMANAME = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"UI.Button");
  self->TYPENAME = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"UI.Button");
  self->CENTER = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Center");
  Arcadia_LeaveConstructor(Arcadia_ADL_UI_ButtonReader);
}

static void
Arcadia_ADL_UI_ButtonReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonReaderDispatch* self
  )
{
  ((Arcadia_ADL_ReaderDispatch*)self)->getTypeName = (Arcadia_String * (*)(Arcadia_Thread*, Arcadia_ADL_Reader*)) & Arcadia_ADL_UI_ButtonReader_getTypeName;
  ((Arcadia_ADL_ReaderDispatch*)self)->read = (Arcadia_ADL_Definition * (*)(Arcadia_Thread*, Arcadia_ADL_Reader*, Arcadia_ADL_Context*, Arcadia_ADL_Definitions*, Arcadia_DDL_Node*)) & Arcadia_ADL_UI_ButtonReader_read;
}

static void
Arcadia_ADL_UI_ButtonReader_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonReader* self
  )
{
  if (self->validationContext) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->validationContext);
  }
}

Arcadia_ADL_UI_ButtonReader*
Arcadia_ADL_UI_ButtonReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_UI_ButtonReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_UI_ButtonReader);
}
