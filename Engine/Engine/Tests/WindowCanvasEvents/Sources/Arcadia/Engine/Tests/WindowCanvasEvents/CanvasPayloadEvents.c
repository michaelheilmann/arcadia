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

typedef struct PayloadProbe PayloadProbe;
typedef struct PayloadProbeDispatch PayloadProbeDispatch;

struct PayloadProbe {
  Arcadia_Engine_Visuals_CanvasSizeChangedEvent _parent;
  Arcadia_String* label;
};

struct PayloadProbeDispatch {
  Arcadia_Engine_Visuals_CanvasSizeChangedEventDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.PayloadProbe", PayloadProbe,
                          u8"Arcadia.Engine.Visuals.CanvasSizeChangedEvent");

static Arcadia_SizeValue destroyed;

static void
constructImpl
  (
    Arcadia_Thread* thread,
    PayloadProbe* self
  )
{
  Arcadia_EnterConstructor(PayloadProbe);
  if (_numberOfArguments != 4) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value timestamp = Arcadia_ValueStack_getValue(thread, 4);
  Arcadia_Value horizontal = Arcadia_ValueStack_getValue(thread, 3);
  Arcadia_Value vertical = Arcadia_ValueStack_getValue(thread, 2);
  Arcadia_ValueStack_pushValue(thread, &timestamp);
  Arcadia_ValueStack_pushValue(thread, &horizontal);
  Arcadia_ValueStack_pushValue(thread, &vertical);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->label = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(PayloadProbe);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    PayloadProbe* self
  )
{
  if (self->label) Arcadia_Object_visit(thread, (Arcadia_Object*)self->label);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    PayloadProbe* self
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

Arcadia_defineObjectType(u8"Tests.PayloadProbe", PayloadProbe,
                         u8"Arcadia.Engine.Visuals.CanvasSizeChangedEvent", Arcadia_Engine_Visuals_CanvasSizeChangedEvent,
                         &typeOperations);

static void
expectFailure
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Value* arguments,
    Arcadia_Natural8Value count,
    Arcadia_Status expected
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    for (Arcadia_Natural8Value i = 0; i < count; ++i) Arcadia_ValueStack_pushValue(thread, &arguments[i]);
    Arcadia_ValueStack_pushNatural8Value(thread, count);
    _Arcadia_EndCreate0(thread, type, base);
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
  Arcadia_Integer32Value pairs[][2] = {
    { 0, 0 }, { 1, -1 }, { 120, 144 },
    { Arcadia_Integer32Value_Minimum, Arcadia_Integer32Value_Maximum },
    { Arcadia_Integer32Value_Maximum, Arcadia_Integer32Value_Minimum }
  };
  for (size_t t = 0; t < sizeof(timestamps) / sizeof(timestamps[0]); ++t) {
    for (size_t p = 0; p < sizeof(pairs) / sizeof(pairs[0]); ++p) {
      Arcadia_Engine_Visuals_CanvasSizeChangedEvent* size = Arcadia_Engine_Visuals_CanvasSizeChangedEvent_create(thread, timestamps[t], pairs[p][0], pairs[p][1]);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, size);
      Arcadia_Engine_Visuals_CanvasDPIChangedEvent* dpi = Arcadia_Engine_Visuals_CanvasDPIChangedEvent_create(thread, timestamps[t], pairs[p][0], pairs[p][1]);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, dpi);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)size)->timestamp == timestamps[t] && size->horizontalSize == pairs[p][0] && size->verticalSize == pairs[p][1]);
      Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)dpi)->timestamp == timestamps[t] && dpi->horizontalDpi == pairs[p][0] && dpi->verticalDpi == pairs[p][1]);
      Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)size, _Arcadia_Engine_Visuals_CanvasEvent_getType(thread)));
      Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)dpi, _Arcadia_Engine_Event_getType(thread)));
      Arcadia_ValueStack_popValues(thread, 2);
    }
    for (unsigned active = 0; active < 2; ++active) {
      Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent* self = Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent_create(thread, timestamps[t], active != 0);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)self)->timestamp == timestamps[t] && self->active == (active != 0));
      Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_CanvasEvent_getType(thread)));
      Arcadia_ValueStack_popValues(thread, 1);
    }
  }
  Arcadia_TypeValue types[] = { _Arcadia_Engine_Visuals_CanvasSizeChangedEvent_getType(thread), _Arcadia_Engine_Visuals_CanvasDPIChangedEvent_getType(thread) };
  for (size_t i = 0; i < sizeof(types) / sizeof(types[0]); ++i) {
    Arcadia_Value args[] = { Arcadia_Value_makeNatural64Value(timestamps[2]), Arcadia_Value_makeInteger32Value(-3), Arcadia_Value_makeInteger32Value(17), Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void) };
    expectFailure(thread, types[i], args, 0, Arcadia_Status_NumberOfArgumentsInvalid);
    expectFailure(thread, types[i], args, 2, Arcadia_Status_NumberOfArgumentsInvalid);
    expectFailure(thread, types[i], args, 4, Arcadia_Status_NumberOfArgumentsInvalid);
    for (size_t p = 0; p < 3; ++p) {
      Arcadia_Value saved = args[p];
      args[p] = p ? Arcadia_Value_makeNatural32Value(1) : Arcadia_Value_makeInteger64Value(1);
      expectFailure(thread, types[i], args, 3, Arcadia_Status_ArgumentTypeInvalid);
      args[p] = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
      expectFailure(thread, types[i], args, 3, Arcadia_Status_ArgumentTypeInvalid);
      args[p] = saved;
    }
  }
  Arcadia_Value args[] = { Arcadia_Value_makeNatural64Value(timestamps[2]), Arcadia_Value_makeBooleanValue(Arcadia_BooleanValue_True), Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void) };
  Arcadia_TypeValue activation = _Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent_getType(thread);
  expectFailure(thread, activation, args, 0, Arcadia_Status_NumberOfArgumentsInvalid);
  expectFailure(thread, activation, args, 1, Arcadia_Status_NumberOfArgumentsInvalid);
  expectFailure(thread, activation, args, 3, Arcadia_Status_NumberOfArgumentsInvalid);
  args[0] = Arcadia_Value_makeInteger64Value(1);
  expectFailure(thread, activation, args, 2, Arcadia_Status_ArgumentTypeInvalid);
  args[0] = Arcadia_Value_makeNatural64Value(timestamps[2]);
  args[1] = Arcadia_Value_makeNatural8Value(1);
  expectFailure(thread, activation, args, 2, Arcadia_Status_ArgumentTypeInvalid);
  args[1] = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  expectFailure(thread, activation, args, 2, Arcadia_Status_ArgumentTypeInvalid);

  destroyed = 0;
  Arcadia_ValueStack_pushNatural64Value(thread, timestamps[2]);
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_Integer32Value_Minimum);
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_Integer32Value_Maximum);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_createFromCxxString(thread, "canvas payload"));
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  PayloadProbe* probe = _Arcadia_EndCreate0(thread, _PayloadProbe_getType(thread), base);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, probe);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 0 && ((Arcadia_Engine_Event*)probe)->timestamp == timestamps[2]);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Visuals_CanvasSizeChangedEvent*)probe)->horizontalSize == Arcadia_Integer32Value_Minimum && ((Arcadia_Engine_Visuals_CanvasSizeChangedEvent*)probe)->verticalSize == Arcadia_Integer32Value_Maximum);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, probe->label, "canvas payload", sizeof("canvas payload") - 1));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 1 && Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Engine_Tests_WindowCanvasEvents_canvasPayloadEvents
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
