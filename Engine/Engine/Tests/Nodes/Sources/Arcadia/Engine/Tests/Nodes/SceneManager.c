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
#include <stdio.h>

struct SceneManagerABI {
  Arcadia_Object _parent;
  Arcadia_Engine* engine;
  Arcadia_Engine_Scene* scene;
  Arcadia_Signal* sceneChangedEvent;
};

_Static_assert(sizeof(Arcadia_Engine_SceneManager) == sizeof(struct SceneManagerABI), "manager size");
_Static_assert(_Alignof(Arcadia_Engine_SceneManager) == _Alignof(struct SceneManagerABI), "manager alignment");
_Static_assert(sizeof(Arcadia_Engine_SceneManagerDispatch) == sizeof(Arcadia_ObjectDispatch), "no own slots");
#define FIELD(Name, Type) \
  _Static_assert(offsetof(Arcadia_Engine_SceneManager, Name) == offsetof(struct SceneManagerABI, Name), #Name " offset"); \
  _Static_assert(_Generic(((Arcadia_Engine_SceneManager*)0)->Name, Type*: 1, default: 0), #Name " type");
FIELD(engine, Arcadia_Engine)
FIELD(scene, Arcadia_Engine_Scene)
FIELD(sceneChangedEvent, Arcadia_Signal)
#undef FIELD
_Static_assert(_Generic(&Arcadia_Engine_SceneManager_create, Arcadia_Engine_SceneManager* (*)(Arcadia_Thread*, Arcadia_Engine*): 1, default: 0), "factory signature");
_Static_assert(_Generic(&Arcadia_Engine_SceneManager_getScene, Arcadia_Engine_Scene* (*)(Arcadia_Thread*, Arcadia_Engine_SceneManager*): 1, default: 0), "getter signature");
_Static_assert(_Generic(&Arcadia_Engine_SceneManager_setScene, void (*)(Arcadia_Thread*, Arcadia_Engine_SceneManager*, Arcadia_Engine_Scene*): 1, default: 0), "setter signature");

typedef struct SceneIdentityProbe SceneIdentityProbe;
typedef struct SceneIdentityProbeDispatch SceneIdentityProbeDispatch;

struct SceneIdentityProbe {
  Arcadia_Engine_Scene _parent;
};

struct SceneIdentityProbeDispatch {
  Arcadia_Engine_SceneDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.SceneIdentityProbe", SceneIdentityProbe,
                          u8"Arcadia.Engine.Scene");

typedef struct SceneManagerProbe SceneManagerProbe;
typedef struct SceneManagerProbeDispatch SceneManagerProbeDispatch;

struct SceneManagerProbe {
  Arcadia_Engine_SceneManager _parent;
};

struct SceneManagerProbeDispatch {
  Arcadia_Engine_SceneManagerDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.SceneManagerProbe", SceneManagerProbe,
                          u8"Arcadia.Engine.SceneManager");

static void
constructManagerProbe
  (
    Arcadia_Thread* thread,
    SceneManagerProbe* self
  )
{
  Arcadia_EnterConstructor(SceneManagerProbe);
  if (_numberOfArguments != 1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value engine = Arcadia_ValueStack_getValue(thread, 1);
  Arcadia_ValueStack_pushValue(thread, &engine);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(SceneManagerProbe);
}

static const Arcadia_ObjectType_Operations managerObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructManagerProbe,
};

static const Arcadia_Type_Operations managerOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &managerObjects,
};

Arcadia_defineObjectType(u8"Tests.SceneManagerProbe", SceneManagerProbe,
                         u8"Arcadia.Engine.SceneManager", Arcadia_Engine_SceneManager,
                         &managerOperations);

static Arcadia_SizeValue comparisons;

static void
constructIdentityProbe
  (
    Arcadia_Thread* thread,
    SceneIdentityProbe* self
  )
{
  Arcadia_EnterConstructor(SceneIdentityProbe);
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
  Arcadia_LeaveConstructor(SceneIdentityProbe);
}

static void
alwaysEqual
  (
    Arcadia_Thread* thread
  )
{
  comparisons++;
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, 0) == 2);
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_ValueStack_pushBooleanValue(thread, true);
}

