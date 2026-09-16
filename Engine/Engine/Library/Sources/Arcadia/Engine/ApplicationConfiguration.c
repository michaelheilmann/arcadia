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

#include "Arcadia/Engine/ApplicationConfiguration.h"

#include <string.h>
#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/Logging/Include.h"

static Arcadia_BooleanValue
isBooleanNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node
  )
{ return Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, (Arcadia_Object*)node), _Arcadia_DDL_BooleanNode_getType(thread)); }

static Arcadia_BooleanValue
isNumberNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node
  )
{ return Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, (Arcadia_Object*)node), _Arcadia_DDL_NumberNode_getType(thread)); }

static Arcadia_BooleanValue
isStringNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node
  )
{ return Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, (Arcadia_Object*)node), _Arcadia_DDL_StringNode_getType(thread)); }

static Arcadia_BooleanValue
isMapEntryNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node
  )
{ return Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, (Arcadia_Object*)node), _Arcadia_DDL_MapEntryNode_getType(thread)); }

static Arcadia_BooleanValue
isMapNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node
  )
{ return Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, (Arcadia_Object*)node), _Arcadia_DDL_MapNode_getType(thread)); }

static Arcadia_SizeValue
_findIndex
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* self,
    Arcadia_SizeValue index,
    Arcadia_String* key
  )
{
  for (Arcadia_SizeValue i = index, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* entry = Arcadia_List_getObjectReferenceValueAt(thread, self->entries, i);
    Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(entry->key->value);
    Arcadia_Value b = Arcadia_Value_makeObjectReferenceValue(key);
    if (Arcadia_Value_isEqualTo(thread, &a, &b)) {
      return i;
    }
  }
  return Arcadia_SizeValue_Maximum;
}

static void
_createBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* node,
    char const* name,
    Arcadia_BooleanValue value
  )
{
  Arcadia_String* nameString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, name, strlen(name))));
  Arcadia_SizeValue i = _findIndex(thread, node, 0, nameString);
  Arcadia_DDL_MapEntryNode* e = NULL;
  if (i != Arcadia_SizeValue_Maximum) {
    Arcadia_List_removeAt(thread, node->entries, i, 1);
  }
  Arcadia_DDL_NameNode* k = Arcadia_DDL_NameNode_create(thread, nameString);
  Arcadia_DDL_Node* v = (Arcadia_DDL_Node*)Arcadia_DDL_BooleanNode_createBoolean(thread, value);
  e = Arcadia_DDL_MapEntryNode_create(thread, k, v);
  Arcadia_List_insertBackObjectReferenceValue(thread, node->entries, (Arcadia_ObjectReferenceValue)e);
}

static void
_createInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* node,
    char const* name,
    Arcadia_Integer32Value value
  )
{
  Arcadia_String* nameString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, name, strlen(name))));
  Arcadia_SizeValue i = _findIndex(thread, node, 0, nameString);
  Arcadia_DDL_MapEntryNode* e = NULL;
  if (i != Arcadia_SizeValue_Maximum) {
    Arcadia_List_removeAt(thread, node->entries, i, 1);
  }
  Arcadia_DDL_NameNode* k = Arcadia_DDL_NameNode_create(thread, nameString);
  Arcadia_DDL_Node* v = (Arcadia_DDL_Node*)Arcadia_DDL_NumberNode_createInteger32(thread, value);
  e = Arcadia_DDL_MapEntryNode_create(thread, k, v);
  Arcadia_List_insertBackObjectReferenceValue(thread, node->entries, (Arcadia_ObjectReferenceValue)e);
}

static void
_createString
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* node,
    char const* name,
    Arcadia_String* value
  )
{
  Arcadia_String* nameString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, name, strlen(name))));
  Arcadia_SizeValue i = _findIndex(thread, node, 0, nameString);
  Arcadia_DDL_MapEntryNode* e = NULL;
  if (i != Arcadia_SizeValue_Maximum) {
    Arcadia_List_removeAt(thread, node->entries, i, 1);
  }
  Arcadia_DDL_NameNode* k = Arcadia_DDL_NameNode_create(thread, nameString);
  Arcadia_DDL_Node* v = (Arcadia_DDL_Node*)Arcadia_DDL_StringNode_createString(thread, value);
  e = Arcadia_DDL_MapEntryNode_create(thread, k, v);
  Arcadia_List_insertBackObjectReferenceValue(thread, node->entries, (Arcadia_ObjectReferenceValue)e);
}

/// - if x[name] is a map node, then return x[name]
/// - if x[name] is not a map node or x[name] does not exit, x[name] = {} and return x[name]
static Arcadia_DDL_MapNode*
_getOrCreateMap
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* node,
    char const* name
  )
{
  Arcadia_String* nameString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, name, strlen(name))));
  Arcadia_SizeValue i = _findIndex(thread, node, 0, nameString);
  Arcadia_DDL_MapEntryNode* e = NULL;
  if (i != Arcadia_SizeValue_Maximum) {
    e = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueAt(thread, node->entries, i);
    if (!isMapNode(thread, e->value)) {
      Arcadia_List_removeAt(thread, node->entries, i, 1);
      e = NULL;
    }
  }
  if (!e) {
    Arcadia_DDL_NameNode* k = Arcadia_DDL_NameNode_create(thread, nameString);
    Arcadia_DDL_Node* v = (Arcadia_DDL_Node*)Arcadia_DDL_MapNode_create(thread);
    e = Arcadia_DDL_MapEntryNode_create(thread, k, v);
    Arcadia_List_insertBackObjectReferenceValue(thread, node->entries, (Arcadia_ObjectReferenceValue)e);
  }
  return (Arcadia_DDL_MapNode*)e->value;
}

