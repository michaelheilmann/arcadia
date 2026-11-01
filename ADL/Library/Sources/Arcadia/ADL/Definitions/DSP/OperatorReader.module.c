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
#include "Arcadia/ADL/Definitions/DSP/OperatorReader.module.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Definitions/DSP/Include.h"

#include <string.h>

static const char* SCHEMA =
  "{\n"
  "  kind : \"Schema\",\n"
  "  name : \"DSPOperator\",\n"
  "  definition : {\n"
  "    kind : \"Map\",\n"
  "    entries : [\n"
  "      { kind : \"MapEntry\", name : \"type\", type : { kind : \"String\" } },\n"
  "      { kind : \"MapEntry\", name : \"name\", type : { kind : \"String\" } },\n"
  "      { kind : \"MapEntry\", name : \"input\", type : { kind : \"String\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"inputs\", type : { kind : \"List\", entry : { kind : \"String\" } }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"gain\", type : { kind : \"Number\" }, optional : \"true\" },\n"
  "      { kind : \"MapEntry\", name : \"cutoff\", type : { kind : \"Number\" }, optional : \"true\" },\n"
  "    ],\n"
  "  },\n"
  "}\n";

static Arcadia_String* getTypeNameImpl(Arcadia_Thread*, Arcadia_ADL_DSPOperatorReader*);
static Arcadia_ADL_Definition* readImpl(Arcadia_Thread*, Arcadia_ADL_DSPOperatorReader*, Arcadia_ADL_Context*, Arcadia_ADL_Definitions*, Arcadia_DDL_Node*);
static Arcadia_DDL_Node* getOptionalNode(Arcadia_Thread*, Arcadia_DDL_MapNode*, Arcadia_String*);
static void constructImpl(Arcadia_Thread*, Arcadia_ADL_DSPOperatorReader*);
static void initializeDispatchImpl(Arcadia_Thread*, Arcadia_ADL_DSPOperatorReaderDispatch*);
static void visitImpl(Arcadia_Thread*, Arcadia_ADL_DSPOperatorReader*);

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

Arcadia_defineObjectType(u8"Arcadia.ADL.DSPOperatorReader", Arcadia_ADL_DSPOperatorReader,
                         u8"Arcadia.ADL.Reader", Arcadia_ADL_Reader, &typeOperations);

static Arcadia_String*
getTypeNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPOperatorReader* self
  )
{
  return Arcadia_String_createFromCxxString(thread, u8"DSP.Gain");
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
    Arcadia_DDL_MapEntryNode* entry = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)mapNode->entries, i, _Arcadia_DDL_MapEntryNode_getType(thread));
    Arcadia_Value keyValue = Arcadia_Value_makeObjectReferenceValue(key);
    if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)entry->key->value, &keyValue)) return entry->value;
  }
  return NULL;
}

