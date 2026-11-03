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

#include "Arcadia/DDLS/Implementation/ValidationContext.h"

#include "Arcadia/DDLS/Extensions.h"

static void
Arcadia_DDLS_ValidationContext_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self
  );

static void
Arcadia_DDLS_ValidationContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContextDispatch* self
  );

static void
Arcadia_DDLS_ValidationContext_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self
  );

static void
Arcadia_DDLS_ValidationContext_runImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_String* name,
    Arcadia_DDL_Node* node
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_ValidationContext_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_ValidationContext_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_ValidationContext_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDLS_ValidationContext_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_ValidationContext_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_ValidationContext_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.ValidationContext", Arcadia_DDLS_ValidationContext,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_DDLS_ValidationContext_typeOperations);

static void
Arcadia_DDLS_ValidationContext_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_ValidationContext);
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
  self->cycleStack = NULL;
  self->cycleKeys = NULL;
  self->visitingSchemata = NULL;
  self->checkedSchemata = NULL;
  self->temporary1 = Arcadia_StringBuilder_create(thread);
  self->stringTable = Arcadia_Languages_StringTable_getOrCreate(thread);
  self->diagnostics = Arcadia_DDLS_Diagnostics_create(thread, self->stringTable);
  self->schemata = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_ValidationContext);
}

static void
Arcadia_DDLS_ValidationContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContextDispatch* self
  )
{
  self->run = &Arcadia_DDLS_ValidationContext_runImpl;
}

static void
Arcadia_DDLS_ValidationContext_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self
  )
{
  if (self->temporary1) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->temporary1);
  }
  if (self->diagnostics) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics);
  }
  if (self->stringTable) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->stringTable);
  }
  if (self->schemata) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->schemata);
  }
  if (self->cycleStack) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cycleStack);
  }
  if (self->cycleKeys) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cycleKeys);
  }
  if (self->visitingSchemata) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->visitingSchemata);
  }
  if (self->checkedSchemata) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->checkedSchemata);
  }
}

// Check all definitions reachable from the starting schema before matching the DDL value.
// A schema node on the stack marks the end of a reference's definition.
// Only references on the current path are cycles; shared acyclic references are valid.
static void
checkSchemaIntegrity
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_DDLS_Node* definition
  )
{
  self->cycleStack = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->visitingSchemata = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  self->checkedSchemata = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_List_insertBackObjectReferenceValue(thread, self->cycleStack, (Arcadia_Object*)definition);
  while (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->cycleStack)) {
    Arcadia_SizeValue last = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->cycleStack) - 1;
    Arcadia_DDLS_Node* current = (Arcadia_DDLS_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, self->cycleStack, last, _Arcadia_DDLS_Node_getType(thread));
    Arcadia_List_removeBack(thread, self->cycleStack, 1);
    if (Arcadia_DDLS_isSchema(thread, current)) {
      Arcadia_Value name = Arcadia_Value_makeObjectReferenceValue(((Arcadia_DDLS_SchemaNode*)current)->name);
      Arcadia_Map_remove(thread, self->visitingSchemata, name, NULL, NULL);
      Arcadia_Map_set(thread, self->checkedSchemata, name, name, NULL, NULL);
    } else if (Arcadia_DDLS_isSchemaReference(thread, current)) {
      Arcadia_String* name = ((Arcadia_DDLS_SchemaReferenceNode*)current)->name;
      Arcadia_Value key = Arcadia_Value_makeObjectReferenceValue(name);
      Arcadia_Value visiting = Arcadia_Map_get(thread, self->visitingSchemata, key);
      if (!Arcadia_Value_isVoidValue(&visiting)) {
        Arcadia_DDLS_Diagnostics_cyclicSchemaReferenceError(thread, self->diagnostics, name);
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Value checked = Arcadia_Map_get(thread, self->checkedSchemata, key);
      if (!Arcadia_Value_isVoidValue(&checked)) {
        continue;
      }
      Arcadia_Value schema = Arcadia_Map_get(thread, self->schemata, key);
      if (Arcadia_Value_isVoidValue(&schema)) {
        Arcadia_DDLS_Diagnostics_unresolvedSchemaReferenceError(thread, self->diagnostics, name);
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_DDLS_SchemaNode* schemaNode = (Arcadia_DDLS_SchemaNode*)Arcadia_Value_getObjectReferenceValue(&schema);
      Arcadia_Map_set(thread, self->visitingSchemata, key, key, NULL, NULL);
      Arcadia_List_insertBackObjectReferenceValue(thread, self->cycleStack, (Arcadia_Object*)schemaNode);
      Arcadia_List_insertBackObjectReferenceValue(thread, self->cycleStack, (Arcadia_Object*)schemaNode->definition);
    } else if (Arcadia_DDLS_isList(thread, current)) {
      Arcadia_List_insertBackObjectReferenceValue(thread, self->cycleStack, (Arcadia_Object*)((Arcadia_DDLS_ListNode*)current)->element);
    } else if (Arcadia_DDLS_isChoice(thread, current)) {
      Arcadia_List* choices = ((Arcadia_DDLS_ChoiceNode*)current)->choices;
      Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)choices);
      if (!n) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      for (Arcadia_SizeValue i = 0; i < n; ++i) {
        Arcadia_List_insertBack(thread, self->cycleStack, Arcadia_List_getAt(thread, choices, i));
      }
    } else if (Arcadia_DDLS_isMap(thread, current)) {
      Arcadia_Map* entries = ((Arcadia_DDLS_MapNode*)current)->entries;
      self->cycleKeys = Arcadia_Map_getKeys(thread, entries);
      for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->cycleKeys); i < n; ++i) {
        Arcadia_List_insertBack(thread, self->cycleStack, Arcadia_Map_get(thread, entries, Arcadia_List_getAt(thread, self->cycleKeys, i)));
      }
      self->cycleKeys = NULL;
    } else if (!Arcadia_DDLS_isAny(thread, current) && !Arcadia_DDLS_isBoolean(thread, current) &&
               !Arcadia_DDLS_isNumber(thread, current) && !Arcadia_DDLS_isString(thread, current) &&
               !Arcadia_DDLS_isVoid(thread, current)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
  }
}

