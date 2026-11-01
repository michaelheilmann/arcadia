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
#include "Arcadia/DDL/Include.h"
#include <stddef.h>
#include <stdio.h>

// Independent model of the original five fields and five dispatch slots.
struct ApplicationABI {
  Arcadia_Object _parent;
  Arcadia_BooleanValue quitRequested;
  Arcadia_Engine* engine;
  Arcadia_List* windows;
  Arcadia_DDL_Node* configuration;
  Arcadia_String* configurationDirectoryName;
};

typedef void
ApplicationAction
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self
  );
typedef void
ApplicationQuit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self,
    Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent* event
  );
typedef void
ApplicationKey
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );
typedef void
ApplicationWindow
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self,
    Arcadia_Engine_Visuals_WindowClosedEvent* event
  );

struct ApplicationDispatchABI {
  Arcadia_ObjectDispatch _parent;
  ApplicationAction* startup;
  ApplicationAction* shutdown;
  ApplicationQuit* onApplicationQuitRequestedEvent;
  ApplicationKey* onKeyboardKeyEvent;
  ApplicationWindow* onWindowClosedEvent;
};

_Static_assert(sizeof(Arcadia_Engine_Application) == sizeof(struct ApplicationABI), "Application size");
_Static_assert(_Alignof(Arcadia_Engine_Application) == _Alignof(struct ApplicationABI), "Application alignment");
_Static_assert(sizeof(Arcadia_Engine_ApplicationDispatch) == sizeof(struct ApplicationDispatchABI), "Application dispatch size");
_Static_assert(_Alignof(Arcadia_Engine_ApplicationDispatch) == _Alignof(struct ApplicationDispatchABI), "Application dispatch alignment");
#define FIELD(Name, Type) \
  _Static_assert(offsetof(Arcadia_Engine_Application, Name) == offsetof(struct ApplicationABI, Name), #Name " offset"); \
  _Static_assert(_Generic(((Arcadia_Engine_Application*)0)->Name, Type: 1, default: 0), #Name " type");
FIELD(quitRequested, Arcadia_BooleanValue)
FIELD(engine, Arcadia_Engine*)
FIELD(windows, Arcadia_List*)
FIELD(configuration, Arcadia_DDL_Node*)
FIELD(configurationDirectoryName, Arcadia_String*)
#undef FIELD
#define SLOT(Name, Type) \
  _Static_assert(offsetof(Arcadia_Engine_ApplicationDispatch, Name) == offsetof(struct ApplicationDispatchABI, Name), #Name " offset"); \
  _Static_assert(_Generic(((Arcadia_Engine_ApplicationDispatch*)0)->Name, Type*: 1, default: 0), #Name " slot type");
SLOT(startup, ApplicationAction)
SLOT(shutdown, ApplicationAction)
SLOT(onApplicationQuitRequestedEvent, ApplicationQuit)
SLOT(onKeyboardKeyEvent, ApplicationKey)
SLOT(onWindowClosedEvent, ApplicationWindow)
#undef SLOT
_Static_assert(_Generic(&Arcadia_Engine_Application_startup, ApplicationAction*: 1, default: 0), "startup signature");
_Static_assert(_Generic(&Arcadia_Engine_Application_shutdown, ApplicationAction*: 1, default: 0), "shutdown signature");
_Static_assert(_Generic(&Arcadia_Engine_Application_getQuitRequested, Arcadia_BooleanValue (*)(Arcadia_Thread*, Arcadia_Engine_Application*): 1, default: 0), "getter signature");
_Static_assert(_Generic(&Arcadia_Engine_Application_setQuitRequested, void (*)(Arcadia_Thread*, Arcadia_Engine_Application*, Arcadia_BooleanValue): 1, default: 0), "setter signature");
_Static_assert(_Generic(&Arcadia_Engine_Application_setConfigurationDirectoryName, void (*)(Arcadia_Thread*, Arcadia_Engine_Application*, Arcadia_String*): 1, default: 0), "directory setter signature");

enum { Startup, Shutdown, QuitEvent, KeyEvent, WindowEvent, OperationCount };
typedef struct ApplicationStateProbe ApplicationStateProbe;
typedef struct ApplicationStateProbeDispatch ApplicationStateProbeDispatch;

struct ApplicationStateProbe {
  Arcadia_Engine_Application _parent;
  Arcadia_SizeValue calls[OperationCount];
  Arcadia_Engine_Event* event; // Borrowed capture; callers own event roots.
  Arcadia_BooleanValue fail;
};

struct ApplicationStateProbeDispatch {
  Arcadia_Engine_ApplicationDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.ApplicationStateProbe", ApplicationStateProbe,
                          u8"Arcadia.Engine.Application");

static void
constructProbe
  (
    Arcadia_Thread* thread,
    ApplicationStateProbe* self
  )
{
  Arcadia_EnterConstructor(ApplicationStateProbe);
  if (_numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  for (unsigned i = 0; i < OperationCount; ++i) self->calls[i] = 0;
  self->event = NULL;
  self->fail = false;
  Arcadia_LeaveConstructor(ApplicationStateProbe);
}

static ApplicationStateProbe*
record
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* receiver,
    unsigned operation
  )
{
  ApplicationStateProbe* self = (void*)receiver;
  if (self->fail) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, receiver->engine && receiver->windows);
  self->calls[operation]++;
  return self;
}

static void
startup
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self
  )
{
  record(thread, self, Startup);
}

static void
shutdown
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self
  )
{
  record(thread, self, Shutdown);
}

static void
quitEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self,
    Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent* event
  )
{
  record(thread, self, QuitEvent)->event = (void*)event;
  if (event) Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)event)->timestamp == UINT64_MAX);
}

