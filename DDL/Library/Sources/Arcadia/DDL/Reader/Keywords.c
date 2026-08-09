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

#define ARCADIA_DDL_MODULE (1)
#include "Arcadia/DDL/Reader/Keywords.h"

typedef struct Keyword Keyword;

struct Keyword {
  Keyword* next;
  Arcadia_String* string;
  Arcadia_Natural32Value type;
};

struct Arcadia_DataDefinitionLanguage_KeywordsDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_DataDefinitionLanguage_Keywords {
  Arcadia_Object _parent;
  Keyword** buckets;
  Arcadia_SizeValue size;
  Arcadia_SizeValue capacity;
};

static void
Arcadia_DataDefinitionLanguage_Keywords_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Keywords* self
  );

static void
Arcadia_DataDefinitionLanguage_Keywords_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_KeywordsDispatch* self
  );

static void
Arcadia_DataDefinitionLanguage_Keywords_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Keywords* self
  );

static void
Arcadia_DataDefinitionLanguage_Keywords_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Keywords* self
  );

static void
Arcadia_DataDefinitionLanguage_Keywords_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Keywords* self
  )
{
  for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
    while (self->buckets[i]) {
      Keyword* node = self->buckets[i];
      self->buckets[i] = self->buckets[i]->next;
      Arcadia_Memory_deallocateUnmanaged(thread, node);
    }
  }
  Arcadia_Memory_deallocateUnmanaged(thread, self->buckets);
  self->buckets = NULL;
}

static void
Arcadia_DataDefinitionLanguage_Keywords_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Keywords* self
  )
{
  for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
    Keyword* node = self->buckets[i];
    while (node) {
      Arcadia_Object_visit(thread, (Arcadia_Object*)node->string);
      node = node->next;
    }
  }
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DataDefinitionLanguage_Keywords_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_DataDefinitionLanguage_Keywords_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DataDefinitionLanguage_Keywords_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DataDefinitionLanguage_Keywords_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DataDefinitionLanguage.Keywords", Arcadia_DataDefinitionLanguage_Keywords,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_DataDefinitionLanguage_Keywords_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Keywords* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DataDefinitionLanguage_Keywords);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->size = 0;
  self->capacity = 8;
  self->buckets = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Keyword*) * self->capacity);
  for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
    self->buckets[i] = NULL;
  }
  Arcadia_LeaveConstructor(Arcadia_DataDefinitionLanguage_Keywords);
}

static void
Arcadia_DataDefinitionLanguage_Keywords_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_KeywordsDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_DataDefinitionLanguage_Keywords*
Arcadia_DataDefinitionLanguage_Keywords_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_DataDefinitionLanguage_Keywords);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_DataDefinitionLanguage_Keywords);
}

void
Arcadia_DataDefinitionLanguage_Keywords_add
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Keywords* self,
    Arcadia_String* string,
    Arcadia_Natural32Value type
  )
{
  Arcadia_Value stringValue = Arcadia_Value_makeObjectReferenceValue(string);
  Arcadia_SizeValue hash = Arcadia_Value_getHash(thread, &stringValue);
  Arcadia_SizeValue index = hash % self->capacity;
  for (Keyword* keyword = self->buckets[index]; NULL != keyword; keyword = keyword->next) {
    Arcadia_Value v[] = {
      Arcadia_Value_makeObjectReferenceValue(keyword->string),
      stringValue
    };
    if (Arcadia_Value_isEqualTo(thread, &v[0], &v[1])) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Exists);
      Arcadia_Thread_jump(thread);
    }
  }
  Keyword* keyword = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Keyword));
  keyword->string = string;
  keyword->type = type;
  keyword->next = self->buckets[index];
  self->buckets[index] = keyword;
  self->size++;
}

Arcadia_BooleanValue
Arcadia_DataDefinitionLanguage_Keywords_scan
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Keywords* self,
    Arcadia_String* string,
    Arcadia_Natural32Value* tokenType
  )
{
  Arcadia_Value stringValue = Arcadia_Value_makeObjectReferenceValue(string);
  Arcadia_SizeValue hash = Arcadia_Value_getHash(thread, &stringValue);
  Arcadia_SizeValue index = hash % self->capacity;
  for (Keyword* keyword = self->buckets[index]; NULL != keyword; keyword = keyword->next) {
    Arcadia_Value v[2] = {
      Arcadia_Value_makeObjectReferenceValue(keyword->string),
      stringValue
    };
    if (Arcadia_Value_isEqualTo(thread, &v[0], &v[1])) {
      *tokenType = keyword->type;
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}
