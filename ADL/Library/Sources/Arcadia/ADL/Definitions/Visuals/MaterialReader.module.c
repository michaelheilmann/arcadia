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
#include "Arcadia/ADL/Definitions/Visuals/MaterialReader.module.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"
#include <string.h>

static const char* SCHEMA =
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"Material\",\n"
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
  "        name : \"materialType\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"ambientColorSource\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"ambientColorTexture\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"blendSourceFunction\",\n"
  "        optional : \"true\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"blendDestinationFunction\",\n"
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
Arcadia_ADL_MaterialReader_getTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self
  );

static Arcadia_ADL_MaterialDefinition*
Arcadia_ADL_MaterialReader_read
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  );

static Arcadia_ADL_MaterialType
getMaterialTypeValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  );

static Arcadia_ADL_AmbientColorSource
getAmbientColorSourceValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  );

static Arcadia_ADL_BlendFunction
getBlendFunctionValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  );

static void
Arcadia_ADL_MaterialReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self
  );

static void
Arcadia_ADL_MaterialReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReaderDispatch* self
  );

static void
Arcadia_ADL_MaterialReader_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_MaterialReader_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_MaterialReader_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_MaterialReader_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.MaterialReader", Arcadia_ADL_MaterialReader,
                         u8"Arcadia.ADL.Reader", Arcadia_ADL_Reader,
                         &_typeOperations);

static Arcadia_String*
Arcadia_ADL_MaterialReader_getTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self
  )
{ return self->TYPENAME; }

static Arcadia_ADL_MaterialDefinition*
Arcadia_ADL_MaterialReader_read
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  )
{
  Arcadia_DDLS_ValidationContext_run(thread, self->validationContext, self->SCHEMANAME, input);

  Arcadia_String* type = Arcadia_ADL_Reader_getStringValue(thread, (Arcadia_DDL_MapNode*)input, self->TYPE);
  Arcadia_String* name = Arcadia_ADL_Reader_getStringValue(thread, (Arcadia_DDL_MapNode*)input, self->NAME);

  Arcadia_ADL_MaterialType materialType = getMaterialTypeValue(thread, self, (Arcadia_DDL_MapNode*)input, self->MATERIALTYPE);
  Arcadia_ADL_AmbientColorSource ambientColorSource = getAmbientColorSourceValue(thread, self, (Arcadia_DDL_MapNode*)input, self->AMBIENTCOLORSOURCE);
  Arcadia_String* ambientColorTextureName = Arcadia_ADL_Reader_getStringValue(thread, (Arcadia_DDL_MapNode*)input, self->AMBIENTCOLORTEXTURE);
  Arcadia_ADL_BlendFunction blendSourceFunction = getBlendFunctionValue(thread, self, (Arcadia_DDL_MapNode*)input, self->BLENDSOURCEFUNCTION);
  Arcadia_ADL_BlendFunction blendDestinationFunction = getBlendFunctionValue(thread, self, (Arcadia_DDL_MapNode*)input, self->BLENDDESTINATIONFUNCTION);

  // Assert the definition has the correct type.
  Arcadia_Value t = Arcadia_Value_makeObjectReferenceValue(self->TYPENAME);
  if (!Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &t)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_ADL_MaterialDefinition* definition = Arcadia_ADL_MaterialDefinition_create(thread, definitions, name, materialType, ambientColorSource, ambientColorTextureName, blendSourceFunction, blendDestinationFunction);
  return definition;
}

