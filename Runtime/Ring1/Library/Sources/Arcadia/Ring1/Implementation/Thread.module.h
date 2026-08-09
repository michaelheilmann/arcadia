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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_THREAD_PRIVATE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_THREAD_PRIVATE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif
#if defined(ARCADIA_RING1_EXPORT)
  #error("this file is not supposed to be exported")
#endif
#include "Arcadia/Ring1/Implementation/Thread.h"

struct Arcadia_Thread {
  Arcadia_Status status;
  Arcadia_JumpTarget* jumpTarget;
  struct {
    Arcadia_Value* elements;
    Arcadia_SizeValue size;
    Arcadia_SizeValue capacity;
  } stack;
  // Weak reference to the process object which owns this thread object.
  // When the thread is constructed, this is NULL.
  // The constructing process assigns itself to this variable after the thread is constructed.
  Arcadia_Process* process;
  // The currently raised value.
  Arcadia_Value raisedValue;
};

/*@undefined thread does not point to an uninitialized Arcadia_Thread object*/
void
Arcadia_Thread_initializeValueStack
  (
    Arcadia_Thread* thread
  );

/*@undefined thread does not point to an uninitialized Arcadia_Thread object*/
void
Arcadia_Thread_uninitializeValueStack
  (
    Arcadia_Thread* thread
  );

/*@undefined thread does not point to an uninitialized Arcadia_Thread object*/
void
Arcadia_Thread_initialize
  (
    Arcadia_Thread* thread
  );

/*@undefined thread does not point to an initialized Arcadia_Thread object*/
void
Arcadia_Thread_uninitialize
  (
    Arcadia_Thread* thread
  );

/*@undefined thread does not point to an initialized Arcadia_Thread object*/
void
Arcadia_Thread_increaseValueStackCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue additionalCapacity
  );

void
Arcadia_Thread_ensureValueStackFreeCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue requiredFreeCapacity
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_THREAD_PRIVATE_H_INCLUDED
