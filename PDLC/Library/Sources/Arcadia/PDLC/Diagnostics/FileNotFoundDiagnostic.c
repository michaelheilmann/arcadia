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
#include "Arcadia/PDLC/Diagnostics/FileNotFoundDiagnostic.h"

#include "Arcadia/PDLC/Include.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileNotFoundDiagnosticDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic* self
  );

static Arcadia_String*
wordTypeToString
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic* self,
    Arcadia_PDLC_WordType wordType
  );

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic* self
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

Arcadia_defineObjectType(u8"Arcadia.PDLC.Diagnostics.FileNotFoundDiagnostic", Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic,
                         u8"Arcadia.Languages.Diagnostic", Arcadia_Languages_Diagnostic,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic);
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value diagnosticType = Arcadia_ValueStack_getValue(thread, 3);
    Arcadia_Value file = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_Value offset = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
    Arcadia_ValueStack_pushValue(thread, &diagnosticType);
    Arcadia_ValueStack_pushValue(thread, &file);
    Arcadia_ValueStack_pushValue(thread, &offset);
    Arcadia_ValueStack_pushNatural8Value(thread, 3);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  Arcadia_EnumerationValue temporary = Arcadia_ValueStack_getEnumerationValue(thread, 1);
  if (temporary.type != _Arcadia_PDLC_FileType_getType(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->fileType = (Arcadia_PDLC_FileType)temporary.value;
  Arcadia_LeaveConstructor(Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileNotFoundDiagnosticDispatch* self
  )
{
  ((Arcadia_Languages_DiagnosticDispatch*)self)->getMessage = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Diagnostic*)) & getMessageImpl;
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic* self
  )
{/*Intentionally empty.*/}

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic* self
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"error: ");
  switch (self->fileType) {
    case Arcadia_PDLC_FileType_CompilationUnit: {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"compilation unit file");
    } break;
    case Arcadia_PDLC_FileType_ConfigurationFile: {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"configuration file");
    } break;
    case Arcadia_PDLC_FileType_ModuleDirectory: {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"module directory");
    } break;
    default: {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"file ");
    } break;
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8" not found\n");
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer));
}

Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic*
Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType diagnosticType,
    Arcadia_Languages_InputFile* file,
    Arcadia_PDLC_FileType fileType
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic);
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Languages_DiagnosticType_getType(thread), diagnosticType));
  if (file) Arcadia_ValueStack_pushObjectReferenceValue(thread, file); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_PDLC_FileType_getType(thread), fileType));
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_PDLC_Diagnostics_FileNotFoundDiagnostic);
}
