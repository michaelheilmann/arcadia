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

#include "Arcadia/VPL/Diagnostics/SymbolAlreadyDefinedDiagnostic.h"

#include "Arcadia/VPL/Include.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnosticDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic* self
  );

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic* self
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

Arcadia_defineObjectType(u8"Arcadia.VPL.Diagnostics.SymbolAlreadyDefinedDiagnostic", Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic,
                         u8"Arcadia.Languages.Diagnostic", Arcadia_Languages_Diagnostic,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic);
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
  self->name = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnosticDispatch* self
  )
{
  ((Arcadia_Languages_DiagnosticDispatch*)self)->getMessage = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Diagnostic*)) & getMessageImpl;
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic* self
  )
{/*Intentionally empty.*/}

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic* self
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"semantical error: symbol `");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, self->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"` already defined\n");
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer));
}

Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic*
Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType type,
    Arcadia_String* name
  )
{ 
  _Arcadia_BeginCreate();
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Languages_DiagnosticType_getType(thread), type));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic);
}
