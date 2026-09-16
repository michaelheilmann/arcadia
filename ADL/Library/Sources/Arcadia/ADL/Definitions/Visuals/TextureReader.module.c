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
#include "Arcadia/ADL/Definitions/Visuals/TextureReader.module.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"
#include <string.h>

static const char* SCHEMA =
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"Texture\",\n"
  "  definition : {\n"
  "    kind : \"Map\",\n"
  "    entries : [\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"type\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"name\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"pixelBuffer\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"magnificationFilter\",\n"
  "        optional : \"true\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"minificationFilter\",\n"
  "        optional : \"true\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"addressModeU\",\n"
  "        optional : \"true\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"addressModeV\",\n"
  "        optional : \"true\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "    ],\n"
  "  },\n"
  "}\n"
  ;

static Arcadia_String*
Arcadia_ADL_TextureReader_getTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self
  );

static Arcadia_ADL_TextureDefinition*
Arcadia_ADL_TextureReader_read
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  );

static void
Arcadia_ADL_TextureReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self
  );

static Arcadia_ADL_TextureFilter
getTextureFilterValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  );

static Arcadia_ADL_TextureAddressMode
getTextureAddressModeValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  );

static void
Arcadia_ADL_TextureReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReaderDispatch* self
  );

static void
Arcadia_ADL_TextureReader_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_TextureReader_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_TextureReader_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_TextureReader_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.TextureReader", Arcadia_ADL_TextureReader,
                         u8"Arcadia.ADL.Reader", Arcadia_ADL_Reader,
                         &_typeOperations);

static Arcadia_String*
Arcadia_ADL_TextureReader_getTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self
  )
{ return self->TYPENAME; }

static Arcadia_ADL_TextureDefinition*
Arcadia_ADL_TextureReader_read
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  )
{
  Arcadia_DDLS_ValidationContext_run(thread, self->validationContext, self->SCHEMANAME, input);

  Arcadia_String* type = Arcadia_ADL_Reader_getStringValue(thread, (Arcadia_DDL_MapNode*)input, self->TYPE);
  Arcadia_String* name = Arcadia_ADL_Reader_getStringValue(thread, (Arcadia_DDL_MapNode*)input, self->NAME);
  Arcadia_String* pixelBufferName = Arcadia_ADL_Reader_getStringValue(thread, (Arcadia_DDL_MapNode*)input, self->PIXELBUFFER);
  Arcadia_ADL_TextureFilter magnificationFilter = getTextureFilterValue(thread, self, (Arcadia_DDL_MapNode*)input, self->MAGNIFICATIONFILTER);
  Arcadia_ADL_TextureFilter minificationFilter = getTextureFilterValue(thread, self, (Arcadia_DDL_MapNode*)input, self->MINIFICATIONFILTER);
  Arcadia_ADL_TextureAddressMode addressModeU = getTextureAddressModeValue(thread, self, (Arcadia_DDL_MapNode*)input, self->ADDRESSMODEU);
  Arcadia_ADL_TextureAddressMode addressModeV = getTextureAddressModeValue(thread, self, (Arcadia_DDL_MapNode*)input, self->ADDRESSMODEV);

  // Assert the definition has the correct type.
  Arcadia_Value t = Arcadia_Value_makeObjectReferenceValue(self->TYPENAME);
  if (!Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &t)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_ADL_TextureDefinition* definition = Arcadia_ADL_TextureDefinition_create(thread, definitions, name, pixelBufferName, magnificationFilter, minificationFilter, addressModeU, addressModeV);
  return definition;
}

