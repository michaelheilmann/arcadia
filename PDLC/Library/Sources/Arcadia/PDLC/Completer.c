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
#include "Arcadia/PDLC/Completer.h"

static void
Arcadia_PDLC_Completer_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Completer* self
  );

static void
Arcadia_PDLC_Completer_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_CompleterDispatch* self
  );

static void
Arcadia_PDLC_Completer_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Completer* self
  );

static void
Arcadia_PDLC_Completer_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Completer* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_Completer_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_PDLC_Completer_destruct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_Completer_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_Completer_visit,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.Completer", Arcadia_PDLC_Completer,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_PDLC_Completer_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Completer* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_Completer);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_PDLC_Completer);
}

static void
Arcadia_PDLC_Completer_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_CompleterDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_Completer_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Completer* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_Completer_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Completer* self
  )
{/*Intentionally empty.*/}

void
Arcadia_PDLC_Completer_complete
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Completer* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  )
{ Arcadia_VirtualCall(Arcadia_PDLC_Completer, complete, self, context, symbol); }
