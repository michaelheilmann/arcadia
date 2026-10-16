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
#include "Arcadia/PDLC/Context.h"

#include "Arcadia/Logging/Include.h"
#include "Arcadia/PDLC/Environment.h"
#include "Arcadia/PDLC/MemberEnterPhase/ClassCompleter.h"
#include "Arcadia/PDLC/MemberEnterPhase/EnumerationCompleter.h"
#include "Arcadia/PDLC/Backend/Implementation.h"
#include "Arcadia/PDLC/Scanner.h"

static void
Arcadia_PDLC_Context_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* self
  );

static void
Arcadia_PDLC_Context_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_ContextDispatch* self
  );

static void
Arcadia_PDLC_Context_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* self
  );

static void
Arcadia_PDLC_Context_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_Context_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_PDLC_Context_destruct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_Context_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_Context_visit,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.Context", Arcadia_PDLC_Context,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_PDLC_Context_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_Context);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->instances = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));

  self->stringTable = Arcadia_Languages_StringTable_getOrCreate(thread);
  self->log = (Arcadia_Log*)Arcadia_ConsoleLog_create(thread);
  self->diagnostics = Arcadia_Languages_Diagnostics_create(thread, self->log);
  self->inputFileManager = Arcadia_Languages_InputFileManager_create(thread, self->diagnostics);

  self->workingDirectoryPath = NULL;

  self->moduleNodes = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->scope = NULL;
  self->environments = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));

  Arcadia_LeaveConstructor(Arcadia_PDLC_Context);
}

static void
Arcadia_PDLC_Context_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_ContextDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_Context_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* self
  )
{
  if (self->instances) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->instances);
  }

  if (self->stringTable) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->stringTable);
  }
  if (self->log) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->log);
  }
  if (self->diagnostics) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics);
  }
  if (self->inputFileManager) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->inputFileManager);
  }

  if (self->workingDirectoryPath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->workingDirectoryPath);
  }
  if (self->moduleNodes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->moduleNodes);
  }

  if (self->scope) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->scope);
  }

  if (self->environments) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->environments);
  }
}

static void
Arcadia_PDLC_Context_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* self
  )
{/*Intentionally empty.*/}

Arcadia_PDLC_Context*
Arcadia_PDLC_Context_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_Context);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_PDLC_Context);
}
