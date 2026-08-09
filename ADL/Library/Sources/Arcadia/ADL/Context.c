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
#include "Arcadia/ADL/Context.h"

#include "Arcadia/ADL/Definitions.h"
#include "Arcadia/ADL/Reader.h"

#include "Arcadia/ADL/Definitions/Audials/SampleBufferReader.module.h"

#include "Arcadia/ADL/Definitions/DSP/ConstantReader.module.h"
#include "Arcadia/ADL/Definitions/DSP/SawtoothWaveReader.module.h"
#include "Arcadia/ADL/Definitions/DSP/SineWaveReader.module.h"
#include "Arcadia/ADL/Definitions/DSP/WhiteNoiseReader.module.h"

#include "Arcadia/ADL/Definitions/Visuals/CheckerboardFillOperationReader.module.h"
#include "Arcadia/ADL/Definitions/Visuals/ColorReader.module.h"
#include "Arcadia/ADL/Definitions/Visuals/FillOperationReader.module.h"
#include "Arcadia/ADL/Definitions/Visuals/MaterialReader.module.h"
#include "Arcadia/ADL/Definitions/Visuals/ModelReader.module.h"
#include "Arcadia/ADL/Definitions/Visuals/MeshReader.module.h"
#include "Arcadia/ADL/Definitions/Visuals/PixelBufferReader.module.h"
#include "Arcadia/ADL/Definitions/Visuals/TextureReader.module.h"

static void
Arcadia_ADL_Context_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self
  );

static void
Arcadia_ADL_Context_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ContextDispatch* self
  );

static void
Arcadia_ADL_Context_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self
  );

static Arcadia_ADL_Context*
Arcadia_ADL_Context_create
  (
    Arcadia_Thread* thread
  );

static Arcadia_String*
getStringValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  );

static Arcadia_ADL_Definition*
readFromString
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* input,
    Arcadia_BooleanValue skipExisting
  );

static Arcadia_ADL_Definition*
readFromNode
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_MapNode* input,
    Arcadia_BooleanValue skipExisting
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_Context_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_Context_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_Context_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.Context", Arcadia_ADL_Context,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_ADL_Context_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_Context);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->readers = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  //
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_SampleBufferReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  //
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_ConstantReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_SawtoothWaveReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_SineWaveReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_WhiteNoiseReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  //
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_ColorReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_PixelBufferReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_PixelBufferOperations_FillOperationReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  //
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_MaterialReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_MeshReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_ModelReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  {
    Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_ADL_TextureReader_create(thread);
    Arcadia_Map_set(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Reader_getTypeName(thread, reader)), Arcadia_Value_makeObjectReferenceValue(reader), NULL, NULL);
  }
  //
  Arcadia_LeaveConstructor(Arcadia_ADL_Context);
}

static void
Arcadia_ADL_Context_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ContextDispatch* self
  )
{ }

static void
Arcadia_ADL_Context_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self
  )
{
  if (self->readers) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->readers);
  }
}

static Arcadia_ADL_Context*
Arcadia_ADL_Context_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_Context);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_Context);
}

static Arcadia_String*
getStringValue
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  )
{
  Arcadia_String* value = NULL;
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
    Arcadia_DDL_NameNode* keyNode = mapEntryNode->key;
    Arcadia_Value t = Arcadia_Value_makeObjectReferenceValue(key);
    if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)keyNode->value, &t)) {
      Arcadia_DDL_Node* valueNode = mapEntryNode->value;
      if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)valueNode, _Arcadia_DDL_StringNode_getType(thread))) {
        Arcadia_logf(Arcadia_LogFlags_Error, u8"map entry with name `type` has no `string` value\n");
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      if (value) {
        Arcadia_logf(Arcadia_LogFlags_Error, u8"map entry with name `type` already specified\n");
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      value = ((Arcadia_DDL_StringNode*)valueNode)->value;
    }
  }
  if (!value) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"map entry with name `type` not specified\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  return value;
}

