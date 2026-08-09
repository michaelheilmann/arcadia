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

#include "Arcadia/Languages/Diagnostics.h"

#include "Arcadia/Languages/Include.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticsDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self
  );

static void
emitLocation  
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self,
    Arcadia_Languages_Diagnostic* diagnostic
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

Arcadia_defineObjectType(u8"Arcadia.Languages.Diagnostics", Arcadia_Languages_Diagnostics,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Languages_Diagnostics);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->log = (Arcadia_Log*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Log_getType(thread));
  self->diagnostics = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_Languages_Diagnostics);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticsDispatch* self
  )
{/*Intentionally empty.*/}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self
  )
{
  if (self->diagnostics) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics); 
  }
  if (self->log) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->log);
  }
}

Arcadia_Languages_Diagnostics*
Arcadia_Languages_Diagnostics_create
  (
    Arcadia_Thread* thread,
    Arcadia_Log* log
  )
{ 
  _Arcadia_BeginCreate(Arcadia_Languages_Diagnostics);
  if (log) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)log); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Languages_Diagnostics);
}

Arcadia_BooleanValue
Arcadia_Languages_Diagnostics_hasErrors
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self
  )
{ 
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->diagnostics); i < n; ++i) {
    Arcadia_Languages_Diagnostic* diagnostic = (Arcadia_Languages_Diagnostic*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, self->diagnostics, i, _Arcadia_Languages_Diagnostic_getType(thread));
    if (Arcadia_Languages_DiagnosticType_Error == Arcadia_Languages_Diagnostic_getType(thread, diagnostic)) {
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

void
Arcadia_Languages_Diagnostics_add
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self,
    Arcadia_Languages_Diagnostic* diagnostic
  )
{
  Arcadia_List_insertBackObjectReferenceValue(thread, self->diagnostics, diagnostic);    
}

static void 
emitLocation
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self,
    Arcadia_Languages_Diagnostic* diagnostic
  )
{ 
  Arcadia_Languages_InputFile* file = Arcadia_Languages_Diagnostic_getInputFile(thread, diagnostic);
  if (file) {
    Arcadia_Log_error(thread, self->log, Arcadia_FilePath_toNative(thread, Arcadia_Languages_InputFile_getPath(thread, file), Arcadia_BooleanValue_False));
    Arcadia_Log_error(thread, self->log, Arcadia_String_createFromCxxString(thread, u8": "));
    if (Arcadia_Value_isSizeValue(&diagnostic->inputPosition)) {
      Arcadia_SizeValue offset = Arcadia_Value_getSizeValue(&diagnostic->inputPosition);
      Arcadia_SizeValue line = Arcadia_Languages_InputFile_getLine(thread, file, offset);
      Arcadia_Log_error(thread, self->log, Arcadia_String_createFromSize(thread, line));
      Arcadia_Log_error(thread, self->log, Arcadia_String_createFromCxxString(thread, u8": "));

    }
  }
}

void
Arcadia_Languages_Diagnostics_emit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self
  )
{ 
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->diagnostics); i < n; ++i) {
    Arcadia_Languages_Diagnostic* diagnostic = (Arcadia_Languages_Diagnostic*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, self->diagnostics, i, _Arcadia_Languages_Diagnostic_getType(thread));
    switch (diagnostic->type) {
      case Arcadia_Languages_DiagnosticType_Error: {
        emitLocation(thread, self, diagnostic);
        Arcadia_Log_error(thread, self->log, Arcadia_Languages_Diagnostic_getMessage(thread, diagnostic));
      } break;
      case Arcadia_Languages_DiagnosticType_Warning: {
        emitLocation(thread, self, diagnostic);
        Arcadia_Log_warning(thread, self->log, Arcadia_Languages_Diagnostic_getMessage(thread, diagnostic));
      } break;
      case Arcadia_Languages_DiagnosticType_Information: {
        emitLocation(thread, self, diagnostic);
        Arcadia_Log_information(thread, self->log, Arcadia_Languages_Diagnostic_getMessage(thread, diagnostic));
      } break;
      default: {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      } break;
    };
  }
}
