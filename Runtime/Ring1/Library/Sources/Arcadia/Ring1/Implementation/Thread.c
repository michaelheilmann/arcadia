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
#include "Arcadia/Ring1/Implementation/Thread.h"

#include "Arcadia/Ring1/Implementation/Thread.module.h"
#include "Arcadia/ARMS/Include.h"
#include <assert.h>

Arcadia_SizeValue
Arcadia_ValueStack_getSize
  (
    Arcadia_Thread* thread
  )
{ return thread->stack.size; }

Arcadia_Value
Arcadia_ValueStack_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue index
  )
{
  if (index >= thread->stack.size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue arrayIndex = thread->stack.size - 1 - index;
  return thread->stack.elements[arrayIndex];
}

void
Arcadia_ValueStack_pushValue
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* value
  )
{
  Arcadia_Thread_ensureValueStackFreeCapacity(thread, 1);
  thread->stack.elements[thread->stack.size] = *value;
  thread->stack.size++;
}

void
Arcadia_ValueStack_popValues
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue count
  )
{
  if (count > thread->stack.size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  thread->stack.size -= count;
}

void
Arcadia_ValueStack_reverse
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue start,
    Arcadia_SizeValue count
  )
{
  if (Arcadia_SizeValue_Maximum - start < count || start + count > thread->stack.size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < count / 2; ++i) {
    Arcadia_SizeValue a = start + i;
    Arcadia_SizeValue b = start + count - 1 - i;
    a = thread->stack.size - 1 - a;
    b = thread->stack.size - 1 - b;
    Arcadia_Value t = thread->stack.elements[a];
    thread->stack.elements[a] = thread->stack.elements[b];
    thread->stack.elements[b] = t;
  }
}


void
Arcadia_Thread_raise
  (
    Arcadia_Thread* thread,
    Arcadia_Value raisedValue
  )
{
  thread->raisedValue = raisedValue;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ValueRaised);
  Arcadia_Thread_jump(thread);
}


void
Arcadia_Thread_pushJumpTarget
  (
    Arcadia_Thread* thread,
    Arcadia_JumpTarget* jumpTarget
  )
{
  assert(NULL != thread && NULL != jumpTarget);
  jumpTarget->previous = thread->jumpTarget;
  thread->jumpTarget = jumpTarget;
}

void
Arcadia_Thread_popJumpTarget
  (
    Arcadia_Thread* thread
  )
{
  assert(NULL != thread && NULL != thread->jumpTarget);
  thread->jumpTarget = thread->jumpTarget->previous;
}

Arcadia_NoReturn() void
Arcadia_Thread_jump
  (
    Arcadia_Thread* thread
  )
{
  assert(NULL != thread && NULL != thread->jumpTarget);
  longjmp(thread->jumpTarget->environment, -1);
}


Arcadia_Status
Arcadia_Thread_getStatus
  (
    Arcadia_Thread* thread
  )
{ return thread->status; }

void
Arcadia_Thread_setStatus
  (
    Arcadia_Thread* thread,
    Arcadia_Status status
  )
{ thread->status = status; }


void
Arcadia_Thread_setRaisedValue
  (
    Arcadia_Thread* thread,
    Arcadia_Value raisedValue
  )
{ thread->raisedValue = raisedValue; }

Arcadia_Value
Arcadia_Thread_getRaisedValue
  (
    Arcadia_Thread* thread
  )
{ return thread->raisedValue; }


Arcadia_Process*
Arcadia_Thread_getProcess
  (
    Arcadia_Thread* thread
  )
{ return thread->process; }
