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
#include "Arcadia/ADL/Definitions/Audials/DSPStreamProviderReader.module.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Definitions/Audials/Include.h"
#include <string.h>

static const char* SCHEMA =
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"DSPStreamProvider\",\n"
  "  definition : {\n"
  "    kind : \"Map\",\n"
  "    entries : [\n"
  "      { kind : \"MapEntry\", name : \"type\", type : { kind : \"String\" } },\n"
  "      { kind : \"MapEntry\", name : \"name\", type : { kind : \"String\" } },\n"
  "      { kind : \"MapEntry\", name : \"dsp\", type : { kind : \"String\" } },\n"
  "      { kind : \"MapEntry\", name : \"sampleRate\", type : { kind : \"Number\" } },\n"
  "      { kind : \"MapEntry\", name : \"duration\", type : { kind : \"Number\" }, optional : \"true\" },\n"
  "    ],\n"
  "  },\n"
  "}\n";

static Arcadia_String*
getTypeNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderReader* self
  );

static Arcadia_ADL_Definition*
readImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderReader* self,
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
    Arcadia_ADL_DSPStreamProviderReader* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderReaderDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderReader* self
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

Arcadia_defineObjectType(u8"Arcadia.ADL.DSPStreamProviderReader", Arcadia_ADL_DSPStreamProviderReader,
                         u8"Arcadia.ADL.Reader", Arcadia_ADL_Reader,
                         &typeOperations);

static Arcadia_String*
getTypeNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderReader* self
  )
{
  return Arcadia_String_createFromCxxString(thread, u8"DSPStreamProvider");
}

static Arcadia_ADL_Definition*
readImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  )
{
  Arcadia_DDL_MapNode* mapNode = (Arcadia_DDL_MapNode*)input;
  Arcadia_DDLS_ValidationContext_run(thread, self->validationContext, self->schemaName, input);
  Arcadia_String* type = Arcadia_ADL_Reader_getStringValue(thread, mapNode, self->typeKey);
  Arcadia_Value expectedType = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"DSPStreamProvider"));
  if (!Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &expectedType)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_String* name = Arcadia_ADL_Reader_getStringValue(thread, mapNode, self->nameKey);
  Arcadia_String* dspName = Arcadia_ADL_Reader_getStringValue(thread, mapNode, self->dspKey);
  Arcadia_Natural32Value sampleRate = Arcadia_ADL_Reader_getNatural32Value(thread, mapNode, self->sampleRateKey);
  Arcadia_DDL_Node* durationNode = getOptionalNode(thread, mapNode, self->durationKey);
  Arcadia_BooleanValue hasDuration = durationNode ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
  Arcadia_Natural32Value duration = 0;
  if (durationNode) {
    duration = Arcadia_String_toNatural32(thread, ((Arcadia_DDL_NumberNode*)durationNode)->value);
  }
  if (!sampleRate || (hasDuration && !duration)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_ADL_Definition*)Arcadia_ADL_DSPStreamProviderDefinition_create
    (
      thread,
      definitions,
      name,
      Arcadia_ADL_Reference_create(thread, definitions, dspName),
      sampleRate,
      hasDuration,
      duration
    );
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
    Arcadia_ADL_DSPStreamProviderReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_DSPStreamProviderReader);
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
  self->dspKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"dsp");
  self->sampleRateKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"sampleRate");
  self->durationKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"duration");
  self->schemaName = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"DSPStreamProvider");
  Arcadia_LeaveConstructor(Arcadia_ADL_DSPStreamProviderReader);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderReaderDispatch* self
  )
{
  ((Arcadia_ADL_ReaderDispatch*)self)->getTypeName = (Arcadia_String*(*)(Arcadia_Thread*, Arcadia_ADL_Reader*))&getTypeNameImpl;
  ((Arcadia_ADL_ReaderDispatch*)self)->read = (Arcadia_ADL_Definition*(*)(Arcadia_Thread*, Arcadia_ADL_Reader*, Arcadia_ADL_Context*, Arcadia_ADL_Definitions*, Arcadia_DDL_Node*))&readImpl;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderReader* self
  )
{
  if (self->validationContext) Arcadia_Object_visit(thread, (Arcadia_Object*)self->validationContext);
  if (self->typeKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->typeKey);
  if (self->nameKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->nameKey);
  if (self->dspKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->dspKey);
  if (self->sampleRateKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->sampleRateKey);
  if (self->durationKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->durationKey);
  if (self->schemaName) Arcadia_Object_visit(thread, (Arcadia_Object*)self->schemaName);
}

Arcadia_ADL_DSPStreamProviderReader*
Arcadia_ADL_DSPStreamProviderReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_DSPStreamProviderReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_DSPStreamProviderReader);
}
