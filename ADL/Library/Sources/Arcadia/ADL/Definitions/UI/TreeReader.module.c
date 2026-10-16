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
#include "Arcadia/ADL/Definitions/UI/TreeReader.module.h"

#include "Arcadia/ADL/Definitions/UI/Include.h"
#include "Arcadia/ADL/Reader.module.h"

#include <string.h>

#define MAP_SCHEMA_BEGIN(name) \
  "{ kind : \"Schema\", name : \"" name "\", definition : { kind : \"Map\", entries : ["
#define MAP_SCHEMA_END "] } }"
#define STRING_FIELD(name) \
  "{ kind : \"MapEntry\", name : \"" name "\", type : { kind : \"String\" } },"
#define NUMBER_FIELD(name) \
  "{ kind : \"MapEntry\", name : \"" name "\", type : { kind : \"Number\" } },"
#define LIST_FIELD(name) \
  "{ kind : \"MapEntry\", name : \"" name "\", type : { kind : \"List\", entry : { kind : \"String\" } } },"
#define OPTIONAL_NUMBER_FIELD(name) \
  "{ kind : \"MapEntry\", name : \"" name "\", type : { kind : \"Number\" }, optional : \"true\" },"
#define OPTIONAL_STRING_FIELD(name) \
  "{ kind : \"MapEntry\", name : \"" name "\", type : { kind : \"String\" }, optional : \"true\" },"

static const char* LABEL_SCHEMA =
  MAP_SCHEMA_BEGIN("UI.Label")
  STRING_FIELD("type") STRING_FIELD("name") NUMBER_FIELD("left") NUMBER_FIELD("bottom")
  NUMBER_FIELD("width") NUMBER_FIELD("height") STRING_FIELD("text")
  STRING_FIELD("horizontalAlignment") STRING_FIELD("verticalAlignment")
  NUMBER_FIELD("red") NUMBER_FIELD("green") NUMBER_FIELD("blue") NUMBER_FIELD("alpha")
  NUMBER_FIELD("pixelSize") MAP_SCHEMA_END;

static const char* STACK_PANEL_SCHEMA =
  MAP_SCHEMA_BEGIN("UI.StackPanel")
  STRING_FIELD("type") STRING_FIELD("name") NUMBER_FIELD("left") NUMBER_FIELD("bottom")
  NUMBER_FIELD("width") NUMBER_FIELD("height") STRING_FIELD("vertical") NUMBER_FIELD("gap")
  NUMBER_FIELD("paddingLeft") NUMBER_FIELD("paddingBottom") NUMBER_FIELD("paddingRight")
  NUMBER_FIELD("paddingTop") STRING_FIELD("mainAlignment") STRING_FIELD("crossAlignment")
  LIST_FIELD("children") OPTIONAL_STRING_FIELD("backgroundModel")
  OPTIONAL_NUMBER_FIELD("backgroundBorderWidth") MAP_SCHEMA_END;

static const char* SEGMENTED_CHOICE_SCHEMA =
  MAP_SCHEMA_BEGIN("UI.SegmentedChoice")
  STRING_FIELD("type") STRING_FIELD("name") NUMBER_FIELD("left") NUMBER_FIELD("bottom")
  NUMBER_FIELD("width") NUMBER_FIELD("height") NUMBER_FIELD("selectedIndex") LIST_FIELD("labels")
  OPTIONAL_NUMBER_FIELD("pixelSize") OPTIONAL_NUMBER_FIELD("borderWidth")
  OPTIONAL_NUMBER_FIELD("borderRed") OPTIONAL_NUMBER_FIELD("borderGreen") OPTIONAL_NUMBER_FIELD("borderBlue")
  MAP_SCHEMA_END;

static const char* SLIDER_SCHEMA =
  MAP_SCHEMA_BEGIN("UI.Slider")
  STRING_FIELD("type") STRING_FIELD("name") NUMBER_FIELD("left") NUMBER_FIELD("bottom")
  NUMBER_FIELD("width") NUMBER_FIELD("height") NUMBER_FIELD("minimum") NUMBER_FIELD("maximum")
  NUMBER_FIELD("value") NUMBER_FIELD("step") MAP_SCHEMA_END;

static const char* TOGGLE_SCHEMA =
  MAP_SCHEMA_BEGIN("UI.Toggle")
  STRING_FIELD("type") STRING_FIELD("name") NUMBER_FIELD("left") NUMBER_FIELD("bottom")
  NUMBER_FIELD("width") NUMBER_FIELD("height") STRING_FIELD("checked") MAP_SCHEMA_END;

#undef MAP_SCHEMA_BEGIN
#undef MAP_SCHEMA_END
#undef STRING_FIELD
#undef NUMBER_FIELD
#undef LIST_FIELD
#undef OPTIONAL_NUMBER_FIELD
#undef OPTIONAL_STRING_FIELD

static Arcadia_String*
getTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_TreeReader* self
  );

static Arcadia_ADL_Definition*
read
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_TreeReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  );

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_TreeReader* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_TreeReaderDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_TreeReader* self
  );

static Arcadia_Real64Value
getReal64Value
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* map,
    Arcadia_String* key
  );

