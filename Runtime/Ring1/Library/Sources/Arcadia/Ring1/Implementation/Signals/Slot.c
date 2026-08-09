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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Implementation/Signals/Slot.h"

#include "Arcadia/Ring1/Implementation/Thread.h"
#include "Arcadia/Ring1/Implementation/ThreadExtensions.h"
#include "Arcadia/Ring1/Implementation/Objects/WeakReference.h"
#include "Arcadia/Ring1/Implementation/Signals/Signal.h"

#include "Arcadia/Ring1/Implementation/Signals/Internal.h"

static void
Arcadia_Slot_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Slot* self
  );

static void
Arcadia_Slot_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Slot* self
  );

static void
Arcadia_Slot_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Slot* self
  );

static void
Arcadia_Slot_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_SlotDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Slot_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Slot_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Slot_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Slot_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Slot", Arcadia_Slot,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Slot_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Slot* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Slot_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Slot* self
  )
{
  if (self->signal) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->receiver);
  }
  if (self->receiver) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->receiver);
  }
}

static void
Arcadia_Slot_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Slot* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Slot);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->signal = Arcadia_WeakReference_create(thread, Arcadia_Value_makeObjectReferenceValue(Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_Signal_getType(thread))));
  self->receiver = Arcadia_WeakReference_create(thread, Arcadia_Value_makeObjectReferenceValue(Arcadia_ValueStack_getObjectReferenceValue(thread, 2)));
  self->procedure = Arcadia_ValueStack_getForeignProcedureValue(thread, 1);
  //
  Arcadia_LeaveConstructor(Arcadia_Slot);
}

static void
Arcadia_Slot_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_SlotDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_Slot*
Arcadia_Slot_create
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* signal,
    Arcadia_Object* receiver,
    Arcadia_ForeignProcedure* procedure
  )
{
  _Arcadia_BeginCreate(Arcadia_Slot);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)signal);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, receiver);
  Arcadia_ValueStack_pushForeignProcedureValue(thread, procedure);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_Slot);
}

void
Arcadia_Slot_disconnect
  (
    Arcadia_Thread* thread,
    Arcadia_Slot* self
  )
{
  self->signal = NULL;
  self->receiver = NULL;
  self->procedure = NULL;
}
