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

#include "Arcadia/Engine/Tests/Nodes/Cases.h"
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Collections/Include.h"
#include <stddef.h>

// Independent model of the pre-migration header, including all nine fields.
struct EngineABI {
  Arcadia_Object _parent;
  Arcadia_Set* visualsBackendTypes;
  Arcadia_Set* visualsNodeFactoryTypes;
  Arcadia_Set* audialsBackendTypes;
  Arcadia_Set* audialsNodeFactoryTypes;
  Arcadia_Engine_BackendContext* visualsBackendContext;
  Arcadia_Engine_NodeFactory* visualsNodeFactory;
  Arcadia_Engine_BackendContext* audialsBackendContext;
  Arcadia_Engine_NodeFactory* audialsNodeFactory;
  Arcadia_Deque* events;
};

struct EngineDispatchABI {
  Arcadia_ObjectDispatch _parent;
  void (*update)(Arcadia_Thread*, Arcadia_Engine*);
};

_Static_assert(sizeof(Arcadia_Engine) == sizeof(struct EngineABI), "Engine size");
_Static_assert(_Alignof(Arcadia_Engine) == _Alignof(struct EngineABI), "Engine alignment");
_Static_assert(sizeof(Arcadia_EngineDispatch) == sizeof(struct EngineDispatchABI), "Engine dispatch size");
_Static_assert(offsetof(Arcadia_EngineDispatch, update) == offsetof(struct EngineDispatchABI, update), "update slot");
_Static_assert(_Generic(((Arcadia_EngineDispatch*)0)->update, void (*)(Arcadia_Thread*, Arcadia_Engine*): 1, default: 0), "update slot type");
#define FIELD(Name, Type) \
  _Static_assert(offsetof(Arcadia_Engine, Name) == offsetof(struct EngineABI, Name), #Name " offset"); \
  _Static_assert(_Generic(((Arcadia_Engine*)0)->Name, Type: 1, default: 0), #Name " type");
FIELD(visualsBackendTypes, Arcadia_Set*)
FIELD(visualsNodeFactoryTypes, Arcadia_Set*)
FIELD(audialsBackendTypes, Arcadia_Set*)
FIELD(audialsNodeFactoryTypes, Arcadia_Set*)
FIELD(visualsBackendContext, Arcadia_Engine_BackendContext*)
FIELD(visualsNodeFactory, Arcadia_Engine_NodeFactory*)
FIELD(audialsBackendContext, Arcadia_Engine_BackendContext*)
FIELD(audialsNodeFactory, Arcadia_Engine_NodeFactory*)
FIELD(events, Arcadia_Deque*)
#undef FIELD
_Static_assert(_Generic(&Arcadia_Engine_getOrCreate, Arcadia_Engine* (*)(Arcadia_Thread*): 1, default: 0), "singleton signature");
_Static_assert(_Generic(&Arcadia_Engine_enqueEvent, void (*)(Arcadia_Thread*, Arcadia_Engine*, Arcadia_Engine_Event*): 1, default: 0), "enqueue signature");
_Static_assert(_Generic(&Arcadia_Engine_dequeEvent, Arcadia_Engine_Event* (*)(Arcadia_Thread*, Arcadia_Engine*): 1, default: 0), "dequeue signature");
_Static_assert(_Generic(&Arcadia_Engine_update, void (*)(Arcadia_Thread*, Arcadia_Engine*): 1, default: 0), "update signature");

typedef struct EngineStateProbe EngineStateProbe;
typedef struct EngineStateProbeDispatch EngineStateProbeDispatch;

struct EngineStateProbe {
  Arcadia_Engine _parent;
  Arcadia_SizeValue calls;
  Arcadia_BooleanValue fail;
};

struct EngineStateProbeDispatch {
  Arcadia_EngineDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.EngineStateProbe", EngineStateProbe,
                          u8"Arcadia.Engine");

static void
constructProbe
  (
    Arcadia_Thread* thread,
    EngineStateProbe* self
  )
{
  Arcadia_EnterConstructor(EngineStateProbe);
  if (_numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->calls = 0;
  self->fail = false;
  Arcadia_LeaveConstructor(EngineStateProbe);
}

static void
updateProbe
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* receiver
  )
{
  EngineStateProbe* self = (EngineStateProbe*)receiver;
  if (self->fail) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, receiver->events && receiver->visualsBackendTypes);
  self->calls++;
}

static void
initializeProbe
  (
    Arcadia_Thread* thread,
    EngineStateProbeDispatch* self
  )
{
  ((Arcadia_EngineDispatch*)self)->update = &updateProbe;
}

static const Arcadia_ObjectType_Operations probeObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructProbe,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeProbe,
};

