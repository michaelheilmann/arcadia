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
#include "Arcadia/ADL/Definitions/Visuals/SphereMeshGeneratorReader.module.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"
#include <string.h>

static const char* SCHEMA =
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"SphereMeshGenerator\",\n"
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
  "        name : \"radius\",\n"
  "        type : {\n"
  "          kind : \"Number\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"segments\",\n"
  "        type : {\n"
  "          kind : \"Number\",\n"
  "        },\n"
  "      },\n"
  "      {\n"
  "        kind : \"MapEntry\",\n"
  "        name : \"ambientColor\",\n"
  "        type : {\n"
  "          kind : \"String\",\n"
  "        },\n"
  "      },\n"
  "    ],\n"
  "  },\n"
  "}\n"
  ;

static Arcadia_String*
Arcadia_ADL_SphereMeshGeneratorReader_getTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorReader* self
  );

static Arcadia_ADL_SphereMeshGenerator*
Arcadia_ADL_SphereMeshGeneratorReader_read
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  );

static void
Arcadia_ADL_SphereMeshGeneratorReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorReader* self
  );

static void
Arcadia_ADL_SphereMeshGeneratorReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorReaderDispatch* self
  );

static void
Arcadia_ADL_SphereMeshGeneratorReader_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorReader* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_SphereMeshGeneratorReader_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_SphereMeshGeneratorReader_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_SphereMeshGeneratorReader_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.SphereMeshGeneratorReader", Arcadia_ADL_SphereMeshGeneratorReader,
                          u8"Arcadia.ADL.Reader", Arcadia_ADL_Reader,
                          &_typeOperations);

static Arcadia_String*
Arcadia_ADL_SphereMeshGeneratorReader_getTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorReader* self
  )
{ return self->TYPENAME; }

static Arcadia_ADL_SphereMeshGenerator*
Arcadia_ADL_SphereMeshGeneratorReader_read
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  )
{
  Arcadia_DDLS_ValidationContext_run(thread, self->validationContext, self->SCHEMANAME, input);

  Arcadia_String* type = Arcadia_ADL_Reader_getStringValue(thread, (Arcadia_DDL_MapNode*)input, self->TYPE);
  Arcadia_String* name = Arcadia_ADL_Reader_getStringValue(thread, (Arcadia_DDL_MapNode*)input, self->NAME);
  Arcadia_Real32Value radius = Arcadia_ADL_Reader_getReal32Value(thread, (Arcadia_DDL_MapNode*)input, self->RADIUS);
  Arcadia_Natural32Value segments = Arcadia_ADL_Reader_getNatural32Value(thread, (Arcadia_DDL_MapNode*)input, self->SEGMENTS);
  Arcadia_String* ambientColor = Arcadia_ADL_Reader_getStringValue(thread, (Arcadia_DDL_MapNode*)input, self->AMBIENTCOLOR);

  Arcadia_Value t = Arcadia_Value_makeObjectReferenceValue(self->TYPENAME);
  if (!Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &t)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_ADL_SphereMeshGenerator* definition =
    Arcadia_ADL_SphereMeshGenerator_create(thread, definitions, name, radius, segments, ambientColor);
  return definition;
}

static void
Arcadia_ADL_SphereMeshGeneratorReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_SphereMeshGeneratorReader);
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
  self->SCHEMANAME = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"SphereMeshGenerator");
  self->TYPENAME = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"SphereMeshGenerator");
  //
  self->RADIUS = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"radius");
  self->SEGMENTS = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"segments");
  self->AMBIENTCOLOR = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, Arcadia_Languages_StringTable_getOrCreate(thread), u8"ambientColor");
  //
  Arcadia_LeaveConstructor(Arcadia_ADL_SphereMeshGeneratorReader);
}

static void
Arcadia_ADL_SphereMeshGeneratorReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorReaderDispatch* self
  )
{
  ((Arcadia_ADL_ReaderDispatch*)self)->getTypeName = (Arcadia_String * (*)(Arcadia_Thread*, Arcadia_ADL_Reader*)) & Arcadia_ADL_SphereMeshGeneratorReader_getTypeName;
  ((Arcadia_ADL_ReaderDispatch*)self)->read = (Arcadia_ADL_Definition * (*)(Arcadia_Thread*, Arcadia_ADL_Reader*, Arcadia_ADL_Context*, Arcadia_ADL_Definitions*, Arcadia_DDL_Node*)) & Arcadia_ADL_SphereMeshGeneratorReader_read;
}

static void
Arcadia_ADL_SphereMeshGeneratorReader_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorReader* self
  )
{
  if (self->validationContext) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->validationContext);
  }
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
  if (self->RADIUS) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->RADIUS);
  }
  if (self->SEGMENTS) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->SEGMENTS);
  }
  if (self->AMBIENTCOLOR) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->AMBIENTCOLOR);
  }
}

Arcadia_ADL_SphereMeshGeneratorReader*
Arcadia_ADL_SphereMeshGeneratorReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_SphereMeshGeneratorReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_SphereMeshGeneratorReader);
}
