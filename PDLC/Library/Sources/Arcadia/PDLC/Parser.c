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

#define ARCADIA_PDLC_PRIVATE (1)
#include "Arcadia/PDLC/Parser.h"

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/AST/Include.h"
#include "Arcadia/PDLC/Scanner.h"
#include "Arcadia/PDLC/Names.h"
#include <assert.h>
#include <string.h>

struct Arcadia_PDLC_ParserDispatch {
  Arcadia_Languages_ParserDispatch _parent;
};

struct Arcadia_PDLC_Parser {
  Arcadia_Languages_Parser _parent;

  Arcadia_PDLC_Context* context;

  Arcadia_Languages_StringTable* stringTable;
  Arcadia_Languages_Diagnostics* diagnostics;
  Arcadia_PDLC_Scanner* scanner;
};

static void
Arcadia_PDLC_Parser_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static void
Arcadia_PDLC_Parser_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_ParserDispatch* self
  );

static void
Arcadia_PDLC_Parser_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static void
Arcadia_PDLC_Parser_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_BooleanValue
is
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_PDLC_WordType type
  );

static void
next
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_Parser*
Arcadia_PDLC_Parser_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  );

static Arcadia_Languages_Diagnostics*
Arcadia_PDLC_Parser_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_UnicodeCodePointReader*
Arcadia_PDLC_Parser_getInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_Languages_StringTable*
Arcadia_PDLC_Parser_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_Value
Arcadia_PDLC_Parser_runImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static void
Arcadia_PDLC_Parser_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_UnicodeCodePointReader* input
  );

/// Report the current word as unexpected and raise `Arcadia_Status_SyntacticalError`.
/// @remark Defined below, next to the other `on*` functions, but declared here because the
/// productions that need it precede that definition.
Arcadia_NoReturn() static void
onUnexpectedWord
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_Parser_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_PDLC_Parser_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_Parser_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_Parser_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MIL.Parser", Arcadia_PDLC_Parser,
                         u8"Arcadia.Languages.Parser", Arcadia_Languages_Parser,
                         &_typeOperations);

static void
Arcadia_PDLC_Parser_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_Parser);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->context = (Arcadia_PDLC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_PDLC_Context_getType(thread));
  self->scanner = Arcadia_PDLC_Scanner_getInstance(thread, self->context);
  self->stringTable = self->context->stringTable;
  self->diagnostics = self->context->diagnostics;
  Arcadia_LeaveConstructor(Arcadia_PDLC_Parser);
}

static void
Arcadia_PDLC_Parser_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_ParserDispatch* self
  )
{
  ((Arcadia_Languages_ParserDispatch*)self)->getDiagnostics = (Arcadia_Languages_Diagnostics * (*)(Arcadia_Thread*, Arcadia_Languages_Parser*)) & Arcadia_PDLC_Parser_getDiagnosticsImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->getInput = (Arcadia_UnicodeCodePointReader * (*)(Arcadia_Thread*, Arcadia_Languages_Parser*)) & Arcadia_PDLC_Parser_getInputImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->getStringTable = (Arcadia_Languages_StringTable * (*)(Arcadia_Thread*, Arcadia_Languages_Parser*)) & Arcadia_PDLC_Parser_getStringTableImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->run = (Arcadia_Value(*)(Arcadia_Thread*, Arcadia_Languages_Parser*)) & Arcadia_PDLC_Parser_runImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->setInput = (void (*)(Arcadia_Thread*, Arcadia_Languages_Parser*, Arcadia_UnicodeCodePointReader*)) & Arcadia_PDLC_Parser_setInputImpl;
}

static void
Arcadia_PDLC_Parser_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_Parser_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  if (self->context) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->context);
  }
  if (self->stringTable) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->stringTable);
  }
  if (self->diagnostics) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics);
  }
  if (self->scanner) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->scanner);
  }
}

static Arcadia_PDLC_WordType
getType
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{ return Arcadia_Languages_Scanner_getWordType(thread, (Arcadia_Languages_Scanner*)self->scanner); }

static Arcadia_SizeValue
getStartOffset
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{ return Arcadia_PDLC_Scanner_getStartOffset(thread, self->scanner); }

static Arcadia_String*
getText
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{ return Arcadia_Languages_Scanner_getWordText(thread, (Arcadia_Languages_Scanner*)self->scanner); }

static Arcadia_BooleanValue
is
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_PDLC_WordType type
  )
{ return type == getType(thread, self); }

static void
next
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  // Whitespace and comments are ignored between words. A line continuation is ignored as well: it joins an
  // instruction with the line that follows it. A line terminator is significant and is not ignored here.
  Arcadia_Languages_Scanner_step(thread, (Arcadia_Languages_Scanner*)self->scanner);
  while (Arcadia_PDLC_WordType_WhiteSpaces == getType(thread, self) || Arcadia_PDLC_WordType_SingleLineComment == getType(thread, self) || Arcadia_PDLC_WordType_MultiLineComment == getType(thread, self) || Arcadia_PDLC_WordType_LineContinuation == getType(thread, self)) {
    Arcadia_Languages_Scanner_step(thread, (Arcadia_Languages_Scanner*)self->scanner);
  }
}

static Arcadia_PDLC_Parser*
Arcadia_PDLC_Parser_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_Parser);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, context); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_PDLC_Parser);
}

Arcadia_PDLC_Parser*
Arcadia_PDLC_Parser_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  Arcadia_Value k = Arcadia_Value_makeTypeValue(_Arcadia_PDLC_Parser_getType(thread));
  Arcadia_Value v = Arcadia_Map_get(thread, context->instances, k);
  if (Arcadia_Value_isVoidValue(&v)) {
    v = Arcadia_Value_makeObjectReferenceValue(Arcadia_PDLC_Parser_create(thread, context));
    Arcadia_Map_set(thread, context->instances, k, v, NULL, NULL);
  }
  return (Arcadia_PDLC_Parser*)Arcadia_Value_getObjectReferenceValue(&v);
}

static Arcadia_PDLC_AST_OperandNode*
onExpression
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

/// Test if the current word can begin an `operand` production.
static Arcadia_BooleanValue
canStartOperand
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

/// Append the operands starting at the current word to `operands` until the current word cannot begin an
/// `operand` production. The end of the operands is the terminator of the enclosing instruction.
static void
onOperands
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_List* operands
  );

static Arcadia_PDLC_AST_IdentifierNode*
onIdentifier
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_InvokeInstructionNode*
onInvokeInstruction
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_ConstructInstructionNode*
onConstructInstruction
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_InstructionNode*
onInstruction
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

/// binaryInstruction : binaryOperator operand operand operand
static Arcadia_PDLC_AST_InstructionNode*
onBinaryInstruction
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_PDLC_AST_BinaryInstructionKind kind
  );

/// unaryInstruction : unaryOperator operand operand
static Arcadia_PDLC_AST_InstructionNode*
onUnaryInstruction
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_PDLC_AST_UnaryInstructionKind kind
  );

static void
onEndOfStatement
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_StatementNode*
onStatement
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_FieldDefinitionNode*
onParameter
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_List*
onParameters
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_ConstructorDefinitionNode*
onConstructorDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_MethodDefinitionNode*
onMethodDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_FieldDefinitionNode*
onFieldDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_Node*
onClassMemberDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_ClassDefinitionNode*
onClassDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_List*
onInterfaceNames
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_Node*
onInterfaceMemberDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_InterfaceDefinitionNode*
onInterfaceDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_Node*
onEnumerationMemberDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_EnumerationDefinitionNode*
onEnumerationDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_ModuleDefinitionNode*
onModuleDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_ProcedureDefinitionNode*
onProcedureDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

static Arcadia_PDLC_AST_CompilationUnitNode*
onCompilationUnit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  );