static const Arcadia_Type_Operations probeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &probeObjects,
};

Arcadia_defineObjectType(u8"Tests.EngineStateProbe", EngineStateProbe,
                         u8"Arcadia.Engine", Arcadia_Engine,
                         &probeOperations);

typedef struct EngineQueueProbe EngineQueueProbe;
typedef struct EngineQueueProbeDispatch EngineQueueProbeDispatch;

struct EngineQueueProbe {
  Arcadia_Deque _parent;
  Arcadia_Engine_Event* event;
  unsigned failOperation;
};

struct EngineQueueProbeDispatch {
  Arcadia_DequeDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.EngineQueueProbe", EngineQueueProbe,
                          u8"Arcadia.Deque");

static void
constructQueue
  (
    Arcadia_Thread* thread,
    EngineQueueProbe* self
  )
{
  Arcadia_EnterConstructor(EngineQueueProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->event = NULL;
  self->failOperation = 0;
  Arcadia_LeaveConstructor(EngineQueueProbe);
}

static void
visitQueue
  (
    Arcadia_Thread* thread,
    EngineQueueProbe* self
  )
{
  if (self->event) Arcadia_Object_visit(thread, (void*)self->event);
}

static void
checkQueue
  (
    Arcadia_Thread* thread,
    EngineQueueProbe* self,
    unsigned operation
  )
{
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, self->event && self->event->timestamp == UINT64_MAX);
  if (self->failOperation == operation) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_BooleanValue
queueIsEmpty
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  )
{
  checkQueue(thread, (void*)self, 1);
  return false;
}

static Arcadia_Value
queueGetFront
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self
  )
{
  checkQueue(thread, (void*)self, 2);
  return Arcadia_Value_makeObjectReferenceValue(((EngineQueueProbe*)self)->event);
}

static void
queueRemoveFront
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self
  )
{
  checkQueue(thread, (void*)self, 3);
}

static void
queueInsertBack
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self,
    Arcadia_Value value
  )
{
  checkQueue(thread, (void*)self, 4);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getObjectReferenceValue(&value) == (Arcadia_Object*)((EngineQueueProbe*)self)->event);
}

static void
initializeQueue
  (
    Arcadia_Thread* thread,
    EngineQueueProbeDispatch* self
  )
{
  ((Arcadia_CollectionDispatch*)self)->isEmpty = &queueIsEmpty;
  ((Arcadia_DequeDispatch*)self)->getFront = &queueGetFront;
  ((Arcadia_DequeDispatch*)self)->removeFront = &queueRemoveFront;
  ((Arcadia_DequeDispatch*)self)->insertBack = &queueInsertBack;
}

static const Arcadia_ObjectType_Operations queueObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructQueue,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitQueue,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeQueue,
};

static const Arcadia_Type_Operations queueOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &queueObjects,
};

Arcadia_defineObjectType(u8"Tests.EngineQueueProbe", EngineQueueProbe,
                         u8"Arcadia.Deque", Arcadia_Deque,
                         &queueOperations);

static Arcadia_SizeValue destroyed[12];

static void
onDestroy
  (
    void* observer,
    Arcadia_Object* object
  )
{
  (*(Arcadia_SizeValue*)observer)++;
}

static void
observe
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    unsigned index
  )
{
  Arcadia_Object_addNotifyDestroyCallback(thread, object, &destroyed[index], &onDestroy);
}

