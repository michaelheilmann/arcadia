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
#include <stddef.h>
#include <math.h>
#include <float.h>
#include <stdio.h>

struct SceneABI {
  Arcadia_Object _parent;
  Arcadia_Engine* engine;
  Arcadia_Engine_SceneManager* sceneManager;
  Arcadia_Signal* applicationQuitRequestSignal;
};

typedef void
SceneTickSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );
typedef void
SceneTick
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value tick
  );
typedef void
SceneKey
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );
typedef void
SceneButton
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );
typedef void
ScenePointer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

struct SceneDispatchABI {
  Arcadia_ObjectDispatch _parent;
  SceneTickSize* updateAudials;
  SceneTick* updateLogics;
  SceneTickSize* updateVisuals;
  SceneKey* handleKeyboardKeyEvent;
  SceneButton* handleMouseButtonEvent;
  ScenePointer* handleMousePointerEvent;
};

_Static_assert(sizeof(Arcadia_Engine_Scene) == sizeof(struct SceneABI), "Scene size");
_Static_assert(_Alignof(Arcadia_Engine_Scene) == _Alignof(struct SceneABI), "Scene alignment");
_Static_assert(sizeof(Arcadia_Engine_SceneDispatch) == sizeof(struct SceneDispatchABI), "Scene dispatch size");
_Static_assert(_Alignof(Arcadia_Engine_SceneDispatch) == _Alignof(struct SceneDispatchABI), "Scene dispatch alignment");
#define FIELD(Name, Type) \
  _Static_assert(offsetof(Arcadia_Engine_Scene, Name) == offsetof(struct SceneABI, Name), #Name " offset"); \
  _Static_assert(_Generic(((Arcadia_Engine_Scene*)0)->Name, Type*: 1, default: 0), #Name " type");
FIELD(engine, Arcadia_Engine)
FIELD(sceneManager, Arcadia_Engine_SceneManager)
FIELD(applicationQuitRequestSignal, Arcadia_Signal)
#undef FIELD
#define SLOT(Name, Type) \
  _Static_assert(offsetof(Arcadia_Engine_SceneDispatch, Name) == offsetof(struct SceneDispatchABI, Name), #Name " offset"); \
  _Static_assert(_Generic(((Arcadia_Engine_SceneDispatch*)0)->Name, Type*: 1, default: 0), #Name " slot type"); \
  _Static_assert(_Generic(&Arcadia_Engine_Scene_##Name, Type*: 1, default: 0), #Name " public signature");
SLOT(updateAudials, SceneTickSize)
SLOT(updateLogics, SceneTick)
SLOT(updateVisuals, SceneTickSize)
SLOT(handleKeyboardKeyEvent, SceneKey)
SLOT(handleMouseButtonEvent, SceneButton)
SLOT(handleMousePointerEvent, ScenePointer)
#undef SLOT

enum { Audials, Logics, Visuals, Keyboard, Button, Pointer, OperationCount };
typedef struct SceneStateProbe SceneStateProbe;
typedef struct SceneStateProbeDispatch SceneStateProbeDispatch;

struct SceneStateProbe {
  Arcadia_Engine_Scene _parent;
  Arcadia_SizeValue calls[OperationCount];
  Arcadia_Real64Value tick;
  Arcadia_Integer32Value width, height;
  Arcadia_Engine_Event* event; // Borrowed capture; callers root event arguments.
  Arcadia_BooleanValue fail;
};

struct SceneStateProbeDispatch {
  Arcadia_Engine_SceneDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.SceneStateProbe", SceneStateProbe,
                          u8"Arcadia.Engine.Scene");

static void
constructProbe
  (
    Arcadia_Thread* thread,
    SceneStateProbe* self
  )
{
  Arcadia_EnterConstructor(SceneStateProbe);
  if (_numberOfArguments != 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value engine = Arcadia_ValueStack_getValue(thread, 2);
  Arcadia_Value manager = Arcadia_ValueStack_getValue(thread, 1);
  Arcadia_ValueStack_pushValue(thread, &engine);
  Arcadia_ValueStack_pushValue(thread, &manager);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  Arcadia_superTypeConstructor(thread, _type, self);
  for (unsigned i = 0; i < OperationCount; ++i) self->calls[i] = 0;
  self->tick = 0.; self->width = self->height = 0;
  self->event = NULL; self->fail = false;
  Arcadia_LeaveConstructor(SceneStateProbe);
}

static SceneStateProbe*
record
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* receiver,
    unsigned operation
  )
{
  SceneStateProbe* self = (void*)receiver;
  if (self->fail) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, receiver->engine && receiver->sceneManager && receiver->applicationQuitRequestSignal);
  self->calls[operation]++;
  return self;
}

static void
updateAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  SceneStateProbe* probe = record(thread, self, Audials);
  probe->tick = tick; probe->width = width; probe->height = height;
}

static void
updateLogics
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value tick
  )
{
  record(thread, self, Logics)->tick = tick;
}

static void
updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  SceneStateProbe* probe = record(thread, self, Visuals);
  probe->tick = tick; probe->width = width; probe->height = height;
}

static void
keyboard
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  record(thread, self, Keyboard)->event = (void*)event;
  if (event) Arcadia_Tests_assertTrue(thread, event->key == Arcadia_Engine_Input_KeyboardKey_Return);
}

static void
button
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  record(thread, self, Button)->event = (void*)event;
  if (event) Arcadia_Tests_assertTrue(thread, event->button == UINT64_MAX);
}

