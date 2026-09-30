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
#include "Arcadia/MILC/Diagnostics/Lexical/UnexpectedSymbolDiagnostic.h"

#include "Arcadia/MILC/Include.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnosticDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic* self
  );

static Arcadia_String*
symbolToString
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic* self,
    Arcadia_Natural32Value symbol
  );

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic* self
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

Arcadia_defineObjectType(u8"Arcadia.MILC.Diagnostics.Lexical.UnexpectedSymbolDiagnostic", Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic,
                         u8"Arcadia.Languages.Diagnostic", Arcadia_Languages_Diagnostic,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic);
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
  self->received = Arcadia_ValueStack_getNatural32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnosticDispatch* self
  )
{
  ((Arcadia_Languages_DiagnosticDispatch*)self)->getMessage = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Diagnostic*)) & getMessageImpl;
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic* self
  )
{/*Intentionally empty.*/}

static Arcadia_String*
symbolToString
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic* self,
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
    Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic* self
  )
{
  Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"lexical error: received ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, symbolToString(thread, self, self->received));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"\n");
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder));
}

Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic*
Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType type,
    Arcadia_Languages_InputFile* file,
    Arcadia_SizeValue offset,
    Arcadia_Natural32Value received
  )
{ 
  _Arcadia_BeginCreate(Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic);
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Languages_DiagnosticType_getType(thread), type));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)file);
  Arcadia_ValueStack_pushSizeValue(thread, offset);
  Arcadia_ValueStack_pushNatural32Value(thread, received);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_MILC_Diagnostics_Lexical_UnexpectedSymbolDiagnostic);
}