static Arcadia_BooleanValue
onValidateList
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_DDLS_Node* ddlsNode,
    Arcadia_DDL_Node* node,
    Arcadia_BooleanValue reportMismatch
  );

static Arcadia_BooleanValue
onValidateMap
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_DDLS_Node* ddlsNode,
    Arcadia_DDL_Node* node,
    Arcadia_BooleanValue reportMismatch
  );

static Arcadia_BooleanValue
onValidate
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_DDLS_Node* ddlsNode,
    Arcadia_DDL_Node* node,
    Arcadia_BooleanValue reportMismatch
  );

static Arcadia_BooleanValue
onValidateList
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_DDLS_Node* ddlsNode,
    Arcadia_DDL_Node* node,
    Arcadia_BooleanValue reportMismatch
  )
{
  Arcadia_DDLS_ListNode* ddlsListNode = (Arcadia_DDLS_ListNode*)ddlsNode;
  Arcadia_DDLS_Node* ddlsListElementNode = ddlsListNode->element;
  Arcadia_DDL_ListNode* listNode = (Arcadia_DDL_ListNode*)node;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)listNode->elements); i < n; ++i) {
    Arcadia_DDL_Node* node = (Arcadia_DDL_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, listNode->elements, i, _Arcadia_DDL_Node_getType(thread));
    if (!onValidate(thread, self, ddlsListElementNode, node, reportMismatch)) {
      return Arcadia_BooleanValue_False;
    }
  }
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
onValidateMap
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_DDLS_Node* ddlsNode,
    Arcadia_DDL_Node* node,
    Arcadia_BooleanValue reportMismatch
  )
{
  /// @todo The implementation shall be non-recursive (same for onValidateList) for example using a stack on which
  ///       validation tasks (a new object type) are pushed.
  Arcadia_Map* symbols = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_DDL_MapNode* mapNode = (Arcadia_DDL_MapNode*)node;
  Arcadia_DDLS_MapNode* ddlsMapNode = (Arcadia_DDLS_MapNode*)ddlsNode;
  // (1) Ensure all map entry names are unique.
  // (2) Ensure all map entry names are permitted by the type.
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)mapNode->entries); i < n; ++i) {
    Arcadia_DDL_MapEntryNode* node = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, mapNode->entries, i, _Arcadia_DDL_MapEntryNode_getType(thread));
    Arcadia_Value newValue = Arcadia_Value_makeObjectReferenceValue(node->value);
    Arcadia_Value newKey = Arcadia_Value_makeObjectReferenceValue(node->key->value);
    Arcadia_Value oldValue = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), oldKey = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
    Arcadia_Map_set(thread, symbols, newKey, newValue, &oldKey, &oldValue);
    if (!Arcadia_Value_isVoidValue(&oldKey)) {
      Arcadia_DDLS_Diagnostics_mapEntryAlreadyDefined(thread, self->diagnostics, (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&newKey));
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Value temporary = Arcadia_Map_get(thread, ddlsMapNode->entries, newKey);
    if (!Arcadia_Value_isObjectReferenceValue(&temporary)) {
      if (reportMismatch) {
        Arcadia_DDLS_Diagnostics_mapEntryExistsError(thread, self->diagnostics, (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&newKey));
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      return Arcadia_BooleanValue_False;
    }
    if (!onValidate(thread, self, (Arcadia_DDLS_Node*)Arcadia_Value_getObjectReferenceValueChecked(thread, temporary, _Arcadia_DDLS_Node_getType(thread)), node->value, reportMismatch)) {
      return Arcadia_BooleanValue_False;
    }
  }
  // (3) Ensure no entry required by the type are missing.
  Arcadia_List* requiredEntries = Arcadia_Map_getKeys(thread, ddlsMapNode->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)requiredEntries); i < n; ++i) {
    Arcadia_Value key = Arcadia_List_getAt(thread, requiredEntries, i);
    Arcadia_Value oldKey;
    Arcadia_Map_remove(thread, symbols, key, &oldKey, NULL);
    if (Arcadia_Value_isVoidValue(&oldKey)) {
      // The entry is missing. If the entry is optional, that is permitted.
      Arcadia_Value optional = Arcadia_Map_get(thread, ddlsMapNode->optional, key);
      if (!Arcadia_Value_isVoidValue(&optional)) {
        continue;
      }
      if (reportMismatch) {
        Arcadia_DDLS_Diagnostics_mapEntryNotExistsError(thread, self->diagnostics, (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&key));
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      return Arcadia_BooleanValue_False;
    }
  }
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
onValidate
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_DDLS_Node* ddlsNode,
    Arcadia_DDL_Node* node,
    Arcadia_BooleanValue reportMismatch
  )
{
  while (Arcadia_DDLS_isSchemaReference(thread, ddlsNode)) {
    Arcadia_Value v = Arcadia_Map_get(thread, self->schemata, Arcadia_Value_makeObjectReferenceValue(((Arcadia_DDLS_SchemaReferenceNode*)ddlsNode)->name));
    if (Arcadia_Value_isVoidValue(&v)) {
      Arcadia_DDLS_Diagnostics_unresolvedSchemaReferenceError(thread, self->diagnostics, ((Arcadia_DDLS_SchemaReferenceNode*)ddlsNode)->name);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_DDLS_SchemaNode* schemaNode = (Arcadia_DDLS_SchemaNode*)Arcadia_Value_getObjectReferenceValue(&v);
    ddlsNode = schemaNode->definition;

  }
  if (Arcadia_DDLS_isAny(thread, ddlsNode)) {
    return Arcadia_BooleanValue_True;
  }
  if (Arcadia_DDLS_isChoice(thread, ddlsNode)) {
    Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_DDLS_ChoiceNode*)ddlsNode)->choices);
    if (n == 0) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    for (Arcadia_SizeValue i = 0; i < n; ++i) {
      Arcadia_DDLS_Node* ddlsChildNode =
      (Arcadia_DDLS_Node*)
      Arcadia_List_getObjectReferenceValueCheckedAt
        (
          thread,
          ((Arcadia_DDLS_ChoiceNode*)ddlsNode)->choices,
          i,
          _Arcadia_DDLS_Node_getType(thread)
        );
      // Only an ordinary mismatch can try the next alternative. Errors still jump.
      if (onValidate(thread, self, ddlsChildNode, node, Arcadia_BooleanValue_False)) {
        return Arcadia_BooleanValue_True;
      }
    }
    if (reportMismatch) {
      Arcadia_DDLS_Diagnostics_noChoiceMatchesError(thread, self->diagnostics);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    return Arcadia_BooleanValue_False;
  }
  if (Arcadia_DDLS_isBoolean(thread, ddlsNode) && Arcadia_DDL_Node_isBoolean(thread, node)) {
    return Arcadia_BooleanValue_True;
  } else if (Arcadia_DDLS_isList(thread, ddlsNode) && Arcadia_DDL_Node_isList(thread, node)) {
    return onValidateList(thread, self, ddlsNode, node, reportMismatch);
  } else if (Arcadia_DDLS_isMap(thread, ddlsNode) && Arcadia_DDL_Node_isMap(thread, node)) {
    return onValidateMap(thread, self, ddlsNode, node, reportMismatch);
  } else if (Arcadia_DDLS_isNumber(thread, ddlsNode) && Arcadia_DDL_Node_isNumber(thread, node)) {
    return Arcadia_BooleanValue_True;
  } else if (Arcadia_DDLS_isString(thread, ddlsNode) && Arcadia_DDL_Node_isString(thread, node)) {
    return Arcadia_BooleanValue_True;
  } else if (Arcadia_DDLS_isVoid(thread, ddlsNode) && Arcadia_DDL_Node_isVoid(thread, node)) {
    return Arcadia_BooleanValue_True;
  } else {
    if (!Arcadia_DDLS_isBoolean(thread, ddlsNode) && !Arcadia_DDLS_isList(thread, ddlsNode) &&
        !Arcadia_DDLS_isMap(thread, ddlsNode) && !Arcadia_DDLS_isNumber(thread, ddlsNode) &&
        !Arcadia_DDLS_isString(thread, ddlsNode) && !Arcadia_DDLS_isVoid(thread, ddlsNode)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    if (reportMismatch) {
      Arcadia_DDLS_Diagnostics_unexpectedTypeError(thread, self->diagnostics, ddlsNode, node);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    return Arcadia_BooleanValue_False;
  }
}

static void
Arcadia_DDLS_ValidationContext_runImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_String* name,
    Arcadia_DDL_Node* node
  )
{
  Arcadia_Value v = Arcadia_Map_get(thread, self->schemata, Arcadia_Value_makeObjectReferenceValue(name));
  if (Arcadia_Value_isVoidValue(&v)) {
    Arcadia_DDLS_Diagnostics_unresolvedSchemaReferenceError(thread, self->diagnostics, name);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDLS_SchemaNode* ddlsNode = (Arcadia_DDLS_SchemaNode*)Arcadia_Value_getObjectReferenceValue(&v);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    checkSchemaIntegrity(thread, self, ddlsNode->definition);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    self->cycleStack = NULL;
    self->cycleKeys = NULL;
    self->visitingSchemata = NULL;
    self->checkedSchemata = NULL;
    Arcadia_Thread_jump(thread);
  }
  self->cycleStack = NULL;
  self->cycleKeys = NULL;
  self->visitingSchemata = NULL;
  self->checkedSchemata = NULL;
  onValidate(thread, self, ddlsNode->definition, node, Arcadia_BooleanValue_True);
}