void
Arcadia_Engine_ApplicationConfiguration_setBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames,
    Arcadia_Integer32Value value
  )
{
  if (!isMapNode(thread, node)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_MapNode* current = (Arcadia_DDL_MapNode*)node;
  for (Arcadia_SizeValue i = 0, n = numberOfNames - 1; i < n; ++i) {
    current = _getOrCreateMap(thread, current, names[i]);
  }
  if (!current) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  _createBoolean(thread, current, names[numberOfNames - 1], value);
}

Arcadia_BooleanValue
Arcadia_Engine_ApplicationConfiguration_getBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames
  )
{
  Arcadia_DDL_Node* current = node;
  for (Arcadia_SizeValue i = 0, n = numberOfNames; i < n; ++i) {
    if (!isMapNode(thread, current)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_String* nameString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, names[i], strlen(names[i]))));
    Arcadia_SizeValue i = _findIndex(thread, (Arcadia_DDL_MapNode*)current, 0, nameString);
    if (i == Arcadia_SizeValue_Maximum) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
      Arcadia_Thread_jump(thread);
    }
    current = Arcadia_List_getObjectReferenceValueAt(thread, ((Arcadia_DDL_MapNode*)current)->entries, i);
    if (!isMapEntryNode(thread, current)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
      Arcadia_Thread_jump(thread);
    }
    current = ((Arcadia_DDL_MapEntryNode*)current)->value;
  }
  if (!current) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  if (!isBooleanNode(thread, current)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_String_toBoolean(thread, ((Arcadia_DDL_NumberNode*)(current))->value);
}

void
Arcadia_Engine_ApplicationConfiguration_setInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames,
    Arcadia_Integer32Value value
  )
{
  if (!isMapNode(thread, node)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_MapNode* current = (Arcadia_DDL_MapNode*)node;
  for (Arcadia_SizeValue i = 0, n = numberOfNames - 1; i < n; ++i) {
    current = _getOrCreateMap(thread, current, names[i]);
  }
  if (!current) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  _createInteger32(thread, current, names[numberOfNames - 1], value);
}

Arcadia_Integer32Value
Arcadia_Engine_ApplicationConfiguration_getInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames
  )
{
  Arcadia_DDL_Node* current = node;
  for (Arcadia_SizeValue i = 0, n = numberOfNames; i < n; ++i) {
    if (!isMapNode(thread, current)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_String* nameString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, names[i], strlen(names[i]))));
    Arcadia_SizeValue i = _findIndex(thread, (Arcadia_DDL_MapNode*)current, 0, nameString);
    if (i == Arcadia_SizeValue_Maximum) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
      Arcadia_Thread_jump(thread);
    }
    current = Arcadia_List_getObjectReferenceValueAt(thread, ((Arcadia_DDL_MapNode*)current)->entries, i);
    if (!isMapEntryNode(thread, current)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
      Arcadia_Thread_jump(thread);
    }
    current = ((Arcadia_DDL_MapEntryNode*)current)->value;
  }
  if (!current) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  if (!isNumberNode(thread, current)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_String_toInteger32(thread, ((Arcadia_DDL_NumberNode*)(current))->value);
}

void
Arcadia_Engine_ApplicationConfiguration_setString
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames,
    Arcadia_String* value
  )
{
  if (!isMapNode(thread, node)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_MapNode* current = (Arcadia_DDL_MapNode*)node;
  for (Arcadia_SizeValue i = 0, n = numberOfNames - 1; i < n; ++i) {
    current = _getOrCreateMap(thread, current, names[i]);
  }
  if (!current) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  _createString(thread, current, names[numberOfNames - 1], value);
}

Arcadia_String*
Arcadia_Engine_ApplicationConfiguration_getString
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames
  )
{
  Arcadia_DDL_Node* current = node;
  for (Arcadia_SizeValue i = 0, n = numberOfNames; i < n; ++i) {
    if (!isMapNode(thread, current)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_String* nameString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, names[i], strlen(names[i]))));
    Arcadia_SizeValue i = _findIndex(thread, (Arcadia_DDL_MapNode*)current, 0, nameString);
    if (i == Arcadia_SizeValue_Maximum) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
      Arcadia_Thread_jump(thread);
    }
    current = Arcadia_List_getObjectReferenceValueAt(thread, ((Arcadia_DDL_MapNode*)current)->entries, i);
    if (!isMapEntryNode(thread, current)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
      Arcadia_Thread_jump(thread);
    }
    current = ((Arcadia_DDL_MapEntryNode*)current)->value;
  }
  if (!current) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  if (!isStringNode(thread, current)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  return ((Arcadia_DDL_NumberNode*)(current))->value;
}