static void
keyEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  record(thread, self, KeyEvent)->event = (void*)event;
  if (event) Arcadia_Tests_assertTrue(thread, event->key == Arcadia_Engine_Input_KeyboardKey_Return);
}

static void
windowEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Application* self,
    Arcadia_Engine_Visuals_WindowClosedEvent* event
  )
{
  record(thread, self, WindowEvent)->event = (void*)event;
  if (event) Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Event*)event)->timestamp == 0);
}

static void
initializeProbe
  (
    Arcadia_Thread* thread,
    ApplicationStateProbeDispatch* self
  )
{
  Arcadia_Engine_ApplicationDispatch* base = (void*)self;
  base->startup = &startup;
  base->shutdown = &shutdown;
  base->onApplicationQuitRequestedEvent = &quitEvent;
  base->onKeyboardKeyEvent = &keyEvent;
  base->onWindowClosedEvent = &windowEvent;
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

Arcadia_defineObjectType(u8"Tests.ApplicationStateProbe", ApplicationStateProbe,
                         u8"Arcadia.Engine.Application", Arcadia_Engine_Application,
                         &probeOperations);

static Arcadia_Engine_Application*
create0
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  return (void*)_Arcadia_EndCreate0(thread, type, base);
}

static void
call
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Engine_Application* self,
    Arcadia_Engine_Event* event
  )
{
  // The original API exported wrappers only for startup/shutdown. Exercise
  // the three original event slots directly in the common comparison cases.
  Arcadia_Engine_ApplicationDispatch* dispatch = (void*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (void*)self));
  switch (operation) {
    case Startup: Arcadia_Engine_Application_startup(thread, self); break;
    case Shutdown: Arcadia_Engine_Application_shutdown(thread, self); break;
    case QuitEvent: dispatch->onApplicationQuitRequestedEvent(thread, self, (void*)event); break;
    case KeyEvent: dispatch->onKeyboardKeyEvent(thread, self, (void*)event); break;
    case WindowEvent: dispatch->onWindowClosedEvent(thread, self, (void*)event); break;
  }
}

static Arcadia_SizeValue destroyed[7];

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
testStateAndLifetime
  (
    Arcadia_Thread* thread
  )
{
  for (unsigned i = 0; i < 7; ++i) destroyed[i] = 0;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_Application* self = create0(thread, _Arcadia_Engine_Application_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_Application_getQuitRequested(thread, self));
  Arcadia_Tests_assertTrue(thread, self->engine == Arcadia_Engine_getOrCreate(thread));
  Arcadia_Tests_assertTrue(thread, !self->configuration && !self->configurationDirectoryName);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)self->windows, _Arcadia_ArrayList_getType(thread)));
  Arcadia_Tests_assertTrue(thread, Arcadia_Collection_isEmpty(thread, (void*)self->windows));
  Arcadia_Engine_ApplicationDispatch* dispatch = (void*)Arcadia_ObjectType_getDispatch(_Arcadia_Engine_Application_getType(thread));
  Arcadia_Tests_assertTrue(thread, !dispatch->startup && !dispatch->shutdown && !dispatch->onApplicationQuitRequestedEvent && !dispatch->onKeyboardKeyEvent && !dispatch->onWindowClosedEvent);
  Arcadia_Engine_Application* other = create0(thread, _Arcadia_Engine_Application_getType(thread));
  Arcadia_Tests_assertTrue(thread, other->engine == self->engine && other->windows != self->windows);
  self->configuration = (void*)Arcadia_DDL_BooleanNode_createBoolean(thread, true);
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "ApplicationTests");
  Arcadia_Engine_Application_setConfigurationDirectoryName(thread, self, name);
  Arcadia_Engine_Visuals_Window* window = Arcadia_Engine_Visuals_Window_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, window);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->windows, window);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Object* objects[] = { (void*)self, (void*)self->engine, (void*)self->windows, (void*)self->configuration, (void*)name, (void*)window, (void*)window->title };
  for (unsigned i = 0; i < 7; ++i) Arcadia_Object_addNotifyDestroyCallback(thread, objects[i], &destroyed[i], &onDestroy);
  for (unsigned value = 0; value < 2; ++value) {
    Arcadia_Engine_Application_setQuitRequested(thread, self, value);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Application_getQuitRequested(thread, self) == value && self->quitRequested == value);
  }
  // Only the application roots its engine, list/window/title, configuration
  // and directory name. These payloads have no redundant cross-field links.
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 7; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_List_getObjectReferenceValueAt(thread, self->windows, 0) == (Arcadia_Object*)window);
  Arcadia_Tests_assertTrue(thread, self->configurationDirectoryName == name);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, name, "ApplicationTests", sizeof("ApplicationTests") - 1));
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 1);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 7; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 1);
}

