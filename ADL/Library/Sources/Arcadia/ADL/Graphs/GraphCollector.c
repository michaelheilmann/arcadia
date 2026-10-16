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
#include "Arcadia/ADL/Graphs/GraphCollector.h"

#include "Arcadia/ADL/Graphs/Graphs.module.h"
#include "Arcadia/ADL/Graphs/GraphFields.module.h"
#include "Arcadia/ADL/Graphs/GraphSymbol.module.h"
#include "Arcadia/ADL/Graphs/ReferenceFields.module.h"

static void
Arcadia_ADL_GraphCollector_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* self
  );

static void
Arcadia_ADL_GraphCollector_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollectorDispatch* self
  );

static void
Arcadia_ADL_GraphCollector_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_GraphCollector_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_GraphCollector_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_GraphCollector_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.GraphCollector", Arcadia_ADL_GraphCollector,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_ADL_GraphCollector_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* self
  )
{
  if (self->templates) Arcadia_Object_visit(thread, (Arcadia_Object*)self->templates);
  if (self->templateList) Arcadia_Object_visit(thread, (Arcadia_Object*)self->templateList);
  if (self->instances) Arcadia_Object_visit(thread, (Arcadia_Object*)self->instances);
  if (self->ordinary) Arcadia_Object_visit(thread, (Arcadia_Object*)self->ordinary);
}

static void
Arcadia_ADL_GraphCollector_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_GraphCollector);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->templates = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  self->templateList = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->instances = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->ordinary = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_ADL_GraphCollector);
}

static void
Arcadia_ADL_GraphCollector_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollectorDispatch* self
  )
{ }

Arcadia_ADL_GraphCollector*
Arcadia_ADL_GraphCollector_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_GraphCollector);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_GraphCollector);
}

// Get the local node name and check that it does not claim the reserved segment.
static Arcadia_String*
getLocalName
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* mapNode
  )
{
  Arcadia_String* name = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, mapNode, "name");
  if (Arcadia_ADL_Graphs_hasReservedSegment(thread, name)) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"name `", name, u8"` must not contain the reserved segment `__graph`");
  }
  return name;
}

static void
collectTemplate
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* self,
    Arcadia_DDL_MapNode* mapNode
  )
{
  Arcadia_String* name = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, mapNode, "name");
  if (Arcadia_ADL_Graphs_hasReservedSegment(thread, name)) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"graph template name `", name, u8"` must not contain the reserved segment `__graph`");
  }
  Arcadia_Value existing = Arcadia_Map_get(thread, self->templates, Arcadia_Value_makeObjectReferenceValue(name));
  if (!Arcadia_Value_isVoidValue(&existing)) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"duplicate graph template `", name, u8"`");
  }
  Arcadia_DDL_MapEntryNode* nodesEntry = Arcadia_ADL_GraphFields_getEntryOrNull(thread, mapNode, Arcadia_String_createFromCxxString(thread, "nodes"));
  if (NULL == nodesEntry || !Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)nodesEntry->value, _Arcadia_DDL_ListNode_getType(thread))) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"graph template `", name, u8"` requires a `nodes` list");
  }
  Arcadia_List* nodeElements = ((Arcadia_DDL_ListNode*)nodesEntry->value)->elements;
  Arcadia_List* nodes = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)nodeElements); i < n; ++i) {
    Arcadia_Value element = Arcadia_List_getAt(thread, nodeElements, i);
    if (!Arcadia_Value_isObjectReferenceValue(&element) ||
        !Arcadia_Object_isInstanceOf(thread, Arcadia_Value_getObjectReferenceValue(&element), _Arcadia_DDL_MapNode_getType(thread))) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"graph template `", name, u8"`: every node must be a map");
    }
    Arcadia_List_insertBack(thread, nodes, element);
  }
  Arcadia_ADL_GraphTemplate* graphTemplate = Arcadia_ADL_GraphTemplate_create(thread, name, nodes);
  Arcadia_Map_set(thread, self->templates, Arcadia_Value_makeObjectReferenceValue(name), Arcadia_Value_makeObjectReferenceValue(graphTemplate), NULL, NULL);
  Arcadia_List_insertBack(thread, self->templateList, Arcadia_Value_makeObjectReferenceValue(graphTemplate));
}