static void
pointer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  record(thread, self, Pointer)->event = (void*)event;
  if (event) Arcadia_Tests_assertTrue(thread, event->x == INT32_MIN && event->y == INT32_MAX);
}

static void
initializeProbe
  (
    Arcadia_Thread* thread,
    SceneStateProbeDispatch* self
  )
{
  Arcadia_Engine_SceneDispatch* base = (void*)self;
  base->updateAudials = &updateAudials;
  base->updateLogics = &updateLogics;
  base->updateVisuals = &updateVisuals;
  base->handleKeyboardKeyEvent = &keyboard;
  base->handleMouseButtonEvent = &button;
  base->handleMousePointerEvent = &pointer;
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

Arcadia_defineObjectType(u8"Tests.SceneStateProbe", SceneStateProbe,
                         u8"Arcadia.Engine.Scene", Arcadia_Engine_Scene,
                         &probeOperations);

static Arcadia_Engine_Scene*
createScene
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Engine* engine,
    Arcadia_Engine_SceneManager* manager
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, manager);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  return (void*)_Arcadia_EndCreate0(thread, type, base);
}

static void
call
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Engine_Event* event
  )
{
  switch (operation) {
    case Audials: Arcadia_Engine_Scene_updateAudials(thread, self, tick, INT32_MIN, INT32_MAX); break;
    case Logics: Arcadia_Engine_Scene_updateLogics(thread, self, tick); break;
    case Visuals: Arcadia_Engine_Scene_updateVisuals(thread, self, tick, INT32_MIN, INT32_MAX); break;
    case Keyboard: Arcadia_Engine_Scene_handleKeyboardKeyEvent(thread, self, (void*)event); break;
    case Button: Arcadia_Engine_Scene_handleMouseButtonEvent(thread, self, (void*)event); break;
    case Pointer: Arcadia_Engine_Scene_handleMousePointerEvent(thread, self, (void*)event); break;
  }
}

static Arcadia_SizeValue destroyed[4];
static Arcadia_SizeValue signalCalls;

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
onQuit
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, 0) == 2);
  signalCalls++;
  // Signal_emit owns this callback frame and removes it after we return.
}