static Arcadia_ADL_Definition*
readImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPOperatorReader* self,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input
  )
{
  Arcadia_DDL_MapNode* mapNode = (Arcadia_DDL_MapNode*)input;
  Arcadia_DDLS_ValidationContext_run(thread, self->validationContext, self->schemaName, input);
  Arcadia_String* type = Arcadia_ADL_Reader_getStringValue(thread, mapNode, self->typeKey);
  Arcadia_String* name = Arcadia_ADL_Reader_getStringValue(thread, mapNode, self->nameKey);
  Arcadia_DDL_Node* inputNode = getOptionalNode(thread, mapNode, self->inputKey);
  Arcadia_DDL_Node* inputsNode = getOptionalNode(thread, mapNode, self->inputsKey);
  Arcadia_DDL_Node* gainNode = getOptionalNode(thread, mapNode, self->gainKey);
  Arcadia_DDL_Node* cutoffNode = getOptionalNode(thread, mapNode, self->cutoffKey);
  Arcadia_ADL_DSPOperatorKind kind;
  Arcadia_ADL_Reference* inputReference = NULL;
  Arcadia_List* inputReferences = NULL;
  Arcadia_Real32Value parameter = 0.f;

  Arcadia_Value gainType = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"DSP.Gain"));
  Arcadia_Value lowPassType = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"DSP.LowPass"));
  Arcadia_Value mixType = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"DSP.Mix"));
  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &gainType)) {
    if (!inputNode || !gainNode || inputsNode || cutoffNode) goto invalidDefinition;
    kind = Arcadia_ADL_DSPOperatorKind_Gain;
    inputReference = Arcadia_ADL_Reference_create(thread, definitions, ((Arcadia_DDL_StringNode*)inputNode)->value);
    parameter = Arcadia_String_toReal32(thread, ((Arcadia_DDL_NumberNode*)gainNode)->value);
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &lowPassType)) {
    if (!inputNode || !cutoffNode || inputsNode || gainNode) goto invalidDefinition;
    kind = Arcadia_ADL_DSPOperatorKind_LowPass;
    inputReference = Arcadia_ADL_Reference_create(thread, definitions, ((Arcadia_DDL_StringNode*)inputNode)->value);
    parameter = Arcadia_String_toReal32(thread, ((Arcadia_DDL_NumberNode*)cutoffNode)->value);
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)type, &mixType)) {
    if (!inputsNode || inputNode || gainNode || cutoffNode) goto invalidDefinition;
    kind = Arcadia_ADL_DSPOperatorKind_Mix;
    inputReferences = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_DDL_ListNode* listNode = (Arcadia_DDL_ListNode*)inputsNode;
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)listNode->elements); i < n; ++i) {
      Arcadia_DDL_StringNode* item = (Arcadia_DDL_StringNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)listNode->elements, i, _Arcadia_DDL_StringNode_getType(thread));
      Arcadia_List_insertBackObjectReferenceValue(thread, inputReferences, (Arcadia_Object*)Arcadia_ADL_Reference_create(thread, definitions, item->value));
    }
    if (!Arcadia_Collection_getSize(thread, (Arcadia_Collection*)inputReferences)) goto invalidDefinition;
  } else {
    goto invalidDefinition;
  }
  return (Arcadia_ADL_Definition*)Arcadia_ADL_DSPOperatorDefinition_create(thread, definitions, name, kind, inputReference, inputReferences, parameter);

invalidDefinition:
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPOperatorReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_DSPOperatorReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDLS_DefaultReader* reader = (Arcadia_DDLS_DefaultReader*)Arcadia_DDLS_DefaultReader_create(thread);
  Arcadia_DDLS_Node* schema = Arcadia_DDLS_DefaultReader_run(thread, reader, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, SCHEMA, strlen(SCHEMA))));
  self->validationContext = Arcadia_DDLS_ValidationContext_create(thread);
  Arcadia_DDLS_ValidationContext_addSchema(thread, self->validationContext, (Arcadia_DDLS_SchemaNode*)schema);
  Arcadia_Languages_StringTable* strings = Arcadia_Languages_StringTable_getOrCreate(thread);
  self->typeKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"type");
  self->nameKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"name");
  self->inputKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"input");
  self->inputsKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"inputs");
  self->gainKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"gain");
  self->cutoffKey = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"cutoff");
  self->schemaName = Arcadia_Languages_StringTable_getOrCreateStringFromCxxString(thread, strings, u8"DSPOperator");
  Arcadia_LeaveConstructor(Arcadia_ADL_DSPOperatorReader);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPOperatorReaderDispatch* self
  )
{
  ((Arcadia_ADL_ReaderDispatch*)self)->getTypeName = (Arcadia_String*(*)(Arcadia_Thread*, Arcadia_ADL_Reader*))&getTypeNameImpl;
  ((Arcadia_ADL_ReaderDispatch*)self)->read = (Arcadia_ADL_Definition*(*)(Arcadia_Thread*, Arcadia_ADL_Reader*, Arcadia_ADL_Context*, Arcadia_ADL_Definitions*, Arcadia_DDL_Node*))&readImpl;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPOperatorReader* self
  )
{
  if (self->validationContext) Arcadia_Object_visit(thread, (Arcadia_Object*)self->validationContext);
  if (self->typeKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->typeKey);
  if (self->nameKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->nameKey);
  if (self->inputKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->inputKey);
  if (self->inputsKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->inputsKey);
  if (self->gainKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->gainKey);
  if (self->cutoffKey) Arcadia_Object_visit(thread, (Arcadia_Object*)self->cutoffKey);
  if (self->schemaName) Arcadia_Object_visit(thread, (Arcadia_Object*)self->schemaName);
}

Arcadia_ADL_DSPOperatorReader*
Arcadia_ADL_DSPOperatorReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_DSPOperatorReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_DSPOperatorReader);
}
