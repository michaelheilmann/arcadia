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
#include "Arcadia/Ring1/Implementation/Signals/Signal.h"

#include "Arcadia/Ring1/Implementation/Thread.h"
#include "Arcadia/Ring1/Implementation/ThreadExtensions.h"
#include "Arcadia/Ring1/Implementation/Objects/WeakReference.h"

#include "Arcadia/Ring1/Implementation/Signals/Slot.h"
#include "Arcadia/Ring1/Implementation/Signals/Internal.h"

#include "Arcadia/Ring1/Implementation/Signals/Slot.h"

static void
Arcadia_Signal_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* self
  );

static void
Arcadia_Signal_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* self
  );

static void
Arcadia_Signal_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* self
  );

static void
Arcadia_Signal_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_SignalDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Signal_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Signal_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Signal_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Signal_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Signal", Arcadia_Signal,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Signal_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* self
  )
{
  Nodes* nodes = (Nodes*)self->pimpl;
  for (size_t i = 0, n = nodes->size; i < n; ++i) {
    Arcadia_Slot* slot = nodes->elements[i];
    Arcadia_Slot_disconnect(thread, slot);
  }
  free(nodes);
}

static void
Arcadia_Signal_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* self
  )
{
  Nodes* nodes = (Nodes*)self->pimpl;
  for (size_t i = 0, n = nodes->size; i < n; ++i) {
    Arcadia_Slot* slot = nodes->elements[i];
    if (slot) {
      Arcadia_Object_visit(thread, (Arcadia_Object*)slot);
    }
  }
}

static void
Arcadia_Signal_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Signal);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  Nodes* nodes = malloc(sizeof(Nodes) + 8 * sizeof(Arcadia_Slot*));
  if (!nodes) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  for (size_t i = 0; i < 8; ++i) {
    nodes->elements[i] = NULL;
  }
  nodes->size = 0;
  nodes->reentrancyCount = 0;
  self->pimpl = nodes;
  //
  Arcadia_LeaveConstructor(Arcadia_Signal);
}

static void
Arcadia_Signal_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_SignalDispatch* self
  )
{ }

Arcadia_Signal*
Arcadia_Signal_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Signal);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Signal);
}

Arcadia_Slot*
Arcadia_Signal_connect
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* self,
    Arcadia_Object* receiver,
    Arcadia_ForeignProcedure* procedure
  )
{
  Nodes* nodes = (Nodes*)self->pimpl;
  // The allocated array size is n * 8, n > 0.
  // If the used size is n * 8 then we grow the allocated array to (n+1)*8.
  if (nodes->size != 0 || nodes->size % 8 == 0) {
    if (nodes->size == MaximumNumberOfSlots) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
      Arcadia_Thread_jump(thread);
    }
    Nodes* newNodes = realloc(nodes, sizeof(Nodes) + sizeof(Arcadia_Slot*) * (nodes->size + 8));
    if (!newNodes) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
      Arcadia_Thread_jump(thread);
    }
    self->pimpl = (void*)newNodes;
    nodes = newNodes;
  }
  Arcadia_Slot* slot = Arcadia_Slot_create(thread, self, receiver, procedure);
  nodes->elements[nodes->size++] = slot;
  return slot;
}

// This takes transforms
// @code
// [arguments, numberOfArguments]
// @endcode
// to
// @code
// [receiver, sender, arguments, numberOfArguments + 2]
// @endcode
// @todo
// Guard against numeric overflows.
static Arcadia_SizeValue pushArguments(Arcadia_Thread* thread, Arcadia_Object* sender, Arcadia_Object* receiver) {
  // (1) Get the number of arguments.
  // The first pass-through argument resides at numberOfArguments
  Arcadia_SizeValue numberOfArguments = Arcadia_ValueStack_getNatural8Value(thread, 0);
  // (2) Push the receiver first, push the sender second.
  // The first pass-through argument resides at 2 + numberOfArguments
  Arcadia_ValueStack_pushObjectReferenceValue(thread, receiver);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, sender);
  for (Arcadia_SizeValue i = numberOfArguments, j = numberOfArguments + 2; i > 0; --i) {
    // (1.2) Iterate over the arguments.
    Arcadia_Value argument = Arcadia_ValueStack_getValue(thread, j);
    // (1.3) Push the argument on the stack again.
    Arcadia_ValueStack_pushValue(thread, &argument);
  }
  // (3) Push the number of arguments plus 2 (plus 2 for the receiver and the sender).
  Arcadia_ValueStack_pushNatural8Value(thread, numberOfArguments + 2);
  // (4) Return the number of values pushed.
  return numberOfArguments + 2 + 1;
}

void
Arcadia_Signal_emit
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* self,
    Arcadia_Object* sender
  )
{
  Nodes* nodes = (Nodes*)self->pimpl;
  if (nodes->reentrancyCount == Arcadia_Natural32Value_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  nodes->reentrancyCount++;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    for (Arcadia_SizeValue i = 0, n = nodes->size; i < n; ++i) {
      Arcadia_Slot* slot = nodes->elements[i];
      if (!slot) {
        continue;
      }
      Arcadia_Value receiverValue = Arcadia_WeakReference_getValue(thread, slot->receiver);
      if (Arcadia_Value_isVoidValue(&receiverValue)) {
        slot->procedure = NULL;
        slot->receiver = NULL;
        slot->signal = NULL;
      } else {
        if (!Arcadia_Value_isObjectReferenceValue(&receiverValue)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
          Arcadia_Thread_jump(thread);
        }
        Arcadia_SizeValue n = pushArguments(thread, sender, Arcadia_Value_getObjectReferenceValue(&receiverValue));
        Arcadia_ForeignProcedure* procedureValue = slot->procedure;
        Arcadia_JumpTarget jumpTarget;
        Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
        if (Arcadia_JumpTarget_save(&jumpTarget)) {
          (*procedureValue)(thread);
          Arcadia_Thread_popJumpTarget(thread);
          Arcadia_ValueStack_popValues(thread, n);
        } else {
          Arcadia_Thread_popJumpTarget(thread);
          Arcadia_ValueStack_popValues(thread, n);
          Arcadia_Thread_jump(thread);
        }
      }
    }
    Arcadia_Thread_popJumpTarget(thread);
    nodes->reentrancyCount--;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    nodes->reentrancyCount--;
    Arcadia_Thread_jump(thread);
  }
}