static void
collectInstance
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* self,
    Arcadia_DDL_MapNode* mapNode
  )
{
  Arcadia_String* name = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, mapNode, "name");
  if (Arcadia_ADL_Graphs_hasReservedSegment(thread, name)) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"graph instance name `", name, u8"` must not contain the reserved segment `__graph`");
  }
  Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, mapNode, "graph");
  if (NULL == Arcadia_ADL_GraphFields_getEntryOrNull(thread, mapNode, Arcadia_String_createFromCxxString(thread, "bindings"))) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"graph instance `", name, u8"` requires a `bindings` field");
  }
  Arcadia_List_insertBack(thread, self->instances, Arcadia_Value_makeObjectReferenceValue(mapNode));
}

void
Arcadia_ADL_GraphCollector_addNode
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* self,
    Arcadia_DDL_Node* node
  )
{
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDL_MapNode_getType(thread))) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"graph batch root nodes must be maps", NULL, NULL);
  }
  Arcadia_DDL_MapNode* mapNode = (Arcadia_DDL_MapNode*)node;
  Arcadia_String* type = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, mapNode, "type");
  if (Arcadia_ADL_Graphs_stringEquals(thread, type, "GraphDefinition")) {
    collectTemplate(thread, self, mapNode);
  } else if (Arcadia_ADL_Graphs_stringEquals(thread, type, "GraphInstance")) {
    collectInstance(thread, self, mapNode);
  } else if (Arcadia_ADL_Graphs_stringEquals(thread, type, "Graph.Parameter") ||
             Arcadia_ADL_Graphs_stringEquals(thread, type, "Graph.Output")) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"type `", type, u8"` is only allowed inside a graph definition");
  } else {
    Arcadia_List_insertBack(thread, self->ordinary, Arcadia_Value_makeObjectReferenceValue(mapNode));
  }
}

// ---------------------------------------------------------------------------
// Template validation.
// ---------------------------------------------------------------------------

static Arcadia_BooleanValue
isPixelBufferOperationOrPixelBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_String* type
  )
{
  return (Arcadia_ADL_ReferenceFields_isPixelBufferOperationType(thread, type) ||
          Arcadia_ADL_Graphs_stringEquals(thread, type, "PixelBuffer")) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}

static Arcadia_ADL_GraphSymbol*
symbolTable_getOrNull
  (
    Arcadia_Thread* thread,
    Arcadia_Map* symbolTable,
    Arcadia_String* name
  )
{
  Arcadia_Value v = Arcadia_Map_get(thread, symbolTable, Arcadia_Value_makeObjectReferenceValue(name));
  if (Arcadia_Value_isVoidValue(&v)) {
    return NULL;
  }
  return (Arcadia_ADL_GraphSymbol*)Arcadia_Value_getObjectReferenceValue(&v);
}

