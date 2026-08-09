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

#include "Arcadia/DDLS/Syntactical/DefaultReader.h"

#include "Arcadia/Logging/Include.h"
#include "Arcadia/DDLS/Extensions.h"
#include "Arcadia/DDLS/Symbols/SymbolReader.h"

#include "Arcadia/DDLS/Nodes/Include.h"

#include "Arcadia/DDLS/Symbols/AnySymbol.h"
#include "Arcadia/DDLS/Symbols/ChoiceSymbol.h"
#include "Arcadia/DDLS/Symbols/ListSymbol.h"
#include "Arcadia/DDLS/Symbols/MapSymbol.h"
#include "Arcadia/DDLS/Symbols/MapEntrySymbol.h"
#include "Arcadia/DDLS/Symbols/ScalarSymbol.h"
#include "Arcadia/DDLS/Symbols/SchemaReferenceSymbol.h"
#include "Arcadia/DDLS/Symbols/SchemaSymbol.h"

static Arcadia_DDLS_AnyNode*
readAnyNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_AnySymbol* source
  );

static Arcadia_DDLS_ChoiceNode*
readChoiceNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_ChoiceSymbol* source
  );

static Arcadia_DDLS_ListNode*
readListNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_ListSymbol* source
  );

static Arcadia_DDLS_MapNode*
readMapNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_MapSymbol* source
  );

static Arcadia_DDLS_Node*
readNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_Symbol* source
  );

static Arcadia_DDLS_ScalarNode*
readScalarNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_ScalarSymbol* source
  );

static Arcadia_DDLS_SchemaNode*
readSchemaNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_SchemaSymbol* source
  );

static Arcadia_DDLS_SchemaReferenceNode*
readSchemaReferenceNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_SchemaReferenceSymbol* source
  );

static void
Arcadia_DDLS_DefaultReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self
  );

static void
Arcadia_DDLS_DefaultReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReaderDispatch* self
  );

static void
Arcadia_DDLS_DefaultReader_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self
  );

static Arcadia_DDLS_Node*
Arcadia_DDLS_DefaultReader_runImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_ByteArray* input
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_DefaultReader_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_DefaultReader_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_DefaultReader_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDLS_DefaultReader_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_DefaultReader_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_DefaultReader_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.DefaultReader", Arcadia_DDLS_DefaultReader,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_DDLS_DefaultReader_typeOperations);

static Arcadia_DDLS_AnyNode*
readAnyNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_AnySymbol* source
  )
{
  Arcadia_DDLS_AnyNode* target = Arcadia_DDLS_AnyNode_create(thread);
  return target;
}

static Arcadia_DDLS_ChoiceNode*
readChoiceNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_ChoiceSymbol* source
  )
{
  Arcadia_DDLS_ChoiceNode* target = Arcadia_DDLS_ChoiceNode_create(thread);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)source->choices); i < n; ++i) {
    Arcadia_DDLS_Symbol* symbolElement = (Arcadia_DDLS_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, source->choices, i, _Arcadia_DDLS_Symbol_getType(thread));
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbolElement, _Arcadia_DDLS_SchemaSymbol_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_DDLS_Node* symbolNode = readNode(thread, self, symbolElement);
    Arcadia_List_insertBackObjectReferenceValue(thread, target->choices, (Arcadia_Object*)symbolNode);
  }
  return target;
}

static Arcadia_DDLS_ListNode*
readListNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_ListSymbol* source
  )
{
  Arcadia_DDLS_Node* listElementNode = readNode(thread, self, source->entry);
  Arcadia_DDLS_ListNode* listNode = Arcadia_DDLS_ListNode_create(thread, listElementNode);
  return listNode;
}

