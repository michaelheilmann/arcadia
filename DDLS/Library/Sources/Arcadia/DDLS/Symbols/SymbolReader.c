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

#include "Arcadia/DDLS/Symbols/SymbolReader.h"

#include "Arcadia/DDLS/Symbols/Scope.h"
#include "Arcadia/DDLS/Extensions.h"

#include "Arcadia/DDLS/Symbols/AnySymbol.h"
#include "Arcadia/DDLS/Symbols/ChoiceSymbol.h"
#include "Arcadia/DDLS/Symbols/ListSymbol.h"
#include "Arcadia/DDLS/Symbols/MapSymbol.h"
#include "Arcadia/DDLS/Symbols/MapEntrySymbol.h"
#include "Arcadia/DDLS/Symbols/ScalarSymbol.h"
#include "Arcadia/DDLS/Symbols/SchemaReferenceSymbol.h"
#include "Arcadia/DDLS/Symbols/SchemaSymbol.h"

#include <assert.h>

static Arcadia_String*
readString
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_Value name
  );

static void
openScope
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static void
closeScope
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_AnySymbol*
readAnySymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_ScalarSymbol*
readBooleanSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_ChoiceSymbol*
readChoiceSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_ListSymbol*
readListSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_MapEntrySymbol*
readMapEntrySymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_MapSymbol*
readMapSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_ScalarSymbol*
readNumberSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_SchemaReferenceSymbol*
readSchemaReferenceSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_ScalarSymbol*
readStringSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_ScalarSymbol*
readVoidSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_Symbol*
readSymbol0
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static Arcadia_DDLS_Symbol*
readSchemaSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  );

static void
Arcadia_DDLS_SymbolReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self
  );

static void
Arcadia_DDLS_SymbolReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReaderDispatch* self
  );

static void
Arcadia_DDLS_SymbolReader_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_SymbolReader_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_SymbolReader_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_SymbolReader_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDLS_SymbolReader_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_SymbolReader_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_SymbolReader_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.SymbolReader", Arcadia_DDLS_SymbolReader,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_DDLS_SymbolReader_typeOperations);

static Arcadia_String*
readString
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_Value name
  )
{
  assert(NULL != self);
  // "name" must be a string.
  if (!Arcadia_Value_isInstanceOf(thread, &name, _Arcadia_String_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value k = name;
  Arcadia_Value v = Arcadia_Map_get(thread, self->scope->symbols, k);
  if (!Arcadia_Value_isInstanceOf(thread, &v, _Arcadia_DDL_StringNode_getType(thread))) {
    // Get the "name" string to emit an error meessage.
    Arcadia_String* s = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&name);
    if (Arcadia_String_getNumberOfBytes(thread, s) > 32) {
      Arcadia_logf(Arcadia_LogFlags_Error, u8"`%.*s...` not specified\n", 32, Arcadia_String_getBytes(thread, s));
    } else {
      Arcadia_logf(Arcadia_LogFlags_Error, u8"`%.*s` not specified\n", Arcadia_String_getNumberOfBytes(thread, s), Arcadia_String_getBytes(thread, s));
    }
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_StringNode* o = (Arcadia_DDL_StringNode*)Arcadia_Value_getObjectReferenceValue(&v);

  assert(NULL != o);
  assert(NULL != o->value);

  return o->value;
}

static void
openScope
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  self->scope = Arcadia_DDLS_Scope_create(thread, self->scope);
  // 1: Ensure no two map entries with the same key exist.
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)source->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* sourceEntry =
      (Arcadia_DDL_MapEntryNode*)
      Arcadia_List_getObjectReferenceValueCheckedAt(thread, source->entries, i, _Arcadia_DDL_MapEntryNode_getType(thread));
    Arcadia_Value k = Arcadia_Value_makeObjectReferenceValue(sourceEntry->key->value);
    Arcadia_Value v = Arcadia_Map_get(thread, self->scope->symbols, k);
    if (!Arcadia_Value_isVoidValue(&v)) {
      Arcadia_String* s = sourceEntry->key->value;
      if (Arcadia_String_getNumberOfBytes(thread, s) > 32) {
        Arcadia_logf(Arcadia_LogFlags_Error, u8"`%.*s...` already specified\n", 32, Arcadia_String_getBytes(thread, s));
      } else {
        Arcadia_logf(Arcadia_LogFlags_Error, u8"`%.*s` already specified\n", Arcadia_String_getNumberOfBytes(thread, s), Arcadia_String_getBytes(thread, s));
      }
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    v = Arcadia_Value_makeObjectReferenceValue(sourceEntry->value);
    Arcadia_Map_set(thread, self->scope->symbols, k, v, NULL, NULL);
  }
}