static Arcadia_Object*
create0
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
checkInitialState
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  )
{
  Arcadia_Set* sets[] = { self->visualsBackendTypes, self->visualsNodeFactoryTypes, self->audialsBackendTypes, self->audialsNodeFactoryTypes };
  for (unsigned i = 0; i < 4; ++i) {
    Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)sets[i], _Arcadia_HashSet_getType(thread)));
    Arcadia_Tests_assertTrue(thread, Arcadia_Collection_isEmpty(thread, (void*)sets[i]));
    for (unsigned j = 0; j < i; ++j) Arcadia_Tests_assertTrue(thread, sets[i] != sets[j]);
  }
  Arcadia_Tests_assertTrue(thread, !self->visualsBackendContext && !self->visualsNodeFactory && !self->audialsBackendContext && !self->audialsNodeFactory);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)self->events, _Arcadia_ArrayDeque_getType(thread)));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_dequeEvent(thread, self));
}

static void
testLifetime
  (
    Arcadia_Thread* thread
  )
{
  for (unsigned i = 0; i < 12; ++i) destroyed[i] = 0;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, UINT64_MAX - i);
  Arcadia_Engine* self = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Tests_assertTrue(thread, self == Arcadia_Engine_getOrCreate(thread));
  checkInitialState(thread, self);
  Arcadia_Tests_assertTrue(thread, !((Arcadia_EngineDispatch*)Arcadia_ObjectType_getDispatch(_Arcadia_Engine_getType(thread)))->update);
  observe(thread, (void*)self, 9);
  Arcadia_Set* sets[] = { self->visualsBackendTypes, self->visualsNodeFactoryTypes, self->audialsBackendTypes, self->audialsNodeFactoryTypes };
  for (unsigned i = 0; i < 4; ++i) {
    observe(thread, (void*)sets[i], i);
    Arcadia_Set_add(thread, sets[i], Arcadia_Value_makeTypeValue(_EngineStateProbe_getType(thread)), NULL);
  }
  self->visualsBackendContext = (void*)create0(thread, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
  observe(thread, (void*)self->visualsBackendContext, 4);
  self->visualsNodeFactory = (void*)create0(thread, _Arcadia_Engine_NodeFactory_getType(thread));
  observe(thread, (void*)self->visualsNodeFactory, 5);
  self->audialsBackendContext = (void*)create0(thread, _Arcadia_Engine_Audials_BackendContext_getType(thread));
  observe(thread, (void*)self->audialsBackendContext, 6);
  self->audialsNodeFactory = (void*)create0(thread, _Arcadia_Engine_NodeFactory_getType(thread));
  observe(thread, (void*)self->audialsNodeFactory, 7);
  observe(thread, (void*)self->events, 8);
  Arcadia_Engine_Input_MousePointerEvent* first = Arcadia_Engine_Input_MousePointerEvent_create(thread, 0, Arcadia_Engine_Input_MousePointerAction_Moved, INT32_MIN, INT32_MAX);
  Arcadia_Engine_enqueEvent(thread, self, (void*)first);
  observe(thread, (void*)first, 10);
  Arcadia_Engine_Input_KeyboardKeyEvent* second = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, UINT64_MAX, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Return);
  Arcadia_Engine_enqueEvent(thread, self, (void*)second);
  observe(thread, (void*)second, 11);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 12; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 0);
  for (unsigned i = 0; i < 4; ++i) Arcadia_Tests_assertTrue(thread, Arcadia_Set_contains(thread, sets[i], Arcadia_Value_makeTypeValue(_EngineStateProbe_getType(thread))));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_dequeEvent(thread, self) == (Arcadia_Engine_Event*)first);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)first)->timestamp == 0);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed[10] == 1 && destroyed[11] == 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_dequeEvent(thread, self) == (Arcadia_Engine_Event*)second);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)second)->timestamp == UINT64_MAX);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_dequeEvent(thread, self));
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 301);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 12; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 1);
  // Destruction resets the weak cache; re-creation has fresh empty state.
  self = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  checkInitialState(thread, self);
  Arcadia_Tests_assertTrue(thread, self == Arcadia_Engine_getOrCreate(thread));
  Arcadia_ValueStack_popValues(thread, 1);
  for (unsigned i = 0; i < 300; ++i) Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural64Value(thread, i) == UINT64_MAX - (299 - i));
  Arcadia_ValueStack_popValues(thread, 300);
}

