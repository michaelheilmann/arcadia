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
#include "Arcadia/DDL/Reader/Parser.h"

#include "Arcadia/Logging/Include.h"
#include "Arcadia/DDL/Include.h"

#include "Arcadia/DDL/Nodes/Include.h"

/// @brief Get the text of the word.
/// @param thread A pointer to this thread.
/// @param self A pointer to this Data Definition Language parser.
/// @return The text of the word.
static Arcadia_String*
Arcadia_DDL_Parser_getWordText
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

/// @brief Get the type of the word.
/// @param thread A pointer to this thread.
/// @param self A pointer to this Data Definition Language parser.
/// @return The type of the word.
static Arcadia_Natural32Value
Arcadia_DDL_Parser_getWordType
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static void
Arcadia_DDL_Parser_next
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static Arcadia_DDL_ListNode*
Arcadia_DDL_Parser_onListValue
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static Arcadia_DDL_MapEntryNode*
Arcadia_DDL_Parser_onMapEntry
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static Arcadia_DDL_MapNode*
Arcadia_DDL_Parser_onMapValue
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static Arcadia_DDL_NameNode*
Arcadia_DDL_Parser_onName
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static Arcadia_DDL_Node*
Arcadia_DDL_Parser_onValue
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static Arcadia_Languages_Diagnostics*
Arcadia_DDL_Parser_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static Arcadia_UnicodeCodePointReader*
Arcadia_DDL_Parser_getInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static Arcadia_Languages_StringTable*
Arcadia_DDL_Parser_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static Arcadia_Value
Arcadia_DDL_Parser_runImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static void
Arcadia_DDL_Parser_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self,
    Arcadia_UnicodeCodePointReader* input
  );

struct Arcadia_DDL_ParserDispatch {
  Arcadia_Languages_ParserDispatch _parent;
};

struct Arcadia_DDL_Parser {
  Arcadia_Languages_Parser _parent;
  Arcadia_DDL_Scanner* scanner;
};

static void
Arcadia_DDL_Parser_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static void
Arcadia_DDL_Parser_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ParserDispatch* self
  );

static void
Arcadia_DDL_Parser_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_Parser_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_Parser_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_Parser_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.Parser", Arcadia_DDL_Parser,
                         u8"Arcadia.Languages.Parser", Arcadia_Languages_Parser,
                         &_typeOperations);

static void
Arcadia_DDL_Parser_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_Parser);
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
  self->scanner = Arcadia_DDL_Scanner_create(thread, Arcadia_Languages_StringTable_getOrCreate(thread),
                                                     Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread)));
  //
  Arcadia_LeaveConstructor(Arcadia_DDL_Parser);
}

static void
Arcadia_DDL_Parser_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ParserDispatch* self
  )
{
  ((Arcadia_Languages_ParserDispatch*)self)->getDiagnostics = (Arcadia_Languages_Diagnostics * (*)(Arcadia_Thread*, Arcadia_Languages_Parser*)) & Arcadia_DDL_Parser_getDiagnosticsImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->getInput = (Arcadia_UnicodeCodePointReader*(*)(Arcadia_Thread*, Arcadia_Languages_Parser*)) & Arcadia_DDL_Parser_getInputImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->getStringTable = (Arcadia_Languages_StringTable* (*)(Arcadia_Thread*, Arcadia_Languages_Parser*))&Arcadia_DDL_Parser_getStringTableImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->run = (Arcadia_Value(*)(Arcadia_Thread*, Arcadia_Languages_Parser*)) & Arcadia_DDL_Parser_runImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->setInput = (void(*)(Arcadia_Thread*, Arcadia_Languages_Parser*, Arcadia_UnicodeCodePointReader*)) & Arcadia_DDL_Parser_setInputImpl;
}

static void
Arcadia_DDL_Parser_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{
  if (self->scanner) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->scanner);
  }
}

Arcadia_DDL_Parser*
Arcadia_DDL_Parser_create
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* scanner
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_Parser);
  if (scanner) Arcadia_ValueStack_pushObjectReferenceValue(thread, scanner); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_Parser);
}

static Arcadia_String*
Arcadia_DDL_Parser_getWordText
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{ return Arcadia_Languages_Scanner_getWordText(thread, (Arcadia_Languages_Scanner*)self->scanner); }

static Arcadia_Natural32Value
Arcadia_DDL_Parser_getWordType
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{ return Arcadia_Languages_Scanner_getWordType(thread, (Arcadia_Languages_Scanner*)self->scanner); }