static void
closeScope
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  self->scope = self->scope->enclosing;
}

static void
Arcadia_DDLS_SymbolReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_SymbolReader);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->scope = Arcadia_DDLS_Scope_create(thread, NULL);
  //
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  self->stringTable = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Languages_StringTable_getType(thread));
#define Define(Variable, Text) \
  Arcadia_StringBuilder_clear(thread, stringBuffer); \
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, Text); \
  self->Variable = Arcadia_Value_makeObjectReferenceValue(Arcadia_Languages_StringTable_getOrCreateString(thread, self->stringTable, stringBuffer));

  Define(ANY, u8"Any");
  Define(BOOLEAN, u8"Boolean");
  Define(CHOICE, u8"Choice");
  Define(CHOICES, u8"choices");
  Define(LIST, u8"List");
  Define(MAP, u8"Map");
  Define(MAPENTRY, u8"MapEntry");
  Define(NUMBER, u8"Number");
  Define(SCHEMA, u8"Schema");
  Define(SCHEMAREFERENCE, u8"SchemaReference");
  Define(STRING, u8"String");
  Define(VOID, u8"Void");

  Define(DEFINITION, u8"definition");
  Define(NAME, u8"name");
  Define(TYPE, u8"type");
  Define(ELEMENT, u8"element");
  Define(ENTRY, u8"entry");
  Define(ENTRIES, u8"entries");
  Define(KIND, u8"kind");

#undef Define
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_SymbolReader);
}

static void
Arcadia_DDLS_SymbolReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReaderDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDLS_SymbolReader_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self
  )
{
  if (self->scope) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->scope);
  }
  if (self->stringTable) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->stringTable);
  }
#define Define(Variable, Text) \
  Arcadia_Value_visit(thread, &self->Variable);

  Define(ANY, u8"Any");
  Define(BOOLEAN, u8"Boolean");
  Define(CHOICE, u8"Choice");
  Define(CHOICES, u8"choices");
  Define(LIST, u8"List");
  Define(MAP, u8"Map");
  Define(MAPENTRY, u8"MapEntry");
  Define(NUMBER, u8"Number");
  Define(SCHEMA, u8"Schema");
  Define(SCHEMAREFERENCE, u8"SchemaReference");
  Define(STRING, u8"String");
  Define(VOID, u8"Void");

  Define(DEFINITION, "definition");
  Define(NAME, u8"name");
  Define(TYPE, u8"type");
  Define(ELEMENT, u8"element");
  Define(ENTRY, u8"entry");
  Define(ENTRIES, u8"entries");
  Define(KIND, u8"kind");

#undef Define
}

Arcadia_DDLS_SymbolReader*
Arcadia_DDLS_SymbolReader_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* stringTable
  )
{
  _Arcadia_BeginCreate(Arcadia_DDLS_SymbolReader);
  if (stringTable) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)stringTable);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDLS_SymbolReader);
}

Arcadia_DDLS_Symbol*
Arcadia_DDLS_SymbolReader_run
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_Node* source
  )
{
  if (!Arcadia_DDL_Node_isMap(thread, source)) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"expected specification\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  self->scope = Arcadia_DDLS_Scope_create(thread, NULL);
  openScope(thread, self, (Arcadia_DDL_MapNode*)source);
  Arcadia_DDLS_Symbol* target = readSchemaSymbol(thread, self, (Arcadia_DDL_MapNode*)source);
  closeScope(thread, self, (Arcadia_DDL_MapNode*)source);
  return target;
}

static Arcadia_DDLS_AnySymbol*
readAnySymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  // (1) Remove the `kind` element.
  Arcadia_Map_remove(thread, self->scope->symbols, self->KIND, NULL, NULL);
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->scope->symbols) > 0) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"unsupported entries\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDLS_AnySymbol* anySymbol = Arcadia_DDLS_AnySymbol_create(thread);
  return anySymbol;
}

static Arcadia_DDLS_ScalarSymbol*
readBooleanSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  // (1) Remove the `kind` element.
  Arcadia_Map_remove(thread, self->scope->symbols, self->KIND, NULL, NULL);
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->scope->symbols) > 0) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"unsupported entries\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDLS_ScalarSymbol* scalarSymbol = Arcadia_DDLS_ScalarSymbol_create(thread);
  scalarSymbol->name = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&self->BOOLEAN);
  return scalarSymbol;
}