static void
testState
  (
    Arcadia_Thread* thread
  )
{
  for (unsigned i = 0; i < 4; ++i) destroyed[i] = 0;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  signalCalls = 0;
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  // A different engine behind the manager prevents its visitor from masking
  // a missing visit of Scene.engine. No public Engine factory is introduced.
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Engine* managerEngine = (void*)_Arcadia_EndCreate0(thread, _Arcadia_Engine_getType(thread), depth);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, managerEngine);
  Arcadia_Engine_SceneManager* manager = Arcadia_Engine_SceneManager_create(thread, managerEngine);
  Arcadia_Value managerValue = Arcadia_Value_makeObjectReferenceValue(manager);
  Arcadia_ValueStack_setValue(thread, 0, &managerValue);
  Arcadia_Engine_Scene* scene = createScene(thread, _Arcadia_Engine_Scene_getType(thread), engine, manager);
  Arcadia_Tests_assertTrue(thread, scene->engine == engine && scene->sceneManager == manager && scene->applicationQuitRequestSignal);
  Arcadia_Value value = Arcadia_Value_makeObjectReferenceValue(scene);
  Arcadia_ValueStack_setValue(thread, 1, &value);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Engine_Scene* another = createScene(thread, _Arcadia_Engine_Scene_getType(thread), engine, manager);
  Arcadia_Tests_assertTrue(thread, another->applicationQuitRequestSignal != scene->applicationQuitRequestSignal);
  Arcadia_Object* objects[] = { (void*)scene, (void*)engine, (void*)manager, (void*)scene->applicationQuitRequestSignal };
  for (unsigned i = 0; i < 4; ++i) Arcadia_Object_addNotifyDestroyCallback(thread, objects[i], &destroyed[i], &onDestroy);
  Arcadia_Signal_connect(thread, scene->applicationQuitRequestSignal, (void*)scene, &onQuit);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Engine_SceneDispatch* dispatch = (void*)Arcadia_ObjectType_getDispatch(_Arcadia_Engine_Scene_getType(thread));
  Arcadia_Tests_assertTrue(thread, dispatch->updateAudials && dispatch->updateLogics && dispatch->updateVisuals && dispatch->handleKeyboardKeyEvent && dispatch->handleMouseButtonEvent && dispatch->handleMousePointerEvent);
  for (unsigned i = 0; i < OperationCount; ++i) call(thread, i, scene, NAN, NULL);
  Arcadia_Tests_assertTrue(thread, signalCalls == 0 && scene->engine == engine && scene->sceneManager == manager);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Signal_emit(thread, scene->applicationQuitRequestSignal, (void*)scene);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, signalCalls == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 1);
  for (unsigned i = 0; i < 4; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 0);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 4; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testDispatch
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  Arcadia_Engine_SceneManager* manager = Arcadia_Engine_SceneManager_create(thread, engine);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, manager);
  SceneStateProbe* probe = (void*)createScene(thread, _SceneStateProbe_getType(thread), engine, manager);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, probe);
  Arcadia_Engine_Event* events[] = {
    (void*)Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, UINT64_MAX, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Return),
    NULL, NULL
  };
  Arcadia_ValueStack_pushObjectReferenceValue(thread, events[0]);
  events[1] = (void*)Arcadia_Engine_Input_MouseButtonEvent_create(thread, 0, Arcadia_Engine_Input_MouseButtonAction_Released, UINT64_MAX, INT32_MAX, INT32_MIN);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, events[1]);
  events[2] = (void*)Arcadia_Engine_Input_MousePointerEvent_create(thread, UINT64_MAX, Arcadia_Engine_Input_MousePointerAction_Moved, INT32_MIN, INT32_MAX);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, events[2]);
  Arcadia_Real64Value ticks[] = { 0., -0., DBL_MAX, -DBL_MAX, INFINITY, -INFINITY, NAN };
  for (unsigned operation = 0; operation < OperationCount; ++operation) {
    for (unsigned i = 0; i < sizeof(ticks) / sizeof(ticks[0]); ++i) {
      call(thread, operation, (void*)probe, ticks[i], operation < Keyboard ? NULL : events[operation - Keyboard]);
      Arcadia_Tests_assertTrue(thread, probe->calls[operation] == i + 1);
      if (operation < Keyboard) {
        Arcadia_Tests_assertTrue(thread, isnan(ticks[i]) ? isnan(probe->tick) : probe->tick == ticks[i] && !!signbit(probe->tick) == !!signbit(ticks[i]));
        if (operation != Logics) Arcadia_Tests_assertTrue(thread, probe->width == INT32_MIN && probe->height == INT32_MAX);
      } else {
        Arcadia_Tests_assertTrue(thread, probe->event == events[operation - Keyboard]);
      }
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 306);
    }
    if (operation >= Keyboard) {
      call(thread, operation, (void*)probe, 0., NULL);
      Arcadia_Tests_assertTrue(thread, probe->event == NULL);
    }
    probe->fail = true;
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      call(thread, operation, (void*)probe, 0., NULL);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 306);
      Arcadia_Thread_setStatus(thread, 0);
    }
    probe->fail = false;
  }
  Arcadia_ValueStack_popValues(thread, 306);
}

