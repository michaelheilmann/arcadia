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

#define ARCADIA_MILC_PRIVATE (1)
#include "Arcadia/MILC/Diagnostics/Syntactical/UnexpectedWordDiagnostic.h"

#include "Arcadia/MILC/Include.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnosticDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic* self
  );

static Arcadia_String*
wordTypeToString
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic* self,
    Arcadia_MILC_WordType wordType
  );

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.Diagnostics.Syntactical.UnexpectedWordDiagnostic", Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic,
                         u8"Arcadia.Languages.Diagnostic", Arcadia_Languages_Diagnostic,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic);
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value diagnosticType = Arcadia_ValueStack_getValue(thread, 4);
    Arcadia_Value file = Arcadia_ValueStack_getValue(thread, 3);
    Arcadia_Value offset = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread, &diagnosticType);
    Arcadia_ValueStack_pushValue(thread, &file);
    Arcadia_ValueStack_pushValue(thread, &offset);
    Arcadia_ValueStack_pushNatural8Value(thread, 3);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  Arcadia_EnumerationValue enumerationValue = Arcadia_ValueStack_getEnumerationValue(thread, 1);
  if (!Arcadia_Type_isDescendantType(thread, enumerationValue.type, _Arcadia_MILC_WordType_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->received = (Arcadia_MILC_WordType)enumerationValue.value;
  Arcadia_LeaveConstructor(Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnosticDispatch* self
  )
{
  ((Arcadia_Languages_DiagnosticDispatch*)self)->getMessage = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Diagnostic*)) & getMessageImpl;
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic* self
  )
{/*Intentionally empty.*/}