/// expression
///   literalExpression
///   name
///   register
/// literalExpression
///   booleanLiteral
///   integerLiteral
///   realLiteral
///   stringLiteral
///   voidLiteral
static Arcadia_PDLC_AST_OperandNode*
onExpression
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  switch (getType(thread, self)) {
    case Arcadia_PDLC_WordType_BooleanLiteral: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      Arcadia_PDLC_AST_OperandNode* expressionNode = (Arcadia_PDLC_AST_OperandNode*)Arcadia_PDLC_AST_LiteralOperandNode_create(thread, startOffset, (Arcadia_PDLC_AST_Node*)Arcadia_PDLC_AST_BooleanLiteralNode_create(thread, startOffset, getText(thread, self)));
      next(thread, self);
      return expressionNode;
    } break;
    case Arcadia_PDLC_WordType_IntegerLiteral: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      Arcadia_PDLC_AST_OperandNode* expressionNode = (Arcadia_PDLC_AST_OperandNode*)Arcadia_PDLC_AST_LiteralOperandNode_create(thread, startOffset, (Arcadia_PDLC_AST_Node*)Arcadia_PDLC_AST_IntegerLiteralNode_create(thread, startOffset, getText(thread, self)));
      next(thread, self);
      return expressionNode;
    } break;
    case Arcadia_PDLC_WordType_RealLiteral: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      Arcadia_PDLC_AST_OperandNode* expressionNode = (Arcadia_PDLC_AST_OperandNode*)Arcadia_PDLC_AST_LiteralOperandNode_create(thread, startOffset, (Arcadia_PDLC_AST_Node*)Arcadia_PDLC_AST_RealLiteralNode_create(thread, startOffset, getText(thread, self)));
      next(thread, self);
      return expressionNode;
    } break;
    case Arcadia_PDLC_WordType_StringLiteral: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      Arcadia_PDLC_AST_OperandNode* expressionNode = (Arcadia_PDLC_AST_OperandNode*)Arcadia_PDLC_AST_LiteralOperandNode_create(thread, startOffset, (Arcadia_PDLC_AST_Node*)Arcadia_PDLC_AST_StringLiteralNode_create(thread, startOffset, getText(thread, self)));
      next(thread, self);
      return expressionNode;
    } break;
    case Arcadia_PDLC_WordType_VoidLiteral: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      Arcadia_PDLC_AST_OperandNode* expressionNode = (Arcadia_PDLC_AST_OperandNode*)Arcadia_PDLC_AST_LiteralOperandNode_create(thread, startOffset, (Arcadia_PDLC_AST_Node*)Arcadia_PDLC_AST_VoidLiteralNode_create(thread, startOffset, getText(thread, self)));
      next(thread, self);
      return expressionNode;
    } break;
    case Arcadia_PDLC_WordType_Name: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (!Arcadia_JumpTarget_save(&jump)) {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_PDLC_AST_IdentifierNode* identifier = onIdentifier(thread, self);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, identifier);
      Arcadia_String* name = Arcadia_PDLC_Names_toTypeName(thread, identifier->names);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
      Arcadia_PDLC_AST_OperandNode* expressionNode = (Arcadia_PDLC_AST_OperandNode*)Arcadia_PDLC_AST_VariableOperandNode_create(thread, startOffset, name);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
      return expressionNode;
    } break;
    case Arcadia_PDLC_WordType_Register: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      Arcadia_PDLC_AST_OperandNode* expressionNode = (Arcadia_PDLC_AST_OperandNode*)Arcadia_PDLC_AST_RegisterOperandNode_create(thread, startOffset, getText(thread, self));
      next(thread, self);
      return expressionNode;
    } break;
    default: {
      onUnexpectedWord(thread, self);
    } break;
  };
}

static Arcadia_BooleanValue
canStartOperand
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  switch (getType(thread, self)) {
    case Arcadia_PDLC_WordType_BooleanLiteral:
    case Arcadia_PDLC_WordType_IntegerLiteral:
    case Arcadia_PDLC_WordType_RealLiteral:
    case Arcadia_PDLC_WordType_StringLiteral:
    case Arcadia_PDLC_WordType_VoidLiteral:
    case Arcadia_PDLC_WordType_Name:
    case Arcadia_PDLC_WordType_Register: {
      return Arcadia_BooleanValue_True;
    } break;
    default: {
      return Arcadia_BooleanValue_False;
    } break;
  };
}

static void
onOperands
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_List* operands
  )
{
  while (canStartOperand(thread, self)) {
    Arcadia_PDLC_AST_OperandNode* operand = onExpression(thread, self);
    Arcadia_List_insertBackObjectReferenceValue(thread, operands, (Arcadia_ObjectReferenceValue)operand);
  }
}

static Arcadia_PDLC_AST_IdentifierNode*
onIdentifier
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  if (!is(thread, self, Arcadia_PDLC_WordType_Name)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  Arcadia_List* names = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, names);
  Arcadia_List_insertBackObjectReferenceValue(thread, names, getText(thread, self));
  next(thread, self);
  while (is(thread, self, Arcadia_PDLC_WordType_Period)) {
    next(thread, self);
    if (!is(thread, self, Arcadia_PDLC_WordType_Name)) {
      onUnexpectedWord(thread, self);
    }
    Arcadia_List_insertBackObjectReferenceValue(thread, names, getText(thread, self));
    next(thread, self);
  }
  Arcadia_ImmutableList* snapshot = Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(names));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, snapshot);
  Arcadia_PDLC_AST_IdentifierNode* node = Arcadia_PDLC_AST_IdentifierNode_create(thread, startOffset, snapshot);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return node;
}

// invokeInstruction : 'invoke' operand name operand operand*
static Arcadia_PDLC_AST_InvokeInstructionNode*
onInvokeInstruction
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue invokeInstructionStartOffset = getStartOffset(thread, self);
  Arcadia_BooleanValue procedureOnly = is(thread, self, Arcadia_PDLC_WordType_Call);
  next(thread, self);
  Arcadia_PDLC_AST_OperandNode* target = onExpression(thread, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, target);
  if (!is(thread, self, Arcadia_PDLC_WordType_Name)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue calleeStartOffset = getStartOffset(thread, self);
  Arcadia_PDLC_AST_IdentifierNode* calleeIdentifier = onIdentifier(thread, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, calleeIdentifier);
  Arcadia_String* calleeName = Arcadia_PDLC_Names_toTypeName(thread, calleeIdentifier->names);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, calleeName);
  Arcadia_List* operands = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, operands);
  // The grammar requires a first argument: the parenthesized argument list it replaced had a spelling for the
  // empty list, the unparenthesized one does not.
  if (!procedureOnly) {
    Arcadia_PDLC_AST_OperandNode* firstOperand = onExpression(thread, self);
    Arcadia_List_insertBackObjectReferenceValue(thread, operands, (Arcadia_ObjectReferenceValue)firstOperand);
  }
  onOperands(thread, self, operands);
  Arcadia_PDLC_AST_VariableOperandNode* calleeNode = Arcadia_PDLC_AST_VariableOperandNode_create(thread, calleeStartOffset, calleeName);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, calleeNode);
  Arcadia_PDLC_AST_InvokeInstructionNode* result = Arcadia_PDLC_AST_InvokeInstructionNode_create(thread, invokeInstructionStartOffset, target, calleeNode, operands);
  result->procedureOnly = procedureOnly;
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

