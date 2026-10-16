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

#include "Arcadia/PDLC/Diagnostics/FileSystemDiagnostic.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileSystemDiagnostic* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_Diagnostics_FileSystemDiagnostic);
  if (_numberOfArguments != 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Languages_DiagnosticType_getType(thread), Arcadia_Languages_DiagnosticType_Error));
  Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->path = (Arcadia_FilePath*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_FilePath_getType(thread));
  self->message = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_PDLC_Diagnostics_FileSystemDiagnostic);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileSystemDiagnostic* self
  )
{
  if (self->path) Arcadia_Object_visit(thread, (Arcadia_Object*)self->path);
  if (self->message) Arcadia_Object_visit(thread, (Arcadia_Object*)self->message);
}

static Arcadia_String*
getMessageImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileSystemDiagnostic* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, builder);
  Arcadia_StringBuilder_insertBackCxxString(thread, builder, "error: ");
  Arcadia_StringBuilder_insertBackString(thread, builder, self->message);
  Arcadia_StringBuilder_insertBackCxxString(thread, builder, " `");
  Arcadia_StringBuilder_insertBackString(thread, builder, Arcadia_FilePath_toGeneric(thread, self->path));
  Arcadia_StringBuilder_insertBackCxxString(thread, builder, "`\n");
  Arcadia_String* result = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(builder));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Diagnostics_FileSystemDiagnosticDispatch* self
  )
{
  ((Arcadia_Languages_DiagnosticDispatch*)self)->getMessage = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Diagnostic*)) &getMessageImpl;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.Diagnostics.FileSystemDiagnostic", Arcadia_PDLC_Diagnostics_FileSystemDiagnostic,
                         u8"Arcadia.Languages.Diagnostic", Arcadia_Languages_Diagnostic,
                         &typeOperations);

Arcadia_PDLC_Diagnostics_FileSystemDiagnostic*
Arcadia_PDLC_Diagnostics_FileSystemDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* path,
    Arcadia_String* message
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_Diagnostics_FileSystemDiagnostic);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, path);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, message);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_PDLC_Diagnostics_FileSystemDiagnostic);
}
