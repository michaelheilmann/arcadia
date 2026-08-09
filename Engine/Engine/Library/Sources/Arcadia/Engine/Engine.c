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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/Engine.h"

static Arcadia_Engine* g_instance = NULL;

static void
Arcadia_Engine_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  );

static void
Arcadia_Engine_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_EngineDispatch* self
  );

static void
Arcadia_Engine_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  );

static void
Arcadia_Engine_destroyCallback
  (
    void* argument1,
    void* argument2
  );

static Arcadia_Engine*
Arcadia_Engine_create
  (
    Arcadia_Thread* thread
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine", Arcadia_Engine,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Engine_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->visualsBackendTypes = (Arcadia_Set*)Arcadia_HashSet_create(thread);
  self->visualsNodeFactoryTypes = (Arcadia_Set*)Arcadia_HashSet_create(thread);
  self->visualsBackendContext = NULL;
  self->visualsNodeFactory = NULL;

  self->audialsBackendTypes = (Arcadia_Set*)Arcadia_HashSet_create(thread);
  self->audialsNodeFactoryTypes = (Arcadia_Set*)Arcadia_HashSet_create(thread);
  self->audialsBackendContext = NULL;
  self->audialsNodeFactory = NULL;

  self->events = (Arcadia_Deque*)Arcadia_ArrayDeque_create(thread);

  Arcadia_LeaveConstructor(Arcadia_Engine);
}

static void
Arcadia_Engine_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_EngineDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  )
{
  if (self->visualsBackendTypes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->visualsBackendTypes);
  }
  if (self->visualsNodeFactoryTypes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->visualsNodeFactoryTypes);
  }
  if (self->visualsBackendContext) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->visualsBackendContext);
  }
  if (self->visualsNodeFactory) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->visualsNodeFactory);
  }

  if (self->audialsBackendTypes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->audialsBackendTypes);
  }
  if (self->audialsNodeFactoryTypes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->audialsNodeFactoryTypes);
  }
  if (self->audialsBackendContext) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->audialsBackendContext);
  }
  if (self->audialsNodeFactory) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->audialsNodeFactory);
  }

  if (self->events) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->events);
  }
}

static void
Arcadia_Engine_destroyCallback
  (
    void* argument1,
    void* argument2
  )
{
  g_instance = NULL;
}

static Arcadia_Engine*
Arcadia_Engine_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine);
}

void
Arcadia_Engine_enqueEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self,
    Arcadia_Engine_Event* event
  )
{
  Arcadia_Deque_insertBack(thread, self->events, Arcadia_Value_makeObjectReferenceValue(event));
}

Arcadia_Engine_Event*
Arcadia_Engine_dequeEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  )
{
  if (Arcadia_Collection_isEmpty(thread, (Arcadia_Collection*)self->events)) {
    return NULL;
  }
  Arcadia_Value eventValue = Arcadia_Deque_getFront(thread, self->events);
  Arcadia_Deque_removeFront(thread, self->events);
  Arcadia_Engine_Event* event = (Arcadia_Engine_Event*)Arcadia_Value_getObjectReferenceValue(&eventValue);
  return event;
}

void
Arcadia_Engine_update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine, update, self); }

Arcadia_Engine*
Arcadia_Engine_getOrCreate
  (
    Arcadia_Thread* thread
  )
{
  if (!g_instance) {
    Arcadia_Engine* instance = Arcadia_Engine_create(thread);
    Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)instance, NULL, (void (*)(void*, Arcadia_Object*)) &Arcadia_Engine_destroyCallback);
    g_instance = instance;
  }
  return g_instance;
}