static void
testConstructorErrors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  Arcadia_Engine_SceneManager* manager = Arcadia_Engine_SceneManager_create(thread, engine);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, manager);
  Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "not a scene dependency");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
  Arcadia_Value bad[] = {
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), Arcadia_Value_makeBooleanValue(false),
    Arcadia_Value_makeInteger32Value(0), Arcadia_Value_makeNatural64Value(0),
    Arcadia_Value_makeSizeValue(0), Arcadia_Value_makeReal64Value(0.),
    Arcadia_Value_makeObjectReferenceValue(wrong), Arcadia_Value_makeTypeValue(_Arcadia_Engine_getType(thread)),
  };
  for (unsigned parameter = 0; parameter < 2; ++parameter) {
    for (unsigned i = 0; i <= sizeof(bad) / sizeof(bad[0]); ++i) {
      Arcadia_Value arguments[] = { Arcadia_Value_makeObjectReferenceValue(engine), Arcadia_Value_makeObjectReferenceValue(manager) };
      arguments[parameter] = i < sizeof(bad) / sizeof(bad[0]) ? bad[i] : arguments[1 - parameter];
      Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        Arcadia_ValueStack_pushValue(thread, &arguments[0]);
        Arcadia_ValueStack_pushValue(thread, &arguments[1]);
        Arcadia_ValueStack_pushNatural8Value(thread, 2);
        _Arcadia_EndCreate0(thread, _Arcadia_Engine_Scene_getType(thread), depth);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
        Arcadia_Thread_setStatus(thread, 0);
      }
    }
  }
  for (unsigned count = 0; count <= 255; ++count) {
    if (count == 2) continue;
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      for (unsigned i = 0; i < count; ++i) Arcadia_ValueStack_pushBooleanValue(thread, false);
      Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)count);
      _Arcadia_EndCreate0(thread, _Arcadia_Engine_Scene_getType(thread), depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_ValueStack_popValues(thread, 303);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testProductionSafety
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  Arcadia_Engine_SceneManager* manager = Arcadia_Engine_SceneManager_create(thread, engine);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, manager);
  for (unsigned operation = 0; operation < OperationCount; ++operation) {
    Arcadia_Engine_Event* event = NULL;
    switch (operation) {
      case Keyboard: event = (void*)Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, UINT64_MAX, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Return); break;
      case Button: event = (void*)Arcadia_Engine_Input_MouseButtonEvent_create(thread, 0, Arcadia_Engine_Input_MouseButtonAction_Released, UINT64_MAX, INT32_MAX, INT32_MIN); break;
      case Pointer: event = (void*)Arcadia_Engine_Input_MousePointerEvent_create(thread, UINT64_MAX, Arcadia_Engine_Input_MousePointerAction_Moved, INT32_MIN, INT32_MAX); break;
    }
    if (event) Arcadia_ValueStack_pushObjectReferenceValue(thread, event);
    SceneStateProbe* probe = (void*)createScene(thread, _SceneStateProbe_getType(thread), engine, manager);
    if (event) Arcadia_ValueStack_popValues(thread, 1);
    // Neither receiver nor event has a caller root during callback collection.
    call(thread, operation, (void*)probe, -0., event);
    Arcadia_Tests_assertTrue(thread, probe->calls[operation] == 1);
    if (event) Arcadia_Tests_assertTrue(thread, probe->event == event);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 302);
  }
  Arcadia_Engine_Scene* scene = createScene(thread, _Arcadia_Engine_Scene_getType(thread), engine, manager);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, scene);
  Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "wrong receiver or event");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
  for (unsigned operation = 0; operation < OperationCount; ++operation) {
    for (unsigned failure = 0; failure < (operation < Keyboard ? 2u : 3u); ++failure) {
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        call(thread, operation, failure == 0 ? NULL : failure == 1 ? (void*)wrong : scene, 0., failure == 2 ? (void*)wrong : NULL);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 304);
        Arcadia_Thread_setStatus(thread, 0);
      }
    }
  }
  Arcadia_ValueStack_popValues(thread, 304);
}

int
Arcadia_Engine_Tests_Nodes_scene
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testState)) { fprintf(stderr, "Scene state failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testDispatch)) { fprintf(stderr, "Scene dispatch failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testConstructorErrors)) { fprintf(stderr, "Scene constructor errors failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testProductionSafety)) { fprintf(stderr, "Scene production safety failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
