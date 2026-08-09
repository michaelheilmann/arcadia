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

#define ARCADIA_TEMPLATEENGINE_PRIVATE (1)
#include "Arcadia.TemplateEngine/Diagnostics/UnexpectedWordDiagnostic.h"

#include "Arcadia.TemplateEngine/Scanner/WordType.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnosticDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic* self
  );

static Arcadia_String*
wordTypeToString
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic* self,
    Arcadia_TemplateEngine_WordType wordType
  );

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic* self
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

Arcadia_defineObjectType(u8"Arcadia.TemplateEngine.Diagnostics.UnexpectedWordDiagnostic", Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic,
                         u8"Arcadia.Languages.Diagnostic", Arcadia_Languages_Diagnostic,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic* self
  )
{
  Arcadia_EnterConstructor(Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value value = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread,  &value);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  Arcadia_EnumerationValue enumerationValue = Arcadia_ValueStack_getEnumerationValue(thread, 1);
  if (!Arcadia_Type_isDescendantType(thread, enumerationValue.type, _Arcadia_TemplateEngine_WordType_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->received = (Arcadia_TemplateEngine_WordType)enumerationValue.value;
  Arcadia_LeaveConstructor(Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnosticDispatch* self
  )
{
  ((Arcadia_Languages_DiagnosticDispatch*)self)->getMessage = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Diagnostic*)) & getMessageImpl;
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic* self
  )
{/*Intentionally empty.*/}

static Arcadia_String*
wordTypeToString
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic* self,
    Arcadia_TemplateEngine_WordType wordType
  )
{
  switch (wordType) {
    case Arcadia_TemplateEngine_WordType_StartOfInput: {
      return Arcadia_String_createFromCxxString(thread, u8"<start of input>");
    } break;
    case Arcadia_TemplateEngine_WordType_EndOfInput: {
      return Arcadia_String_createFromCxxString(thread, u8"<end of input>");
    } break;

    case Arcadia_TemplateEngine_WordType_Error: {
      return Arcadia_String_createFromCxxString(thread, u8"<error>");
    } break;
    case Arcadia_TemplateEngine_WordType_AtLiteral: {
      return Arcadia_String_createFromCxxString(thread, u8"<at literal>");
    } break;

    case Arcadia_TemplateEngine_WordType_Comma: {
      return Arcadia_String_createFromCxxString(thread, u8"`,`");
    } break;

    case Arcadia_TemplateEngine_WordType_LeftCurlyBracket: {
      return Arcadia_String_createFromCxxString(thread, u8"`{`");
    } break;
    case Arcadia_TemplateEngine_WordType_RightCurlyBracket: {
      return Arcadia_String_createFromCxxString(thread, u8"`}`");
    } break;

    case Arcadia_TemplateEngine_WordType_LeftParenthesis: {
      return Arcadia_String_createFromCxxString(thread, u8"`(`");
    } break;
    case Arcadia_TemplateEngine_WordType_RightParenthesis: {
      return Arcadia_String_createFromCxxString(thread, u8"`)`");
    } break;

    case Arcadia_TemplateEngine_WordType_StartOfDirective: {
      return Arcadia_String_createFromCxxString(thread, u8"<start of directive>");
    } break;

    case Arcadia_TemplateEngine_WordType_Name: {
      return Arcadia_String_createFromCxxString(thread, u8"<name>");
    } break;
    case Arcadia_TemplateEngine_WordType_Text: {
      return Arcadia_String_createFromCxxString(thread, u8"<text>");
    } break;

    case Arcadia_TemplateEngine_WordType_StringLiteral: {
      return Arcadia_String_createFromCxxString(thread, u8"<string literal>");
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
    Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic* self
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"lexical error: received ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, wordTypeToString(thread, self, self->received));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"\n");
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer));
}

Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic*
Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType type,
    Arcadia_TemplateEngine_WordType received
  )
{ 
  _Arcadia_BeginCreate(Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic);
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Languages_DiagnosticType_getType(thread), type));
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_TemplateEngine_WordType_getType(thread), received));
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_TemplateEngine_Diagnostics_UnexpectedWordDiagnostic);
}