// constructInstruction : 'construct' operand identifier operand operand*
static Arcadia_PDLC_AST_ConstructInstructionNode*
onConstructInstruction
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_SizeValue constructInstructionStartOffset = getStartOffset(thread, self);
  next(thread, self);
  Arcadia_PDLC_AST_OperandNode* target = onExpression(thread, self);
  if (!is(thread, self, Arcadia_PDLC_WordType_Name)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue calleeStartOffset = getStartOffset(thread, self);
  Arcadia_PDLC_AST_IdentifierNode* className = onIdentifier(thread, self);
  Arcadia_String* calleeName = Arcadia_PDLC_Names_toTypeName(thread, className->names);
  Arcadia_List* operands = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  // The grammar requires a first argument: the parenthesized argument list it replaced had a spelling for the
  // empty list, the unparenthesized one does not.
  Arcadia_PDLC_AST_OperandNode* firstOperand = onExpression(thread, self);
  Arcadia_List_insertBackObjectReferenceValue(thread, operands, (Arcadia_ObjectReferenceValue)firstOperand);
  onOperands(thread, self, operands);
  Arcadia_PDLC_AST_VariableOperandNode* calleeNode = Arcadia_PDLC_AST_VariableOperandNode_create(thread, calleeStartOffset, calleeName);
  return Arcadia_PDLC_AST_ConstructInstructionNode_create(thread, constructInstructionStartOffset, target, calleeNode, operands);
}

// instruction : ( binaryInstruction
//               | unaryInstruction
//               | invokeInstruction
//               | constructInstruction
//               | jumpInstruction
//               | argumentInstruction
//               | fieldInstruction
//               | 'raise'
//               | returnInstruction ) instructionTerminator
// instructionTerminator : ( Lexical.EndOfInput | Lexical.EndOfLine )
//
// operand : booleanLiteral | integerLiteral | realLiteral | stringLiteral | voidLiteral | name | register
//
// binaryInstruction : binaryOperator operand operand operand
// binaryOperator : 'add' | 'subtract' | 'multiply' | 'divide' | 'and' | 'or' | 'concatenate'
//                | 'isEqualTo' | 'isNotEqualTo' | 'isLowerThan' | 'isLowerThanOrEqualTo'
//                | 'isGreaterThan' | 'isGreaterThanOrEqualTo'
//
// unaryInstruction : unaryOperator operand operand
// unaryOperator : 'negate' | 'not' | 'set'
//
// invokeInstruction : 'invoke' operand name operand operand*
// constructInstruction : 'construct' operand identifier operand operand*
// callInstruction : 'call' operand name operand*
//
// jumpInstruction : | 'jump' name
//                 | 'jumpIfTrue' operand name
//                 | 'jumpIfFalse' operand name
//
// argumentInstruction : | 'loadArgument' operand operand
//                     | 'storeArgument' operand operand operand
//
// fieldInstruction : | 'loadField' operand operand operand
//                  | 'storeField' operand operand operand
//
// returnInstruction : 'return' operand?
//
//
// next() skips a line continuation, so an instruction continued in the following line is terminated by the
// line terminator of its last line.

static Arcadia_PDLC_AST_InstructionNode*
onInstruction
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  switch (getType(thread, self)) {
    // binaryExpression
    case Arcadia_PDLC_WordType_Add: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_Add);
    } break;
    case Arcadia_PDLC_WordType_And: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_And);
    } break;
    case Arcadia_PDLC_WordType_Concatenate: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_Concatenate);
    } break;
    case Arcadia_PDLC_WordType_Divide: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_Divide);
    } break;
    case Arcadia_PDLC_WordType_Multiply: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_Multiply);
    } break;
    case Arcadia_PDLC_WordType_Or: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_Or);
    } break;
    case Arcadia_PDLC_WordType_Subtract: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_Subtract);
    } break;
    // relational operations
    case Arcadia_PDLC_WordType_IsEqualTo: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_IsEqualTo);
    } break;
    case Arcadia_PDLC_WordType_IsNotEqualTo: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_IsNotEqualTo);
    } break;
    case Arcadia_PDLC_WordType_IsLowerThan: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_IsLowerThan);
    } break;
    case Arcadia_PDLC_WordType_IsLowerThanOrEqualTo: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_IsLowerThanOrEqualTo);
    } break;
    case Arcadia_PDLC_WordType_IsGreaterThan: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_IsGreaterThan);
    } break;
    case Arcadia_PDLC_WordType_IsGreaterThanOrEqualTo: {
      return onBinaryInstruction(thread, self, Arcadia_PDLC_AST_BinaryInstructionKind_IsGreaterThanOrEqualTo);
    } break;
    // unaryExpression
    case Arcadia_PDLC_WordType_Negate: {
      return onUnaryInstruction(thread, self, Arcadia_PDLC_AST_UnaryInstructionKind_Negate);
    } break;
    case Arcadia_PDLC_WordType_Not: {
      return onUnaryInstruction(thread, self, Arcadia_PDLC_AST_UnaryInstructionKind_Not);
    } break;
    case Arcadia_PDLC_WordType_Call:
    case Arcadia_PDLC_WordType_Invoke: {
      return (Arcadia_PDLC_AST_InstructionNode*)onInvokeInstruction(thread, self);
    } break;
    case Arcadia_PDLC_WordType_Construct: {
      return (Arcadia_PDLC_AST_InstructionNode*)onConstructInstruction(thread, self);
    } break;
    case Arcadia_PDLC_WordType_Set: {
      return onUnaryInstruction(thread, self, Arcadia_PDLC_AST_UnaryInstructionKind_Set);
    } break;
    case Arcadia_PDLC_WordType_Raise: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      next(thread, self);
      return (Arcadia_PDLC_AST_InstructionNode*)Arcadia_PDLC_AST_RaiseInstructionNode_create(thread, startOffset);
    } break;
    case Arcadia_PDLC_WordType_Return: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      next(thread, self);
      Arcadia_PDLC_AST_OperandNode* operand = NULL;
      if (canStartOperand(thread, self)) {
        operand = onExpression(thread, self);
      }
      return (Arcadia_PDLC_AST_InstructionNode*)Arcadia_PDLC_AST_ReturnInstructionNode_create(thread, startOffset, operand);
    } break;
    case Arcadia_PDLC_WordType_Jump: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      next(thread, self);
      if (!is(thread, self, Arcadia_PDLC_WordType_Name)) {
        onUnexpectedWord(thread, self);
      }
      Arcadia_String* name = getText(thread, self);
      next(thread, self);
      Arcadia_PDLC_AST_JumpInstructionNode* jumpNode = Arcadia_PDLC_AST_JumpInstructionNode_create(thread, startOffset, Arcadia_PDLC_AST_JumpInstructionKind_Always, NULL, name);
      name = NULL;
      return (Arcadia_PDLC_AST_InstructionNode*)jumpNode;
    } break;
    case Arcadia_PDLC_WordType_JumpIfFalse: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      next(thread, self);
      Arcadia_PDLC_AST_OperandNode* operand = onExpression(thread, self);
      if (!is(thread, self, Arcadia_PDLC_WordType_Name)) {
        onUnexpectedWord(thread, self);
      }
      Arcadia_String* name = getText(thread, self);
      next(thread, self);
      Arcadia_PDLC_AST_JumpInstructionNode* jumpNode = Arcadia_PDLC_AST_JumpInstructionNode_create(thread, startOffset, Arcadia_PDLC_AST_JumpInstructionKind_IfFalse, operand, name);
      name = NULL;
      operand = NULL;
      return (Arcadia_PDLC_AST_InstructionNode*)jumpNode;
    } break;
    case Arcadia_PDLC_WordType_JumpIfTrue: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      next(thread, self);
      Arcadia_PDLC_AST_OperandNode* operand = onExpression(thread, self);
      if (!is(thread, self, Arcadia_PDLC_WordType_Name)) {
        onUnexpectedWord(thread, self);
      }
      Arcadia_String* name = getText(thread, self);
      next(thread, self);
      Arcadia_PDLC_AST_JumpInstructionNode* jumpNode = Arcadia_PDLC_AST_JumpInstructionNode_create(thread, startOffset, Arcadia_PDLC_AST_JumpInstructionKind_IfTrue, operand, name);
      name = NULL;
      operand = NULL;
      return (Arcadia_PDLC_AST_InstructionNode*)jumpNode;
    } break;
    case Arcadia_PDLC_WordType_LoadArgument: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      next(thread, self);
      Arcadia_PDLC_AST_OperandNode* target = onExpression(thread, self);
      Arcadia_PDLC_AST_OperandNode* name = onExpression(thread, self);
      Arcadia_PDLC_AST_LoadArgumentInstructionNode* node = Arcadia_PDLC_AST_LoadArgumentInstructionNode_create(thread, startOffset, target, name);
      name = NULL;
      target = NULL;
      return (Arcadia_PDLC_AST_InstructionNode*)node;
    } break;
    case Arcadia_PDLC_WordType_StoreArgument: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      next(thread, self);
      Arcadia_PDLC_AST_OperandNode* target = onExpression(thread, self);
      Arcadia_PDLC_AST_OperandNode* name = onExpression(thread, self);
      Arcadia_PDLC_AST_OperandNode* source = onExpression(thread, self);
      Arcadia_PDLC_AST_StoreArgumentInstructionNode* node = Arcadia_PDLC_AST_StoreArgumentInstructionNode_create(thread, startOffset, target, name, source);
      source = NULL;
      name = NULL;
      target = NULL;
      return (Arcadia_PDLC_AST_InstructionNode*)node;
    } break;
    case Arcadia_PDLC_WordType_LoadField: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      next(thread, self);
      Arcadia_PDLC_AST_OperandNode* target = onExpression(thread, self);
      Arcadia_PDLC_AST_OperandNode* source = onExpression(thread, self);
      Arcadia_PDLC_AST_OperandNode* name = onExpression(thread, self);
      Arcadia_PDLC_AST_LoadFieldInstructionNode* node = Arcadia_PDLC_AST_LoadFieldInstructionNode_create(thread, startOffset, target, source, name);
      name = NULL;
      source = NULL;
      target = NULL;
      return (Arcadia_PDLC_AST_InstructionNode*)node;
    } break;
    case Arcadia_PDLC_WordType_StoreField: {
      Arcadia_SizeValue startOffset = getStartOffset(thread, self);
      next(thread, self);
      Arcadia_PDLC_AST_OperandNode* target = onExpression(thread, self);
      Arcadia_PDLC_AST_OperandNode* name = onExpression(thread, self);
      Arcadia_PDLC_AST_OperandNode* source = onExpression(thread, self);
      Arcadia_PDLC_AST_StoreFieldInstructionNode* node = Arcadia_PDLC_AST_StoreFieldInstructionNode_create(thread, startOffset, target, name, source);
      source = NULL;
      name = NULL;
      target = NULL;
      return (Arcadia_PDLC_AST_InstructionNode*)node;
    } break;
    default: {
      onUnexpectedWord(thread, self);
    } break;
  }
}

