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

#include "Arcadia.TemplateEngine/Parser/Parser.h"

#include "Arcadia.TemplateEngine/Diagnostics/UnexpectedWordDiagnostic.h"

static void
Arcadia_TemplateEngine_Parser_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static void
Arcadia_TemplateEngine_Parser_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static void
Arcadia_TemplateEngine_Parser_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static void
Arcadia_TemplateEngine_Parser_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_ParserDispatch* self
  );

static Arcadia_BooleanValue
isLeftParenthesis
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static Arcadia_BooleanValue
isRightParenthesis
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static Arcadia_String*
getWordText
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static void
step
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static Directives_Tree*
parseExpression
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static Directives_Tree*
parseStatement
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static Directives_Tree*
parseDirective
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static Directives_Tree*
parseFile
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static Arcadia_Languages_Diagnostics*
Arcadia_TemplateEngine_Parser_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static Arcadia_UnicodeCodePointReader*
Arcadia_TemplateEngine_Parser_getInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static Arcadia_Languages_StringTable*
Arcadia_TemplateEngine_Parser_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static void
Arcadia_TemplateEngine_Parser_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self,
    Arcadia_UnicodeCodePointReader* reader
  );

static Arcadia_Value
Arcadia_TemplateEngine_Parser_runImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_TemplateEngine_Parser_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_TemplateEngine_Parser_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_TemplateEngine_Parser_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_TemplateEngine_Parser_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.TemplateEngine.Parser", Arcadia_TemplateEngine_Parser,
                         u8"Arcadia.Languages.Parser", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_TemplateEngine_Parser_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_TemplateEngine_Parser_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{
  if (self->diagnostics) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics);
  }
  if (self->scanner) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->scanner);
  }
}

static void
Arcadia_TemplateEngine_Parser_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{
  Arcadia_EnterConstructor(Arcadia_TemplateEngine_Parser);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->diagnostics = (Arcadia_Languages_Diagnostics*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _Arcadia_Languages_Diagnostics_getType(thread));
  Arcadia_String* file = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_String_getType(thread));
  Arcadia_Natural64Value line = Arcadia_ValueStack_getNatural64Value(thread, 2);
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_UnicodeCodePointReader_getType(thread));
  self->scanner = Arcadia_TemplateEngine_Scanner_create(thread, self->diagnostics, file, line, reader);
  Arcadia_LeaveConstructor(Arcadia_TemplateEngine_Parser);
}

static void
Arcadia_TemplateEngine_Parser_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_ParserDispatch* self
  )
{
  ((Arcadia_Languages_ParserDispatch*)self)->getDiagnostics = (Arcadia_Languages_Diagnostics * (*)(Arcadia_Thread*, Arcadia_Languages_Parser*)) & Arcadia_TemplateEngine_Parser_getDiagnosticsImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->getInput = (Arcadia_UnicodeCodePointReader * (*)(Arcadia_Thread*, Arcadia_Languages_Parser*)) & Arcadia_TemplateEngine_Parser_getInputImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->getStringTable = (Arcadia_Languages_StringTable* (*)(Arcadia_Thread*, Arcadia_Languages_Parser*)) & Arcadia_TemplateEngine_Parser_getStringTableImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->run = (Arcadia_Value (*)(Arcadia_Thread*, Arcadia_Languages_Parser*))& Arcadia_TemplateEngine_Parser_runImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->setInput = (void (*)(Arcadia_Thread*, Arcadia_Languages_Parser*, Arcadia_UnicodeCodePointReader*)) & Arcadia_TemplateEngine_Parser_setInputImpl;
}

static Arcadia_BooleanValue
isLeftParenthesis
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{
  return self->scanner->wordType == Arcadia_TemplateEngine_WordType_LeftParenthesis;
}

static Arcadia_BooleanValue
isRightParenthesis
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{
  return self->scanner->wordType == Arcadia_TemplateEngine_WordType_RightParenthesis;
}

static Arcadia_String*
getWordText
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{
  return Arcadia_Languages_Scanner_getWordText(thread, (Arcadia_Languages_Scanner*)self->scanner);
}

static void
step
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{
  Arcadia_Languages_Scanner_step(thread, (Arcadia_Languages_Scanner*)self->scanner);
}

