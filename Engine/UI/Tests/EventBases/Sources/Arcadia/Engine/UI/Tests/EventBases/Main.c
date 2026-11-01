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

#include "Arcadia/Testing/Include.h"

#include "Arcadia/Engine/UI/Include.h"
#include "Arcadia/Engine/UI/Tests/EventBases/Interfaces.h"
#include "Arcadia/Engine/UI/Tests/EventBases/Alignment.h"
#include "Arcadia/Engine/UI/Tests/EventBases/Node.h"
#include "Arcadia/Engine/UI/Tests/EventBases/Populate.h"

typedef struct WidgetProbe WidgetProbe;
typedef struct WidgetProbeDispatch WidgetProbeDispatch;

struct WidgetProbe { Arcadia_Engine_UI_WidgetNode _parent; };

struct WidgetProbeDispatch { Arcadia_Engine_UI_WidgetNodeDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.EventWidgetProbe", WidgetProbe,
                          u8"Arcadia.Engine.UI.WidgetNode");

static Arcadia_SizeValue destroyed;

static void
constructImpl
  (
    Arcadia_Thread* thread,
    WidgetProbe* self
  )
{
  Arcadia_EnterConstructor(WidgetProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  ((Arcadia_Engine_UI_WidgetNode*)self)->name = Arcadia_String_createFromCxxString(thread, "event source payload");
  Arcadia_LeaveConstructor(WidgetProbe);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    WidgetProbe* self
  )
{ destroyed++; }

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Tests.EventWidgetProbe", WidgetProbe,
                         u8"Arcadia.Engine.UI.WidgetNode", Arcadia_Engine_UI_WidgetNode,
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
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == status && Arcadia_ValueStack_getSize(thread) == base);
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
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_TypeValue types[] = {
    _Arcadia_Engine_UI_Event_getType(thread), _Arcadia_Engine_UI_WidgetEvent_getType(thread),
    _Arcadia_Engine_UI_WidgetPositionChangedEvent_getType(thread), _Arcadia_Engine_UI_WidgetSizeChangedEvent_getType(thread)
  };
  Arcadia_Natural64Value timestamps[] = { 0, 1, Arcadia_Natural64Value_Maximum };
  Arcadia_Integer32Value pairs[][2] = {
    { 0, 0 }, { -3, 17 },
    { Arcadia_Integer32Value_Minimum, Arcadia_Integer32Value_Maximum },
    { Arcadia_Integer32Value_Maximum, Arcadia_Integer32Value_Minimum }
  };
  Arcadia_Engine_UI_WidgetNode* source = (void*)create(thread, _WidgetProbe_getType(thread), NULL, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, source);
  for (size_t t = 0; t < sizeof(timestamps) / sizeof(timestamps[0]); ++t) {
    for (size_t p = 0; p < sizeof(pairs) / sizeof(pairs[0]); ++p) {
      Arcadia_Engine_UI_WidgetPositionChangedEvent* position = Arcadia_Engine_UI_WidgetPositionChangedEvent_create(thread, timestamps[t], source, pairs[p][0], pairs[p][1]);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, position);
      Arcadia_Engine_UI_WidgetSizeChangedEvent* size = Arcadia_Engine_UI_WidgetSizeChangedEvent_create(thread, timestamps[t], source, pairs[p][0], pairs[p][1]);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, size);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_Event*)position)->timestamp == timestamps[t] && ((Arcadia_Engine_UI_WidgetEvent*)position)->source == source);
      Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_Event*)size)->timestamp == timestamps[t] && ((Arcadia_Engine_UI_WidgetEvent*)size)->source == source);
      Arcadia_Tests_assertTrue(thread, position->horizontalPosition == pairs[p][0] && position->verticalPosition == pairs[p][1]);
      Arcadia_Tests_assertTrue(thread, size->horizontalSize == pairs[p][0] && size->verticalSize == pairs[p][1]);
      Arcadia_ValueStack_popValues(thread, 2);
    }
  }
  for (size_t k = 0; k < 4; ++k) {
    Arcadia_Natural8Value count = k == 0 ? 1 : k == 1 ? 2 : 4;
    Arcadia_Value args[] = {
      Arcadia_Value_makeNatural64Value(Arcadia_Natural64Value_Maximum), Arcadia_Value_makeObjectReferenceValue(source),
      Arcadia_Value_makeInteger32Value(-3), Arcadia_Value_makeInteger32Value(17), Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void)
    };
    expectFailure(thread, types[k], args, count - 1, Arcadia_Status_NumberOfArgumentsInvalid);
    expectFailure(thread, types[k], args, count + 1, Arcadia_Status_NumberOfArgumentsInvalid);
    for (size_t i = 0; i < count; ++i) {
      Arcadia_Value saved = args[i];
      args[i] = Arcadia_Value_makeNatural32Value(1);
      expectFailure(thread, types[k], args, count, Arcadia_Status_ArgumentTypeInvalid);
      if (i != 1) {
        args[i] = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
        expectFailure(thread, types[k], args, count, Arcadia_Status_ArgumentTypeInvalid);
      }
      args[i] = saved;
    }
    if (k) {
      args[1] = Arcadia_Value_makeObjectReferenceValue(source->name);
      expectFailure(thread, types[k], args, count, Arcadia_Status_ArgumentTypeInvalid);
    }
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  destroyed = 0;
  // Each event is the sole GC root of its widget and the widget's managed data.
  for (size_t k = 1; k < 4; ++k) {
    source = (void*)create(thread, _WidgetProbe_getType(thread), NULL, 0);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, source);
    Arcadia_Value args[] = {
      Arcadia_Value_makeNatural64Value(Arcadia_Natural64Value_Maximum), Arcadia_Value_makeObjectReferenceValue(source),
      Arcadia_Value_makeInteger32Value(-3), Arcadia_Value_makeInteger32Value(17)
    };
    Arcadia_Object* event = create(thread, types[k], args, k == 1 ? 2 : 4);
    Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, event);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, destroyed == k - 1 && Arcadia_String_isEqualTo_pn(thread, source->name, "event source payload", sizeof("event source payload") - 1));
    Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, destroyed == k);
  }
  // Public factories box a missing source as void.
  Arcadia_Engine_UI_WidgetPositionChangedEvent* position = Arcadia_Engine_UI_WidgetPositionChangedEvent_create(thread, 1, NULL, -3, 17);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, position);
  Arcadia_Engine_UI_WidgetSizeChangedEvent* size = Arcadia_Engine_UI_WidgetSizeChangedEvent_create(thread, 1, NULL, 17, -3);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, size);
  Arcadia_Tests_assertTrue(thread, !((Arcadia_Engine_UI_WidgetEvent*)position)->source && !((Arcadia_Engine_UI_WidgetEvent*)size)->source);
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

static int
runEventHierarchy
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "EventHierarchy", &runEventHierarchy },
    { "Alignment", &Arcadia_Engine_UI_Tests_alignment },
    { "Node", &Arcadia_Engine_UI_Tests_node },
    { "Populate", &Arcadia_Engine_UI_Tests_EventBases_populate },
    { "Interfaces", &Arcadia_Engine_UI_Tests_EventBases_interfaces },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