static void
Arcadia_DDL_Parser_next
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{
  do {
    Arcadia_Languages_Scanner_step(thread, (Arcadia_Languages_Scanner*)self->scanner);
  } while (Arcadia_DDL_WordType_MultiLineComment == Arcadia_DDL_Parser_getWordType(thread, self) ||
           Arcadia_DDL_WordType_SingleLineComment == Arcadia_DDL_Parser_getWordType(thread, self) ||
           Arcadia_DDL_WordType_WhiteSpace == Arcadia_DDL_Parser_getWordType(thread, self) ||
           Arcadia_DDL_WordType_LineTerminator == Arcadia_DDL_Parser_getWordType(thread, self));
}

static Arcadia_DDL_ListNode*
Arcadia_DDL_Parser_onListValue
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{
  Arcadia_DDL_ListNode* listNode = Arcadia_DDL_ListNode_create(thread);
  Arcadia_DDL_Parser_next(thread, self);
  // As long as we do nor hit a right square bracket or the end of the input, stay in loop.
  while (Arcadia_DDL_WordType_RightSquareBracket != Arcadia_DDL_Parser_getWordType(thread, self) &&
         Arcadia_DDL_WordType_EndOfInput != Arcadia_DDL_Parser_getWordType(thread, self)) {
    Arcadia_DDL_Node* listElementNode = Arcadia_DDL_Parser_onValue(thread, self);
    Arcadia_List_insertBackObjectReferenceValue(thread, listNode->elements, (Arcadia_Object*)listElementNode);
    // If no comma follows, break the loop.
    if (Arcadia_DDL_WordType_Comma != Arcadia_DDL_Parser_getWordType(thread, self)) {
      break;
    } else {
      Arcadia_DDL_Parser_next(thread, self);
    }
  }
  if (Arcadia_DDL_WordType_RightSquareBracket != Arcadia_DDL_Parser_getWordType(thread, self)) {
    Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"expected right square bracket\n");
    Arcadia_Languages_DiagnosticsOld_emit(thread, message);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_Parser_next(thread, self);
  return listNode;
}