static Arcadia_ADL_Definition*
readFromString
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* input,
    Arcadia_BooleanValue skipExisting
  )
{
  Arcadia_DDL_DefaultReader* reader = (Arcadia_DDL_DefaultReader*)Arcadia_DDL_DefaultReader_create(thread);
  Arcadia_UnicodeCodePointReader* x = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_String_toByteArray(thread, input)));
  Arcadia_DDL_Node* node = Arcadia_DDL_DefaultReader_run(thread, reader, x);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDL_MapNode_getType(thread))) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"argument input is not a of type Arcadia.DDL.MapNode\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  return readFromNode(thread, self, definitions, (Arcadia_DDL_MapNode*)node, skipExisting);
}


static Arcadia_ADL_Definition*
readFromNode
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_MapNode* input,
    Arcadia_BooleanValue skipExisting
  )
{
  // (1) Retrieve reader.
  Arcadia_String* entryName = Arcadia_String_createFromCxxString(thread, "type");
  Arcadia_String* entryValue = getStringValue(thread, self, input, entryName);
  Arcadia_Value readerValue = Arcadia_Map_get(thread, self->readers, Arcadia_Value_makeObjectReferenceValue(entryValue));
  if (Arcadia_Value_isVoidValue(&readerValue)) {
    Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, "no reader registered for type `")));
    Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeObjectReferenceValue(entryValue));
    Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, "`")));
    Arcadia_Languages_DiagnosticsOld_emit(thread, stringBuffer);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_Reader* reader = (Arcadia_ADL_Reader*)Arcadia_Value_getObjectReferenceValue(&readerValue);
  // (2) Read the definition.
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Reader_read(thread, reader, self, definitions, (Arcadia_DDL_Node*)input);
  // (3) Assert there is no definition of the same name in the definitions already.
  Arcadia_Value temporary = Arcadia_Map_get(thread, definitions->definitions, Arcadia_Value_makeObjectReferenceValue(definition->name));
  if (!Arcadia_Value_isVoidValue(&temporary)) {
    if (!skipExisting) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Exists);
      Arcadia_Thread_jump(thread);
    }
    return (Arcadia_ADL_Definition*)Arcadia_Value_getObjectReferenceValueChecked(thread, temporary, _Arcadia_ADL_Definition_getType(thread));
  } else {
    // (4) Register the definition.
    Arcadia_Map_set(thread, definitions->definitions, Arcadia_Value_makeObjectReferenceValue(definition->name), Arcadia_Value_makeObjectReferenceValue(definition), NULL, NULL);
    // (5) Return the definition.
    return definition;
  }
}

static Arcadia_ADL_Context* g_instance = NULL;

static void
Arcadia_ADL_Context_destroyCallback
  (
    void* observer,
    void* observed
  )
{ g_instance = NULL; }

Arcadia_ADL_Context*
Arcadia_ADL_Context_getOrCreate
  (
    Arcadia_Thread* thread
  )
{
  if (!g_instance) {
    Arcadia_ADL_Context* instance = Arcadia_ADL_Context_create(thread);
    Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)instance, NULL, (void (*)(void*, Arcadia_Object*)) & Arcadia_ADL_Context_destroyCallback);
    g_instance = instance;
  }
  return g_instance;
}

Arcadia_ADL_Definition*
Arcadia_ADL_Context_readFromNode
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input,
    Arcadia_BooleanValue skipExisting
  )
{
  if (!input) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"argument input is a null pointer\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)input, _Arcadia_DDL_MapNode_getType(thread))) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"argument input is not a of type Arcadia.DDL.MapNode\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  return readFromNode(thread, self, definitions, (Arcadia_DDL_MapNode*)input, skipExisting);
}

Arcadia_ADL_Definition*
Arcadia_ADL_Context_readFromString
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* input,
    Arcadia_BooleanValue skipExisting
  )
{
  if (!input) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"argument input is a null pointer\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return readFromString(thread, self, definitions, input, skipExisting);
}
