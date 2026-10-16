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

#include "Arcadia/Engine/Tests/CoreEvents/Cases.h"
#include "Arcadia/Engine/Include.h"

static void
expectFailure
  (
    Arcadia_Thread* thread,
    Arcadia_Value argument,
    Arcadia_Natural8Value count,
    Arcadia_Status expected
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    for (Arcadia_Natural8Value i = 0; i < count; ++i) Arcadia_ValueStack_pushValue(thread, &argument);
    Arcadia_ValueStack_pushNatural8Value(thread, count);
    _Arcadia_EndCreate0(thread, _Arcadia_Engine_Visuals_CanvasEvent_getType(thread), base);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == expected);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, 0);
  }
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Natural64Value timestamps[] = { 0, 1, Arcadia_Natural64Value_Maximum };
  for (size_t i = 0; i < sizeof(timestamps) / sizeof(timestamps[0]); ++i) {
    Arcadia_ValueStack_pushNatural64Value(thread, timestamps[i]);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Engine_Visuals_CanvasEvent* self = _Arcadia_EndCreate0(thread, _Arcadia_Engine_Visuals_CanvasEvent_getType(thread), base);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
    Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_CanvasEvent_getType(thread)));
    Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Event_getType(thread)));
    Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)self)->timestamp == timestamps[i]);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)self)->timestamp == timestamps[i]);
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_Engine_Visuals_CanvasSizeChangedEvent* size = Arcadia_Engine_Visuals_CanvasSizeChangedEvent_create(thread, timestamps[2], Arcadia_Integer32Value_Maximum, Arcadia_Integer32Value_Minimum);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, size);
  Arcadia_Engine_Visuals_CanvasDPIChangedEvent* dpi = Arcadia_Engine_Visuals_CanvasDPIChangedEvent_create(thread, timestamps[2], 120, 144);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, dpi);
  Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent* active = Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent_create(thread, timestamps[2], Arcadia_BooleanValue_True);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, active);
  Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent* inactive = Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent_create(thread, 0, Arcadia_BooleanValue_False);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, inactive);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)size)->timestamp == timestamps[2] && size->horizontalSize == Arcadia_Integer32Value_Maximum && size->verticalSize == Arcadia_Integer32Value_Minimum);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)dpi)->timestamp == timestamps[2] && dpi->horizontalDpi == 120 && dpi->verticalDpi == 144);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)active)->timestamp == timestamps[2] && active->active);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)inactive)->timestamp == 0 && !inactive->active);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)dpi, _Arcadia_Engine_Visuals_CanvasEvent_getType(thread)));
  Arcadia_ValueStack_popValues(thread, 4);
  expectFailure(thread, Arcadia_Value_makeNatural64Value(1), 0, Arcadia_Status_NumberOfArgumentsInvalid);
  expectFailure(thread, Arcadia_Value_makeNatural64Value(1), 2, Arcadia_Status_NumberOfArgumentsInvalid);
  expectFailure(thread, Arcadia_Value_makeNatural32Value(1), 1, Arcadia_Status_ArgumentTypeInvalid);
  expectFailure(thread, Arcadia_Value_makeInteger64Value(1), 1, Arcadia_Status_ArgumentTypeInvalid);
  expectFailure(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), 1, Arcadia_Status_ArgumentTypeInvalid);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Engine_Tests_CoreEvents_canvasEvent
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
