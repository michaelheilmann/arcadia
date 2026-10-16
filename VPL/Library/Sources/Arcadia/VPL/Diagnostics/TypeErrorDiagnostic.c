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

#include "Arcadia/VPL/Diagnostics/TypeErrorDiagnostic.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_TypeErrorDiagnostic* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_TypeErrorDiagnostic* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_TypeErrorDiagnosticDispatch* self
  );

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_TypeErrorDiagnostic* self
  )
{
  return self->message;
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Diagnostics.TypeErrorDiagnostic", Arcadia_VPL_Diagnostics_TypeErrorDiagnostic,
                         u8"Arcadia.Languages.Diagnostic", Arcadia_Languages_Diagnostic,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_TypeErrorDiagnostic* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Diagnostics_TypeErrorDiagnostic);
  {
    Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Languages_DiagnosticType_getType(thread), Arcadia_Languages_DiagnosticType_Error));
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
    Arcadia_ValueStack_pushNatural8Value(thread, 3);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->message = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_Diagnostics_TypeErrorDiagnostic);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_TypeErrorDiagnostic* self
  )
{
  if (self->message) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->message);
  }
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Diagnostics_TypeErrorDiagnosticDispatch* self
  )
{
  ((Arcadia_Languages_DiagnosticDispatch*)self)->getMessage = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Diagnostic*))&getMessageImpl;
}

Arcadia_VPL_Diagnostics_TypeErrorDiagnostic*
Arcadia_VPL_Diagnostics_TypeErrorDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* message
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Diagnostics_TypeErrorDiagnostic);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)message);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_VPL_Diagnostics_TypeErrorDiagnostic);
}
