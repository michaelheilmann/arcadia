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

#if !defined(_POSIX_C_SOURCE)
  #define _POSIX_C_SOURCE 200809L
#endif

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/Clock.h"

#include <math.h>
#include <stddef.h>

#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  #include <time.h>
#else
  #error("operating system not (yet) supported")
#endif

struct Arcadia_Engine_Clock {
  Arcadia_Object _parent;
  Arcadia_Real64Value lastUpdate;
  Arcadia_BooleanValue paused;
};

static void
failClockRead
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
  Arcadia_Thread_jump(thread);
}

static void
requireClock
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Clock* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_Real64Value
readMonotonicTime
  (
    Arcadia_Thread* thread
  )
{
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  LARGE_INTEGER counter;
  LARGE_INTEGER frequency;
  if (!QueryPerformanceFrequency(&frequency) || frequency.QuadPart <= 0 ||
      !QueryPerformanceCounter(&counter)) {
    failClockRead(thread);
  }
  return (Arcadia_Real64Value)counter.QuadPart / (Arcadia_Real64Value)frequency.QuadPart;
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  struct timespec value;
  if (clock_gettime(CLOCK_MONOTONIC, &value)) {
    failClockRead(thread);
  }
  return (Arcadia_Real64Value)value.tv_sec + (Arcadia_Real64Value)value.tv_nsec / 1000000000.0;
#endif
}

static Arcadia_Real64Value
readClock
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Real64Value now = readMonotonicTime(thread);
  if (!isfinite(now)) {
    failClockRead(thread);
  }
  return now;
}

static void
constructClock
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Clock* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Clock);
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->lastUpdate = 0.0;
  self->paused = Arcadia_BooleanValue_True;
  self->lastUpdate = readClock(thread);
  Arcadia_LeaveConstructor(Arcadia_Engine_Clock);
}

static void
initializeClockDispatch
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_ClockDispatch* self
  )
{
  (void)thread;
  (void)self;
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructClock,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeClockDispatch,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Clock", Arcadia_Engine_Clock,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

Arcadia_Engine_Clock*
Arcadia_Engine_Clock_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Clock);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_Clock);
}

void
Arcadia_Engine_Clock_setPaused
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Clock* self,
    Arcadia_BooleanValue paused
  )
{
  requireClock(thread, self);
  if (paused != Arcadia_BooleanValue_True && paused != Arcadia_BooleanValue_False) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (self->paused == paused) {
    return;
  }
  if (paused == Arcadia_BooleanValue_False) {
    // Reset the baseline on resume; do not count paused time.
    Arcadia_Real64Value now = readClock(thread);
    self->lastUpdate = now;
  }
  self->paused = paused;
}

Arcadia_BooleanValue
Arcadia_Engine_Clock_getPaused
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Clock* self
  )
{
  requireClock(thread, self);
  return self->paused;
}

Arcadia_Real64Value
Arcadia_Engine_Clock_getTick
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Clock* self
  )
{
  requireClock(thread, self);
  if (self->paused) {
    return 0.0;
  }
  Arcadia_Real64Value now = readClock(thread);
  if (now < self->lastUpdate) {
    return 0.0;
  }
  Arcadia_Real64Value delta = now - self->lastUpdate;
  self->lastUpdate = now;
  return delta;
}
