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

#include "Arcadia/Engine/Tests/InputEvents/Cases.h"
#include "Arcadia/Engine/Include.h"

typedef struct MouseProbe MouseProbe;
typedef struct MouseProbeDispatch MouseProbeDispatch;

struct MouseProbe {
  Arcadia_Engine_Input_MousePointerEvent _parent;
  Arcadia_String* label;
};

struct MouseProbeDispatch {
  Arcadia_Engine_Input_MousePointerEventDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.MouseProbe", MouseProbe,
                          u8"Arcadia.Engine.Input.MousePointerEvent");

static Arcadia_SizeValue destroyed;

static void
constructImpl
  (
    Arcadia_Thread* thread,
    MouseProbe* self
  )
{
  Arcadia_EnterConstructor(MouseProbe);
  if (_numberOfArguments != 5) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value args[4];
  for (Arcadia_SizeValue i = 0; i < 4; ++i) args[i] = Arcadia_ValueStack_getValue(thread, 5 - i);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) Arcadia_ValueStack_pushValue(thread, &args[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->label = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(MouseProbe);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    MouseProbe* self
  )
{
  if (self->label) Arcadia_Object_visit(thread, (Arcadia_Object*)self->label);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    MouseProbe* self
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

Arcadia_defineObjectType(u8"Tests.MouseProbe", MouseProbe,
                         u8"Arcadia.Engine.Input.MousePointerEvent", Arcadia_Engine_Input_MousePointerEvent,
                         &typeOperations);

static Arcadia_Object*
create
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Value* arguments,
    Arcadia_Natural8Value count
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_Natural8Value i = 0; i < count; ++i) Arcadia_ValueStack_pushValue(thread, &arguments[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, count);
  return _Arcadia_EndCreate0(thread, type, base);
}

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
    create(thread, type, arguments, count);
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
testFactories
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural64Value boundaries[] = { 0, 1, Arcadia_Natural64Value_Maximum };
  Arcadia_Integer32Value pairs[][2] = {
    { 0, 0 }, { -3, 17 },
    { Arcadia_Integer32Value_Minimum, Arcadia_Integer32Value_Maximum },
    { Arcadia_Integer32Value_Maximum, Arcadia_Integer32Value_Minimum }
  };
  for (size_t t = 0; t < sizeof(boundaries) / sizeof(boundaries[0]); ++t) {
    for (size_t p = 0; p < sizeof(pairs) / sizeof(pairs[0]); ++p) {
      for (Arcadia_Integer32Value action = Arcadia_Engine_Input_MouseButtonAction_Pressed; action <= Arcadia_Engine_Input_MouseButtonAction_Released; ++action) {
        for (size_t b = 0; b < sizeof(boundaries) / sizeof(boundaries[0]); ++b) {
          Arcadia_Engine_Input_MouseButtonEvent* self = Arcadia_Engine_Input_MouseButtonEvent_create(thread, boundaries[t], action, boundaries[b], pairs[p][0], pairs[p][1]);
          Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
          Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
          Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)self)->timestamp == boundaries[t] && self->action == action && self->button == boundaries[b]);
          Arcadia_Tests_assertTrue(thread, self->x == pairs[p][0] && self->y == pairs[p][1]);
          Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Event_getType(thread)));
          Arcadia_ValueStack_popValues(thread, 1);
        }
      }
      for (Arcadia_Integer32Value action = Arcadia_Engine_Input_MousePointerAction_Entered; action <= Arcadia_Engine_Input_MousePointerAction_Moved; ++action) {
        Arcadia_Engine_Input_MousePointerEvent* self = Arcadia_Engine_Input_MousePointerEvent_create(thread, boundaries[t], action, pairs[p][0], pairs[p][1]);
        Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
        Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
        Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)self)->timestamp == boundaries[t] && self->action == action);
        Arcadia_Tests_assertTrue(thread, self->x == pairs[p][0] && self->y == pairs[p][1]);
        Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Event_getType(thread)));
        Arcadia_ValueStack_popValues(thread, 1);
      }
    }
  }
}

