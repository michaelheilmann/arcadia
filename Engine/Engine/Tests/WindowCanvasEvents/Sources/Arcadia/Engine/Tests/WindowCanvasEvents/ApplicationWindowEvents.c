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

#include "Arcadia/Engine/Tests/WindowCanvasEvents/Cases.h"
#include "Arcadia/Engine/Include.h"

typedef struct WindowProbe WindowProbe;
typedef struct WindowProbeDispatch WindowProbeDispatch;

struct WindowProbe {
  Arcadia_Engine_Visuals_Window _parent;
};

struct WindowProbeDispatch {
  Arcadia_Engine_Visuals_WindowDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.WindowProbe", WindowProbe,
                          u8"Arcadia.Engine.Visuals.Window");

static Arcadia_SizeValue destroyed;

static void
constructImpl
  (
    Arcadia_Thread* thread,
    WindowProbe* self
  )
{
  Arcadia_EnterConstructor(WindowProbe);
  if (_numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(WindowProbe);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    WindowProbe* self
  )
{ destroyed++; }

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Tests.WindowProbe", WindowProbe,
                         u8"Arcadia.Engine.Visuals.Window", Arcadia_Engine_Visuals_Window,
                         &typeOperations);

static Arcadia_Object*
create
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Value* args,
    Arcadia_Natural8Value count
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_Natural8Value i = 0; i < count; ++i) Arcadia_ValueStack_pushValue(thread, &args[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, count);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
expectFailure
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Value* args,
    Arcadia_Natural8Value count,
    Arcadia_Status status
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    create(thread, type, args, count);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == status);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, 0);
  }
}

