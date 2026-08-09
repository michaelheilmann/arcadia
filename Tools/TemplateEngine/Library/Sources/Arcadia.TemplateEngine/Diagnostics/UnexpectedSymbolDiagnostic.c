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

#define ARCADIA_TEMPLATENGINE_PRIVATE (1)
#include "Arcadia.TemplateEngine/Diagnostics/UnexpectedSymbolDiagnostic.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnosticDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic* self
  );

static Arcadia_String*
symbolToString
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic* self,
    Arcadia_Natural32Value symbol
  );

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.TemplateEngine.Diagnostics.UnexpectedSymbolDiagnostic", Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic,
                         u8"Arcadia.Languages.Diagnostic", Arcadia_Languages_Diagnostic,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic* self
  )
{
  Arcadia_EnterConstructor(Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic);
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
  self->received = Arcadia_ValueStack_getNatural32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnosticDispatch* self
  )
{
  ((Arcadia_Languages_DiagnosticDispatch*)self)->getMessage = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Diagnostic*)) & getMessageImpl;
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic* self
  )
{/*Intentionally empty.*/}

static Arcadia_String*
symbolToString
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic* self,
    Arcadia_Natural32Value symbol
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_Natural32Value digit; 
  do {
    digit = symbol % 10;
    symbol /= 10;
    digit += 48;
    Arcadia_StringBuilder_insertFrontCodePoint(thread, stringBuffer, digit);
  } while (symbol);
  Arcadia_StringBuilder_insertFrontString(thread, stringBuffer, Arcadia_String_createFromCxxString(thread, u8"U+"));
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer));
}

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic* self
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"lexical error: received ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, symbolToString(thread, self, self->received));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"\n");
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer));
}

Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic*
Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType type,
    Arcadia_Natural32Value received
  )
{ 
  _Arcadia_BeginCreate(Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic);
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Languages_DiagnosticType_getType(thread), type));
  Arcadia_ValueStack_pushNatural32Value(thread, received);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic);
}