static void
testCallbacks
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TypeValue types[] = { _Arcadia_Engine_Input_MouseButtonEvent_getType(thread), _Arcadia_Engine_Input_MousePointerEvent_getType(thread) };
  Arcadia_TypeValue enums[] = { _Arcadia_Engine_Input_MouseButtonAction_getType(thread), _Arcadia_Engine_Input_MousePointerAction_getType(thread) };
  for (size_t kind = 0; kind < 2; ++kind) {
    Arcadia_Natural8Value count = kind ? 4 : 5;
    Arcadia_Value args[6] = {
      Arcadia_Value_makeNatural64Value(Arcadia_Natural64Value_Maximum),
      Arcadia_Value_makeInteger32Value(1),
      Arcadia_Value_makeNatural64Value(Arcadia_Natural64Value_Maximum),
      Arcadia_Value_makeInteger32Value(-3),
      Arcadia_Value_makeInteger32Value(17),
      Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void)
    };
    if (kind) { args[2] = args[3]; args[3] = args[4]; }
    for (unsigned representation = 0; representation < 2; ++representation) {
      args[1] = representation ? Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(enums[kind], 1)) : Arcadia_Value_makeInteger32Value(1);
      Arcadia_Object* object = create(thread, types[kind], args, count);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, object);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)object)->timestamp == Arcadia_Natural64Value_Maximum);
      if (kind) {
        Arcadia_Engine_Input_MousePointerEvent* self = (void*)object;
        Arcadia_Tests_assertTrue(thread, self->action == 1 && self->x == -3 && self->y == 17);
      } else {
        Arcadia_Engine_Input_MouseButtonEvent* self = (void*)object;
        Arcadia_Tests_assertTrue(thread, self->action == 1 && self->button == Arcadia_Natural64Value_Maximum && self->x == -3 && self->y == 17);
      }
      Arcadia_ValueStack_popValues(thread, 1);
    }
    args[1] = Arcadia_Value_makeInteger32Value(1);
    expectFailure(thread, types[kind], args, 0, Arcadia_Status_NumberOfArgumentsInvalid);
    expectFailure(thread, types[kind], args, count - 1, Arcadia_Status_NumberOfArgumentsInvalid);
    expectFailure(thread, types[kind], args, count + 1, Arcadia_Status_NumberOfArgumentsInvalid);
    for (size_t p = 0; p < count; ++p) {
      Arcadia_Value saved = args[p];
      args[p] = Arcadia_Value_makeNatural32Value(1);
      expectFailure(thread, types[kind], args, count, Arcadia_Status_ArgumentTypeInvalid);
      args[p] = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
      expectFailure(thread, types[kind], args, count, Arcadia_Status_ArgumentTypeInvalid);
      args[p] = saved;
    }
    args[1] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(enums[1 - kind], 1));
    expectFailure(thread, types[kind], args, count, Arcadia_Status_ArgumentTypeInvalid);
  }
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  // Exercise stack-depth handling beyond the constructor-count range.
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  testFactories(thread);
  testCallbacks(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
  destroyed = 0;
  Arcadia_ValueStack_pushNatural64Value(thread, Arcadia_Natural64Value_Maximum);
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_Engine_Input_MousePointerAction_Moved);
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_Integer32Value_Minimum);
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_Integer32Value_Maximum);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, Arcadia_String_createFromCxxString(thread, "mouse payload"));
  Arcadia_ValueStack_pushNatural8Value(thread, 5);
  MouseProbe* probe = _Arcadia_EndCreate0(thread, _MouseProbe_getType(thread), base);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, probe);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 0 && ((Arcadia_Engine_Event*)probe)->timestamp == Arcadia_Natural64Value_Maximum);
  Arcadia_Tests_assertTrue(thread, probe->_parent.action == Arcadia_Engine_Input_MousePointerAction_Moved && probe->_parent.x == Arcadia_Integer32Value_Minimum && probe->_parent.y == Arcadia_Integer32Value_Maximum);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, probe->label, "mouse payload", sizeof("mouse payload") - 1));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 1 && Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Engine_Tests_InputEvents_mouseEvents
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
