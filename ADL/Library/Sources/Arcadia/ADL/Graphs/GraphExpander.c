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
#include "Arcadia/ADL/Graphs/GraphExpander.h"

#include "Arcadia/ADL/Graphs/Graphs.module.h"
#include "Arcadia/ADL/Graphs/GraphFields.module.h"
#include "Arcadia/ADL/Graphs/GraphSymbol.module.h"
#include "Arcadia/ADL/Graphs/ReferenceFields.module.h"
#include "Arcadia/ADL/Graphs/ValueFields.module.h"

#include "Arcadia/ADL/Context.h"

static void
Arcadia_ADL_GraphExpander_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphExpander* self
  );

static void
Arcadia_ADL_GraphExpander_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphExpanderDispatch* self
  );

static void
Arcadia_ADL_GraphExpander_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphExpander* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_GraphExpander_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_GraphExpander_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_GraphExpander_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.GraphExpander", Arcadia_ADL_GraphExpander,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_ADL_GraphExpander_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphExpander* self
  )
{
  if (self->collector) Arcadia_Object_visit(thread, (Arcadia_Object*)self->collector);
}

static void
Arcadia_ADL_GraphExpander_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphExpander* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_GraphExpander);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->collector = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ADL_GraphCollector_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_ADL_GraphExpander);
}

static void
Arcadia_ADL_GraphExpander_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphExpanderDispatch* self
  )
{ }

Arcadia_ADL_GraphExpander*
Arcadia_ADL_GraphExpander_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* collector
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_GraphExpander);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)collector);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_ADL_GraphExpander);
}

// ---------------------------------------------------------------------------
// Generic helpers.
// ---------------------------------------------------------------------------

// Clone a DDL node into fresh nodes (no dependence on the stored template AST).
static Arcadia_DDL_Node*
cloneNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node
  )
{
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDL_StringNode_getType(thread))) {
    return (Arcadia_DDL_Node*)Arcadia_DDL_StringNode_createString(thread, ((Arcadia_DDL_StringNode*)node)->value);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDL_NumberNode_getType(thread))) {
    return (Arcadia_DDL_Node*)Arcadia_DDL_NumberNode_createString(thread, ((Arcadia_DDL_NumberNode*)node)->value);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDL_BooleanNode_getType(thread))) {
    return (Arcadia_DDL_Node*)Arcadia_DDL_BooleanNode_createBoolean(thread, ((Arcadia_DDL_BooleanNode*)node)->value);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDL_VoidNode_getType(thread))) {
    return (Arcadia_DDL_Node*)Arcadia_DDL_VoidNode_createVoid(thread, Arcadia_VoidValue_Void);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDL_ListNode_getType(thread))) {
    Arcadia_List* elements = ((Arcadia_DDL_ListNode*)node)->elements;
    Arcadia_DDL_ListNode* listNode = Arcadia_DDL_ListNode_create(thread);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)elements); i < n; ++i) {
      Arcadia_Value element = Arcadia_List_getAt(thread, elements, i);
      Arcadia_DDL_Node* child = (Arcadia_DDL_Node*)Arcadia_Value_getObjectReferenceValue(&element);
      Arcadia_List_insertBack(thread, listNode->elements, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)cloneNode(thread, child)));
    }
    return (Arcadia_DDL_Node*)listNode;
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDL_MapNode_getType(thread))) {
    Arcadia_DDL_MapNode* mapNode = (Arcadia_DDL_MapNode*)node;
    Arcadia_DDL_MapNode* result = Arcadia_DDL_MapNode_create(thread);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)mapNode->entries); i < n; ++i) {
      Arcadia_DDL_MapEntryNode* entry = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)mapNode->entries, i, _Arcadia_DDL_MapEntryNode_getType(thread));
      Arcadia_DDL_NameNode* key = Arcadia_DDL_NameNode_create(thread, entry->key->value);
      Arcadia_DDL_Node* value = cloneNode(thread, entry->value);
      Arcadia_DDL_MapNode_insertBack(thread, result, Arcadia_DDL_MapEntryNode_create(thread, key, value));
    }
    return (Arcadia_DDL_Node*)result;
  }
  Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_NotImplemented, u8"unsupported node type in graph template", NULL, NULL);
  return NULL;
}

// ---------------------------------------------------------------------------
// Template symbol lookup.
// ---------------------------------------------------------------------------