static void
initializeIdentityProbe
  (
    Arcadia_Thread* thread,
    SceneIdentityProbeDispatch* self
  )
{
  ((Arcadia_ObjectDispatch*)self)->isEqualTo = &alwaysEqual;
  ((Arcadia_ObjectDispatch*)self)->isIdenticalTo = &alwaysEqual;
}

static const Arcadia_ObjectType_Operations identityObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructIdentityProbe,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeIdentityProbe,
};

static const Arcadia_Type_Operations identityOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &identityObjects,
};

Arcadia_defineObjectType(u8"Tests.SceneIdentityProbe", SceneIdentityProbe,
                         u8"Arcadia.Engine.Scene", Arcadia_Engine_Scene,
                         &identityOperations);

static Arcadia_Engine_Scene*
createScene
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_SceneManager* manager
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, manager);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  return (void*)_Arcadia_EndCreate0(thread, _SceneIdentityProbe_getType(thread), base);
}

static Arcadia_Engine*
createEngine
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  return (void*)_Arcadia_EndCreate0(thread, _Arcadia_Engine_getType(thread), base);
}

static Arcadia_Engine_SceneManager* observedManager;
static Arcadia_Engine_Scene* expectedOld[8];
static Arcadia_Engine_Scene* expectedNew[8];
static unsigned expectedCount, notifications;
static Arcadia_BooleanValue failNotification, reenter;
static Arcadia_Engine_Scene* reentrantScene;

static Arcadia_Engine_Scene*
sceneArgument
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue offset
  )
{
  if (Arcadia_ValueStack_isVoidValue(thread, offset)) return NULL;
  return (void*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, offset, _Arcadia_Engine_Scene_getType(thread));
}

static void
onSceneChanged
  (
    Arcadia_Thread* thread
  )
{
  // Signal_emit owns cleanup. The frame is receiver, sender, old, new, count.
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, 0) == 4);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _Arcadia_Engine_SceneManager_getType(thread)) == (Arcadia_Object*)observedManager);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_Engine_SceneManager_getType(thread)) == (Arcadia_Object*)observedManager);
  Arcadia_Engine_Scene* oldScene = sceneArgument(thread, 2);
  Arcadia_Engine_Scene* newScene = sceneArgument(thread, 1);
  unsigned index = notifications++;
  Arcadia_Tests_assertTrue(thread, index < expectedCount && oldScene == expectedOld[index] && newScene == expectedNew[index]);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_SceneManager_getScene(thread, observedManager) == newScene);
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  if (oldScene) Arcadia_Tests_assertTrue(thread, oldScene->engine && oldScene->applicationQuitRequestSignal);
  if (newScene) Arcadia_Tests_assertTrue(thread, newScene->engine && newScene->applicationQuitRequestSignal);
  if (reenter) {
    reenter = false;
    Arcadia_Engine_SceneManager_setScene(thread, observedManager, reentrantScene);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_SceneManager_getScene(thread, observedManager) == reentrantScene);
    // The outer notification retains its original payload across recursion.
    Arcadia_Tests_assertTrue(thread, sceneArgument(thread, 2) == oldScene && sceneArgument(thread, 1) == newScene);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
  if (failNotification) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
expectTransition
  (
    Arcadia_Engine_Scene* oldScene,
    Arcadia_Engine_Scene* newScene
  )
{
  expectedOld[expectedCount] = oldScene;
  expectedNew[expectedCount++] = newScene;
}