static Arcadia_DDLS_MapNode*
readMapNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_MapSymbol* source
  )
{
  Arcadia_DDLS_MapNode* target = Arcadia_DDLS_MapNode_create(thread);
  Arcadia_List* sourceKeys = Arcadia_Map_getKeys(thread, source->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)sourceKeys); i < n; ++i) {
    Arcadia_Value v = Arcadia_Map_get(thread, source->entries, Arcadia_List_getAt(thread, sourceKeys, i));
    if (!Arcadia_Value_isInstanceOf(thread, &v, _Arcadia_DDLS_MapEntrySymbol_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_DDLS_MapEntrySymbol* o = (Arcadia_DDLS_MapEntrySymbol*)Arcadia_Value_getObjectReferenceValue(&v);
    Arcadia_String* targetName = o->entryName;
    Arcadia_DDLS_Node* targetValue = readNode(thread, self, o->entrySymbol);
    Arcadia_Map_set(thread, target->entries, Arcadia_Value_makeObjectReferenceValue(targetName),
                                             Arcadia_Value_makeObjectReferenceValue(targetValue),
                                             NULL,
                                             NULL);
  }
  return target;
}

static Arcadia_DDLS_Node*
readNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_Symbol* source
  )
{
  switch (source->kind) {
   case Arcadia_DDLS_SymbolKind_Any: {
     return (Arcadia_DDLS_Node*)readAnyNode(thread, self, (Arcadia_DDLS_AnySymbol*)source);
   } break;
   case Arcadia_DDLS_SymbolKind_Choice: {
     return (Arcadia_DDLS_Node*)readChoiceNode(thread, self, (Arcadia_DDLS_ChoiceSymbol*)source);
   } break;
   case Arcadia_DDLS_SymbolKind_List: {
     return (Arcadia_DDLS_Node*)readListNode(thread, self, (Arcadia_DDLS_ListSymbol*)source);
   } break;
   case Arcadia_DDLS_SymbolKind_Map: {
      return (Arcadia_DDLS_Node*)readMapNode(thread, self, (Arcadia_DDLS_MapSymbol*)source);
    } break;
    case Arcadia_DDLS_SymbolKind_Scalar: {
      return (Arcadia_DDLS_Node*)readScalarNode(thread, self, (Arcadia_DDLS_ScalarSymbol*)source);
    } break;
    case Arcadia_DDLS_SymbolKind_SchemaReference: {
      return (Arcadia_DDLS_Node*)readSchemaReferenceNode(thread, self, (Arcadia_DDLS_SchemaReferenceSymbol*)source);
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

static Arcadia_DDLS_ScalarNode*
readScalarNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_ScalarSymbol* source
  )
{
  if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)source->name, &self->BOOLEAN)) {
    Arcadia_DDLS_ScalarNode* target = Arcadia_DDLS_ScalarNode_create(thread);
    target->name = source->name;
    return target;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)source->name, &self->NUMBER)) {
    Arcadia_DDLS_ScalarNode* target = Arcadia_DDLS_ScalarNode_create(thread);
    target->name = source->name;
    return target;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)source->name, &self->STRING)) {
    Arcadia_DDLS_ScalarNode* target = Arcadia_DDLS_ScalarNode_create(thread);
    target->name = source->name;
    return target;
  } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)source->name, &self->VOID)) {
    Arcadia_DDLS_ScalarNode* target = Arcadia_DDLS_ScalarNode_create(thread);
    target->name = source->name;
    return target;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_DDLS_SchemaNode*
readSchemaNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_SchemaSymbol* source
  )
{
  Arcadia_DDLS_SchemaSymbol* ddlsSchemaSymbol = (Arcadia_DDLS_SchemaSymbol*)source;
  Arcadia_DDLS_SchemaNode* ddlsSchemaNode = Arcadia_DDLS_SchemaNode_create(thread, ddlsSchemaSymbol->name);
  Arcadia_DDLS_Node* ddlsDefinitionNode = readNode(thread, self, ddlsSchemaSymbol->definition);
  ddlsSchemaNode->definition = ddlsDefinitionNode;
  return ddlsSchemaNode;
}

static Arcadia_DDLS_SchemaReferenceNode*
readSchemaReferenceNode
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_DDLS_SchemaReferenceSymbol* source
  )
{
  Arcadia_DDLS_SchemaReferenceNode* ddlsSchemaReferenceNode = Arcadia_DDLS_SchemaReferenceNode_create(thread, source->name);
  return ddlsSchemaReferenceNode;
}

static void
Arcadia_DDLS_DefaultReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_DefaultReader);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->parser = Arcadia_DDL_Parser_create(thread, Arcadia_DDL_Scanner_create(thread, Arcadia_Languages_StringTable_getOrCreate(thread),
                                                                                      Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread))));
  //
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_Languages_StringTable* stringTable = Arcadia_Languages_Parser_getStringTable(thread, (Arcadia_Languages_Parser*)self->parser);
#define Define(Variable, Text) \
  Arcadia_StringBuilder_clear(thread, stringBuffer); \
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, Text); \
  self->Variable = Arcadia_Value_makeObjectReferenceValue(Arcadia_Languages_StringTable_getOrCreateString(thread, stringTable, stringBuffer));

  Define(BOOLEAN, u8"Boolean");
  Define(NUMBER, u8"Number");
  Define(STRING, u8"String");
  Define(VOID, u8"Void");

#undef Define
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_DefaultReader);
}

static void
Arcadia_DDLS_DefaultReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReaderDispatch* self
  )
{
  self->run = &Arcadia_DDLS_DefaultReader_runImpl;
}

static void
Arcadia_DDLS_DefaultReader_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self
  )
{
  if (self->parser) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->parser);
  }
#define Define(Variable, Text) \
  Arcadia_Value_visit(thread, &self->Variable);

  Define(BOOLEAN, u8"Boolean");
  Define(NUMBER, u8"Number");
  Define(STRING, u8"String");
  Define(VOID, u8"Void");

#undef Define
}

static Arcadia_DDLS_Node*
Arcadia_DDLS_DefaultReader_runImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_ByteArray* input
  )
{
  Arcadia_Languages_StringTable* stringTable = Arcadia_Languages_StringTable_getOrCreate(thread);
  Arcadia_UnicodeCodePointReader* inputReader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, input));
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)self->parser, inputReader);
  Arcadia_DDL_Node* node = (Arcadia_DDL_Node*)Arcadia_Value_getObjectReferenceValueChecked(thread, Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)self->parser), _Arcadia_DDL_Node_getType(thread));
  Arcadia_DDLS_SymbolReader* symbolReader = Arcadia_DDLS_SymbolReader_create(thread, stringTable);
  Arcadia_DDLS_Symbol* symbol = Arcadia_DDLS_SymbolReader_run(thread, symbolReader, node);
  if (Arcadia_DDLS_SymbolKind_Schema != symbol->kind) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_DDLS_Node*)readSchemaNode(thread, self, (Arcadia_DDLS_SchemaSymbol*)symbol);
}

Arcadia_DDLS_DefaultReader*
Arcadia_DDLS_DefaultReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_DDLS_DefaultReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_DDLS_DefaultReader);
}

Arcadia_DDLS_Node*
Arcadia_DDLS_DefaultReader_run
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_DefaultReader* self,
    Arcadia_ByteArray* input
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_DDLS_DefaultReader, run, self, input); }