static void
testApplicationEvents
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Natural64Value timestamps[] = { 0, 1, Arcadia_Natural64Value_Maximum };
  Arcadia_TypeValue types[] = {
    _Arcadia_Engine_Visuals_ApplicationEvent_getType(thread),
    _Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_getType(thread)
  };
  for (size_t t = 0; t < sizeof(timestamps) / sizeof(timestamps[0]); ++t) {
    Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent* self = Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_create(thread, timestamps[t]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)self)->timestamp == timestamps[t]);
    Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, types[0]));
    Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Event_getType(thread)));
    Arcadia_ValueStack_popValues(thread, 1);
  }
  for (size_t k = 0; k < 2; ++k) {
    Arcadia_Value args[] = { Arcadia_Value_makeNatural64Value(1), Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void) };
    expectFailure(thread, types[k], args, 0, Arcadia_Status_NumberOfArgumentsInvalid);
    expectFailure(thread, types[k], args, 2, Arcadia_Status_NumberOfArgumentsInvalid);
    args[0] = Arcadia_Value_makeInteger64Value(1);
    expectFailure(thread, types[k], args, 1, Arcadia_Status_ArgumentTypeInvalid);
    args[0] = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
    expectFailure(thread, types[k], args, 1, Arcadia_Status_ArgumentTypeInvalid);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testWindowEvents
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Natural64Value timestamps[] = { 0, 1, Arcadia_Natural64Value_Maximum };
  Arcadia_Integer32Value pairs[][2] = {
    { 0, 0 }, { -3, 17 },
    { Arcadia_Integer32Value_Minimum, Arcadia_Integer32Value_Maximum },
    { Arcadia_Integer32Value_Maximum, Arcadia_Integer32Value_Minimum }
  };
  Arcadia_TypeValue types[] = {
    _Arcadia_Engine_Visuals_WindowClosedEvent_getType(thread),
    _Arcadia_Engine_Visuals_WindowPositionChangedEvent_getType(thread),
    _Arcadia_Engine_Visuals_WindowSizeChangedEvent_getType(thread)
  };
  Arcadia_Engine_Visuals_Window* window = (void*)create(thread, _WindowProbe_getType(thread), NULL, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, window);
  for (size_t t = 0; t < sizeof(timestamps) / sizeof(timestamps[0]); ++t) {
    for (size_t p = 0; p < sizeof(pairs) / sizeof(pairs[0]); ++p) {
      Arcadia_Engine_Visuals_WindowClosedEvent* closed = Arcadia_Engine_Visuals_WindowClosedEvent_create(thread, timestamps[t], window);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, closed);
      Arcadia_Engine_Visuals_WindowPositionChangedEvent* position = Arcadia_Engine_Visuals_WindowPositionChangedEvent_create(thread, timestamps[t], window, pairs[p][0], pairs[p][1]);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, position);
      Arcadia_Engine_Visuals_WindowSizeChangedEvent* size = Arcadia_Engine_Visuals_WindowSizeChangedEvent_create(thread, timestamps[t], window, pairs[p][0], pairs[p][1]);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, size);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)closed)->timestamp == timestamps[t] && closed->window == window);
      Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)position)->timestamp == timestamps[t] && position->window == window);
      Arcadia_Tests_assertTrue(thread, position->horizontalPosition == pairs[p][0] && position->verticalPosition == pairs[p][1]);
      Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)size)->timestamp == timestamps[t] && size->window == window);
      Arcadia_Tests_assertTrue(thread, size->horizontalSize == pairs[p][0] && size->verticalSize == pairs[p][1]);
      Arcadia_ValueStack_popValues(thread, 3);
    }
  }
  for (size_t k = 0; k < 3; ++k) {
    Arcadia_Natural8Value count = k ? 4 : 2;
    Arcadia_Value args[] = {
      Arcadia_Value_makeNatural64Value(Arcadia_Natural64Value_Maximum),
      Arcadia_Value_makeObjectReferenceValue(window),
      Arcadia_Value_makeInteger32Value(-3), Arcadia_Value_makeInteger32Value(17),
      Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void)
    };
    expectFailure(thread, types[k], args, 0, Arcadia_Status_NumberOfArgumentsInvalid);
    expectFailure(thread, types[k], args, count - 1, Arcadia_Status_NumberOfArgumentsInvalid);
    expectFailure(thread, types[k], args, count + 1, Arcadia_Status_NumberOfArgumentsInvalid);
    for (size_t p = 0; p < count; ++p) {
      Arcadia_Value saved = args[p];
      args[p] = Arcadia_Value_makeNatural32Value(1);
      expectFailure(thread, types[k], args, count, Arcadia_Status_ArgumentTypeInvalid);
      if (p != 1) {
        args[p] = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
        expectFailure(thread, types[k], args, count, Arcadia_Status_ArgumentTypeInvalid);
      }
      args[p] = saved;
    }
    Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "wrong window type");
    Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
    args[1] = Arcadia_Value_makeObjectReferenceValue(wrong);
    expectFailure(thread, types[k], args, count, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  destroyed = 0;
  // Only the event roots this window and its managed title during collection.
  for (size_t k = 0; k < 3; ++k) {
    window = (void*)create(thread, _WindowProbe_getType(thread), NULL, 0);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, window);
    Arcadia_Value args[] = {
      Arcadia_Value_makeNatural64Value(Arcadia_Natural64Value_Maximum),
      Arcadia_Value_makeObjectReferenceValue(window),
      Arcadia_Value_makeInteger32Value(-3), Arcadia_Value_makeInteger32Value(17)
    };
    Arcadia_Object* event = create(thread, types[k], args, k ? 4 : 2);
    Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, event);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, destroyed == k);
    Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, window->title, "Arcadia Engine Window", sizeof("Arcadia Engine Window") - 1));
    Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, destroyed == k + 1);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testNullWindow
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_Visuals_WindowClosedEvent* closed = Arcadia_Engine_Visuals_WindowClosedEvent_create(thread, 1, NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, closed);
  Arcadia_Engine_Visuals_WindowPositionChangedEvent* position = Arcadia_Engine_Visuals_WindowPositionChangedEvent_create(thread, 1, NULL, -3, 17);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, position);
  Arcadia_Engine_Visuals_WindowSizeChangedEvent* size = Arcadia_Engine_Visuals_WindowSizeChangedEvent_create(thread, 1, NULL, 17, -3);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, size);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, !closed->window && !position->window && !size->window);
  Arcadia_Tests_assertTrue(thread, position->horizontalPosition == -3 && size->verticalSize == -3);
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  testApplicationEvents(thread);
  testWindowEvents(thread);
  testNullWindow(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_WindowCanvasEvents_applicationWindowEvents
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