static Arcadia_DDLS_ChoiceSymbol*
readChoiceSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  Arcadia_DDLS_ChoiceSymbol* choiceSymbol = Arcadia_DDLS_ChoiceSymbol_create(thread);
  // (1) Remove the `kind` element.
  Arcadia_Map_remove(thread, self->scope->symbols, self->KIND, NULL, NULL);
  // (2) Read `choices` element.
  Arcadia_Value v = Arcadia_Map_get(thread, self->scope->symbols, self->CHOICES);
  if (!Arcadia_Value_isInstanceOf(thread, &v, _Arcadia_DDL_ListNode_getType(thread))) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"`choices` not specified\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Map_remove(thread, self->scope->symbols, self->CHOICES, NULL, NULL);
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->scope->symbols) > 0) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"unsupported entries\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_DDL_ListNode* listNode = (Arcadia_DDL_ListNode*)Arcadia_Value_getObjectReferenceValue(&v);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)listNode->elements); i < n; ++i) {
    Arcadia_DDL_MapNode* listElementNode = (Arcadia_DDL_MapNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)listNode->elements, i, _Arcadia_DDL_MapNode_getType(thread));
    Arcadia_List_insertBackObjectReferenceValue(thread, choiceSymbol->choices, (Arcadia_Object*)readSymbol0(thread, self, listElementNode));
  }

  return choiceSymbol;
}

static Arcadia_DDLS_ListSymbol*
readListSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  Arcadia_DDLS_ListSymbol* listSymbol = Arcadia_DDLS_ListSymbol_create(thread);
  // (1) Remove the `kind` element.
  Arcadia_Map_remove(thread, self->scope->symbols, self->KIND, NULL, NULL);
  // (2) Read `entry` element.
  Arcadia_Value v = Arcadia_Map_get(thread, self->scope->symbols, self->ENTRY);
  if (!Arcadia_Value_isInstanceOf(thread, &v, _Arcadia_DDL_MapNode_getType(thread))) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"`entry` not specified\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Map_remove(thread, self->scope->symbols, self->ENTRY, NULL, NULL);
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->scope->symbols) > 0) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"unsupported entries\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_MapNode* mapNode = (Arcadia_DDL_MapNode*)Arcadia_Value_getObjectReferenceValue(&v);
  Arcadia_DDLS_Symbol* entrySymbol = readSymbol0(thread, self, mapNode);
  listSymbol->entry = entrySymbol;
  return listSymbol;
}

static Arcadia_DDLS_ScalarSymbol*
readStringSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  // (1) Remove the `kind` element.
  Arcadia_Map_remove(thread, self->scope->symbols, self->KIND, NULL, NULL);
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->scope->symbols) > 0) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"unsupported entries\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDLS_ScalarSymbol* scalarSymbol = Arcadia_DDLS_ScalarSymbol_create(thread);
  scalarSymbol->name = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&self->STRING);
  return scalarSymbol;
}

static Arcadia_DDLS_MapEntrySymbol*
readMapEntrySymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  // (1) Remove the `kind` element.
  Arcadia_Map_remove(thread, self->scope->symbols, self->KIND, NULL, NULL);

  // (2) Read `name` element.
  Arcadia_String* name = readString(thread, self, self->NAME);
  Arcadia_Map_remove(thread, self->scope->symbols, self->NAME, NULL, NULL);

  // (3) Read `type` element.
  Arcadia_Value v = Arcadia_Map_get(thread, self->scope->symbols, self->TYPE);
  if (!Arcadia_Value_isInstanceOf(thread, &v, _Arcadia_DDL_MapNode_getType(thread))) {
    // @todo Be more specific on the nature of the error.
    Arcadia_logf(Arcadia_LogFlags_Error, u8"`type` not specified\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_MapNode* symbolSource = (Arcadia_DDL_MapNode*)Arcadia_Value_getObjectReferenceValue(&v);
  Arcadia_DDLS_Symbol* symbol = readSymbol0(thread, self, symbolSource);
  Arcadia_Map_remove(thread, self->scope->symbols, self->TYPE, NULL, NULL);

  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->scope->symbols) > 0) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"unsupported entries\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_DDLS_MapEntrySymbol* mapEntrySymbol = Arcadia_DDLS_MapEntrySymbol_create(thread);
  mapEntrySymbol->entryName = name;
  mapEntrySymbol->entrySymbol = symbol;

  return mapEntrySymbol;
}