static void
testDirectoryValidation
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_Engine_Application* self = create0(thread, _Arcadia_Engine_Application_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  const char* names[] = { "App", " ", "../name", "C:/absolute", "\xc3\xa9", "\0" };
  Arcadia_SizeValue lengths[] = { 3, 1, 7, 11, 2, 1 };
  for (unsigned i = 0; i < sizeof(names) / sizeof(names[0]); ++i) {
    Arcadia_String* name = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, names[i], lengths[i])));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
    Arcadia_Engine_Application_setConfigurationDirectoryName(thread, self, name);
    Arcadia_Tests_assertTrue(thread, self->configurationDirectoryName == name);
    Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  }
  Arcadia_String* retained = self->configurationDirectoryName;
  Arcadia_String* empty = Arcadia_String_createFromCxxString(thread, "");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, empty);
  for (unsigned i = 0; i < 2; ++i) {
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Engine_Application_setConfigurationDirectoryName(thread, self, i ? empty : NULL);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Tests_assertTrue(thread, self->configurationDirectoryName == retained);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 302);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_ValueStack_popValues(thread, 302);
}

static void
testDispatch
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  ApplicationStateProbe* self = (void*)create0(thread, _ApplicationStateProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Engine_Event* events[3];
  events[0] = (void*)Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_create(thread, UINT64_MAX);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, events[0]);
  events[1] = (void*)Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, UINT64_MAX, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Return);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, events[1]);
  events[2] = (void*)Arcadia_Engine_Visuals_WindowClosedEvent_create(thread, 0, NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, events[2]);
  for (unsigned operation = 0; operation < OperationCount; ++operation) {
    call(thread, operation, (void*)self, operation < QuitEvent ? NULL : events[operation - QuitEvent]);
    Arcadia_Tests_assertTrue(thread, self->calls[operation] == 1);
    if (operation >= QuitEvent) {
      Arcadia_Tests_assertTrue(thread, self->event == events[operation - QuitEvent]);
      call(thread, operation, (void*)self, NULL);
      Arcadia_Tests_assertTrue(thread, self->event == NULL && self->calls[operation] == 2);
    }
    self->fail = true;
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      call(thread, operation, (void*)self, NULL);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 304);
      Arcadia_Tests_assertTrue(thread, self->calls[operation] == (operation < QuitEvent ? 1 : 2));
      Arcadia_Thread_setStatus(thread, 0);
    }
    self->fail = false;
  }
  Arcadia_ValueStack_popValues(thread, 304);
}