static Arcadia_ADL_GraphSymbol*
template_symbolOrNull
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphTemplate* graphTemplate,
    Arcadia_String* name
  )
{
  Arcadia_Value v = Arcadia_Map_get(thread, graphTemplate->symbolTable, Arcadia_Value_makeObjectReferenceValue(name));
  if (Arcadia_Value_isVoidValue(&v)) {
    return NULL;
  }
  return (Arcadia_ADL_GraphSymbol*)Arcadia_Value_getObjectReferenceValue(&v);
}

// ---------------------------------------------------------------------------
// Binding validation.
// ---------------------------------------------------------------------------

// Get the `source` string of the binding for the specified parameter, or NULL if unbound.
static Arcadia_String*
bindingSourceOrNull
  (
    Arcadia_Thread* thread,
    Arcadia_List* bindings,
    Arcadia_String* parameterName
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)bindings); i < n; ++i) {
    Arcadia_Value element = Arcadia_List_getAt(thread, bindings, i);
    Arcadia_DDL_MapNode* binding = (Arcadia_DDL_MapNode*)Arcadia_Value_getObjectReferenceValue(&element);
    Arcadia_String* parameter = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, binding, "parameter");
    if (Arcadia_ADL_Graphs_stringEqualsString(thread, parameter, parameterName)) {
      return Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, binding, "source");
    }
  }
  return NULL;
}

// Get the `value` number node of the binding for the specified parameter, or NULL if unbound.
static Arcadia_DDL_NumberNode*
bindingValueOrNull
  (
    Arcadia_Thread* thread,
    Arcadia_List* bindings,
    Arcadia_String* parameterName
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)bindings); i < n; ++i) {
    Arcadia_Value element = Arcadia_List_getAt(thread, bindings, i);
    Arcadia_DDL_MapNode* binding = (Arcadia_DDL_MapNode*)Arcadia_Value_getObjectReferenceValue(&element);
    Arcadia_String* parameter = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, binding, "parameter");
    if (Arcadia_ADL_Graphs_stringEqualsString(thread, parameter, parameterName)) {
      Arcadia_DDL_MapEntryNode* valueEntry = Arcadia_ADL_GraphFields_getEntryOrNull(thread, binding, Arcadia_String_createFromCxxString(thread, "value"));
      if (NULL == valueEntry) {
        return NULL;
      }
      if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)valueEntry->value, _Arcadia_DDL_NumberNode_getType(thread))) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"binding value for parameter `", parameterName, u8"` must be a number");
      }
      return (Arcadia_DDL_NumberNode*)valueEntry->value;
    }
  }
  return NULL;
}

// Get if the specified binding map carries a `value` field.
static Arcadia_BooleanValue
bindingHasValueField
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* binding
  )
{
  return NULL != Arcadia_ADL_GraphFields_getEntryOrNull(thread, binding, Arcadia_String_createFromCxxString(thread, "value")) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}

// Validate the bindings of one instance: structural, unknown parameter, duplicate, missing, source/value exists.
static void
validateInstanceBindings
  (
    Arcadia_Thread* thread,
    Arcadia_String* instanceName,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_List* bindings,
    Arcadia_ADL_GraphTemplate* graphTemplate
  )
{
  Arcadia_Map* seen = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)bindings); i < n; ++i) {
    Arcadia_Value element = Arcadia_List_getAt(thread, bindings, i);
    if (!Arcadia_Value_isObjectReferenceValue(&element) ||
        !Arcadia_Object_isInstanceOf(thread, Arcadia_Value_getObjectReferenceValue(&element), _Arcadia_DDL_MapNode_getType(thread))) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: every binding must be a map");
    }
    Arcadia_DDL_MapNode* binding = (Arcadia_DDL_MapNode*)Arcadia_Value_getObjectReferenceValue(&element);
    Arcadia_String* parameter = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, binding, "parameter");
    Arcadia_ADL_GraphSymbol* symbol = template_symbolOrNull(thread, graphTemplate, parameter);
    if (NULL == symbol || !Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "Graph.Parameter")) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: binding for an unknown parameter");
    }
    Arcadia_Value previous = Arcadia_Map_get(thread, seen, Arcadia_Value_makeObjectReferenceValue(parameter));
    if (!Arcadia_Value_isVoidValue(&previous)) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: duplicate binding for a parameter");
    }
    Arcadia_Map_set(thread, seen, Arcadia_Value_makeObjectReferenceValue(parameter), Arcadia_Value_makeSizeValue(1), NULL, NULL);
    Arcadia_BooleanValue isValueParameter = (NULL != symbol->valueType) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
    Arcadia_BooleanValue hasSource = (NULL != Arcadia_ADL_GraphFields_getEntryOrNull(thread, binding, Arcadia_String_createFromCxxString(thread, "source"))) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
    Arcadia_BooleanValue hasValue = bindingHasValueField(thread, binding);
    if (Arcadia_BooleanValue_True == isValueParameter) {
      if (Arcadia_BooleanValue_True == hasSource) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: a value parameter must be bound with `value`, not `source`");
      }
      if (Arcadia_BooleanValue_True != hasValue) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: value parameter binding requires a `value` field");
      }
      Arcadia_DDL_NumberNode* valueNode = bindingValueOrNull(thread, bindings, parameter);
      if (NULL == valueNode) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: value parameter binding requires a `value` field");
      }
    } else {
      if (Arcadia_BooleanValue_True == hasValue) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: a reference parameter must be bound with `source`, not `value`");
      }
      if (Arcadia_BooleanValue_True != hasSource) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: reference parameter binding requires a `source` field");
      }
      Arcadia_String* source = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, binding, "source");
      if (NULL == Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, source)) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: binding source does not name a definition");
      }
    }
  }
  Arcadia_List* parameterNames = Arcadia_Map_getKeys(thread, graphTemplate->parameterByName);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)parameterNames); i < n; ++i) {
    Arcadia_Value keyValue = Arcadia_List_getAt(thread, parameterNames, i);
    Arcadia_String* parameterName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&keyValue);
    Arcadia_Value previous = Arcadia_Map_get(thread, seen, Arcadia_Value_makeObjectReferenceValue(parameterName));
    if (Arcadia_Value_isVoidValue(&previous)) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: missing binding for a parameter");
    }
  }
}