static Arcadia_DDL_NumberNode*
getNumberNodeOptional
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* map,
    Arcadia_String* key
  );

static Arcadia_Natural32Value
getNatural32ValueOptional
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* map,
    char const* key,
    Arcadia_Natural32Value defaultValue
  );

static Arcadia_Natural8Value
getNatural8ValueOptional
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* map,
    char const* key,
    Arcadia_Natural8Value defaultValue
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &_objectTypeOperations };

Arcadia_defineObjectType(u8"Arcadia.ADL.UI.TreeReader", Arcadia_ADL_UI_TreeReader,
                         u8"Arcadia.ADL.Reader", Arcadia_ADL_Reader, &_typeOperations);

static Arcadia_String*
getTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_TreeReader* self
  )
{ return self->typeName; }

static Arcadia_ADL_Definition*
read
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_TreeReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  )
{
  Arcadia_DDL_MapNode* map = (Arcadia_DDL_MapNode*)input;
  Arcadia_String* type = Arcadia_ADL_Reader_getStringValue(thread, map, Arcadia_String_createFromCxxString(thread, u8"type"));
  Arcadia_DDLS_ValidationContext_run(thread, self->validationContext, type, input);
  Arcadia_String* name = Arcadia_ADL_Reader_getStringValue(thread, map, Arcadia_String_createFromCxxString(thread, u8"name"));
  Arcadia_Value labelType = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"UI.Label"));
  Arcadia_Value stackPanelType = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"UI.StackPanel"));
  Arcadia_Value segmentedChoiceType = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"UI.SegmentedChoice"));
  Arcadia_Value sliderType = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"UI.Slider"));
  Arcadia_Value toggleType = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"UI.Toggle"));

  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &labelType)) {
    return (Arcadia_ADL_Definition*)Arcadia_ADL_UI_LabelDefinition_create(thread, definitions, name,
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"left")),
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"bottom")),
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"width")),
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"height")),
      Arcadia_ADL_Reader_getStringValue(thread, map, Arcadia_String_createFromCxxString(thread, u8"text")),
      Arcadia_ADL_Reader_getStringValue(thread, map, Arcadia_String_createFromCxxString(thread, u8"horizontalAlignment")),
      Arcadia_ADL_Reader_getStringValue(thread, map, Arcadia_String_createFromCxxString(thread, u8"verticalAlignment")),
      Arcadia_ADL_Reader_getNatural8Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"red")),
      Arcadia_ADL_Reader_getNatural8Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"green")),
      Arcadia_ADL_Reader_getNatural8Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"blue")),
      Arcadia_ADL_Reader_getNatural8Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"alpha")),
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"pixelSize")));
  }

  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &stackPanelType)) {
    Arcadia_ADL_UI_StackPanelDefinition* definition = Arcadia_ADL_UI_StackPanelDefinition_create(thread, definitions, name,
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"left")),
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"bottom")),
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"width")),
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"height")),
      Arcadia_String_toBoolean(thread, Arcadia_ADL_Reader_getStringValue(thread, map, Arcadia_String_createFromCxxString(thread, u8"vertical"))),
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"gap")),
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"paddingLeft")),
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"paddingBottom")),
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"paddingRight")),
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"paddingTop")),
      Arcadia_ADL_Reader_getStringValue(thread, map, Arcadia_String_createFromCxxString(thread, u8"mainAlignment")),
      Arcadia_ADL_Reader_getStringValue(thread, map, Arcadia_String_createFromCxxString(thread, u8"crossAlignment")),
      Arcadia_ADL_Reader_getStringValueOptional(thread, map, Arcadia_String_createFromCxxString(thread, u8"backgroundModel")),
      getNatural32ValueOptional(thread, map, u8"backgroundBorderWidth", 0));
    Arcadia_List* childNames = Arcadia_ADL_Reader_getListValue(thread, map, Arcadia_String_createFromCxxString(thread, u8"children"));
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)childNames); i < n; ++i) {
      Arcadia_DDL_StringNode* childName = (Arcadia_DDL_StringNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, childNames, i, _Arcadia_DDL_StringNode_getType(thread));
      Arcadia_ADL_UI_ContainerDefinition_addChildReference(thread, (Arcadia_Object*)definition, childName->value);
    }
    return (Arcadia_ADL_Definition*)definition;
  }

  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &segmentedChoiceType)) {
    Arcadia_List* inputLabels = Arcadia_ADL_Reader_getListValue(thread, map, Arcadia_String_createFromCxxString(thread, u8"labels"));
    Arcadia_List* labels = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)inputLabels); i < n; ++i) {
      Arcadia_DDL_StringNode* label = (Arcadia_DDL_StringNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, inputLabels, i, _Arcadia_DDL_StringNode_getType(thread));
      Arcadia_List_insertBackObjectReferenceValue(thread, labels, (Arcadia_Object*)label->value);
    }
    return (Arcadia_ADL_Definition*)Arcadia_ADL_UI_SegmentedChoiceDefinition_create(thread, definitions, name,
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"left")),
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"bottom")),
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"width")),
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"height")), labels,
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"selectedIndex")),
      getNatural32ValueOptional(thread, map, u8"pixelSize", 24),
      getNatural32ValueOptional(thread, map, u8"borderWidth", 0),
      getNatural8ValueOptional(thread, map, u8"borderRed", 87),
      getNatural8ValueOptional(thread, map, u8"borderGreen", 104),
      getNatural8ValueOptional(thread, map, u8"borderBlue", 118));
  }

  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &sliderType)) {
    return (Arcadia_ADL_Definition*)Arcadia_ADL_UI_SliderDefinition_create(thread, definitions, name,
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"left")),
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"bottom")),
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"width")),
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"height")),
      getReal64Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"minimum")),
      getReal64Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"maximum")),
      getReal64Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"value")),
      getReal64Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"step")));
  }

  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &toggleType)) {
    return (Arcadia_ADL_Definition*)Arcadia_ADL_UI_ToggleDefinition_create(thread, definitions, name,
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"left")),
      Arcadia_ADL_Reader_getInteger32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"bottom")),
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"width")),
      Arcadia_ADL_Reader_getNatural32Value(thread, map, Arcadia_String_createFromCxxString(thread, u8"height")),
      Arcadia_String_toBoolean(thread, Arcadia_ADL_Reader_getStringValue(thread, map, Arcadia_String_createFromCxxString(thread, u8"checked"))));
  }

  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