static void
testConstructorErrors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  for (unsigned count = 1; count <= 255; ++count) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      for (unsigned i = 0; i < count; ++i) Arcadia_ValueStack_pushBooleanValue(thread, false);
      Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)count);
      _Arcadia_EndCreate0(thread, _Arcadia_Engine_Application_getType(thread), depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_Value invalid[] = { Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), Arcadia_Value_makeBooleanValue(false), Arcadia_Value_makeInteger32Value(0), Arcadia_Value_makeNatural64Value(0), Arcadia_Value_makeSizeValue(0) };
  for (unsigned i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_ValueStack_pushValue(thread, &invalid[i]);
      _Arcadia_EndCreate0(thread, _Arcadia_Engine_Application_getType(thread), depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

// The three historically slot-only event methods now also have conventional
// generated typed wrappers. This does not add or reorder any dispatch slots.
_Static_assert(_Generic(&Arcadia_Engine_Application_onApplicationQuitRequestedEvent, ApplicationQuit*: 1, default: 0), "quit event wrapper");
_Static_assert(_Generic(&Arcadia_Engine_Application_onKeyboardKeyEvent, ApplicationKey*: 1, default: 0), "key event wrapper");
_Static_assert(_Generic(&Arcadia_Engine_Application_onWindowClosedEvent, ApplicationWindow*: 1, default: 0), "window event wrapper");

static void
callGenerated
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Engine_Application* self,
    Arcadia_Engine_Event* event
  )
{
  switch (operation) {
    case Startup: Arcadia_Engine_Application_startup(thread, self); break;
    case Shutdown: Arcadia_Engine_Application_shutdown(thread, self); break;
    case QuitEvent: Arcadia_Engine_Application_onApplicationQuitRequestedEvent(thread, self, (void*)event); break;
    case KeyEvent: Arcadia_Engine_Application_onKeyboardKeyEvent(thread, self, (void*)event); break;
    case WindowEvent: Arcadia_Engine_Application_onWindowClosedEvent(thread, self, (void*)event); break;
  }
}

static void
testProductionRoots
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  for (unsigned operation = 0; operation < OperationCount; ++operation) {
    ApplicationStateProbe* self = (void*)create0(thread, _ApplicationStateProbe_getType(thread));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
    Arcadia_Engine_Event* event = NULL;
    switch (operation) {
      case QuitEvent: event = (void*)Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_create(thread, UINT64_MAX); break;
      case KeyEvent: event = (void*)Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, UINT64_MAX, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Return); break;
      case WindowEvent: event = (void*)Arcadia_Engine_Visuals_WindowClosedEvent_create(thread, 0, NULL); break;
    }
    Arcadia_ValueStack_popValues(thread, 1);
    // Both pointers are otherwise unrooted during the native callback's GC.
    callGenerated(thread, operation, (void*)self, event);
    Arcadia_Tests_assertTrue(thread, self->calls[operation] == 1);
    if (event) {
      Arcadia_Tests_assertTrue(thread, self->event == event);
      callGenerated(thread, operation, (void*)self, NULL);
      Arcadia_Tests_assertTrue(thread, self->event == NULL && self->calls[operation] == 2);
    }
    Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
    self->fail = true;
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      callGenerated(thread, operation, (void*)self, NULL);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 301);
      Arcadia_Thread_setStatus(thread, 0);
    }
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_ValueStack_popValues(thread, 300);
}

static void
testProductionErrors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_Engine_Application* self = create0(thread, _Arcadia_Engine_Application_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_String* wrongReceiver = Arcadia_String_createFromCxxString(thread, "not an application");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrongReceiver);
  Arcadia_Engine_Visuals_Window* wrongName = Arcadia_Engine_Visuals_Window_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrongName);
  for (unsigned operation = 0; operation < OperationCount; ++operation) {
    for (unsigned failure = 0; failure < 3; ++failure) {
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        callGenerated(thread, operation, failure == 0 ? self : failure == 1 ? NULL : (void*)wrongReceiver, NULL);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (failure == 0 ? Arcadia_Status_OperationInvalid : Arcadia_Status_ArgumentTypeInvalid));
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 303);
        Arcadia_Thread_setStatus(thread, 0);
      }
    }
  }
  for (unsigned failure = 0; failure < 7; ++failure) {
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      switch (failure) {
        case 0: Arcadia_Engine_Application_getQuitRequested(thread, NULL); break;
        case 1: Arcadia_Engine_Application_getQuitRequested(thread, (void*)wrongReceiver); break;
        case 2: Arcadia_Engine_Application_setQuitRequested(thread, NULL, true); break;
        case 3: Arcadia_Engine_Application_setQuitRequested(thread, (void*)wrongReceiver, true); break;
        case 4: Arcadia_Engine_Application_setConfigurationDirectoryName(thread, NULL, wrongReceiver); break;
        case 5: Arcadia_Engine_Application_setConfigurationDirectoryName(thread, (void*)wrongReceiver, wrongReceiver); break;
        case 6: Arcadia_Engine_Application_setConfigurationDirectoryName(thread, self, (void*)wrongName); break;
      }
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 303);
      Arcadia_Tests_assertTrue(thread, !self->quitRequested && !self->configurationDirectoryName);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_ValueStack_popValues(thread, 303);
}

int
Arcadia_Engine_Tests_Nodes_application
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testStateAndLifetime)) { fprintf(stderr, "Application state/lifetime failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testDirectoryValidation)) { fprintf(stderr, "Application directory validation failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testDispatch)) { fprintf(stderr, "Application dispatch failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testConstructorErrors)) { fprintf(stderr, "Application constructor errors failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testProductionRoots)) { fprintf(stderr, "Application production roots failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testProductionErrors)) { fprintf(stderr, "Application production errors failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