static void
testTransitions
  (
    Arcadia_Thread* thread
  )
{
  comparisons = 0; notifications = expectedCount = 0;
  failNotification = reenter = false;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  Arcadia_Engine_SceneManager* self = (void*)_Arcadia_EndCreate0(thread, _SceneManagerProbe_getType(thread), argumentsBase);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  observedManager = self;
  Arcadia_Tests_assertTrue(thread, self->engine == engine && !self->scene && self->sceneChangedEvent);
  Arcadia_Engine_SceneManager* other = Arcadia_Engine_SceneManager_create(thread, engine);
  Arcadia_Tests_assertTrue(thread, other->sceneChangedEvent != self->sceneChangedEvent);
  Arcadia_Engine_Scene* a = createScene(thread, engine, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, a);
  Arcadia_Engine_Scene* b = createScene(thread, engine, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, b);
  Arcadia_Engine_Scene* c = createScene(thread, engine, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, c);
  Arcadia_Value av = Arcadia_Value_makeObjectReferenceValue(a), bv = Arcadia_Value_makeObjectReferenceValue(b);
  Arcadia_Tests_assertTrue(thread, a != b && Arcadia_Value_isEqualTo(thread, &av, &bv) && comparisons == 1);
  comparisons = 0;
  Arcadia_Signal_connect(thread, self->sceneChangedEvent, (void*)self, &onSceneChanged);
  Arcadia_Engine_SceneManager_setScene(thread, self, NULL);
  Arcadia_Tests_assertTrue(thread, notifications == 0);
  expectTransition(NULL, a);
  Arcadia_Engine_SceneManager_setScene(thread, self, a);
  Arcadia_Engine_SceneManager_setScene(thread, self, a);
  Arcadia_Tests_assertTrue(thread, notifications == 1);
  expectTransition(a, b);
  Arcadia_Engine_SceneManager_setScene(thread, self, b);
  expectTransition(b, NULL);
  Arcadia_Engine_SceneManager_setScene(thread, self, NULL);
  // Reentrancy: outer NULL->a notification causes a nested a->c change.
  expectTransition(NULL, a);
  expectTransition(a, c);
  reenter = true; reentrantScene = c;
  Arcadia_Engine_SceneManager_setScene(thread, self, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_SceneManager_getScene(thread, self) == c && notifications == 5);
  // Fail after publishing c->b. The new state survives; all frames unwind.
  expectTransition(c, b);
  failNotification = true;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_SceneManager_setScene(thread, self, b);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
    Arcadia_Thread_setStatus(thread, 0);
  }
  failNotification = false;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_SceneManager_getScene(thread, self) == b && notifications == 6);
  Arcadia_Engine_SceneManager_setScene(thread, self, b);
  Arcadia_Tests_assertTrue(thread, notifications == 6);
  expectTransition(b, NULL);
  Arcadia_Engine_SceneManager_setScene(thread, self, NULL);
  Arcadia_Tests_assertTrue(thread, notifications == expectedCount && comparisons == 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 305);
  Arcadia_ValueStack_popValues(thread, 305);
}