static Arcadia_PDLC_AST_InstructionNode*
onBinaryInstruction
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_PDLC_AST_BinaryInstructionKind kind
  )
{
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  next(thread, self);
  Arcadia_PDLC_AST_OperandNode* target = onExpression(thread, self);
  Arcadia_PDLC_AST_OperandNode* firstOperand = onExpression(thread, self);
  Arcadia_PDLC_AST_OperandNode* secondOperand = onExpression(thread, self);
  return (Arcadia_PDLC_AST_InstructionNode*)Arcadia_PDLC_AST_BinaryInstructionNode_create(thread, startOffset, kind, target, firstOperand, secondOperand);
}

static Arcadia_PDLC_AST_InstructionNode*
onUnaryInstruction
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_PDLC_AST_UnaryInstructionKind kind
  )
{
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  next(thread, self);
  Arcadia_PDLC_AST_OperandNode* target = onExpression(thread, self);
  Arcadia_PDLC_AST_OperandNode* operand = onExpression(thread, self);
  return (Arcadia_PDLC_AST_InstructionNode*)Arcadia_PDLC_AST_UnaryInstructionNode_create(thread, startOffset, kind, target, operand);
}

static void
onEndOfStatement
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  if (is(thread, self, Arcadia_PDLC_WordType_EndOfInput)) {
    return;
  }
  if (!is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket) && !is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    onUnexpectedWord(thread, self);
  }
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
}

// instructionStatement : instruction endOfStatement
static Arcadia_PDLC_AST_InstructionStatementNode*
onInstructionStatement
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  Arcadia_PDLC_AST_InstructionNode* instruction = onInstruction(thread, self);
  ((Arcadia_PDLC_AST_StatementNode*)instruction)->startOffset = startOffset;
  Arcadia_PDLC_AST_InstructionStatementNode* statement = (Arcadia_PDLC_AST_InstructionStatementNode*)instruction;
  onEndOfStatement(thread, self);
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  return statement;
}

/// variableDefinitionStatement : 'variable' name endOfStatement
static Arcadia_PDLC_AST_VariableDefinitionStatementNode*
onVariableDefinitionStatement
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  if (Arcadia_PDLC_WordType_Variable != getType(thread, self)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  next(thread, self);
  if (Arcadia_PDLC_WordType_Name != getType(thread, self)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_PDLC_AST_VariableDefinitionStatementNode* statement = Arcadia_PDLC_AST_VariableDefinitionStatementNode_create(thread, startOffset, getText(thread, self));
  ((Arcadia_PDLC_AST_StatementNode*)statement)->startOffset = startOffset;
  next(thread, self);
  onEndOfStatement(thread, self);
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  return statement;
}

/// statement : instructionStatement
///           | labelDefinitionStatement endOfStatement?
///           | variableDefinitionStatement
///           | emptyStatement endOfStatement
/// labelDefinitionStatement : name ':'
static Arcadia_PDLC_AST_StatementNode*
onStatement
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  // skip empty statement
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  // variableDefinitionStatement : 'variable' name
  if (is(thread, self, Arcadia_PDLC_WordType_Variable)) {
    Arcadia_PDLC_AST_VariableDefinitionStatementNode* statementNode = onVariableDefinitionStatement(thread, self);
    return (Arcadia_PDLC_AST_StatementNode*)statementNode;
  }
  // labelDefinitionStatement : name ':'
  if (is(thread, self, Arcadia_PDLC_WordType_Name)) {
    Arcadia_PDLC_AST_StatementNode* statementNode = NULL;
    Arcadia_String* name = getText(thread, self);
    Arcadia_SizeValue startOffset = getStartOffset(thread, self);
    next(thread, self);
    if (!is(thread, self, Arcadia_PDLC_WordType_Colon)) {
      onUnexpectedWord(thread, self);
    }
    next(thread, self);
    statementNode = (Arcadia_PDLC_AST_StatementNode*)Arcadia_PDLC_AST_LabelDefinitionStatementNode_create(thread, startOffset, name);
    statementNode->startOffset = startOffset;
    onEndOfStatement(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    return statementNode;
  }
  // instruction statement
  {
    Arcadia_PDLC_AST_InstructionStatementNode* statementNode = onInstructionStatement(thread, self);
    return (Arcadia_PDLC_AST_StatementNode*)statementNode;
  }
}

/// Report the current word as unexpected and raise `Arcadia_Status_SyntacticalError`.
/// @remark Every syntactical error must be reported this way: raising the status without adding a
/// diagnostic makes the compilation fail without telling the user what is wrong.
Arcadia_NoReturn() static void
onUnexpectedWord
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_Languages_Diagnostics_add
    (
      thread,
      self->diagnostics,
      (Arcadia_Languages_Diagnostic*)
      Arcadia_PDLC_Diagnostics_Syntactical_UnexpectedWordDiagnostic_create
        (
          thread,
          Arcadia_Languages_DiagnosticType_Error,
          Arcadia_PDLC_Scanner_getFile(thread, (Arcadia_PDLC_Scanner*)self->scanner),
          getStartOffset(thread, self),
          getType(thread, self)
        )
    );
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
  Arcadia_Thread_jump(thread);
}

static Arcadia_PDLC_AST_FieldDefinitionNode* onFieldDefinition(Arcadia_Thread*, Arcadia_PDLC_Parser*);

static Arcadia_BooleanValue
isName
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    const char* name
  )
{
  return is(thread, self, Arcadia_PDLC_WordType_Name) && Arcadia_String_isEqualTo_pn(thread, getText(thread, self), name, strlen(name));
}