static Directives_Tree*
parseExpression
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{
  Directives_Tree* ast = NULL;
  if (self->scanner->wordType != Arcadia_TemplateEngine_WordType_Name) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_String* name = getWordText(thread, self);
  step(thread, self);
  if (isLeftParenthesis(thread, self)) {
    step(thread, self);
    Arcadia_List* arguments = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    while (self->scanner->wordType == Arcadia_TemplateEngine_WordType_StringLiteral) {
      Arcadia_String* argument = getWordText(thread, self);
      Arcadia_List_insertBackObjectReferenceValue(thread, arguments, (Arcadia_ObjectReferenceValue)argument);
      step(thread, self);
      if (self->scanner->wordType == Arcadia_TemplateEngine_WordType_Comma) {
        step(thread, self);
      }
    }
    if (!isRightParenthesis(thread, self)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    step(thread, self);
    ast = Directives_Tree_createInvokeExpr(thread, name, arguments);
  } else {
    ast = Directives_Tree_createNameExpr(thread, name);
  }
  return ast;
}

static Directives_Tree*
parseStatement
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{
  return parseExpression(thread, self);
}

static Directives_Tree*
parseDirective
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{
  step(thread, self);
  switch (self->scanner->wordType) {
    case Arcadia_TemplateEngine_WordType_AtLiteral: {
      Directives_Tree* ast = Directives_Tree_createAtLiteralExpr(thread);
      return ast;
    } break;
    case Arcadia_TemplateEngine_WordType_LeftCurlyBracket: {
      step(thread, self);
      Directives_Tree* ast = parseStatement(thread, self);
      if (self->scanner->wordType != Arcadia_TemplateEngine_WordType_RightCurlyBracket) {
        Arcadia_Languages_Diagnostic* diagnostic = (Arcadia_Languages_Diagnostic*)Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, self->scanner->wordType);
        Arcadia_Languages_Diagnostics_add(thread, Arcadia_Languages_Parser_getDiagnostics(thread, (Arcadia_Languages_Parser*)self), diagnostic);
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      return ast;
    } break;
    default: {
      Arcadia_Languages_Diagnostic* diagnostic = (Arcadia_Languages_Diagnostic*)Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, self->scanner->wordType);
      Arcadia_Languages_Diagnostics_add(thread, Arcadia_Languages_Parser_getDiagnostics(thread, (Arcadia_Languages_Parser*)self), diagnostic);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

static Directives_Tree*
parseFile
  ( 
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{ 
  self->scanner->textMode = Arcadia_BooleanValue_True;
  if (Arcadia_TemplateEngine_WordType_StartOfInput != Arcadia_Languages_Scanner_getWordType(thread, (Arcadia_Languages_Scanner*)self->scanner)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
    Arcadia_Thread_jump(thread);
  }
  Directives_Tree* fileAST = Directives_Tree_createFile(thread);
  while (Arcadia_TemplateEngine_WordType_EndOfInput != Arcadia_Languages_Scanner_getWordType(thread, (Arcadia_Languages_Scanner*)self->scanner) &&
    Arcadia_TemplateEngine_WordType_Error != Arcadia_Languages_Scanner_getWordType(thread, (Arcadia_Languages_Scanner*)self->scanner)) {
    Arcadia_Languages_Scanner_step(thread, (Arcadia_Languages_Scanner*)self->scanner);
    if (Arcadia_TemplateEngine_WordType_StartOfDirective == Arcadia_Languages_Scanner_getWordType(thread, (Arcadia_Languages_Scanner*)self->scanner)) {
      self->scanner->textMode = Arcadia_BooleanValue_False;
      Directives_Tree* ast = parseDirective(thread, self);
      Arcadia_List_insertBackObjectReferenceValue(thread, fileAST->file.children, (Arcadia_Object*)ast);
      self->scanner->textMode = Arcadia_BooleanValue_True;
    } else if (Arcadia_TemplateEngine_WordType_Text == Arcadia_Languages_Scanner_getWordType(thread, (Arcadia_Languages_Scanner*)self->scanner)) {
      Directives_Tree* ast = Directives_Tree_createText(thread, Arcadia_Languages_Scanner_getWordText(thread, (Arcadia_Languages_Scanner*)self->scanner));
      Arcadia_List_insertBackObjectReferenceValue(thread, fileAST->file.children, (Arcadia_Object*)ast);
    }
  }
  return fileAST;
}

static Arcadia_Languages_Diagnostics*
Arcadia_TemplateEngine_Parser_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{ return Arcadia_Languages_Scanner_getDiagnostics(thread, (Arcadia_Languages_Scanner*)self->scanner); }

static Arcadia_UnicodeCodePointReader*
Arcadia_TemplateEngine_Parser_getInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{ return Arcadia_Languages_Scanner_getInput(thread, (Arcadia_Languages_Scanner*)self->scanner); }

static Arcadia_Languages_StringTable*
Arcadia_TemplateEngine_Parser_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{ return Arcadia_Languages_Scanner_getStringTable(thread, (Arcadia_Languages_Scanner*)self->scanner); }

static void
Arcadia_TemplateEngine_Parser_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self,
    Arcadia_UnicodeCodePointReader* reader
  )
{
  Arcadia_Languages_Scanner_setInput(thread, (Arcadia_Languages_Scanner*)self->scanner, reader);
}

static Arcadia_Value
Arcadia_TemplateEngine_Parser_runImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self
  )
{ return Arcadia_Value_makeObjectReferenceValue(parseFile(thread, self)); }

Arcadia_TemplateEngine_Parser*
Arcadia_TemplateEngine_Parser_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_String* file,
	  Arcadia_Natural64Value line,
    Arcadia_UnicodeCodePointReader* reader
  )
{
  _Arcadia_BeginCreate(Arcadia_TemplateEngine_Parser);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)diagnostics);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)file);
  Arcadia_ValueStack_pushNatural64Value(thread, line);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)reader);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_TemplateEngine_Parser);
}

void
Arcadia_TemplateEngine_Parser_setPosition
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self,
    Arcadia_String* file,
    Arcadia_Natural64Value line
  )
{ 
  Arcadia_TemplateEngine_Scanner_setPosition(thread, self->scanner, file, line);
}
