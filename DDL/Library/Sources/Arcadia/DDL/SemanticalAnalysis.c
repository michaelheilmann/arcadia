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

#include "Arcadia/DDL/SemanticalAnalysis.h"

#include "Arcadia/DDL/Include.h"

struct Arcadia_DataDefinitionLanguage_SemanticalAnalysisDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_DataDefinitionLanguage_SemanticalAnalysis {
  Arcadia_Object _parent;
  Arcadia_Map* map;
};

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitBooleanValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_BooleanNode* node
  );

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitListValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_ListNode* node
  );

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitMapValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_MapNode* node
  );

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitNumberValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_NumberNode* node
  );

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitStringValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_StringNode* node
  );

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_Node* node
  );

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitVoidValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_VoidNode* node
  );

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self
  );

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysisDispatch* self
  );

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DataDefinitionLanguage_SemanticalAnalysis_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DataDefinitionLanguage_SemanticalAnalysis_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DataDefinitionLanguage.SemanticalAnalysis", Arcadia_DataDefinitionLanguage_SemanticalAnalysis,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitBooleanValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_BooleanNode* node
  )
{/*Intentionally empty.*/}

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitListValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_ListNode* node
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->elements); i < n; ++i) {
    Arcadia_DDL_Node* elementNode = Arcadia_List_getObjectReferenceValueAt(thread, node->elements, i);
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitValue(thread, self, elementNode);
  }
}

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitMapValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_MapNode* node
  )
{
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->map);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* mapEntryNode = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueAt(thread, node->entries, i);
    Arcadia_Value existing = Arcadia_Map_get(thread, self->map, Arcadia_Value_makeObjectReferenceValue(mapEntryNode->key->value));
    if (!Arcadia_Value_isVoidValue(&existing)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Map_set(thread, self->map, Arcadia_Value_makeObjectReferenceValue(mapEntryNode->key->value), Arcadia_Value_makeObjectReferenceValue(mapEntryNode->value), NULL, NULL);
  }
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->map);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* mapEntryNode = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueAt(thread, node->entries, i);
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitValue(thread, self, mapEntryNode->value);
  }
}

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitNumberValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_NumberNode* node
  )
{/*Intentionally empty.*/}

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitStringValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_StringNode* node
  )
{/*Intentionally empty.*/}

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_Node* node
  )
{
  switch (node->type) {
    case Arcadia_DDL_NodeType_Boolean: {
      Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitBooleanValue(thread, self, (Arcadia_DDL_BooleanNode*)node);
    } break;
    case Arcadia_DDL_NodeType_List: {
      Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitListValue(thread, self, (Arcadia_DDL_ListNode*)node);
    } break;
    case Arcadia_DDL_NodeType_Map: {
      Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitMapValue(thread, self, (Arcadia_DDL_MapNode*)node);
    } break;
    case Arcadia_DDL_NodeType_Number: {
      Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitNumberValue(thread, self, (Arcadia_DDL_NumberNode*)node);
    } break;
    case Arcadia_DDL_NodeType_String: {
      Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitStringValue(thread, self, (Arcadia_DDL_StringNode*)node);
    } break;
    case Arcadia_DDL_NodeType_Void: {
      Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitVoidValue(thread, self, (Arcadia_DDL_VoidNode*)node);
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  }
}

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitVoidValue
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_VoidNode* node
  )
{/*Intentionally empty.*/}

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DataDefinitionLanguage_SemanticalAnalysis);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->map = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  //
  Arcadia_LeaveConstructor(Arcadia_DataDefinitionLanguage_SemanticalAnalysis);
}

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysisDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self
  )
{
  if (self->map) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->map);
  }
}

Arcadia_DataDefinitionLanguage_SemanticalAnalysis*
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_DataDefinitionLanguage_SemanticalAnalysis);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_DataDefinitionLanguage_SemanticalAnalysis);
}

void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_run
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_Node* node
  )
{
  Arcadia_DataDefinitionLanguage_SemanticalAnalysis_visitValue(thread, self, node);
}
