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


#include "Arcadia.TemplateEngine/Library.h"
#include "Arcadia.TemplateEngine/Environment.h"
#include <stdlib.h>
#include <string.h>

// Return x:Arcadia.Integer8 where x denotes the day of the week [1,7] in UTC time.
static void
getUtcDayOfWeek
  (
    Arcadia_Thread* thread
  );

// Return x:Arcadia.String where x denotes the name of the day of the month [1,31] in UTC time.
static void
getUtcDayOfMonth
  (
    Arcadia_Thread* thread
  );

// Return x:Arcadia.String where x denotes the name of the day of the week name in UTC time.
static void
getUtcDayOfWeekName
  (
    Arcadia_Thread* thread
  );

// Returns x:Arcadia.Integer8 where x denotes the month of the year [1,12] in UTC time.
static void
getUtcMonth
  (
    Arcadia_Thread* thread
  );

// Returns x:Arcadia.String where x denotes the name of the month of the year in UTC time.
static void
getUtcMonthName
  (
    Arcadia_Thread* thread
  );

// [..., 1:Arcadia.Integer8} -> [..., x:Arcadia.Integer32] where x denotes the year in UTC time.
static void
getUtcYear
  (
    Arcadia_Thread* thread
  );

static void
getUtcDayOfWeek
  (
    Arcadia_Thread* thread
  )
{
  if (0 != Arcadia_ValueStack_getNatural8Value(thread, 0)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_PointInTime* pointInTime = Arcadia_PointInTime_create(thread, Arcadia_TimeStamp_getNow(thread));
  Arcadia_Integer8Value v = Arcadia_PointInTime_getDayOfWeek(thread, pointInTime, Arcadia_TimeSpecification_Utc);
  Arcadia_ValueStack_pushInteger8Value(thread, v + 1);
}


static void
getUtcDayOfMonth
  (
    Arcadia_Thread* thread
  )
{
  if (0 != Arcadia_ValueStack_getNatural8Value(thread, 0)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_PointInTime* pointInTime = Arcadia_PointInTime_create(thread, Arcadia_TimeStamp_getNow(thread));
  Arcadia_Integer8Value v = Arcadia_PointInTime_getDayOfMonth(thread, pointInTime, Arcadia_TimeSpecification_Utc);
  Arcadia_ValueStack_pushInteger8Value(thread, v + 1);
}

static void
getUtcDayOfWeekName
  (
    Arcadia_Thread* thread
  )
{
  getUtcDayOfWeek(thread);
  Arcadia_Integer8Value day = Arcadia_ValueStack_getInteger8Value(thread, 0);
  Arcadia_ValueStack_popValues(thread, 1);
  switch (day) {
    case 1: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"Monday", sizeof(u8"Monday") - 1))));
    } break;
    case 2: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"Tuesday", sizeof(u8"Tuesday") - 1))));
    } break;
    case 3: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"Wednesday", sizeof(u8"Wednesday") - 1))));
    } break;
    case 4: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"Thursday", sizeof(u8"Thursday") - 1))));
    } break;
    case 5: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"Friday", sizeof(u8"Friday") - 1))));
    } break;
    case 6: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"Saturday", sizeof(u8"Saturday") - 1))));
    } break;
    case 7: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"Sunday", sizeof(u8"Sunday") - 1))));
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

static void
getUtcMonth
  (
    Arcadia_Thread* thread
  )
{
  if (0 != Arcadia_ValueStack_getNatural8Value(thread, 0)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_PointInTime* pointInTime = Arcadia_PointInTime_create(thread, Arcadia_TimeStamp_getNow(thread));
  Arcadia_Integer8Value v = Arcadia_PointInTime_getMonth(thread, pointInTime, Arcadia_TimeSpecification_Utc);
  Arcadia_ValueStack_pushInteger8Value(thread, v + 1);
}

static void
getUtcMonthName
  (
    Arcadia_Thread* thread
  )
{
  getUtcMonth(thread);
  Arcadia_Integer8Value month = Arcadia_ValueStack_getInteger8Value(thread, 0);
  Arcadia_ValueStack_popValues(thread, 1);
  switch (month) {
    case 1: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"January", sizeof(u8"January") - 1))));
    } break;
    case 2: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"February", sizeof(u8"February") - 1))));
    } break;
    case 3: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"March", sizeof(u8"March") - 1))));
    } break;
    case 4: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"April", sizeof(u8"April") - 1))));
    } break;
    case 5: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"May", sizeof(u8"May") - 1))));
    } break;
    case 6: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"June", sizeof(u8"June") - 1))));
    } break;
    case 7: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"July", sizeof(u8"July") - 1))));
    } break;
    case 8: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"August", sizeof(u8"August") - 1))));
    } break;
    case 9: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"September", sizeof(u8"September") - 1))));
    } break;
    case 10: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"October", sizeof(u8"October") - 1))));
    } break;
    case 11: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"November", sizeof(u8"November") - 1))));
    } break;
    case 12: {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"December", sizeof(u8"December") - 1))));
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

static void
getUtcYear
  (
    Arcadia_Thread* thread
  )
{
  if (0 != Arcadia_ValueStack_getNatural8Value(thread, 0)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_PointInTime* pointInTime = Arcadia_PointInTime_create(thread, Arcadia_TimeStamp_getNow(thread));
  Arcadia_Integer32Value v = Arcadia_PointInTime_getYear(thread, pointInTime, Arcadia_TimeSpecification_Utc);
  Arcadia_ValueStack_pushInteger32Value(thread, v + 1900);
}

void
Arcadia_TemplateEngine_registerTimeLibrary
  (
    Arcadia_Thread* thread,
    Environment *environment
  )
{
  Arcadia_Value k, v;

  k = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"dayOfWeek", sizeof(u8"dayOfWeek") - 1))));
  v = Arcadia_Value_makeForeignProcedureValue(&getUtcDayOfWeek);
  Environment_set(thread, environment, k, v);

  k = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"dayOfMonth", sizeof(u8"dayOfMonth") - 1))));
  v = Arcadia_Value_makeForeignProcedureValue(&getUtcDayOfMonth);
  Environment_set(thread, environment, k, v);

  k = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"dayOfWeekName", sizeof(u8"dayOfWeekName") - 1))));
  v = Arcadia_Value_makeForeignProcedureValue(&getUtcDayOfWeekName);
  Environment_set(thread, environment, k, v);

  k = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"month", sizeof(u8"month") - 1))));
  v = Arcadia_Value_makeForeignProcedureValue(&getUtcMonth);
  Environment_set(thread, environment, k, v);

  k = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"monthName", sizeof(u8"monthName") - 1))));
  v = Arcadia_Value_makeForeignProcedureValue(&getUtcMonthName);
  Environment_set(thread, environment, k, v);

  k = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"year", sizeof(u8"year") - 1))));
  v = Arcadia_Value_makeForeignProcedureValue(&getUtcYear);
  Environment_set(thread, environment, k, v);
}