static Arcadia_PDLC_AST_FieldDefinitionNode*
onAnnotatedType
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_SizeValue offset,
    Arcadia_String* name
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_PDLC_AST_FieldDefinitionNode* result = Arcadia_PDLC_AST_FieldDefinitionNode_create(thread, offset, name, NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, result);
  if (isName(thread, self, "borrowed")) { result->ownership = Arcadia_PDLC_Ownership_Borrowed; next(thread, self); }
  else if (isName(thread, self, "owned")) { result->ownership = Arcadia_PDLC_Ownership_Owned; next(thread, self); }
  else if (isName(thread, self, "retained")) { result->ownership = Arcadia_PDLC_Ownership_Retained; next(thread, self); }
  if (isName(thread, self, "output")) { result->isOutput = Arcadia_BooleanValue_True; next(thread, self); }
  if (isName(thread, self, "readonly")) { result->isReadOnly = Arcadia_BooleanValue_True; next(thread, self); }
  if (isName(thread, self, "reference")) { result->isReference = Arcadia_BooleanValue_True; next(thread, self); }
  if (isName(thread, self, "record")) {
    next(thread, self);
    if (!is(thread, self, Arcadia_PDLC_WordType_LeftCurlyBracket)) onUnexpectedWord(thread, self);
    result->recordFields = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_Variable)) {
      Arcadia_PDLC_AST_FieldDefinitionNode* child = onFieldDefinition(thread, self);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, child);
      Arcadia_List_insertBackObjectReferenceValue(thread, result->recordFields, child);
      Arcadia_ValueStack_popValues(thread, 1);
    }
    if (!is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) onUnexpectedWord(thread, self);
    next(thread, self);
  } else {
    result->type = onIdentifier(thread, self);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

// parameter : <name> ':' <type>
// <name> : lexical.name
// <type> : lexical.name
static Arcadia_PDLC_AST_FieldDefinitionNode*
onParameter
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  if (!is(thread, self, Arcadia_PDLC_WordType_Variable)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  next(thread, self);
  if (!is(thread, self, Arcadia_PDLC_WordType_Name)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_String* name = getText(thread, self);
  next(thread, self);
  if (!is(thread, self, Arcadia_PDLC_WordType_Colon)) {
    onUnexpectedWord(thread, self);
  }
  next(thread, self);
  return onAnnotatedType(thread, self, startOffset, name);
}

// parameters : '(' (parameter (',' parameter)*)? ')'
static Arcadia_List*
onParameters
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_List* parameters = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, parameters);
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  if (is(thread, self, Arcadia_PDLC_WordType_LeftParenthesis)) {
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    Arcadia_PDLC_AST_FieldDefinitionNode* parameter = NULL;
    if (is(thread, self, Arcadia_PDLC_WordType_Variable)) {
      parameter = onParameter(thread, self);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, parameter);
      Arcadia_List_insertBackObjectReferenceValue(thread, parameters, (Arcadia_ObjectReferenceValue)parameter);
      Arcadia_ValueStack_popValues(thread, 1);
      while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
        next(thread, self);
      }
      while (is(thread, self, Arcadia_PDLC_WordType_Comma)) {
        next(thread, self);
        while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
          next(thread, self);
        }
        parameter = onParameter(thread, self);
        Arcadia_ValueStack_pushObjectReferenceValue(thread, parameter);
        Arcadia_List_insertBackObjectReferenceValue(thread, parameters, (Arcadia_ObjectReferenceValue)parameter);
        Arcadia_ValueStack_popValues(thread, 1);
        while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
          next(thread, self);
        }
      }
    }
    if (!is(thread, self, Arcadia_PDLC_WordType_RightParenthesis)) {
      onUnexpectedWord(thread, self);
    }
    next(thread, self);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return parameters;
}

// constructorDefinition : 'constructor' ('native' string)? constructorParameters? constructorBody?
// constructorParameters : parameters
// constructorBody : '{' statements '}
static Arcadia_PDLC_AST_ConstructorDefinitionNode*
onConstructorDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  if (!is(thread, self, Arcadia_PDLC_WordType_Constructor)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  next(thread, self);
  Arcadia_String* nativeName = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_Native)) {
    next(thread, self);
    if (!is(thread, self, Arcadia_PDLC_WordType_StringLiteral)) {
      onUnexpectedWord(thread, self);
    }
    nativeName = getText(thread, self);
    next(thread, self);
  }
  Arcadia_List* constructorParameters = onParameters(thread, self);
  Arcadia_List* constructorBody = NULL;
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  if (is(thread, self, Arcadia_PDLC_WordType_LeftCurlyBracket)) {
    constructorBody = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    while (!is(thread, self, Arcadia_PDLC_WordType_EndOfInput) && !is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      Arcadia_PDLC_AST_StatementNode* statementNode = onStatement(thread, self);
      Arcadia_List_insertBackObjectReferenceValue(thread, constructorBody, (Arcadia_ObjectReferenceValue)statementNode);
    }
    if (!is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      onUnexpectedWord(thread, self);
    }
    next(thread, self);
  }
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  if (is(thread, self, Arcadia_PDLC_WordType_LeftParenthesis)) {
    if (!is(thread, self, Arcadia_PDLC_WordType_RightParenthesis)) {
      onUnexpectedWord(thread, self);
    }
  }
  Arcadia_PDLC_AST_ConstructorDefinitionNode* constructorDefinitionNode = Arcadia_PDLC_AST_ConstructorDefinitionNode_create(thread, startOffset, nativeName, constructorParameters, constructorBody);
  return constructorDefinitionNode;
}

// methodDefinition : 'method' ('native' string)? methodName methodParameters? ':' returnType methodBody?
// methodParameters : parameters
// methodBody : '{' statements '}'
// returnType : identifier
static Arcadia_PDLC_AST_MethodDefinitionNode*
onMethodDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  if (!is(thread, self, Arcadia_PDLC_WordType_Method)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  next(thread, self);
  Arcadia_BooleanValue abstract = is(thread, self, Arcadia_PDLC_WordType_Abstract);
  if (abstract) next(thread, self);
  Arcadia_BooleanValue nonvirtual = isName(thread, self, "nonvirtual");
  if (nonvirtual) next(thread, self);
  Arcadia_String* nativeName = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_Native)) {
    next(thread, self);
    if (!is(thread, self, Arcadia_PDLC_WordType_StringLiteral)) {
      onUnexpectedWord(thread, self);
    }
    nativeName = getText(thread, self);
    next(thread, self);
  }
  if (!is(thread, self, Arcadia_PDLC_WordType_Name)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_String* methodName = getText(thread, self);
  next(thread, self);
  Arcadia_List* methodParameters = onParameters(thread, self);
  if (!is(thread, self, Arcadia_PDLC_WordType_Colon)) {
    onUnexpectedWord(thread, self);
  }
  next(thread, self);
  Arcadia_PDLC_AST_IdentifierNode* returnType = onIdentifier(thread, self);
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_List* methodBody = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_LeftCurlyBracket)) {
    methodBody = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    while (!is(thread, self, Arcadia_PDLC_WordType_EndOfInput) && !is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      Arcadia_PDLC_AST_StatementNode* statementNode = onStatement(thread, self);
      Arcadia_List_insertBackObjectReferenceValue(thread, methodBody, (Arcadia_ObjectReferenceValue)statementNode);
    }
    if (!is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      onUnexpectedWord(thread, self);
    }
    next(thread, self);
  }
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_PDLC_AST_MethodDefinitionNode* methodDefinitionNode = Arcadia_PDLC_AST_MethodDefinitionNode_create(thread, startOffset, nativeName, methodName, methodParameters, returnType, methodBody);
  methodDefinitionNode->isNonvirtual = nonvirtual;
  methodDefinitionNode->isAbstract = abstract;
  return methodDefinitionNode;
}

