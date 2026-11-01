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

typedef struct EventProbe EventProbe;
typedef struct EventProbeDispatch EventProbeDispatch;

struct EventProbe {
  Arcadia_Engine_Event _parent;
  Arcadia_String* label;
};

struct EventProbeDispatch {
  Arcadia_Engine_EventDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.EventProbe", EventProbe,
                          u8"Arcadia.Engine.Event");

static Arcadia_SizeValue destroyed;

static void
constructImpl
  (
    Arcadia_Thread* thread,
    EventProbe* self
  )
{
  Arcadia_EnterConstructor(EventProbe);
  if (_numberOfArguments != 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value timestamp = Arcadia_ValueStack_getValue(thread, 2);
  Arcadia_ValueStack_pushValue(thread, &timestamp);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->label = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(EventProbe);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    EventProbe* self
  )
{
  if (self->label) Arcadia_Object_visit(thread, (Arcadia_Object*)self->label);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    EventProbe* self
  )
{ destroyed++; }

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Tests.EventProbe", EventProbe,
                         u8"Arcadia.Engine.Event", Arcadia_Engine_Event,
                         &typeOperations);

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
    _Arcadia_EndCreate0(thread, _Arcadia_Engine_Event_getType(thread), base);
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
  Arcadia_Natural64Value values[] = { 0, 1, Arcadia_Natural64Value_Maximum };
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
    Arcadia_ValueStack_pushNatural64Value(thread, values[i]);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Engine_Event* self = _Arcadia_EndCreate0(thread, _Arcadia_Engine_Event_getType(thread), base);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
    Arcadia_Tests_assertTrue(thread, self->timestamp == values[i]);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, self->timestamp == values[i]);
    Arcadia_ValueStack_popValues(thread, 1);
  }
  destroyed = 0;
  Arcadia_ValueStack_pushNatural64Value(thread, Arcadia_Natural64Value_Maximum);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_createFromCxxString(thread, "event probe"));
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  EventProbe* probe = _Arcadia_EndCreate0(thread, _EventProbe_getType(thread), base);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, probe);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)probe)->timestamp == Arcadia_Natural64Value_Maximum);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, probe->label, "event probe", sizeof("event probe") - 1));
  Arcadia_Engine_Input_MouseButtonEvent* button = Arcadia_Engine_Input_MouseButtonEvent_create(thread, values[2], Arcadia_Engine_Input_MouseButtonAction_Pressed, values[2], Arcadia_Integer32Value_Minimum, Arcadia_Integer32Value_Maximum);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, button);
  Arcadia_Engine_Input_MousePointerEvent* pointer = Arcadia_Engine_Input_MousePointerEvent_create(thread, values[2], Arcadia_Engine_Input_MousePointerAction_Moved, -7, 19);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, pointer);
  Arcadia_Engine_Visuals_CanvasSizeChangedEvent* canvas = Arcadia_Engine_Visuals_CanvasSizeChangedEvent_create(thread, values[2], 640, 480);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, canvas);
  Arcadia_Engine_Input_KeyboardKeyEvent* keyboard = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, values[2], Arcadia_Engine_Input_KeyboardKeyAction_Pressed, (Arcadia_Engine_Input_KeyboardKey)65);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, keyboard);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)button)->timestamp == values[2] && button->button == values[2] && button->action == Arcadia_Engine_Input_MouseButtonAction_Pressed);
  Arcadia_Tests_assertTrue(thread, button->x == Arcadia_Integer32Value_Minimum && button->y == Arcadia_Integer32Value_Maximum);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)pointer)->timestamp == values[2] && pointer->action == Arcadia_Engine_Input_MousePointerAction_Moved && pointer->x == -7 && pointer->y == 19);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)canvas)->timestamp == values[2] && canvas->horizontalSize == 640 && canvas->verticalSize == 480);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)keyboard)->timestamp == values[2] && Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, keyboard) == Arcadia_Engine_Input_KeyboardKeyAction_Pressed && Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, keyboard) == (Arcadia_Engine_Input_KeyboardKey)65);
  Arcadia_ValueStack_popValues(thread, 5);
  expectFailure(thread, Arcadia_Value_makeNatural64Value(1), 0, Arcadia_Status_NumberOfArgumentsInvalid);
  expectFailure(thread, Arcadia_Value_makeNatural64Value(1), 2, Arcadia_Status_NumberOfArgumentsInvalid);
  expectFailure(thread, Arcadia_Value_makeInteger64Value(1), 1, Arcadia_Status_ArgumentTypeInvalid);
  expectFailure(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), 1, Arcadia_Status_ArgumentTypeInvalid);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 1 && Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Engine_Tests_CoreEvents_event
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
