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
#include "Arcadia/ADL/Definitions/Audials/SoundSourceNodeReader.module.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Definitions/Audials/Include.h"

#include <string.h>

static const char* SCHEMA =
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"AudialsNode\",\n"
  "  definition : {\n"
  "    kind : \"Map\",\n"
  "    entries : [\n"
  "      { kind : \"MapEntry\", name : \"type\", type : { kind : \"String\" } },\n"
  "      { kind : \"MapEntry\", name : \"name\", type : { kind : \"String\" } },\n"
  "      { kind : \"MapEntry\", name : \"sampleBuffer\", type : { kind : \"String\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"provider\", type : { kind : \"String\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"isLooping\", type : { kind : \"Boolean\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"volume\", type : { kind : \"Number\" }, optional : \"true\" },\n"
  "    ],\n"
  "  },\n"
  "}\n";

static Arcadia_String*
getTypeNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeReader* self
  );

static Arcadia_ADL_Definition*
readImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  );

static Arcadia_DDL_Node*
getOptionalNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  );

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeReader* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeReaderDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeReader* self
  );

static const Arcadia_ObjectType_Operations objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.SoundSourceNodeReader", Arcadia_ADL_SoundSourceNodeReader,
                         u8"Arcadia.ADL.Reader", Arcadia_ADL_Reader,
                         &typeOperations);

static Arcadia_String*
getTypeNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeReader* self
  )
{
  return Arcadia_String_createFromCxxString(thread, u8"SoundSourceNode");
}

static Arcadia_ADL_Definition*
readImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  )
{
  Arcadia_DDL_MapNode* mapNode = (Arcadia_DDL_MapNode*)input;
  Arcadia_DDLS_ValidationContext_run(thread, self->validationContext, self->schemaName, input);
  Arcadia_String* type = Arcadia_ADL_Reader_getStringValue(thread, mapNode, self->typeKey);
  Arcadia_String* name = Arcadia_ADL_Reader_getStringValue(thread, mapNode, self->nameKey);
  Arcadia_String* sourceName = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, self->sampleBufferKey);
  Arcadia_String* providerName = Arcadia_ADL_Reader_getStringValueOptional(thread, mapNode, self->providerKey);
  Arcadia_DDL_Node* loopingNode = getOptionalNode(thread, mapNode, self->isLoopingKey);
  Arcadia_DDL_Node* volumeNode = getOptionalNode(thread, mapNode, self->volumeKey);
  Arcadia_Real32Value volume = 1.f;
  Arcadia_BooleanValue isLooping = Arcadia_BooleanValue_False;
  if (volumeNode) {
    volume = Arcadia_String_toReal32(thread, ((Arcadia_DDL_NumberNode*)volumeNode)->value);
  }
  if (loopingNode) {
    isLooping = Arcadia_String_toBoolean(thread, ((Arcadia_DDL_BooleanNode*)loopingNode)->value);
  }
  Arcadia_Value sourceType = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"SoundSourceNode"));
  Arcadia_Value streamingType = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"StreamingSoundSourceNode"));
  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &sourceType)) {
    if (!sourceName || providerName) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    return (Arcadia_ADL_Definition*)Arcadia_ADL_SoundSourceNodeDefinition_create
      (
        thread,
        definitions,
        name,
        Arcadia_ADL_Reference_create(thread, definitions, sourceName),
        isLooping,
        volume
      );
  }
  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &streamingType)) {
    if (!providerName || sourceName || loopingNode) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    return (Arcadia_ADL_Definition*)Arcadia_ADL_StreamingSoundSourceNodeDefinition_create
      (
        thread,
        definitions,
        name,
        Arcadia_ADL_Reference_create(thread, definitions, providerName),
        volume
      );
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

static Arcadia_DDL_Node*
getOptionalNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)mapNode->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* entry = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueCheckedAt
      (
        thread,
        (Arcadia_List*)mapNode->entries,
        i,
        _Arcadia_DDL_MapEntryNode_getType(thread)
      );
    Arcadia_Value keyValue = Arcadia_Value_makeObjectReferenceValue(key);
    if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)entry->key->value, &keyValue)) {
      return entry->value;
    }
  }
  return NULL;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_SoundSourceNodeReader);
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
  self->validationContext = Arcadia_DDLS_ValidationContext_create(thread);
  Arcadia_DDLS_ValidationContext_addSchema(thread, self->validationContext, (Arcadia_DDLS_SchemaNode*)node);
  Arcadia_Languages_StringTable* strings = Arcadia_Languages_StringTable_getOrCreate(thread);
  self->typeKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"type");
  self->nameKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"name");
  self->sampleBufferKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"sampleBuffer");
  self->providerKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"provider");
  self->isLoopingKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"isLooping");
  self->volumeKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"volume");
  self->schemaName = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"AudialsNode");
  Arcadia_LeaveConstructor(Arcadia_ADL_SoundSourceNodeReader);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeReaderDispatch* self
  )
{
  ((Arcadia_ADL_ReaderDispatch*)self)->getTypeName = (Arcadia_String*(*)(Arcadia_Thread*, Arcadia_ADL_Reader*))&getTypeNameImpl;
  ((Arcadia_ADL_ReaderDispatch*)self)->read = (Arcadia_ADL_Definition*(*)(Arcadia_Thread*, Arcadia_ADL_Reader*, Arcadia_ADL_Context*, Arcadia_ADL_Definitions*, Arcadia_DDL_Node*))&readImpl;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeReader* self
  )
{
  if (self->validationContext) Arcadia_Object_visit(thread, (Arcadia_Object*)self->validationContext);
  if (self->typeKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->typeKey);
  if (self->nameKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->nameKey);
  if (self->sampleBufferKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->sampleBufferKey);
  if (self->providerKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->providerKey);
  if (self->isLoopingKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->isLoopingKey);
  if (self->volumeKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->volumeKey);
  if (self->schemaName) Arcadia_Object_visit(thread, (Arcadia_Object*)self->schemaName);
}

Arcadia_ADL_SoundSourceNodeReader*
Arcadia_ADL_SoundSourceNodeReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_SoundSourceNodeReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_SoundSourceNodeReader);
}