// ---------------------------------------------------------------------------
// Expansion.
// ---------------------------------------------------------------------------

// Rewrite the reference fields of a freshly cloned ordinary node from local names to generated names.
// `generatedNames` maps local name to generated Arcadia_String.
static void
rewriteReferenceFields
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* node,
    Arcadia_String* typeName,
    Arcadia_Map* generatedNames
  )
{
  for (Arcadia_SizeValue e = 0, ne = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->entries); e < ne; ++e) {
    Arcadia_DDL_MapEntryNode* entry = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)node->entries, e, _Arcadia_DDL_MapEntryNode_getType(thread));
    Arcadia_ADL_ReferenceFieldDescriptor const* descriptor = Arcadia_ADL_ReferenceFields_find(thread, typeName, entry->key->value);
    if (NULL == descriptor) {
      continue;
    }
    if (Arcadia_ADL_ReferenceFieldKind_Single == descriptor->kind) {
      Arcadia_String* local = ((Arcadia_DDL_StringNode*)entry->value)->value;
      Arcadia_Value generated = Arcadia_Map_get(thread, generatedNames, Arcadia_Value_makeObjectReferenceValue(local));
      entry->value = (Arcadia_DDL_Node*)Arcadia_DDL_StringNode_createString(thread, (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&generated));
    } else {
      Arcadia_List* elements = ((Arcadia_DDL_ListNode*)entry->value)->elements;
      Arcadia_List* rewritten = (Arcadia_List*)Arcadia_ArrayList_create(thread);
      for (Arcadia_SizeValue le = 0, nle = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)elements); le < nle; ++le) {
        Arcadia_Value leValue = Arcadia_List_getAt(thread, elements, le);
        Arcadia_String* local = ((Arcadia_DDL_StringNode*)Arcadia_Value_getObjectReferenceValue(&leValue))->value;
        Arcadia_Value generated = Arcadia_Map_get(thread, generatedNames, Arcadia_Value_makeObjectReferenceValue(local));
        Arcadia_List_insertBack(thread, rewritten, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)Arcadia_DDL_StringNode_createString(thread, (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&generated))));
      }
      ((Arcadia_DDL_ListNode*)entry->value)->elements = rewritten;
    }
  }
}

// Parse a value node as an Integer32, mapping a DDL conversion failure (fractional value or
// out-of-range integer) to a semantical error with a stable status for callers.
static Arcadia_Integer32Value
parseInteger32Value
  (
    Arcadia_Thread* thread,
    Arcadia_String* text,
    const char* fieldName
  )
{
  Arcadia_Integer32Value value = 0;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    /* Happy path: no jump was taken. */
    value = Arcadia_String_toInteger32(thread, text);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    /* A jump occurred: the DDL conversion failed. */
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"value field `", Arcadia_String_createFromCxxString(thread, fieldName), u8"` must be an `Integer32` value");
  }
  return value;
}