static Arcadia_Real64Value
getReal64Value
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* map,
    Arcadia_String* key
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)map->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* entry = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread,
      (Arcadia_List*)map->entries, i, _Arcadia_DDL_MapEntryNode_getType(thread));
    Arcadia_Value expectedKey = Arcadia_Value_makeObjectReferenceValue(key);
    if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)entry->key->value, &expectedKey)) {
      if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)entry->value, _Arcadia_DDL_NumberNode_getType(thread))) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      return Arcadia_String_toReal64(thread, ((Arcadia_DDL_NumberNode*)entry->value)->value);
    }
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

static Arcadia_DDL_NumberNode*
getNumberNodeOptional
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* map,
    Arcadia_String* key
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)map->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* entry = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread,
      (Arcadia_List*)map->entries, i, _Arcadia_DDL_MapEntryNode_getType(thread));
    Arcadia_Value expectedKey = Arcadia_Value_makeObjectReferenceValue(key);
    if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)entry->key->value, &expectedKey)) {
      if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)entry->value, _Arcadia_DDL_NumberNode_getType(thread))) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      return (Arcadia_DDL_NumberNode*)entry->value;
    }
  }
  return NULL;
}

static Arcadia_Natural32Value
getNatural32ValueOptional
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* map,
    char const* key,
    Arcadia_Natural32Value defaultValue
  )
{
  Arcadia_DDL_NumberNode* number = getNumberNodeOptional(thread, map, Arcadia_String_createFromCxxString(thread, key));
  return number ? Arcadia_String_toNatural32(thread, number->value) : defaultValue;
}

static Arcadia_Natural8Value
getNatural8ValueOptional
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* map,
    char const* key,
    Arcadia_Natural8Value defaultValue
  )
{
  Arcadia_DDL_NumberNode* number = getNumberNodeOptional(thread, map, Arcadia_String_createFromCxxString(thread, key));
  return number ? Arcadia_String_toNatural8(thread, number->value) : defaultValue;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_TreeReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_UI_TreeReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
  const char* schemas[] = { LABEL_SCHEMA, STACK_PANEL_SCHEMA, SEGMENTED_CHOICE_SCHEMA, SLIDER_SCHEMA, TOGGLE_SCHEMA };
  for (Arcadia_SizeValue i = 0; i < sizeof(schemas) / sizeof(schemas[0]); ++i) {
    Arcadia_DDLS_DefaultReader* reader = Arcadia_DDLS_DefaultReader_create(thread);
    Arcadia_DDLS_SchemaNode* schema = (Arcadia_DDLS_SchemaNode*)Arcadia_DDLS_DefaultReader_run(thread, reader,
      Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, schemas[i], strlen(schemas[i]))));
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, schema);
  }
  self->validationContext = validationContext;
  self->typeName = Arcadia_String_createFromCxxString(thread, u8"UI.Tree");
  Arcadia_LeaveConstructor(Arcadia_ADL_UI_TreeReader);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_TreeReaderDispatch* self
  )
{
  ((Arcadia_ADL_ReaderDispatch*)self)->getTypeName = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_ADL_Reader*))&getTypeName;
  ((Arcadia_ADL_ReaderDispatch*)self)->read = (Arcadia_ADL_Definition* (*)(Arcadia_Thread*, Arcadia_ADL_Reader*, Arcadia_ADL_Context*, Arcadia_ADL_Definitions*, Arcadia_DDL_Node*))&read;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_TreeReader* self
  )
{
  if (self->typeName) Arcadia_Object_visit(thread, (Arcadia_Object*)self->typeName);
  if (self->validationContext) Arcadia_Object_visit(thread, (Arcadia_Object*)self->validationContext);
}

Arcadia_ADL_UI_TreeReader*
Arcadia_ADL_UI_TreeReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_UI_TreeReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_UI_TreeReader);
}