// variableDefinition : 'variable' name ':' type
static Arcadia_PDLC_AST_FieldDefinitionNode*
onFieldDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_BooleanValue constant = isName(thread, self, "constant");
  if (Arcadia_PDLC_WordType_Variable != getType(thread, self) && !constant) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  next(thread, self);
  if (Arcadia_PDLC_WordType_Name != getType(thread, self)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_String* name = getText(thread, self);
  next(thread, self);
  if (Arcadia_PDLC_WordType_Colon != getType(thread, self)) {
    onUnexpectedWord(thread, self);
  }
  next(thread, self);
  Arcadia_PDLC_AST_FieldDefinitionNode* variableDefinitionNode = onAnnotatedType(thread, self, startOffset, name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, variableDefinitionNode);
  if (constant) {
    if (!is(thread, self, Arcadia_PDLC_WordType_EqualsSign)) onUnexpectedWord(thread, self);
    next(thread, self);
    if (!is(thread, self, Arcadia_PDLC_WordType_IntegerLiteral)) onUnexpectedWord(thread, self);
    variableDefinitionNode->initializer = Arcadia_PDLC_AST_IntegerLiteralNode_create(thread, getStartOffset(thread, self), getText(thread, self));
    next(thread, self);
  }
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_ValueStack_popValues(thread, 1);
  return variableDefinitionNode;
}

// classBodyDefinition : classMemberDefinition
// classMemberDefinition : constructorDefinition | methodDefinition | variableDefinition
static Arcadia_PDLC_AST_Node*
onClassMemberDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  if (isName(thread, self, "constant")) return (Arcadia_PDLC_AST_Node*)onFieldDefinition(thread, self);
  switch (getType(thread, self)) {
    case Arcadia_PDLC_WordType_Constructor: {
      return (Arcadia_PDLC_AST_Node*)onConstructorDefinition(thread, self);
    } break;
    case Arcadia_PDLC_WordType_Method: {
      return (Arcadia_PDLC_AST_Node*)onMethodDefinition(thread, self);
    } break;
    case Arcadia_PDLC_WordType_Variable: {
      return (Arcadia_PDLC_AST_Node*)onFieldDefinition(thread, self);
    } break;
    default: {
      onUnexpectedWord(thread, self);
    } break;
  };
}

static Arcadia_BooleanValue
isValidNativeHeaderPath
  (
    Arcadia_Thread* thread,
    Arcadia_String* path
  )
{
  const char* bytes = Arcadia_String_getBytes(thread, path);
  Arcadia_SizeValue length = Arcadia_String_getNumberOfBytes(thread, path);
  Arcadia_SizeValue start = 0;
  for (Arcadia_SizeValue i = 0; i <= length; ++i) {
    if (i == length || bytes[i] == '/') {
      Arcadia_SizeValue count = i - start;
      if (!count || (count == 1 && bytes[start] == '.') ||
          (count == 2 && bytes[start] == '.' && bytes[start + 1] == '.')) return Arcadia_BooleanValue_False;
      start = i + 1;
    } else {
      unsigned char c = (unsigned char)bytes[i];
      if (c < 32 || c == 127 || c == '\\' || c == '"' || c == '<' || c == '>' || c == ':') return Arcadia_BooleanValue_False;
    }
  }
  return Arcadia_BooleanValue_True;
}

// classDefinition : 'abstract'? ('native' 'implementation'? stringLiteral? 'class' | 'class' stringLiteral?) name ('extends' name)? ('implements' name (',' name)*)? classBody
// classBody : '{' classBodyDefinition* '}'
static Arcadia_PDLC_AST_ClassDefinitionNode*
onClassDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_BooleanValue isNative = Arcadia_BooleanValue_False;
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  Arcadia_BooleanValue nativeImplementation = Arcadia_BooleanValue_False;
  Arcadia_BooleanValue isAbstract = Arcadia_BooleanValue_False;
  Arcadia_String* nativeHeaderPath = NULL;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  if (is(thread, self, Arcadia_PDLC_WordType_Abstract)) {
    isAbstract = Arcadia_BooleanValue_True;
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) next(thread, self);
  }
  if (is(thread, self, Arcadia_PDLC_WordType_Native)) {
    isNative = Arcadia_BooleanValue_True;
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    if (isName(thread, self, "implementation")) {
      nativeImplementation = Arcadia_BooleanValue_True;
      next(thread, self);
      while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) next(thread, self);
    }
    if (is(thread, self, Arcadia_PDLC_WordType_StringLiteral)) {
      nativeHeaderPath = getText(thread, self);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, nativeHeaderPath);
      if (!isValidNativeHeaderPath(thread, nativeHeaderPath)) {
        Arcadia_Languages_Diagnostics_add(thread, self->diagnostics, (Arcadia_Languages_Diagnostic*)
          Arcadia_PDLC_Diagnostics_InvalidPathDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, nativeHeaderPath));
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
        Arcadia_Thread_jump(thread);
      }
      next(thread, self);
      while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) next(thread, self);
    }
  }
  if (!is(thread, self, Arcadia_PDLC_WordType_Class)) {
    onUnexpectedWord(thread, self);
  }
  next(thread, self);
  if (!isNative && is(thread, self, Arcadia_PDLC_WordType_StringLiteral)) {
    nativeHeaderPath = getText(thread, self);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, nativeHeaderPath);
    Arcadia_SizeValue length = Arcadia_String_getNumberOfBytes(thread, nativeHeaderPath);
    const char* bytes = Arcadia_String_getBytes(thread, nativeHeaderPath);
    if (!isValidNativeHeaderPath(thread, nativeHeaderPath) || length < 2 || bytes[length - 2] != '.' || bytes[length - 1] != 'h') {
      Arcadia_Languages_Diagnostics_add(thread, self->diagnostics, (Arcadia_Languages_Diagnostic*)
        Arcadia_PDLC_Diagnostics_InvalidPathDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, nativeHeaderPath));
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
      Arcadia_Thread_jump(thread);
    }
    next(thread, self);
  }
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_PDLC_AST_IdentifierNode* className = onIdentifier(thread, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, className);
  Arcadia_PDLC_AST_IdentifierNode* extendedClassName = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_Extends)) {
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    extendedClassName = onIdentifier(thread, self);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, extendedClassName);
  }
  Arcadia_List* implementsInterfaceNames = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_Implements)) {
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    implementsInterfaceNames = onInterfaceNames(thread, self);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, implementsInterfaceNames);
  }
  Arcadia_List* classBody = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_LeftCurlyBracket)) {
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    classBody = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, classBody);
    while (!is(thread, self, Arcadia_PDLC_WordType_EndOfInput) && !is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      Arcadia_PDLC_AST_Node* classMemberDefinitionNode = onClassMemberDefinition(thread, self);
      Arcadia_List_insertBackObjectReferenceValue(thread, classBody, (Arcadia_ObjectReferenceValue)classMemberDefinitionNode);
    }
    if (!is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      onUnexpectedWord(thread, self);
    }
    next(thread, self);
  }
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_PDLC_AST_ClassDefinitionNode* classDefinitionNode = Arcadia_PDLC_AST_ClassDefinitionNode_create(thread, startOffset, isNative, isAbstract, className, extendedClassName, implementsInterfaceNames, classBody);
  classDefinitionNode->nativeHeaderPath = nativeHeaderPath;
  classDefinitionNode->isNativeImplementation = nativeImplementation;
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return classDefinitionNode;
}

// interfaceNames : identifier (',' identifier)*
static Arcadia_List*
onInterfaceNames
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_List* names = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, names, (Arcadia_ObjectReferenceValue)onIdentifier(thread, self));
  while (is(thread, self, Arcadia_PDLC_WordType_Comma)) {
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    Arcadia_List_insertBackObjectReferenceValue(thread, names, (Arcadia_ObjectReferenceValue)onIdentifier(thread, self));
  }
  return names;
}