// Add each reference target name of the specified node to the symbol's `referencedNames`.
// HashMap treats void values as removal, so edges need a non-void presence marker.
static void
collectReferencedNames
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphSymbol* symbol
  )
{
  Arcadia_DDL_MapNode* node = symbol->node;
  if (Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "Graph.Output")) {
    Arcadia_String* source = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, node, "source");
    Arcadia_Map_set(thread, symbol->referencedNames, Arcadia_Value_makeObjectReferenceValue(source), Arcadia_Value_makeSizeValue(1), NULL, NULL);
    return;
  }
  // Value fields name a value-typed parameter; record the edge so that the
  // parameter resolution and cycle checks treat them uniformly.
  if (Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "DSP.SineWave")) {
    Arcadia_String* frequency = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, node, "frequency");
    Arcadia_Map_set(thread, symbol->referencedNames, Arcadia_Value_makeObjectReferenceValue(frequency), Arcadia_Value_makeSizeValue(1), NULL, NULL);
    return;
  }
  if (Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "DSP.Mix")) {
    Arcadia_DDL_MapEntryNode* inputs = Arcadia_ADL_GraphFields_getEntryOrNull(thread, node, Arcadia_String_createFromCxxString(thread, "inputs"));
    if (NULL == inputs ||
        !Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)inputs->value, _Arcadia_DDL_ListNode_getType(thread)) ||
        0 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_DDL_ListNode*)inputs->value)->elements)) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"graph-local `DSP.Mix` requires a nonempty `inputs` list: ", symbol->name, NULL);
    }
  } else if (!isPixelBufferOperationOrPixelBuffer(thread, symbol->typeName)) {
    return;
  }
  for (Arcadia_SizeValue e = 0, ne = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->entries); e < ne; ++e) {
    Arcadia_DDL_MapEntryNode* entry = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)node->entries, e, _Arcadia_DDL_MapEntryNode_getType(thread));
    Arcadia_ADL_ReferenceFieldDescriptor const* descriptor = Arcadia_ADL_ReferenceFields_find(thread, symbol->typeName, entry->key->value);
    if (NULL == descriptor) {
      continue;
    }
    if (Arcadia_ADL_ReferenceFieldKind_Single == descriptor->kind) {
      if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)entry->value, _Arcadia_DDL_StringNode_getType(thread))) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"field `", Arcadia_String_createFromCxxString(thread, descriptor->fieldName), u8"` must be a reference string");
      }
      Arcadia_Map_set(thread, symbol->referencedNames, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)((Arcadia_DDL_StringNode*)entry->value)->value), Arcadia_Value_makeSizeValue(1), NULL, NULL);
    } else {
      if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)entry->value, _Arcadia_DDL_ListNode_getType(thread))) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"field `", Arcadia_String_createFromCxxString(thread, descriptor->fieldName), u8"` must be a reference list");
      }
      Arcadia_List* elements = ((Arcadia_DDL_ListNode*)entry->value)->elements;
      for (Arcadia_SizeValue le = 0, nle = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)elements); le < nle; ++le) {
        Arcadia_Value leValue = Arcadia_List_getAt(thread, elements, le);
        if (!Arcadia_Value_isObjectReferenceValue(&leValue) ||
            !Arcadia_Object_isInstanceOf(thread, Arcadia_Value_getObjectReferenceValue(&leValue), _Arcadia_DDL_StringNode_getType(thread))) {
          Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"field `", Arcadia_String_createFromCxxString(thread, descriptor->fieldName), u8"` must be a list of reference strings");
        }
        Arcadia_Map_set(thread, symbol->referencedNames, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)((Arcadia_DDL_StringNode*)Arcadia_Value_getObjectReferenceValue(&leValue))->value), Arcadia_Value_makeSizeValue(1), NULL, NULL);
      }
    }
  }
}