void
Arcadia_Engine_ApplicationConfiguration_setSection
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames
  )
{
  if (!isMapNode(thread, node)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_MapNode* current = (Arcadia_DDL_MapNode*)node;
  for (Arcadia_SizeValue i = 0, n = numberOfNames; i < n; ++i) {
    current = _getOrCreateMap(thread, current, names[i]);
  }
}

void
Arcadia_Engine_ApplicationConfiguration_saveConfiguration
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* configuration
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);

  // (1) Ensure the organization's configuration folder exists.
  Arcadia_FilePath* file = Arcadia_FileSystem_getConfigurationDirectory(thread, fileSystem);
  Arcadia_FileSystem_createDirectoryFile(thread, fileSystem, file);

  // (2) Ensure the product's configuration folder exists.
  file = Arcadia_FilePath_clone(thread, file);
  Arcadia_FilePath_append(thread, file, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Demo")));
  Arcadia_FileSystem_createDirectoryFile(thread, fileSystem, file);

  // (3) Create/update configuration file.
  file = Arcadia_FilePath_clone(thread, file);
  Arcadia_FilePath_append(thread, file, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Configuration.txt")));

  Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Unicode_Encoder* encoder = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  Arcadia_DataDefinitionLanguage_Unparser* unparser = Arcadia_DataDefinitionLanguage_Unparser_create(thread, encoder);
  Arcadia_DataDefinitionLanguage_Unparser_run(thread, unparser, (Arcadia_DDL_Node*)configuration, byteBuffer);
  Arcadia_FileSystem_setFileContents(thread, fileSystem, file, byteBuffer);
}

Arcadia_DDL_MapNode*
Arcadia_Engine_ApplicationConfiguration_loadConfiguration
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FilePath* file;

  // (1) Ensure the organization's configuration folder exists.
  file = Arcadia_FileSystem_getConfigurationDirectory(thread, fileSystem);
  Arcadia_FileSystem_createDirectoryFile(thread, fileSystem, file);

  // (2) Ensure the product's configuration folder exists.
  file = Arcadia_FilePath_clone(thread, file);
  Arcadia_FilePath_append(thread, file, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Demo")));
  Arcadia_FileSystem_createDirectoryFile(thread, fileSystem, file);

  // (3) Ensure the product's configuration file exists.
  file = Arcadia_FilePath_clone(thread, file);
  Arcadia_FilePath_append(thread, file, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Configuration.txt")));
  if (!Arcadia_FileSystem_regularFileExists(thread, fileSystem, file)) {
    // Create a configuration file with an empty map.
    Arcadia_Unicode_Encoder* encoder = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
    Arcadia_DataDefinitionLanguage_Unparser* unparser = Arcadia_DataDefinitionLanguage_Unparser_create(thread, encoder);
    Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
    Arcadia_DataDefinitionLanguage_Unparser_run(thread, unparser, (Arcadia_DDL_Node*)Arcadia_DDL_MapNode_create(thread), byteBuffer);
    Arcadia_FileSystem_setFileContents(thread, fileSystem, file, byteBuffer);
  }

  // (4) Product's configuration file exists.
  // If syntactical or semantical analysis fails, replace its contents by an empty configuration file.
  Arcadia_DDL_MapNode* rootNode = NULL;
  Arcadia_JumpTarget jumpTarget;

  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* semanticalAnalysis = Arcadia_DataDefinitionLanguage_SemanticalAnalysis_create(thread);
    Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_FileSystem_getFileContents(thread, fileSystem, file);
    Arcadia_ByteArray* x = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, Arcadia_ByteArrayBuilder_getBytes(thread, byteBuffer), Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, byteBuffer)));
    Arcadia_UnicodeCodePointReader* y = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, x));
    Arcadia_DDL_Parser* parser = Arcadia_DDL_Parser_create(thread, Arcadia_DDL_Scanner_create(thread, Arcadia_Languages_StringTable_getOrCreate(thread),
                                                                                                      Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread))));
    Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)parser, y);
    Arcadia_DDL_Node* node = (Arcadia_DDL_Node*)Arcadia_Value_getObjectReferenceValueChecked(thread, Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)parser), _Arcadia_DDL_Node_getType(thread));
    if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, (Arcadia_Object*)node), _Arcadia_DDL_MapNode_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis_run(thread, semanticalAnalysis, node);
    rootNode = (Arcadia_DDL_MapNode*)node;
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    // Create a configuration file with an empty map.
    Arcadia_Unicode_Encoder* encoder = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
    Arcadia_DataDefinitionLanguage_Unparser* unparser = Arcadia_DataDefinitionLanguage_Unparser_create(thread, encoder);
    Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
    rootNode = Arcadia_DDL_MapNode_create(thread);
    Arcadia_DataDefinitionLanguage_Unparser_run(thread, unparser, (Arcadia_DDL_Node*)rootNode, byteBuffer);
    Arcadia_FileSystem_setFileContents(thread, fileSystem, file, byteBuffer);
  }
  return rootNode;
}
