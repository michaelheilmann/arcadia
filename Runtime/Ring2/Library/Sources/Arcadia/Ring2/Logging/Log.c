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

#define ARCADIA_RING2_MODULE (1)
#include "Arcadia/Ring2/Logging/Log.h"

static void
Arcadia_Log_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Log* self
  );

static void
Arcadia_Log_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_LogDispatch* self
  );

static void
Arcadia_Log_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Log* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Log_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Log_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Log_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Log", Arcadia_Log,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Log_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Log* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Log);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Log);
}

static void
Arcadia_Log_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_LogDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Log_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Log* self
  )
{ }

void
Arcadia_Log_information
  (
    Arcadia_Thread* thread,
    Arcadia_Log* self,
    Arcadia_String* message
  )
{ Arcadia_VirtualCall(Arcadia_Log, information, self, message); }

void
Arcadia_Log_warning
  (
    Arcadia_Thread* thread,
    Arcadia_Log* self,
    Arcadia_String* message
  )
{ Arcadia_VirtualCall(Arcadia_Log, warning, self, message); }

void
Arcadia_Log_error
  (
    Arcadia_Thread* thread,
    Arcadia_Log* self,
    Arcadia_String* message
  )
{ Arcadia_VirtualCall(Arcadia_Log, error, self, message); }