static Arcadia_String*
wordTypeToString
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic* self,
    Arcadia_MILC_WordType wordType
  )
{
  switch (wordType) {
    case Arcadia_MILC_WordType_StartOfInput: {
      return Arcadia_String_createFromCxxString(thread, u8"<start of input>");
    } break;
    case Arcadia_MILC_WordType_EndOfInput: {
      return Arcadia_String_createFromCxxString(thread, u8"<end of input>");
    } break;

    case Arcadia_MILC_WordType_LineTerminator: {
      return Arcadia_String_createFromCxxString(thread, u8"<line terminator>");
    } break;
    case Arcadia_MILC_WordType_WhiteSpaces: {
      return Arcadia_String_createFromCxxString(thread, u8"<white spaces>");
    } break;

    case Arcadia_MILC_WordType_EqualsSign: {
      return Arcadia_String_createFromCxxString(thread, u8"`=`");
    } break;
    case Arcadia_MILC_WordType_Colon: {
      return Arcadia_String_createFromCxxString(thread, u8"`:`");
    } break;
    case Arcadia_MILC_WordType_Comma: {
      return Arcadia_String_createFromCxxString(thread, u8"`,`");
    } break;

    case Arcadia_MILC_WordType_LeftCurlyBracket: {
      return Arcadia_String_createFromCxxString(thread, u8"`{`");
    } break;
    case Arcadia_MILC_WordType_RightCurlyBracket: {
      return Arcadia_String_createFromCxxString(thread, u8"`}`");
    } break;

    case Arcadia_MILC_WordType_LeftParenthesis: {
      return Arcadia_String_createFromCxxString(thread, u8"`(`");
    } break;
    case Arcadia_MILC_WordType_RightParenthesis: {
      return Arcadia_String_createFromCxxString(thread, u8"`)`");
    } break;

    case Arcadia_MILC_WordType_Period: {
      return Arcadia_String_createFromCxxString(thread, u8"`.`");
    } break;

    case Arcadia_MILC_WordType_Name: {
      return Arcadia_String_createFromCxxString(thread, u8"<name>");
    } break;
    case Arcadia_MILC_WordType_Register: {
      return Arcadia_String_createFromCxxString(thread, u8"<register>");
    } break;

    case Arcadia_MILC_WordType_BooleanLiteral: {
      return Arcadia_String_createFromCxxString(thread, u8"<boolean literal>");
    } break;
    case Arcadia_MILC_WordType_RealLiteral: {
      return Arcadia_String_createFromCxxString(thread, u8"<real literal>");
    } break;
    case Arcadia_MILC_WordType_IntegerLiteral: {
      return Arcadia_String_createFromCxxString(thread, u8"<integer literal>");
    } break;
    case Arcadia_MILC_WordType_StringLiteral: {
      return Arcadia_String_createFromCxxString(thread, u8"<string literal>");
    } break;
    case Arcadia_MILC_WordType_VoidLiteral: {
      return Arcadia_String_createFromCxxString(thread, u8"<void literal>");
    } break;

    case Arcadia_MILC_WordType_Class: {
      return Arcadia_String_createFromCxxString(thread, u8"`class`");
    } break;
    case Arcadia_MILC_WordType_Constructor: {
      return Arcadia_String_createFromCxxString(thread, u8"`constructor`");
    } break;
    case Arcadia_MILC_WordType_Enumeration: {
      return Arcadia_String_createFromCxxString(thread, u8"`enumeration`");
    } break;
    case Arcadia_MILC_WordType_Extends: {
      return Arcadia_String_createFromCxxString(thread, u8"`extends`");
    } break;
    case Arcadia_MILC_WordType_Implements: {
      return Arcadia_String_createFromCxxString(thread, u8"`implements`");
    } break;
    case Arcadia_MILC_WordType_Procedure: {
      return Arcadia_String_createFromCxxString(thread, u8"`procedure`");
    } break;
    case Arcadia_MILC_WordType_Method: {
      return Arcadia_String_createFromCxxString(thread, u8"`method`");
    } break;
    case Arcadia_MILC_WordType_Module: {
      return Arcadia_String_createFromCxxString(thread, u8"`module`");
    } break;
    case Arcadia_MILC_WordType_Invoke: {
      return Arcadia_String_createFromCxxString(thread, u8"`invoke`");
    } break;
    case Arcadia_MILC_WordType_Native: {
      return Arcadia_String_createFromCxxString(thread, u8"`native`");
    } break;
    case Arcadia_MILC_WordType_Entry: {
      return Arcadia_String_createFromCxxString(thread, u8"`entry`");
    } break;
    case Arcadia_MILC_WordType_Return: {
      return Arcadia_String_createFromCxxString(thread, u8"`return`");
    } break;
    case Arcadia_MILC_WordType_Variable: {
      return Arcadia_String_createFromCxxString(thread, u8"`variable`");
    } break;
    case Arcadia_MILC_WordType_Jump: {
      return Arcadia_String_createFromCxxString(thread, u8"`jump`");
    } break;
    case Arcadia_MILC_WordType_JumpIfTrue: {
      return Arcadia_String_createFromCxxString(thread, u8"`jumpIfTrue`");
    } break;
    case Arcadia_MILC_WordType_JumpIfFalse: {
      return Arcadia_String_createFromCxxString(thread, u8"`jumpIfFalse`");
    } break;

    case Arcadia_MILC_WordType_Raise: {
      return Arcadia_String_createFromCxxString(thread, u8"`raise`");
    } break;

    case Arcadia_MILC_WordType_Add: {
      return Arcadia_String_createFromCxxString(thread, u8"`add`");
    } break;
    case Arcadia_MILC_WordType_Subtract: {
      return Arcadia_String_createFromCxxString(thread, u8"`subtract`");
    } break;
    case Arcadia_MILC_WordType_Multiply: {
      return Arcadia_String_createFromCxxString(thread, u8"`multiply`");
    } break;
    case Arcadia_MILC_WordType_Divide: {
      return Arcadia_String_createFromCxxString(thread, u8"`divide`");
    } break;

    case Arcadia_MILC_WordType_IsEqualTo: {
      return Arcadia_String_createFromCxxString(thread, u8"`isEqualTo`");
    } break;
    case Arcadia_MILC_WordType_IsNotEqualTo: {
      return Arcadia_String_createFromCxxString(thread, u8"`isNotEqualTo`");
    } break;
    case Arcadia_MILC_WordType_IsLowerThan: {
      return Arcadia_String_createFromCxxString(thread, u8"`isLowerThan`");
    } break;
    case Arcadia_MILC_WordType_IsLowerThanOrEqualTo: {
      return Arcadia_String_createFromCxxString(thread, u8"`isLowerThanOrEqualTo`");
    } break;
    case Arcadia_MILC_WordType_IsGreaterThan: {
      return Arcadia_String_createFromCxxString(thread, u8"`isGreaterThan`");
    } break;
    case Arcadia_MILC_WordType_IsGreaterThanOrEqualTo: {
      return Arcadia_String_createFromCxxString(thread, u8"`isGreaterThanOrEqualTo`");
    } break;
    case Arcadia_MILC_WordType_Negate: {
      return Arcadia_String_createFromCxxString(thread, u8"`negate`");
    } break;
    case Arcadia_MILC_WordType_Not: {
      return Arcadia_String_createFromCxxString(thread, u8"`not`");
    } break;
    case Arcadia_MILC_WordType_And: {
      return Arcadia_String_createFromCxxString(thread, u8"`and`");
    } break;
    case Arcadia_MILC_WordType_Or: {
      return Arcadia_String_createFromCxxString(thread, u8"`or`");
    } break;
    case Arcadia_MILC_WordType_Concatenate: {
      return Arcadia_String_createFromCxxString(thread, u8"`concatenate`");
    } break;
    case Arcadia_MILC_WordType_Set: {
      return Arcadia_String_createFromCxxString(thread, u8"`set`");
    } break;
    case Arcadia_MILC_WordType_SingleLineComment: {
      return Arcadia_String_createFromCxxString(thread, u8"<single line comment>");
    } break;
    case Arcadia_MILC_WordType_MultiLineComment: {
      return Arcadia_String_createFromCxxString(thread, u8"<multi line comment>");
    } break;

    case Arcadia_MILC_WordType_LoadArgument: {
      return Arcadia_String_createFromCxxString(thread, u8"`loadArgument`");
    } break;
    case Arcadia_MILC_WordType_StoreArgument: {
      return Arcadia_String_createFromCxxString(thread, u8"`storeArgument`");
    } break;
    case Arcadia_MILC_WordType_LoadField: {
      return Arcadia_String_createFromCxxString(thread, u8"`loadField`");
    } break;
    case Arcadia_MILC_WordType_StoreField: {
      return Arcadia_String_createFromCxxString(thread, u8"`storeField`");
    } break;

    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  }; 
}

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic* self
  )
{
  Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"syntactial error: received ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, wordTypeToString(thread, self, self->received));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"\n");
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder));
}

Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic*
Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType type,
    Arcadia_Languages_InputFile* file,
    Arcadia_SizeValue offset,
    Arcadia_MILC_WordType received
  )
{ 
  _Arcadia_BeginCreate(Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic);
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Languages_DiagnosticType_getType(thread), type));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)file);
  Arcadia_ValueStack_pushSizeValue(thread, offset);
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_MILC_WordType_getType(thread), received));
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_MILC_Diagnostics_Syntactical_UnexpectedWordDiagnostic);
}