static Arcadia_SizeValue destroyed[6];

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
testLifetime
  (
    Arcadia_Thread* thread
  )
{
  notifications = expectedCount = 0;
  failNotification = reenter = false;
  for (unsigned i = 0; i < 6; ++i) destroyed[i] = 0;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  Arcadia_Engine_SceneManager* self = Arcadia_Engine_SceneManager_create(thread, engine);
  Arcadia_Value selfValue = Arcadia_Value_makeObjectReferenceValue(self);
  Arcadia_ValueStack_setValue(thread, 0, &selfValue);
  observedManager = self;
  // A separate engine behind the scenes prevents their visitors from masking
  // a missing visit of SceneManager.engine.
  Arcadia_Engine* sceneEngine = createEngine(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, sceneEngine);
  Arcadia_Engine_Scene* a = createScene(thread, sceneEngine, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, a);
  Arcadia_Engine_Scene* b = createScene(thread, sceneEngine, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, b);
  Arcadia_Object* objects[] = { (void*)self, (void*)engine, (void*)self->sceneChangedEvent, (void*)a, (void*)b, (void*)sceneEngine };
  for (unsigned i = 0; i < 6; ++i) Arcadia_Object_addNotifyDestroyCallback(thread, objects[i], &destroyed[i], &onDestroy);
  Arcadia_Engine_SceneManager_setScene(thread, self, a);
  Arcadia_Signal_connect(thread, self->sceneChangedEvent, (void*)self, &onSceneChanged);
  // Remove all scene/scene-engine caller roots. a is now retained only by the
  // manager; b will be passed immediately to the setter as a C local.
  Arcadia_ValueStack_popValues(thread, 3);
  expectTransition(a, b);
  Arcadia_Engine_SceneManager_setScene(thread, self, b);
  for (unsigned i = 0; i < 6; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 0);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed[3] == 1 && destroyed[4] == 0);
  expectTransition(b, NULL);
  Arcadia_Engine_SceneManager_setScene(thread, self, NULL);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed[4] == 1 && destroyed[5] == 1 && destroyed[0] == 0 && destroyed[1] == 0 && destroyed[2] == 0);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 6; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testConstructorErrors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_String* wrongObject = Arcadia_String_createFromCxxString(thread, "not an engine");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrongObject);
  Arcadia_Value bad[] = {
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), Arcadia_Value_makeBooleanValue(false),
    Arcadia_Value_makeInteger8Value(0), Arcadia_Value_makeInteger16Value(0), Arcadia_Value_makeInteger64Value(0),
    Arcadia_Value_makeNatural8Value(0), Arcadia_Value_makeNatural16Value(0), Arcadia_Value_makeNatural32Value(0),
    Arcadia_Value_makeReal32Value(0.f), Arcadia_Value_makeObjectReferenceValue(wrongObject),
    Arcadia_Value_makeInteger32Value(0), Arcadia_Value_makeNatural64Value(0),
    Arcadia_Value_makeSizeValue(0), Arcadia_Value_makeReal64Value(0.),
  };
  for (unsigned i = 0; i < sizeof(bad) / sizeof(bad[0]); ++i) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_ValueStack_pushValue(thread, &bad[i]);
      Arcadia_ValueStack_pushNatural8Value(thread, 1);
      _Arcadia_EndCreate0(thread, _Arcadia_Engine_SceneManager_getType(thread), depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  for (unsigned count = 0; count <= 255; ++count) {
    if (count == 1) continue;
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      for (unsigned i = 0; i < count; ++i) Arcadia_ValueStack_pushBooleanValue(thread, false);
      Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)count);
      _Arcadia_EndCreate0(thread, _Arcadia_Engine_SceneManager_getType(thread), depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_JumpTarget nullInput;
  Arcadia_Thread_pushJumpTarget(thread, &nullInput);
  if (Arcadia_JumpTarget_save(&nullInput)) {
    Arcadia_Engine_SceneManager_create(thread, NULL);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 301);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_ValueStack_popValues(thread, 301);
}

static void
testWrapperSafety
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  Arcadia_Engine_SceneManager* self = Arcadia_Engine_SceneManager_create(thread, engine);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  for (unsigned operation = 0; operation < 5; ++operation) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      switch (operation) {
        case 0: Arcadia_Engine_SceneManager_getScene(thread, NULL); break;
        case 1: Arcadia_Engine_SceneManager_getScene(thread, (void*)engine); break;
        case 2: Arcadia_Engine_SceneManager_setScene(thread, NULL, NULL); break;
        case 3: Arcadia_Engine_SceneManager_setScene(thread, (void*)engine, NULL); break;
        case 4: Arcadia_Engine_SceneManager_setScene(thread, self, (void*)engine); break;
      }
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth && !self->scene);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  // The new scene belongs to another manager, so tracing the notification's
  // scene arguments cannot accidentally keep the receiver alive.
  Arcadia_Engine_SceneManager* other = Arcadia_Engine_SceneManager_create(thread, engine);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, other);
  Arcadia_Engine_Scene* scene = createScene(thread, engine, other);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, scene);
  destroyed[0] = 0;
  Arcadia_Object_addNotifyDestroyCallback(thread, (void*)self, &destroyed[0], &onDestroy);
  observedManager = self;
  expectedCount = notifications = 0;
  reenter = failNotification = false;
  expectTransition(NULL, scene);
  Arcadia_Signal_connect(thread, self->sceneChangedEvent, (void*)self, &onSceneChanged);
  Arcadia_ValueStack_popValues(thread, 4);
  Arcadia_Engine_SceneManager_setScene(thread, self, scene);
  Arcadia_Tests_assertTrue(thread, destroyed[0] == 0 && notifications == 1 && self->scene == scene);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed[0] == 1);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_Nodes_sceneManager
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testTransitions)) { fprintf(stderr, "SceneManager transitions failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testLifetime)) { fprintf(stderr, "SceneManager lifetime failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testConstructorErrors)) { fprintf(stderr, "SceneManager constructor errors failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testWrapperSafety)) { fprintf(stderr, "SceneManager wrapper safety failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