static void
testDispatchAndArity
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  EngineStateProbe* probe = (void*)create0(thread, _EngineStateProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, probe);
  checkInitialState(thread, (void*)probe);
  Arcadia_Engine_update(thread, (void*)probe);
  Arcadia_Tests_assertTrue(thread, probe->calls == 1);
  probe->fail = true;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_update(thread, (void*)probe);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_Tests_assertTrue(thread, probe->calls == 1 && Arcadia_ValueStack_getSize(thread) == base + 301);
  Arcadia_ValueStack_popValues(thread, 1);
  for (unsigned count = 1; count <= 255; ++count) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget arity;
    Arcadia_Thread_pushJumpTarget(thread, &arity);
    if (Arcadia_JumpTarget_save(&arity)) {
      for (unsigned i = 0; i < count; ++i) Arcadia_ValueStack_pushBooleanValue(thread, true);
      Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)count);
      _Arcadia_EndCreate0(thread, _Arcadia_Engine_getType(thread), depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_ValueStack_popValues(thread, 300);
}

static void
testBadCountTags
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Value tags[] = {
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void),
    Arcadia_Value_makeBooleanValue(false),
    Arcadia_Value_makeInteger32Value(0),
    Arcadia_Value_makeNatural64Value(0),
    Arcadia_Value_makeSizeValue(0),
    Arcadia_Value_makeReal64Value(0.),
  };
  for (unsigned i = 0; i < sizeof(tags) / sizeof(tags[0]); ++i) {
    Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_ValueStack_pushValue(thread, &tags[i]);
      _Arcadia_EndCreate0(thread, _Arcadia_Engine_getType(thread), base);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
}

static void
testQueueErrors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_Engine* self = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  EngineQueueProbe* queue = (void*)create0(thread, _EngineQueueProbe_getType(thread));
  self->events = (void*)queue;
  queue->event = (void*)Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, UINT64_MAX, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Return);
  Arcadia_Engine_enqueEvent(thread, self, queue->event);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_dequeEvent(thread, self) == queue->event);
  for (unsigned operation = 1; operation <= 4; ++operation) {
    queue->failOperation = operation;
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      if (operation == 4) Arcadia_Engine_enqueEvent(thread, self, queue->event);
      else Arcadia_Engine_dequeEvent(thread, self);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 301);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_ValueStack_popValues(thread, 301);
}

static void
testProductionSafety
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  // The generated wrapper must root an otherwise unrooted native receiver
  // while its callback forces collection.
  EngineStateProbe* probe = (void*)create0(thread, _EngineStateProbe_getType(thread));
  Arcadia_Engine_update(thread, (void*)probe);
  Arcadia_Tests_assertTrue(thread, probe->calls == 1);
  Arcadia_Engine* self = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "not an engine or event");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
  for (unsigned operation = 0; operation < 8; ++operation) {
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      switch (operation) {
        case 0: Arcadia_Engine_update(thread, self); break;
        case 1: Arcadia_Engine_update(thread, NULL); break;
        case 2: Arcadia_Engine_update(thread, (void*)wrong); break;
        case 3: Arcadia_Engine_dequeEvent(thread, NULL); break;
        case 4: Arcadia_Engine_dequeEvent(thread, (void*)wrong); break;
        case 5: Arcadia_Engine_enqueEvent(thread, NULL, NULL); break;
        case 6: Arcadia_Engine_enqueEvent(thread, (void*)wrong, NULL); break;
        case 7: Arcadia_Engine_enqueEvent(thread, self, (void*)wrong); break;
      }
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (operation == 0 ? Arcadia_Status_OperationInvalid : Arcadia_Status_ArgumentTypeInvalid));
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 302);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_ValueStack_popValues(thread, 302);
}

int
Arcadia_Engine_Tests_Nodes_engine
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testLifetime)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(testLifetime)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(testBadCountTags)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(testQueueErrors)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(testProductionSafety)) return EXIT_FAILURE;
  return Arcadia_Tests_safeExecute(testDispatchAndArity) ? EXIT_SUCCESS : EXIT_FAILURE;
}
