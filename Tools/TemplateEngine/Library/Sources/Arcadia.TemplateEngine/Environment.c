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

#include "Arcadia.TemplateEngine/Environment.h"

#include "Arcadia.TemplateEngine/Parser/Tree.h"
#include "Arcadia/Logging/Include.h"
#include "Arcadia/DDL/Include.h"

static void
Environment_visit
  (
    Arcadia_Thread* thread,
    Environment* self
  );

static void
Environment_constructImpl
  (
    Arcadia_Thread* thread,
    Environment* self
  );

static void
Environment_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    EnvironmentDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Environment_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Environment_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Environment_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.TemplateEngine.Environment", Environment,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Environment_visit
  (
    Arcadia_Thread* thread,
    Environment* self
  )
{
  if (self->enclosing) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enclosing);
  }
  if (self->variables) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->variables);
  }
}

static void
Environment_constructImpl
  (
    Arcadia_Thread* thread,
    Environment* self
  )
{
  Arcadia_EnterConstructor(Environment);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_ValueStack_isVoidValue(thread, 1)) {
    self->enclosing = NULL;
  } else {
    self->enclosing = (Environment*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Environment_getType(thread));
  }
  self->variables = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  //
  Arcadia_LeaveConstructor(Environment);
}

static void
Environment_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    EnvironmentDispatch* self
  )
{ }

Environment*
Environment_create
  (
    Arcadia_Thread* thread,
    Environment* enclosing
  )
{
  _Arcadia_BeginCreate(Environment);
  if (enclosing) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)enclosing);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Environment);
}

Environment*
Environment_loadString
  (
    Arcadia_Thread* thread,
    Arcadia_String* source
  )
{
  Arcadia_DDL_Node* node = NULL;
  Arcadia_DDL_Parser* syntacticalAnalysis = Arcadia_DDL_Parser_create(thread, Arcadia_DDL_Scanner_create(thread, Arcadia_Languages_StringTable_getOrCreate(thread),
                                                                                                                 Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread))));
  Arcadia_DataDefinitionLanguage_SemanticalAnalysis* semanticalAnalysis = Arcadia_DataDefinitionLanguage_SemanticalAnalysis_create(thread);
  Arcadia_ByteArray* byteArray = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source)));
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, byteArray));
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)syntacticalAnalysis, reader);
  node = (Arcadia_DDL_Node*)Arcadia_Value_getObjectReferenceValueChecked(thread, Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)syntacticalAnalysis), _Arcadia_DDL_Node_getType(thread));
  Arcadia_DataDefinitionLanguage_SemanticalAnalysis_run(thread, semanticalAnalysis, node);
  Environment* variables = Environment_create(thread, NULL);
  if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, (Arcadia_Object*)node), _Arcadia_DDL_MapNode_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_MapNode* mapNode = (Arcadia_DDL_MapNode*)node;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)mapNode->entries); i < n; ++i) {
    node = Arcadia_List_getObjectReferenceValueAt(thread, mapNode->entries, i);
    if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, (Arcadia_Object*)node), _Arcadia_DDL_MapEntryNode_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_DDL_MapEntryNode* mapEntryNode = (Arcadia_DDL_MapEntryNode*)node;
    Arcadia_String* keyString = mapEntryNode->key->value;
    node = mapEntryNode->value;
    if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, (Arcadia_Object*)node), _Arcadia_DDL_StringNode_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_String* valueString = ((Arcadia_DDL_StringNode*)mapEntryNode->value)->value;
    Arcadia_Map_set(thread, variables->variables, Arcadia_Value_makeObjectReferenceValue(keyString), Arcadia_Value_makeObjectReferenceValue(valueString), NULL, NULL);
  }
  return variables;
}

Arcadia_Value
Environment_get
  (
    Arcadia_Thread* thread,
    Environment* self,
    Arcadia_Value key,
    Arcadia_BooleanValue recursive
  )
{
  Arcadia_Value value = Arcadia_Map_get(thread, self->variables, key);
  if (Arcadia_Value_isVoidValue(&value) && recursive) {
    Environment* current = self->enclosing;
    if (current) {
      do {
        value = Arcadia_Map_get(thread, current->variables, key);
        current = current->enclosing;
      } while (Arcadia_Value_isVoidValue(&value) && NULL != current);
    }
  }
  return value;
}

void
Environment_set
  (
    Arcadia_Thread* thread,
    Environment* self,
    Arcadia_Value key,
    Arcadia_Value value
  )
{
  Arcadia_Map_set(thread, self->variables, key, value, NULL, NULL);
}