static Arcadia_ADL_MaterialType
getMaterialTypeValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  )
{
  Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getStringValue(thread, mapNode, key));
  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->BLINNPHONG, &a)) {
    return Arcadia_ADL_MaterialType_BlinnPhong;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_ADL_AmbientColorSource
getAmbientColorSourceValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  )
{
  Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getStringValue(thread, mapNode, key));
  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->MESH, &a)) {
    return Arcadia_ADL_AmbientColorSource_Mesh;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->VERTEX, &a)) {
    return Arcadia_ADL_AmbientColorSource_Vertex;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->TEXTURE, &a)) {
    return Arcadia_ADL_AmbientColorSource_Texture;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_ADL_BlendFunction
getBlendFunctionValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  )
{
  Arcadia_String* string = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, key);
  if (NULL == string) {
    return Arcadia_ADL_BlendFunction_None;
  }
  Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(string);
  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->ZERO, &a)) {
    return Arcadia_ADL_BlendFunction_Zero;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->ONE, &a)) {
    return Arcadia_ADL_BlendFunction_One;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->SOURCECOLOR, &a)) {
    return Arcadia_ADL_BlendFunction_SourceColor;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->ONEMINUSSOURCECOLOR, &a)) {
    return Arcadia_ADL_BlendFunction_OneMinusSourceColor;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->DESTINATIONCOLOR, &a)) {
    return Arcadia_ADL_BlendFunction_DestinationColor;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->ONEMINUSDESTINATIONCOLOR, &a)) {
    return Arcadia_ADL_BlendFunction_OneMinusDestinationColor;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->SOURCEALPHA, &a)) {
    return Arcadia_ADL_BlendFunction_SourceAlpha;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->ONEMINUSSOURCEALPHA, &a)) {
    return Arcadia_ADL_BlendFunction_OneMinusSourceAlpha;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->DESTINATIONALPHA, &a)) {
    return Arcadia_ADL_BlendFunction_DestinationAlpha;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->ONEMINUSDESTINATIONALPHA, &a)) {
    return Arcadia_ADL_BlendFunction_OneMinusDestinationAlpha;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_ADL_MaterialReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_MaterialReader);
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
  self->SCHEMANAME = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Material");
  self->TYPENAME = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Material");
  //
  self->MATERIALTYPE = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"materialType");
  self->BLINNPHONG = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Blinn-Phong");
  //
  self->AMBIENTCOLORSOURCE = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"ambientColorSource");
  self->MESH = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Mesh");
  self->VERTEX = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Vertex");
  self->TEXTURE = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Texture");
  //
  self->AMBIENTCOLORTEXTURE = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"ambientColorTexture");
  //
  self->BLENDSOURCEFUNCTION = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"blendSourceFunction");
  self->BLENDDESTINATIONFUNCTION = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"blendDestinationFunction");
  self->ZERO = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"Zero");
  self->ONE = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"One");
  self->SOURCECOLOR = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"SourceColor");
  self->ONEMINUSSOURCECOLOR = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"OneMinusSourceColor");
  self->DESTINATIONCOLOR = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"DestinationColor");
  self->ONEMINUSDESTINATIONCOLOR = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"OneMinusDestinationColor");
  self->SOURCEALPHA = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"SourceAlpha");
  self->ONEMINUSSOURCEALPHA = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"OneMinusSourceAlpha");
  self->DESTINATIONALPHA = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"DestinationAlpha");
  self->ONEMINUSDESTINATIONALPHA = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"OneMinusDestinationAlpha");
  //
  Arcadia_LeaveConstructor(Arcadia_ADL_MaterialReader);
}

static void
Arcadia_ADL_MaterialReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReaderDispatch* self
  )
{
  ((Arcadia_ADL_ReaderDispatch*)self)->getTypeName = (Arcadia_String * (*)(Arcadia_Thread*, Arcadia_ADL_Reader*)) & Arcadia_ADL_MaterialReader_getTypeName;
  ((Arcadia_ADL_ReaderDispatch*)self)->read = (Arcadia_ADL_Definition * (*)(Arcadia_Thread*, Arcadia_ADL_Reader*, Arcadia_ADL_Context*, Arcadia_ADL_Definitions*, Arcadia_DDL_Node*)) & Arcadia_ADL_MaterialReader_read;
}

static void
Arcadia_ADL_MaterialReader_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialReader* self
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
  if (self->MATERIALTYPE) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->MATERIALTYPE);
  }
  if (self->BLINNPHONG) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->BLINNPHONG);
  }
  //
  if (self->AMBIENTCOLORSOURCE) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->AMBIENTCOLORSOURCE);
  }
  if (self->MESH) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->MESH);
  }
  if (self->VERTEX) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->VERTEX);
  }
  if (self->TEXTURE) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->TEXTURE);
  }
  //
  if (self->AMBIENTCOLORTEXTURE) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->AMBIENTCOLORTEXTURE);
  }
  //
  if (self->BLENDSOURCEFUNCTION) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->BLENDSOURCEFUNCTION);
  }
  if (self->BLENDDESTINATIONFUNCTION) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->BLENDDESTINATIONFUNCTION);
  }
  if (self->ZERO) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ZERO);
  }
  if (self->ONE) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ONE);
  }
  if (self->SOURCECOLOR) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->SOURCECOLOR);
  }
  if (self->ONEMINUSSOURCECOLOR) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ONEMINUSSOURCECOLOR);
  }
  if (self->DESTINATIONCOLOR) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->DESTINATIONCOLOR);
  }
  if (self->ONEMINUSDESTINATIONCOLOR) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ONEMINUSDESTINATIONCOLOR);
  }
  if (self->SOURCEALPHA) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->SOURCEALPHA);
  }
  if (self->ONEMINUSSOURCEALPHA) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ONEMINUSSOURCEALPHA);
  }
  if (self->DESTINATIONALPHA) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->DESTINATIONALPHA);
  }
  if (self->ONEMINUSDESTINATIONALPHA) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ONEMINUSDESTINATIONALPHA);
  }
}

Arcadia_ADL_MaterialReader*
Arcadia_ADL_MaterialReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_MaterialReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_MaterialReader);
}