// Rewrite the value fields of a freshly cloned ordinary node: replace the graph-source-level
// reference string with a typed DDL number node carrying the bound integer.
// `bindings` is the instance binding list; the parameter is looked up by its local name.
// The parser checked integer-ness and range; negative frequencies are rejected to keep
// the generated sine well-formed.
static void
rewriteValueFields
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* node,
    Arcadia_String* typeName,
    Arcadia_List* bindings
  )
{
  for (Arcadia_SizeValue e = 0, ne = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->entries); e < ne; ++e) {
    Arcadia_DDL_MapEntryNode* entry = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)node->entries, e, _Arcadia_DDL_MapEntryNode_getType(thread));
    Arcadia_ADL_ValueFieldDescriptor const* descriptor = Arcadia_ADL_ValueFields_find(thread, typeName, entry->key->value);
    if (NULL == descriptor) {
      continue;
    }
    if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)entry->value, _Arcadia_DDL_StringNode_getType(thread))) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"value field `", Arcadia_String_createFromCxxString(thread, descriptor->fieldName), u8"` must be a parameter reference string");
    }
    Arcadia_String* parameterName = ((Arcadia_DDL_StringNode*)entry->value)->value;
    Arcadia_DDL_NumberNode* valueNode = bindingValueOrNull(thread, bindings, parameterName);
    if (NULL == valueNode) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"value field `", Arcadia_String_createFromCxxString(thread, descriptor->fieldName), u8"` is not bound");
    }
    Arcadia_Integer32Value value = parseInteger32Value(thread, valueNode->value, descriptor->fieldName);
    if (value < 0) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"value field `", Arcadia_String_createFromCxxString(thread, descriptor->fieldName), u8"` must not be negative");
    }
    entry->value = (Arcadia_DDL_Node*)Arcadia_DDL_NumberNode_createInteger32(thread, value);
  }
}

// Build the generated name map for one instance:
//   Graph.Parameter  -> the bound external source name
//   ordinary node    -> <instance>.__graph.<local>
//   Graph.Output     -> <instance>.<output-name>  (the output's source node is published directly)
static Arcadia_Map*
buildGeneratedNames
  (
    Arcadia_Thread* thread,
    Arcadia_String* instanceName,
    Arcadia_List* bindings,
    Arcadia_ADL_GraphTemplate* graphTemplate
  )
{
  Arcadia_Map* names = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)graphTemplate->symbols); i < n; ++i) {
    Arcadia_Value symbolValue = Arcadia_List_getAt(thread, graphTemplate->symbols, i);
    Arcadia_ADL_GraphSymbol* symbol = (Arcadia_ADL_GraphSymbol*)Arcadia_Value_getObjectReferenceValue(&symbolValue);
    if (Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "Graph.Parameter")) {
      if (NULL != symbol->valueType) {
        // A value parameter is substituted directly into a value field; it has no generated name.
        continue;
      }
      Arcadia_String* source = bindingSourceOrNull(thread, bindings, symbol->name);
      Arcadia_Map_set(thread, names, Arcadia_Value_makeObjectReferenceValue(symbol->name), Arcadia_Value_makeObjectReferenceValue(source), NULL, NULL);
    } else if (Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "Graph.Output")) {
      // The output node itself is not emitted; only its source is published.
    } else {
      Arcadia_Map_set(thread, names, Arcadia_Value_makeObjectReferenceValue(symbol->name), Arcadia_Value_makeObjectReferenceValue(Arcadia_ADL_Graphs_concat(thread, instanceName, ".__graph.", symbol->name)), NULL, NULL);
    }
  }
  // The output source node is published under <instance>.<output-name>.
  Arcadia_String* outputName = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, graphTemplate->outputSymbol->node, "name");
  Arcadia_String* sourceName = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, graphTemplate->outputSymbol->node, "source");
  Arcadia_String* publishedName = Arcadia_ADL_Graphs_concat(thread, instanceName, ".", outputName);
  Arcadia_Map_set(thread, names, Arcadia_Value_makeObjectReferenceValue(sourceName), Arcadia_Value_makeObjectReferenceValue(publishedName), NULL, NULL);
  return names;
}