static Arcadia_DDL_MapEntryNode*
Arcadia_DDL_Parser_onMapEntry
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{
  Arcadia_DDL_NameNode* keyNode = Arcadia_DDL_Parser_onName(thread, self);
  if (Arcadia_DDL_WordType_Colon != Arcadia_DDL_Parser_getWordType(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_Parser_next(thread, self);
  Arcadia_DDL_Node* valueNode = Arcadia_DDL_Parser_onValue(thread, self);
  Arcadia_DDL_MapEntryNode* mapEntryNode =
    Arcadia_DDL_MapEntryNode_create(thread, keyNode, valueNode);
  return mapEntryNode;
}

static Arcadia_DDL_MapNode*
Arcadia_DDL_Parser_onMapValue
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{
  Arcadia_DDL_MapNode* mapNode = Arcadia_DDL_MapNode_create(thread);
  Arcadia_DDL_Parser_next(thread, self);
  // As long as we do nor hit a right curly bracket or the end of the input, stay in loop.
  while (Arcadia_DDL_WordType_RightCurlyBracket != Arcadia_DDL_Parser_getWordType(thread, self) &&
         Arcadia_DDL_WordType_EndOfInput != Arcadia_DDL_Parser_getWordType(thread, self)) {
    Arcadia_DDL_MapEntryNode* mapEntryNode =
      Arcadia_DDL_Parser_onMapEntry(thread, self);
    Arcadia_List_insertBackObjectReferenceValue(thread, mapNode->entries, (Arcadia_Object*)mapEntryNode);
    // If no comma follows, break the loop.
    if (Arcadia_DDL_WordType_Comma != Arcadia_DDL_Parser_getWordType(thread, self)) {
      break;
    } else {
      Arcadia_DDL_Parser_next(thread, self);
    }
  }
  if (Arcadia_DDL_WordType_RightCurlyBracket != Arcadia_DDL_Parser_getWordType(thread, self)) {
    Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"expected right curly bracket\n");
    Arcadia_Languages_DiagnosticsOld_emit(thread, message);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_Parser_next(thread, self);
  return mapNode;
}

static Arcadia_DDL_NameNode*
Arcadia_DDL_Parser_onName
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{
  if (Arcadia_DDL_WordType_Name != Arcadia_DDL_Parser_getWordType(thread, self)) {
    Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"expected name\n");
    Arcadia_Languages_DiagnosticsOld_emit(thread, message);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_NameNode* nameNode =
      Arcadia_DDL_NameNode_create(thread, Arcadia_DDL_Parser_getWordText(thread, self));
  Arcadia_DDL_Parser_next(thread, self);
  return nameNode;
}

static Arcadia_DDL_Node*
Arcadia_DDL_Parser_onValue
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{
  switch (Arcadia_DDL_Parser_getWordType(thread, self)) {
    case Arcadia_DDL_WordType_BooleanLiteral: {
      Arcadia_DDL_Node* node = NULL;
      node = (Arcadia_DDL_Node*)
        Arcadia_DDL_BooleanNode_createString(thread, Arcadia_DDL_Parser_getWordText(thread, self));
      Arcadia_DDL_Parser_next(thread, self);
      return node;
    } break;
    case Arcadia_DDL_WordType_IntegerLiteral: {
      Arcadia_DDL_Node* node = NULL;
      node = (Arcadia_DDL_Node*)
        Arcadia_DDL_NumberNode_createString(thread, Arcadia_DDL_Parser_getWordText(thread, self));
      Arcadia_DDL_Parser_next(thread, self);
      return node;
    } break;
    case Arcadia_DDL_WordType_LeftCurlyBracket: {
      Arcadia_DDL_Node* node = NULL;
      node = (Arcadia_DDL_Node*)
        Arcadia_DDL_Parser_onMapValue(thread, self);
      return node;
    } break;
    case Arcadia_DDL_WordType_LeftSquareBracket: {
      Arcadia_DDL_Node* node = NULL;
      node = (Arcadia_DDL_Node*)
        Arcadia_DDL_Parser_onListValue(thread, self);
      return node;
    } break;
    case Arcadia_DDL_WordType_RealLiteral: {
      Arcadia_DDL_Node* node = NULL;
      node = (Arcadia_DDL_Node*)
        Arcadia_DDL_NumberNode_createString(thread, Arcadia_DDL_Parser_getWordText(thread, self));
      Arcadia_DDL_Parser_next(thread, self);
      return node;
    } break;
    case Arcadia_DDL_WordType_StringLiteral: {
      Arcadia_DDL_Node* node = NULL;
      node = (Arcadia_DDL_Node*)
        Arcadia_DDL_StringNode_createString(thread, Arcadia_DDL_Parser_getWordText(thread, self));
      Arcadia_DDL_Parser_next(thread, self);
      return node;
    } break;
    case Arcadia_DDL_WordType_VoidLiteral: {
      Arcadia_DDL_Node* node = NULL;
      node = (Arcadia_DDL_Node*)
        Arcadia_DDL_VoidNode_createString(thread, Arcadia_DDL_Parser_getWordText(thread, self));
      Arcadia_DDL_Parser_next(thread, self);
      return node;
    } break;
    default: {
      Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
      Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"expected scalar or aggregate\n");
      Arcadia_Languages_DiagnosticsOld_emit(thread, message);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

static  Arcadia_Languages_Diagnostics*
Arcadia_DDL_Parser_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{ return Arcadia_Languages_Scanner_getDiagnostics(thread, (Arcadia_Languages_Scanner*)self->scanner); }

static Arcadia_UnicodeCodePointReader*
Arcadia_DDL_Parser_getInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{ return Arcadia_Languages_Scanner_getInput(thread, (Arcadia_Languages_Scanner*)self->scanner); }

static Arcadia_Languages_StringTable*
Arcadia_DDL_Parser_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{ return Arcadia_Languages_Scanner_getStringTable(thread, (Arcadia_Languages_Scanner*)self->scanner); }

static Arcadia_Value
Arcadia_DDL_Parser_runImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self
  )
{
  if (Arcadia_DDL_WordType_StartOfInput != Arcadia_DDL_Parser_getWordType(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_DDL_Parser_next(thread, self);
  Arcadia_DDL_Node* valueNode = Arcadia_DDL_Parser_onValue(thread, self);
  if (Arcadia_DDL_WordType_Comma == Arcadia_DDL_Parser_getWordType(thread, self)) {
    Arcadia_DDL_Parser_next(thread, self);
  }
  if (Arcadia_DDL_WordType_EndOfInput != Arcadia_DDL_Parser_getWordType(thread, self)) {
    Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"expected end of input\n");
    Arcadia_Languages_DiagnosticsOld_emit(thread, message);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Value_makeObjectReferenceValue(valueNode);
}

static void
Arcadia_DDL_Parser_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* self,
    Arcadia_UnicodeCodePointReader* input
  )
{
  Arcadia_Languages_Scanner_setInput(thread, (Arcadia_Languages_Scanner*)self->scanner, input);
}