// interfaceBodyDefinition : operationDefinition
// operationDefinition : methodDefinition
static Arcadia_PDLC_AST_Node*
onInterfaceMemberDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  switch (getType(thread, self)) {
    case Arcadia_PDLC_WordType_Method: {
      return (Arcadia_PDLC_AST_Node*)onMethodDefinition(thread, self);
    } break;
    default: {
      onUnexpectedWord(thread, self);
    } break;
  };
}

// interfaceDefinition : 'interface' name ('extends' interfaceNames)? interfaceBody
// interfaceBody : '{' interfaceBodyDefinition* '}'
static Arcadia_PDLC_AST_InterfaceDefinitionNode*
onInterfaceDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  if (!is(thread, self, Arcadia_PDLC_WordType_Interface)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  next(thread, self);
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_PDLC_AST_IdentifierNode* interfaceName = onIdentifier(thread, self);
  Arcadia_List* extendedInterfaceNames = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_Extends)) {
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    extendedInterfaceNames = onInterfaceNames(thread, self);
  }
  Arcadia_List* operations = NULL;
  if (!is(thread, self, Arcadia_PDLC_WordType_LeftCurlyBracket)) {
    // The interface body is mandatory.
    onUnexpectedWord(thread, self);
  }
  {
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    operations = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    while (!is(thread, self, Arcadia_PDLC_WordType_EndOfInput) && !is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      Arcadia_PDLC_AST_Node* operationDefinitionNode = onInterfaceMemberDefinition(thread, self);
      Arcadia_List_insertBackObjectReferenceValue(thread, operations, (Arcadia_ObjectReferenceValue)operationDefinitionNode);
    }
    if (!is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      onUnexpectedWord(thread, self);
    }
    next(thread, self);
  }
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_PDLC_AST_InterfaceDefinitionNode* interfaceDefinitionNode =
    Arcadia_PDLC_AST_InterfaceDefinitionNode_create
      (
        thread,
        startOffset,
        interfaceName,
        extendedInterfaceNames,
        operations
      );
  return interfaceDefinitionNode;
}

static Arcadia_PDLC_AST_Node*
onEnumerationMemberDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  if (!is(thread, self, Arcadia_PDLC_WordType_Name)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  Arcadia_String* enumerationConstantName = getText(thread, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, enumerationConstantName);
  next(thread, self);
  Arcadia_PDLC_AST_IntegerLiteralNode* enumerationConstantInitializer = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_EqualsSign)) {
    next(thread, self);
    if (!is(thread, self, Arcadia_PDLC_WordType_IntegerLiteral)) {
      onUnexpectedWord(thread, self);
    }
    Arcadia_SizeValue startOffset = Arcadia_PDLC_Scanner_getStartOffset(thread, self->scanner);
    enumerationConstantInitializer = Arcadia_PDLC_AST_IntegerLiteralNode_create(thread, startOffset, getText(thread, self));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, enumerationConstantInitializer);
    next(thread, self);
  }
  Arcadia_PDLC_AST_EnumerationConstantDefinitionNode* enumerationConstantDefinitionNode = Arcadia_PDLC_AST_EnumerationConstantDefinitionNode_create(thread, startOffset, enumerationConstantName, enumerationConstantInitializer);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, enumerationConstantDefinitionNode);

  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return (Arcadia_PDLC_AST_Node*)enumerationConstantDefinitionNode;
}

// enumerationDefinition : 'enumeration' stringLiteral? identifier enumerationBody
// enumerationBody : '{' enumerationBodyDefinition* '}'
static Arcadia_PDLC_AST_EnumerationDefinitionNode*
onEnumerationDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  if (!is(thread, self, Arcadia_PDLC_WordType_Enumeration)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  next(thread, self);
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_String* headerPath = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_StringLiteral)) {
    headerPath = getText(thread, self);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, headerPath);
    Arcadia_SizeValue length = Arcadia_String_getNumberOfBytes(thread, headerPath);
    const char* bytes = Arcadia_String_getBytes(thread, headerPath);
    if (!isValidNativeHeaderPath(thread, headerPath) || length < 2 || bytes[length - 2] != '.' || bytes[length - 1] != 'h') {
      Arcadia_Languages_Diagnostics_add(thread, self->diagnostics, (Arcadia_Languages_Diagnostic*)
        Arcadia_PDLC_Diagnostics_InvalidPathDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, headerPath));
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
      Arcadia_Thread_jump(thread);
    }
    next(thread, self);
  }
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) next(thread, self);
  Arcadia_PDLC_AST_IdentifierNode* enumerationName = onIdentifier(thread, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, enumerationName);
  Arcadia_List* enumerationBody = NULL;
  if (!is(thread, self, Arcadia_PDLC_WordType_LeftCurlyBracket)) onUnexpectedWord(thread, self);
  if (is(thread, self, Arcadia_PDLC_WordType_LeftCurlyBracket)) {
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    enumerationBody = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, enumerationBody);
    while (!is(thread, self, Arcadia_PDLC_WordType_EndOfInput) && !is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      Arcadia_PDLC_AST_Node* enumerationMemberDefinitionNode = onEnumerationMemberDefinition(thread, self);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, enumerationMemberDefinitionNode);
      Arcadia_List_insertBackObjectReferenceValue(thread, enumerationBody, (Arcadia_ObjectReferenceValue)enumerationMemberDefinitionNode);
      Arcadia_ValueStack_popValues(thread, 1);
    }
    if (!is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      onUnexpectedWord(thread, self);
    }
    next(thread, self);
  }
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_PDLC_AST_EnumerationDefinitionNode* enumerationDefinitionNode = Arcadia_PDLC_AST_EnumerationDefinitionNode_create(thread, startOffset, enumerationName, enumerationBody);
  enumerationDefinitionNode->nativeHeaderPath = headerPath;
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return enumerationDefinitionNode;
}


// moduleDefinition : 'module' name moduleBody
// moduleBody : '{' '}'
static Arcadia_PDLC_AST_ModuleDefinitionNode*
onModuleDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  if (!is(thread, self, Arcadia_PDLC_WordType_Module)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  next(thread, self);
  if (!is(thread, self, Arcadia_PDLC_WordType_Name)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_PDLC_AST_IdentifierNode* moduleName = onIdentifier(thread, self);
  Arcadia_List* moduleBody = NULL;
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  if (is(thread, self, Arcadia_PDLC_WordType_LeftCurlyBracket)) {
    moduleBody = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    if (!is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      onUnexpectedWord(thread, self);
    }
    next(thread, self);
  }
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_PDLC_AST_ModuleDefinitionNode* moduleDefinitionNode = Arcadia_PDLC_AST_ModuleDefinitionNode_create(thread, startOffset, moduleName, moduleBody);
  return moduleDefinitionNode;
}

// procedureDefinition : 'procedure' 'entry'? ('native' string)? name procedureParameters? ':' returnType procedureBody?
// procedureParameters : parameters
// procedureBody : '{' statements '}'
// returnType : identifier
static Arcadia_PDLC_AST_ProcedureDefinitionNode*
onProcedureDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_BooleanValue entry = Arcadia_BooleanValue_False;
  if (!is(thread, self, Arcadia_PDLC_WordType_Procedure)) {
    onUnexpectedWord(thread, self);
  }
  Arcadia_SizeValue startOffset = getStartOffset(thread, self);
  next(thread, self);
  if (is(thread, self, Arcadia_PDLC_WordType_Entry)) {
    next(thread, self);
    entry = Arcadia_BooleanValue_True;
  }
  Arcadia_String* nativeName = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_Native)) {
    next(thread, self);
    if (!is(thread, self, Arcadia_PDLC_WordType_StringLiteral)) {
      onUnexpectedWord(thread, self);
    }
    nativeName = getText(thread, self);
    next(thread, self);
  }
  Arcadia_PDLC_AST_IdentifierNode* procedureName = onIdentifier(thread, self);
  Arcadia_List* procedureParameters = onParameters(thread, self);
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  if (!is(thread, self, Arcadia_PDLC_WordType_Colon)) {
    onUnexpectedWord(thread, self);
  }
  next(thread, self);
  Arcadia_PDLC_AST_IdentifierNode* returnType = onIdentifier(thread, self);
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_List* procedureBody = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_LeftCurlyBracket)) {
    procedureBody = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    while (!is(thread, self, Arcadia_PDLC_WordType_EndOfInput) && !is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      Arcadia_PDLC_AST_StatementNode* statementNode = onStatement(thread, self);
      Arcadia_List_insertBackObjectReferenceValue(thread, procedureBody, (Arcadia_ObjectReferenceValue)statementNode);
    }
    if (!is(thread, self, Arcadia_PDLC_WordType_RightCurlyBracket)) {
      onUnexpectedWord(thread, self);
    }
    next(thread, self);
  }
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
    next(thread, self);
  }
  Arcadia_PDLC_AST_ProcedureDefinitionNode* procedureDefinitionNode = Arcadia_PDLC_AST_ProcedureDefinitionNode_create(thread, startOffset, entry, nativeName, procedureName, procedureParameters, returnType, procedureBody);
  return procedureDefinitionNode;
}