// Expand one instance: build generated names, clone + rewrite each local ordinary node,
// and register each generated node into `stage` (a map from generated name to DDL node).
static void
expandInstance
  (
    Arcadia_Thread* thread,
    Arcadia_String* instanceName,
    Arcadia_List* bindings,
    Arcadia_ADL_GraphTemplate* graphTemplate,
    Arcadia_Map* stage
  )
{
  Arcadia_Map* names = buildGeneratedNames(thread, instanceName, bindings, graphTemplate);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)graphTemplate->symbols); i < n; ++i) {
    Arcadia_Value symbolValue = Arcadia_List_getAt(thread, graphTemplate->symbols, i);
    Arcadia_ADL_GraphSymbol* symbol = (Arcadia_ADL_GraphSymbol*)Arcadia_Value_getObjectReferenceValue(&symbolValue);
    if (Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "Graph.Parameter") ||
        Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "Graph.Output")) {
      continue;
    }
    Arcadia_DDL_MapNode* clone = (Arcadia_DDL_MapNode*)cloneNode(thread, (Arcadia_DDL_Node*)symbol->node);
    rewriteReferenceFields(thread, clone, symbol->typeName, names);
    rewriteValueFields(thread, clone, symbol->typeName, bindings);
    // Rename the node to its generated internal name.
    Arcadia_Value generated = Arcadia_Map_get(thread, names, Arcadia_Value_makeObjectReferenceValue(symbol->name));
    Arcadia_String* generatedName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&generated);
    for (Arcadia_SizeValue e = 0, ne = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)clone->entries); e < ne; ++e) {
      Arcadia_DDL_MapEntryNode* entry = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)clone->entries, e, _Arcadia_DDL_MapEntryNode_getType(thread));
      if (Arcadia_ADL_Graphs_stringEquals(thread, entry->key->value, "name")) {
        entry->value = (Arcadia_DDL_Node*)Arcadia_DDL_StringNode_createString(thread, generatedName);
      }
    }
    Arcadia_Map_set(thread, stage, Arcadia_Value_makeObjectReferenceValue(generatedName), Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)clone), NULL, NULL);
  }
}

// ---------------------------------------------------------------------------
// Public entry.
// ---------------------------------------------------------------------------

void
Arcadia_ADL_GraphExpander_expandInto
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphExpander* self,
    Arcadia_ADL_Definitions* definitions
  )
{
  Arcadia_ADL_GraphCollector* collector = self->collector;
  Arcadia_List* instances = collector->instances;

  // Phase 1: validate every binding and stage every generated node for every instance.
  // Nothing is published to `definitions` until all instances have succeeded.
  Arcadia_List* stagedNodes = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)instances); i < n; ++i) {
    Arcadia_Value instanceValue = Arcadia_List_getAt(thread, instances, i);
    Arcadia_DDL_MapNode* instanceNode = (Arcadia_DDL_MapNode*)Arcadia_Value_getObjectReferenceValue(&instanceValue);
    Arcadia_String* instanceName = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, instanceNode, "name");
    Arcadia_String* graphName = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, instanceNode, "graph");
    Arcadia_Value templateValue = Arcadia_Map_get(thread, collector->templates, Arcadia_Value_makeObjectReferenceValue(graphName));
    if (Arcadia_Value_isVoidValue(&templateValue)) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: unknown graph");
    }
    Arcadia_ADL_GraphTemplate* graphTemplate = (Arcadia_ADL_GraphTemplate*)Arcadia_Value_getObjectReferenceValue(&templateValue);
    Arcadia_DDL_MapEntryNode* bindingsEntry = Arcadia_ADL_GraphFields_getEntryOrNull(thread, instanceNode, Arcadia_String_createFromCxxString(thread, "bindings"));
    if (NULL == bindingsEntry || !Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)bindingsEntry->value, _Arcadia_DDL_ListNode_getType(thread))) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"instance `", instanceName, u8"`: `bindings` must be a list");
    }
    Arcadia_List* bindings = ((Arcadia_DDL_ListNode*)bindingsEntry->value)->elements;

    validateInstanceBindings(thread, instanceName, definitions, bindings, graphTemplate);

    Arcadia_Map* stage = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
    expandInstance(thread, instanceName, bindings, graphTemplate, stage);
    Arcadia_List_insertBack(thread, stagedNodes, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)stage));
  }

  // Phase 2: publish. Register generated nodes into `definitions` and read them with the ordinary readers.
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)stagedNodes); i < n; ++i) {
    Arcadia_Value stageValue = Arcadia_List_getAt(thread, stagedNodes, i);
    Arcadia_Map* stage = (Arcadia_Map*)Arcadia_Value_getObjectReferenceValue(&stageValue);
    Arcadia_List* generatedNodes = Arcadia_Map_getValues(thread, stage);
    for (Arcadia_SizeValue g = 0, ng = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)generatedNodes); g < ng; ++g) {
      Arcadia_Value nodeValue = Arcadia_List_getAt(thread, generatedNodes, g);
      Arcadia_DDL_MapNode* node = (Arcadia_DDL_MapNode*)Arcadia_Value_getObjectReferenceValue(&nodeValue);
      Arcadia_ADL_Context_readFromNode(thread, context, definitions, (Arcadia_DDL_Node*)node, Arcadia_BooleanValue_False);
    }
  }
}
