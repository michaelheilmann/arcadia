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
#include "Arcadia/Ring2/Time/PointInTime.h"

#include <time.h>
#include "Arcadia/Ring2/Implementation/ArgumentsValidation.h"

Arcadia_Integer64Value
Arcadia_TimeStamp_getNow
  (
    Arcadia_Thread* thread
  )
{
  time_t t;
  time(&t);
  return t;
}

static void
Arcadia_PointInTime_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PointInTime* self
  );

static void
Arcadia_PointInTime_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PointInTimeDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PointInTime_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PointInTime_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PointInTime", Arcadia_PointInTime,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_PointInTime_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PointInTime* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PointInTime);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->timeStamp = Arcadia_ValueStack_getInteger64Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_PointInTime);
}

static void
Arcadia_PointInTime_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PointInTimeDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_PointInTime*
Arcadia_PointInTime_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value timeStamp
  )
{
  _Arcadia_BeginCreate(Arcadia_PointInTime);
  Arcadia_ValueStack_pushInteger64Value(thread, timeStamp);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_PointInTime);
}

Arcadia_Integer8Value
Arcadia_PointInTime_getDayOfWeek
  (
    Arcadia_Thread* thread,
    Arcadia_PointInTime* self,
    Arcadia_TimeSpecification timeSpecification
  )
{
  switch (timeSpecification) {
    case Arcadia_TimeSpecification_LocalTime: {
      time_t t1 = self->timeStamp;
      struct tm t2;
    #if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
      if (localtime_s(&t2, &t1)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #else
      if (!localtime_r(&t1, &t2)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #endif
      return t2.tm_wday;
    } break;
    case Arcadia_TimeSpecification_Utc: {
      time_t t1 = self->timeStamp;
      struct tm t2;
    #if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
      if (gmtime_s(&t2, &t1)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #else
      if (!gmtime_r(&t1, &t2)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #endif
      return t2.tm_wday;
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  }
}

Arcadia_Integer8Value
Arcadia_PointInTime_getDayOfMonth
  (
    Arcadia_Thread* thread,
    Arcadia_PointInTime* self,
    Arcadia_TimeSpecification timeSpecification
  )
{
  switch (timeSpecification) {
    case Arcadia_TimeSpecification_LocalTime: {
      time_t t1 = self->timeStamp;
      struct tm t2;
    #if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
      if (localtime_s(&t2, &t1)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #else
      if (!localtime_r(&t1, &t2)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #endif
      return t2.tm_mday - 1;
    } break;
    case Arcadia_TimeSpecification_Utc: {
      time_t t1 = self->timeStamp;
      struct tm t2;
    #if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
      if (gmtime_s(&t2, &t1)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #else
      if (!gmtime_r(&t1, &t2)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #endif
      return t2.tm_mday - 1;
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  }
}

Arcadia_Integer8Value
Arcadia_PointInTime_getMonth
  (
    Arcadia_Thread* thread,
    Arcadia_PointInTime* self,
    Arcadia_TimeSpecification timeSpecification
  )
{
  switch (timeSpecification) {
    case Arcadia_TimeSpecification_LocalTime: {
      time_t t1 = self->timeStamp;
      struct tm t2;
  #if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
      if (localtime_s(&t2, &t1)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
  #else
      if (!localtime_r(&t1, &t2)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
  #endif
      return t2.tm_mon;
    } break;
    case Arcadia_TimeSpecification_Utc: {
      time_t t1 = self->timeStamp;
      struct tm t2;
    #if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
      if (gmtime_s(&t2, &t1)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #else
      if (!gmtime_r(&t1, &t2)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #endif
      return t2.tm_mon;
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  }
}

Arcadia_Integer32Value
Arcadia_PointInTime_getYear
  (
    Arcadia_Thread* thread,
    Arcadia_PointInTime* self,
    Arcadia_TimeSpecification timeSpecification
  )
{
  switch (timeSpecification) {
    case Arcadia_TimeSpecification_LocalTime: {
      time_t t1 = self->timeStamp;
      struct tm t2;
    #if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
      if (localtime_s(&t2, &t1)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #else
      if (!localtime_r(&t1, &t2)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #endif
      return t2.tm_year;
    } break;
    case Arcadia_TimeSpecification_Utc: {
      time_t t1 = self->timeStamp;
      struct tm t2;
    #if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
      if (gmtime_s(&t2, &t1)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #else
      if (!gmtime_r(&t1, &t2)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    #endif
      return t2.tm_year;
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  }
}