static Arcadia_PDLC_AST_DefinitionNode*
onNativeDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_SizeValue offset = getStartOffset(thread, self);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  next(thread, self);
  while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) next(thread, self);
  Arcadia_BooleanValue nativeImplementation = isName(thread, self, "implementation");
  if (nativeImplementation) {
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) next(thread, self);
  }
  Arcadia_String* header = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_StringLiteral)) {
    header = getText(thread, self);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, header);
    if (!isValidNativeHeaderPath(thread, header)) {
      Arcadia_Languages_Diagnostics_add(thread, self->diagnostics, (Arcadia_Languages_Diagnostic*)
        Arcadia_PDLC_Diagnostics_InvalidPathDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, header));
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SyntacticalError);
      Arcadia_Thread_jump(thread);
    }
    next(thread, self);
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) next(thread, self);
  }
  Arcadia_PDLC_AST_DefinitionNode* result = NULL;
  if (is(thread, self, Arcadia_PDLC_WordType_Class)) {
    Arcadia_PDLC_AST_ClassDefinitionNode* c = onClassDefinition(thread, self);
    c->isNative = Arcadia_BooleanValue_True;
    c->isNativeImplementation = nativeImplementation;
    c->nativeHeaderPath = header;
    result = (Arcadia_PDLC_AST_DefinitionNode*)c;
  } else if (is(thread, self, Arcadia_PDLC_WordType_Enumeration)) {
    if (nativeImplementation) onUnexpectedWord(thread, self);
    Arcadia_PDLC_AST_EnumerationDefinitionNode* e = onEnumerationDefinition(thread, self);
    if (e->nativeHeaderPath) onUnexpectedWord(thread, self);
    e->isNative = Arcadia_BooleanValue_True;
    e->nativeHeaderPath = header;
    result = (Arcadia_PDLC_AST_DefinitionNode*)e;
  } else {
    onUnexpectedWord(thread, self);
  }
  result->startOffset = offset;
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

/// compilationUnit : (classDefinition|enumerationDefinition|interfaceDefinition|moduleDefinition|proceduredefinition)*
static Arcadia_PDLC_AST_CompilationUnitNode*
onCompilationUnit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode = Arcadia_PDLC_AST_CompilationUnitNode_create(thread);
  if (!is(thread, self, Arcadia_PDLC_WordType_StartOfInput)) {
    onUnexpectedWord(thread, self);
  }
  next(thread, self);
  while (!is(thread, self, Arcadia_PDLC_WordType_EndOfInput)) {
    while (is(thread, self, Arcadia_PDLC_WordType_LineTerminator)) {
      next(thread, self);
    }
    Arcadia_PDLC_AST_DefinitionNode* definitionNode = NULL;
    switch (getType(thread, self)) {
      case Arcadia_PDLC_WordType_Abstract:
      case Arcadia_PDLC_WordType_Class: {
        definitionNode = (Arcadia_PDLC_AST_DefinitionNode*)onClassDefinition(thread, self);
        Arcadia_PDLC_AST_CompilationUnitNode_appendDefinition(thread, compilationUnitNode, definitionNode);
        definitionNode->compilationUnitNode = compilationUnitNode;
        assert(NULL != definitionNode->compilationUnitNode);
      } break;
      case Arcadia_PDLC_WordType_Native: {
        definitionNode = onNativeDefinition(thread, self);
        Arcadia_PDLC_AST_CompilationUnitNode_appendDefinition(thread, compilationUnitNode, definitionNode);
        definitionNode->compilationUnitNode = compilationUnitNode;
        assert(NULL != definitionNode->compilationUnitNode);
      } break;
      case Arcadia_PDLC_WordType_Enumeration: {
        definitionNode = (Arcadia_PDLC_AST_DefinitionNode*)onEnumerationDefinition(thread, self);
        Arcadia_PDLC_AST_CompilationUnitNode_appendDefinition(thread, compilationUnitNode, definitionNode);
        definitionNode->compilationUnitNode = compilationUnitNode;
        assert(NULL != definitionNode->compilationUnitNode);
      } break;
      case Arcadia_PDLC_WordType_Interface: {
        definitionNode = (Arcadia_PDLC_AST_DefinitionNode*)onInterfaceDefinition(thread, self);
        Arcadia_PDLC_AST_CompilationUnitNode_appendDefinition(thread, compilationUnitNode, definitionNode);
        definitionNode->compilationUnitNode = compilationUnitNode;
        assert(NULL != definitionNode->compilationUnitNode);
      } break;
      case Arcadia_PDLC_WordType_Module: {
        definitionNode = (Arcadia_PDLC_AST_DefinitionNode*)onModuleDefinition(thread, self);
        Arcadia_PDLC_AST_CompilationUnitNode_appendDefinition(thread, compilationUnitNode, definitionNode);
        definitionNode->compilationUnitNode = compilationUnitNode;
        assert(NULL != definitionNode->compilationUnitNode);
      } break;
      case Arcadia_PDLC_WordType_Procedure: {
        definitionNode = (Arcadia_PDLC_AST_DefinitionNode*)onProcedureDefinition(thread, self);
        Arcadia_PDLC_AST_CompilationUnitNode_appendDefinition(thread, compilationUnitNode, definitionNode);
        definitionNode->compilationUnitNode = compilationUnitNode;
        assert(NULL != definitionNode->compilationUnitNode);
      } break;
      default: {
        onUnexpectedWord(thread, self);
      } break;
    };
  }
  if (!is(thread, self, Arcadia_PDLC_WordType_EndOfInput)) {
    onUnexpectedWord(thread, self);
  }
  return compilationUnitNode;
}

static Arcadia_Languages_Diagnostics*
Arcadia_PDLC_Parser_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{ return self->diagnostics; }

static Arcadia_UnicodeCodePointReader*
Arcadia_PDLC_Parser_getInputImpl
(
  Arcadia_Thread* thread,
  Arcadia_PDLC_Parser* self
)   {
 return Arcadia_Languages_Scanner_getInput(thread, (Arcadia_Languages_Scanner*)self->scanner); }

static Arcadia_Languages_StringTable*
Arcadia_PDLC_Parser_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{ return self->stringTable; }


static Arcadia_Value
Arcadia_PDLC_Parser_runImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self
  )
{
  Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode = onCompilationUnit(thread, self);
  return Arcadia_Value_makeObjectReferenceValue(compilationUnitNode);
}

static void
Arcadia_PDLC_Parser_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_UnicodeCodePointReader* input
  )
{ Arcadia_Languages_Scanner_setInput(thread, (Arcadia_Languages_Scanner*)self->scanner, input); }

void
Arcadia_PDLC_Parser_setFile
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Parser* self,
    Arcadia_Languages_InputFile* file
  )
{ Arcadia_PDLC_Scanner_setFile(thread, self->scanner, file); }
