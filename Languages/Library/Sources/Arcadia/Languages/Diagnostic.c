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

#include "Arcadia/Languages/Diagnostic.h"

#include "Arcadia/Languages/Include.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
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

Arcadia_defineObjectType(u8"Arcadia.Languages.Diagnostic", Arcadia_Languages_Diagnostic,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Languages_Diagnostic);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_EnumerationValue enumerationValue = Arcadia_ValueStack_getEnumerationValue(thread, 3);
  if (enumerationValue.type != _Arcadia_Languages_DiagnosticType_getType(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->type = (Arcadia_Languages_DiagnosticType)enumerationValue.value;
  if (Arcadia_ValueStack_isVoidValue(thread, 2)) {
    self->inputFile = NULL;
  } else {
    self->inputFile = (Arcadia_Languages_InputFile*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Languages_InputFile_getType(thread));
  }
  self->inputPosition = Arcadia_ValueStack_getValue(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_Languages_Diagnostic);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticDispatch* self
  )
{/*Intentionally empty.*/}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  )
{
  if (self->inputFile) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->inputFile);
  }
}

Arcadia_Languages_InputFile*
Arcadia_Languages_Diagnostic_getInputFile
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  )
{ return self->inputFile; }

Arcadia_Value
Arcadia_Languages_Diagnostic_getInputPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  )
{ return self->inputPosition; }

Arcadia_Languages_DiagnosticType
Arcadia_Languages_Diagnostic_getType
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  )
{ return self->type; }

Arcadia_String*
Arcadia_Languages_Diagnostic_getMessage
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Languages_Diagnostic, getMessage, self); }