static Arcadia_DDLS_MapSymbol*
readMapSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  Arcadia_DDLS_MapSymbol* mapSymbol = Arcadia_DDLS_MapSymbol_create(thread);
  // (1) Remove the `kind` element.
  Arcadia_Map_remove(thread, self->scope->symbols, self->KIND, NULL, NULL);
  // (2) Read `entries` element.
  Arcadia_Value v = Arcadia_Map_get(thread, self->scope->symbols, self->ENTRIES);
  if (!Arcadia_Value_isInstanceOf(thread, &v, _Arcadia_DDL_ListNode_getType(thread))) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"`entries` not specified\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Map_remove(thread, self->scope->symbols, self->ENTRIES, NULL, NULL);
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->scope->symbols) > 0) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"unsupported entries\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_ListNode* xe = (Arcadia_DDL_ListNode*)Arcadia_Value_getObjectReferenceValue(&v);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)xe->elements); i < n; ++i) {
    Arcadia_DDL_MapNode* e = (Arcadia_DDL_MapNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, xe->elements, i, _Arcadia_DDL_MapNode_getType(thread));
    Arcadia_DDLS_Symbol* s = readSymbol0(thread, self, e);
    if (s->kind != Arcadia_DDLS_SymbolKind_MapEntry) {
      Arcadia_logf(Arcadia_LogFlags_Error, u8"expected specification of kind `MapEntry`\n");
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_DDLS_MapEntrySymbol* mapEntrySymbol = (Arcadia_DDLS_MapEntrySymbol*)s;
    // We have a map entry.
    v = Arcadia_Map_get(thread, mapSymbol->entries, Arcadia_Value_makeObjectReferenceValue(mapEntrySymbol->entryName));
    if (!Arcadia_Value_isInstanceOf(thread, &v, _Arcadia_VoidValue_getType(thread))) {
      Arcadia_String* s = mapEntrySymbol->entryName;
      if (Arcadia_String_getNumberOfBytes(thread, s) > 32) {
        Arcadia_logf(Arcadia_LogFlags_Error, u8"`%.*s...` already specified\n", 32, Arcadia_String_getBytes(thread, s));
      } else {
        Arcadia_logf(Arcadia_LogFlags_Error, u8"`%.*s` already specified\n", Arcadia_String_getNumberOfBytes(thread, s), Arcadia_String_getBytes(thread, s));
      }
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Map_set(thread, mapSymbol->entries, Arcadia_Value_makeObjectReferenceValue(mapEntrySymbol->entryName),
                                                Arcadia_Value_makeObjectReferenceValue(mapEntrySymbol), NULL, NULL);
  }
  return mapSymbol;
}

static Arcadia_DDLS_ScalarSymbol*
readNumberSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  // (1) Remove the `kind` element.
  Arcadia_Map_remove(thread, self->scope->symbols, self->KIND, NULL, NULL);
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->scope->symbols) > 0) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"unsupported entries\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDLS_ScalarSymbol* scalarSymbol = Arcadia_DDLS_ScalarSymbol_create(thread);
  scalarSymbol->name = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&self->NUMBER);
  return scalarSymbol;
}

static Arcadia_DDLS_SchemaReferenceSymbol*
readSchemaReferenceSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  Arcadia_DDLS_SchemaReferenceSymbol* schemaReferenceSymbol = Arcadia_DDLS_SchemaReferenceSymbol_create(thread);
  // (1) Remove `kind` element.
  Arcadia_Map_remove(thread, self->scope->symbols, self->KIND, NULL, NULL);
  // (2) Read and remove `name` element.
  Arcadia_String* name = readString(thread, self, self->NAME);
  Arcadia_Map_remove(thread, self->scope->symbols, self->NAME, NULL, NULL);
  // (3) Assert no unsupported elements exist.
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->scope->symbols) > 0) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"unsupported entries\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  schemaReferenceSymbol->name = name;
  return schemaReferenceSymbol;
}


static Arcadia_DDLS_ScalarSymbol*
readVoidSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  // (1) Remove the `kind` element.
  Arcadia_Map_remove(thread, self->scope->symbols, self->KIND, NULL, NULL);
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->scope->symbols) > 0) {
    Arcadia_logf(Arcadia_LogFlags_Error, u8"unsupported entries\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDLS_ScalarSymbol* scalarSymbol = Arcadia_DDLS_ScalarSymbol_create(thread);
  scalarSymbol->name = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&self->VOID);
  return scalarSymbol;
}