Arcadia_DDLS_ValidationContext*
Arcadia_DDLS_ValidationContext_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_DDLS_ValidationContext);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_DDLS_ValidationContext);
}

void
Arcadia_DDLS_ValidationContext_addSchema
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_DDLS_SchemaNode* schemaNode
  )
{
  Arcadia_Value k = Arcadia_Value_makeObjectReferenceValue(schemaNode->name);
  Arcadia_Value v = Arcadia_Map_get(thread, self->schemata, k);
  if (!Arcadia_Value_isVoidValue(&v)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  v = Arcadia_Value_makeObjectReferenceValue(schemaNode);
  Arcadia_Map_set(thread, self->schemata, k, v, NULL, NULL);
}

Arcadia_DDLS_SchemaNode*
Arcadia_DDLS_ValidationContext_getSchema
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_String* name
  )
{
  Arcadia_Value k = Arcadia_Value_makeObjectReferenceValue(name);
  Arcadia_Value v = Arcadia_Map_get(thread, self->schemata, k);
  if (Arcadia_Value_isVoidValue(&v)) {
    return NULL;
  }
  return (Arcadia_DDLS_SchemaNode*)Arcadia_Value_getObjectReferenceValue(&v);
}

Arcadia_DDLS_SchemaNode*
Arcadia_DDLS_ValidationContext_removeSchema
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_String* name
  )
{
  Arcadia_Value k = Arcadia_Value_makeObjectReferenceValue(name);
  Arcadia_Value v;
  Arcadia_Map_remove(thread, self->schemata, k, NULL, &v);
  if (Arcadia_Value_isVoidValue(&v)) {
    return NULL;
  }
  return (Arcadia_DDLS_SchemaNode*)Arcadia_Value_getObjectReferenceValue(&v);
}

void
Arcadia_DDLS_ValidationContext_run
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_String* name,
    Arcadia_DDL_Node* node
  )
{ Arcadia_VirtualCall(Arcadia_DDLS_ValidationContext, run, self, name, node); }