static Arcadia_ADL_TextureFilter
getTextureFilterValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  )
{
  Arcadia_String* string = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, key);
  if (NULL == string) {
    return Arcadia_ADL_TextureFilter_None;
  }
  Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(string);
  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->LINEAR, &a)) {
    return Arcadia_ADL_TextureFilter_Linear;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->NEAREST, &a)) {
    return Arcadia_ADL_TextureFilter_Nearest;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_ADL_TextureAddressMode
getTextureAddressModeValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  )
{
  Arcadia_String* string = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, key);
  if (NULL == string) {
    return Arcadia_ADL_TextureAddressMode_None;
  }
  Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(string);
  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->CLAMPTOBORDER, &a)) {
    return Arcadia_ADL_TextureAddressMode_ClampToBorder;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->CLAMPTOEDGE, &a)) {
    return Arcadia_ADL_TextureAddressMode_ClampToEdge;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->MIRROREDREPEAT, &a)) {
    return Arcadia_ADL_TextureAddressMode_MirroredRepeat;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->REPEAT, &a)) {
    return Arcadia_ADL_TextureAddressMode_Repeat;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_ADL_TextureReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_TextureReader);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_DDLS_DefaultReader* reader = (Arcadia_DDLS_DefaultReader*)Arcadia_DDLS_DefaultReader_create(thread);
    Arcadia_DDLS_Node* node = Arcadia_DDLS_DefaultReader_run(thread, reader, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, SCHEMA, strlen(SCHEMA))));
    if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDLS_SchemaNode_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_DDLS_ValidationContext* validationContext = Arcadia_DDLS_ValidationContext_create(thread);
    Arcadia_DDLS_ValidationContext_addSchema(thread, validationContext, (Arcadia_DDLS_SchemaNode*)node);
    self->validationContext = validationContext;
  }
  //
  self->TYPE = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"type");
  self->NAME = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"name");
  self->SCHEMANAME = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Texture");
  self->TYPENAME = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Texture");
  //
  self->PIXELBUFFER = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"pixelBuffer");
  //
  self->MAGNIFICATIONFILTER = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"magnificationFilter");
  self->MINIFICATIONFILTER = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"minificationFilter");
  self->ADDRESSMODEU = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"addressModeU");
  self->ADDRESSMODEV = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"addressModeV");
  self->LINEAR = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Linear");
  self->NEAREST = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Nearest");
  self->CLAMPTOBORDER = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"ClampToBorder");
  self->CLAMPTOEDGE = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"ClampToEdge");
  self->MIRROREDREPEAT = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"MirroredRepeat");
  self->REPEAT = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Repeat");
  //
  Arcadia_LeaveConstructor(Arcadia_ADL_TextureReader);
}

static void
Arcadia_ADL_TextureReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReaderDispatch* self
  )
{
  ((Arcadia_ADL_ReaderDispatch*)self)->getTypeName = (Arcadia_String * (*)(Arcadia_Thread*, Arcadia_ADL_Reader*)) & Arcadia_ADL_TextureReader_getTypeName;
  ((Arcadia_ADL_ReaderDispatch*)self)->read = (Arcadia_ADL_Definition * (*)(Arcadia_Thread*, Arcadia_ADL_Reader*, Arcadia_ADL_Context*, Arcadia_ADL_Definitions*, Arcadia_DDL_Node*)) & Arcadia_ADL_TextureReader_read;
}

static void
Arcadia_ADL_TextureReader_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_TextureReader* self
  )
{
  //
  if (self->validationContext) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->validationContext);
  }
  //
  if (self->TYPE) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->TYPE);
  }
  if (self->NAME) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->NAME);
  }
  if (self->SCHEMANAME) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->SCHEMANAME);
  }
  if (self->TYPENAME) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->TYPENAME);
  }
  //
  if (self->PIXELBUFFER) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->PIXELBUFFER);
  }
  //
  if (self->MAGNIFICATIONFILTER) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->MAGNIFICATIONFILTER);
  }
  if (self->MINIFICATIONFILTER) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->MINIFICATIONFILTER);
  }
  if (self->ADDRESSMODEU) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ADDRESSMODEU);
  }
  if (self->ADDRESSMODEV) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ADDRESSMODEV);
  }
  if (self->LINEAR) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->LINEAR);
  }
  if (self->NEAREST) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->NEAREST);
  }
  if (self->CLAMPTOBORDER) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->CLAMPTOBORDER);
  }
  if (self->CLAMPTOEDGE) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->CLAMPTOEDGE);
  }
  if (self->MIRROREDREPEAT) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->MIRROREDREPEAT);
  }
  if (self->REPEAT) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->REPEAT);
  }
  //
}

Arcadia_ADL_TextureReader*
Arcadia_ADL_TextureReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_TextureReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_TextureReader);
}