static Arcadia_DDLS_Symbol*
readSymbol0
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  // 1: Open a scope.
  openScope(thread, self, source);
  // 2: Get `kind` entry.
  Arcadia_Value kind = Arcadia_Value_makeObjectReferenceValue(readString(thread, self, self->KIND));
  // 3: Read the specification.
  Arcadia_DDLS_Symbol* target = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    if (Arcadia_Value_isEqualTo(thread, &kind, &self->ANY)) {
      target = (Arcadia_DDLS_Symbol*)readAnySymbol(thread, self, source);
    } else if (Arcadia_Value_isEqualTo(thread, &kind, &self->BOOLEAN)) {
      target = (Arcadia_DDLS_Symbol*)readBooleanSymbol(thread, self, source);
    } else if (Arcadia_Value_isEqualTo(thread, &kind, &self->CHOICE)) {
      target = (Arcadia_DDLS_Symbol*)readChoiceSymbol(thread, self, source);
    } else if (Arcadia_Value_isEqualTo(thread, &kind, &self->LIST)) {
      target = (Arcadia_DDLS_Symbol*)readListSymbol(thread, self, source);
    } else if (Arcadia_Value_isEqualTo(thread, &kind, &self->MAP)) {
      target = (Arcadia_DDLS_Symbol*)readMapSymbol(thread, self, source);
    } else if (Arcadia_Value_isEqualTo(thread, &kind, &self->MAPENTRY)) {
      target = (Arcadia_DDLS_Symbol*)readMapEntrySymbol(thread, self, source);
    } else if (Arcadia_Value_isEqualTo(thread, &kind, &self->NUMBER)) {
      target = (Arcadia_DDLS_Symbol*)readNumberSymbol(thread, self, source);
    } else if (Arcadia_Value_isEqualTo(thread, &kind, &self->SCHEMAREFERENCE)) {
      target = (Arcadia_DDLS_Symbol*)readSchemaReferenceSymbol(thread, self, source);
    } else if (Arcadia_Value_isEqualTo(thread, &kind, &self->STRING)) {
      target = (Arcadia_DDLS_Symbol*)readStringSymbol(thread, self, source);
    } else if (Arcadia_Value_isEqualTo(thread, &kind, &self->VOID)) {
      target = (Arcadia_DDLS_Symbol*)readVoidSymbol(thread, self, source);
    } else {
      closeScope(thread, self, source);
      Arcadia_logf(Arcadia_LogFlags_Error, u8"unknown type\n");
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Thread_popJumpTarget(thread);
    closeScope(thread, self, source);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    closeScope(thread, self, source);
    Arcadia_Thread_jump(thread);
  }
  return target;
}

static Arcadia_DDLS_Symbol*
readSchemaSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_SymbolReader* self,
    Arcadia_DDL_MapNode* source
  )
{
  // 1: Open a scope.
  openScope(thread, self, source);
  // 2: Read the specification.
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    // 2.1: Get `kind`. Assert `kind` is `Schema`.
    Arcadia_Value kind = Arcadia_Value_makeObjectReferenceValue(readString(thread, self, self->KIND));
    if (!Arcadia_Value_isEqualTo(thread, &kind, &self->SCHEMA)) {
      Arcadia_logf(Arcadia_LogFlags_Error, u8"expected `Schema`\n");
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Map_remove(thread, self->scope->symbols, self->KIND, NULL, NULL);
    // 2.2: Get `name` entry.
    Arcadia_Value name = Arcadia_Value_makeObjectReferenceValue(readString(thread, self, self->NAME));
    if (!Arcadia_Value_isInstanceOf(thread, &name, _Arcadia_String_getType(thread))) {
      Arcadia_logf(Arcadia_LogFlags_Error, u8"`name` not specified\n");
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Map_remove(thread, self->scope->symbols, self->NAME, NULL, NULL);
    Arcadia_DDLS_SchemaSymbol* schemaSymbol = Arcadia_DDLS_SchemaSymbol_create(thread, (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&name));
    // 2.3: Read `definition` element.
    Arcadia_Value definition = Arcadia_Map_get(thread, self->scope->symbols, self->DEFINITION);
    if (!Arcadia_Value_isInstanceOf(thread, &definition, _Arcadia_DDL_MapNode_getType(thread))) {
      Arcadia_logf(Arcadia_LogFlags_Error, u8"`definition` not specified\n");
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Map_remove(thread, self->scope->symbols, self->DEFINITION, NULL, NULL);

    if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->scope->symbols) > 0) {
      Arcadia_logf(Arcadia_LogFlags_Error, u8"unsupported entries\n");
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }

    schemaSymbol->definition = readSymbol0(thread, self, (Arcadia_DDL_MapNode*)Arcadia_Value_getObjectReferenceValue(&definition));

    closeScope(thread, self, source);
    Arcadia_Thread_popJumpTarget(thread);
    return (Arcadia_DDLS_Symbol*)schemaSymbol;
  } else {
    closeScope(thread, self, source);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_jump(thread);
  }
}