// Validate one template. Does not touch any definitions container.
static void
validateGraphTemplate
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphTemplate* graphTemplate
  )
{
  Arcadia_SizeValue const numberOfNodes = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)graphTemplate->nodes);
  Arcadia_List* symbols = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_Map* symbolTable = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Map* parameterByName = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  graphTemplate->symbols = symbols;
  graphTemplate->symbolTable = symbolTable;
  graphTemplate->parameterByName = parameterByName;
  Arcadia_SizeValue numberOfOutputs = 0;

  for (Arcadia_SizeValue i = 0; i < numberOfNodes; ++i) {
    Arcadia_Value element = Arcadia_List_getAt(thread, graphTemplate->nodes, i);
    Arcadia_DDL_MapNode* node = (Arcadia_DDL_MapNode*)Arcadia_Value_getObjectReferenceValue(&element);
    Arcadia_String* type = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, node, "type");
    Arcadia_String* name = getLocalName(thread, node);
    if (NULL != symbolTable_getOrNull(thread, symbolTable, name)) {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"duplicate graph-local name `", name, u8"`");
    }
    Arcadia_ADL_GraphSymbol* symbol = Arcadia_ADL_GraphSymbol_create(thread);
    symbol->typeName = type;
    symbol->name = name;
    symbol->node = node;
    if (Arcadia_ADL_Graphs_stringEquals(thread, type, "Graph.Parameter")) {
      Arcadia_DDL_MapEntryNode* referenceTypeEntry = Arcadia_ADL_GraphFields_getEntryOrNull(thread, node, Arcadia_String_createFromCxxString(thread, "referenceType"));
      Arcadia_DDL_MapEntryNode* valueTypeEntry = Arcadia_ADL_GraphFields_getEntryOrNull(thread, node, Arcadia_String_createFromCxxString(thread, "valueType"));
      if (NULL != referenceTypeEntry && NULL != valueTypeEntry) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"a `Graph.Parameter` must not declare both `referenceType` and `valueType`: ", name, NULL);
      }
      if (NULL != valueTypeEntry) {
        Arcadia_String* valueType = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, node, "valueType");
        if (!Arcadia_ADL_Graphs_stringEquals(thread, valueType, "Integer32")) {
          Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"unsupported parameter value type `", valueType, u8"`");
        }
        symbol->valueType = valueType;
      } else {
        Arcadia_String* referenceType = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, node, "referenceType");
        if (!Arcadia_ADL_Graphs_stringEquals(thread, referenceType, "Color")) {
          Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"unsupported parameter reference type `", referenceType, u8"`");
        }
        symbol->referenceType = referenceType;
      }
      Arcadia_Map_set(thread, parameterByName, Arcadia_Value_makeObjectReferenceValue(name), Arcadia_Value_makeObjectReferenceValue(symbol), NULL, NULL);
    } else if (Arcadia_ADL_Graphs_stringEquals(thread, type, "Graph.Output")) {
      Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, node, "source");
      Arcadia_String* referenceType = Arcadia_ADL_GraphFields_getNonEmptyStringField(thread, node, "referenceType");
      if (!Arcadia_ADL_Graphs_stringEquals(thread, referenceType, "PixelBuffer") &&
          !Arcadia_ADL_ReferenceFields_isDSPDefinitionType(thread, referenceType)) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"unsupported graph output reference type `", referenceType, u8"`");
      }
      graphTemplate->outputSymbol = symbol;
      ++numberOfOutputs;
    } else if (Arcadia_ADL_Graphs_stringEquals(thread, type, "Color") ||
               Arcadia_ADL_Graphs_stringEquals(thread, type, "PixelBuffer") ||
               Arcadia_ADL_ReferenceFields_isDSPDefinitionType(thread, type) ||
               Arcadia_ADL_ReferenceFields_isPixelBufferOperationType(thread, type)) {
      // supported ordinary local node type
    } else {
      Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"unsupported graph-local node type `", type, u8"`");
    }
    Arcadia_List_insertBack(thread, symbols, Arcadia_Value_makeObjectReferenceValue(symbol));
    Arcadia_Map_set(thread, symbolTable, Arcadia_Value_makeObjectReferenceValue(name), Arcadia_Value_makeObjectReferenceValue(symbol), NULL, NULL);
  }

  if (1 != numberOfOutputs) {
    Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"graph `", graphTemplate->name, u8"` must contain exactly one `Graph.Output`");
  }

  // Gather reference edges and resolve each target against the local symbol table.
  for (Arcadia_SizeValue i = 0; i < numberOfNodes; ++i) {
    Arcadia_Value symbolValue = Arcadia_List_getAt(thread, symbols, i);
    Arcadia_ADL_GraphSymbol* symbol = (Arcadia_ADL_GraphSymbol*)Arcadia_Value_getObjectReferenceValue(&symbolValue);
    collectReferencedNames(thread, symbol);

    Arcadia_List* names = Arcadia_Map_getKeys(thread, symbol->referencedNames);
    for (Arcadia_SizeValue r = 0, nr = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)names); r < nr; ++r) {
      Arcadia_Value keyValue = Arcadia_List_getAt(thread, names, r);
      Arcadia_String* targetName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&keyValue);
      Arcadia_ADL_GraphSymbol* target = symbolTable_getOrNull(thread, symbolTable, targetName);
      if (NULL == target) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"unresolved graph-local reference `", targetName, u8"`");
      }
      if (Arcadia_ADL_Graphs_stringEquals(thread, target->typeName, "Graph.Output")) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"a `Graph.Output` node must not be referenced: ", targetName, NULL);
      }
      if (Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "DSP.Mix") &&
          !Arcadia_ADL_ReferenceFields_isDSPDefinitionType(thread, target->typeName)) {
        Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"a mix input must reference a graph-local DSP definition: ", targetName, NULL);
      }
      if (Arcadia_ADL_Graphs_stringEquals(thread, target->typeName, "Graph.Parameter")) {
        // A value-bearing field must be fed by a value-typed parameter, and a
        // reference field by a reference-typed parameter of the matching type.
        if (Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "DSP.SineWave")) {
          if (NULL == target->valueType || !Arcadia_ADL_Graphs_stringEquals(thread, target->valueType, "Integer32")) {
            Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"parameter value type mismatch: ", targetName, NULL);
          }
        } else {
          if (Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "PixelBuffer")) {
            Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"a `Graph.Parameter` must not be used as a pixel buffer operation: ", targetName, NULL);
          }
          if (NULL == target->referenceType || !Arcadia_ADL_Graphs_stringEquals(thread, target->referenceType, "Color")) {
            Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"parameter reference type mismatch: ", targetName, NULL);
          }
        }
      } else {
        if (Arcadia_ADL_ReferenceFields_isPixelBufferOperationType(thread, symbol->typeName) &&
            !Arcadia_ADL_Graphs_stringEquals(thread, target->typeName, "Color")) {
          Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"a checkerboard color must reference a `Color` node: ", targetName, NULL);
        }
        if (Arcadia_ADL_Graphs_stringEquals(thread, symbol->typeName, "PixelBuffer") &&
            !Arcadia_ADL_ReferenceFields_isPixelBufferOperationType(thread, target->typeName)) {
          Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"a pixel buffer operation must reference a pixel buffer operation node: ", targetName, NULL);
        }
      }
    }
  }

  // Detect cycles among local reference edges (an acyclic graph is required).
  {
    Arcadia_Natural8Value* marks = (Arcadia_Natural8Value*)Arcadia_Memory_allocateUnmanaged(thread, numberOfNodes * sizeof(Arcadia_Natural8Value));
    for (Arcadia_SizeValue i = 0; i < numberOfNodes; ++i) {
      marks[i] = 0 /* white */;
    }
    for (Arcadia_SizeValue start = 0; start < numberOfNodes; ++start) {
      if (0 != marks[start]) {
        continue;
      }
      Arcadia_List* stack = (Arcadia_List*)Arcadia_ArrayList_create(thread);
      Arcadia_List_insertBackNatural32Value(thread, stack, start);
      while (0 != Arcadia_Collection_getSize(thread, (Arcadia_Collection*)stack)) {
        Arcadia_SizeValue stackSize = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)stack);
        Arcadia_SizeValue nodeIndex = Arcadia_List_getNatural32ValueAt(thread, stack, stackSize - 1);
        if (1 /* gray */ == marks[nodeIndex]) {
          Arcadia_List_removeBack(thread, stack, 1);
          marks[nodeIndex] = 2 /* black */;
          continue;
        }
        marks[nodeIndex] = 1 /* gray */;
        Arcadia_Value symbolValue = Arcadia_List_getAt(thread, symbols, nodeIndex);
        Arcadia_ADL_GraphSymbol* symbol = (Arcadia_ADL_GraphSymbol*)Arcadia_Value_getObjectReferenceValue(&symbolValue);
        Arcadia_List* names = Arcadia_Map_getKeys(thread, symbol->referencedNames);
        for (Arcadia_SizeValue r = 0, nr = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)names); r < nr; ++r) {
          Arcadia_Value keyValue = Arcadia_List_getAt(thread, names, r);
          Arcadia_String* targetName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&keyValue);
          Arcadia_ADL_GraphSymbol* target = symbolTable_getOrNull(thread, symbolTable, targetName);
          if (NULL == target ||
              Arcadia_ADL_Graphs_stringEquals(thread, target->typeName, "Graph.Parameter") ||
              Arcadia_ADL_Graphs_stringEquals(thread, target->typeName, "Graph.Output")) {
            continue;
          }
          Arcadia_SizeValue targetIndex = 0;
          for (Arcadia_SizeValue k = 0; k < numberOfNodes; ++k) {
            Arcadia_Value candidateValue = Arcadia_List_getAt(thread, symbols, k);
            if ((Arcadia_ADL_GraphSymbol*)Arcadia_Value_getObjectReferenceValue(&candidateValue) == target) {
              targetIndex = k;
              break;
            }
          }
          if (1 == marks[targetIndex]) {
            Arcadia_ADL_Graphs_raise(thread, Arcadia_Status_SemanticalError, u8"cyclic graph-local reference through `", targetName, u8"`");
          }
          if (0 == marks[targetIndex]) {
            Arcadia_List_insertBackNatural32Value(thread, stack, targetIndex);
          }
        }
      }
    }
    Arcadia_Memory_deallocateUnmanaged(thread, marks);
  }
}

void
Arcadia_ADL_GraphCollector_validate
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* self
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->templateList); i < n; ++i) {
    Arcadia_ADL_GraphTemplate* graphTemplate =
      (Arcadia_ADL_GraphTemplate*)
      Arcadia_List_getObjectReferenceValueCheckedAt(thread, self->templateList, i, _Arcadia_ADL_GraphTemplate_getType(thread));
    validateGraphTemplate(thread, graphTemplate);
  }
}
