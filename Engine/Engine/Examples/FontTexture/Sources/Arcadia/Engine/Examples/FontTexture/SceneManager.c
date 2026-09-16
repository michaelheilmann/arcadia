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

#include "Arcadia/Engine/Examples/FontTexture/SceneManager.h"

static void
Arcadia_Engine_Demo_SceneManager_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManager* self
  );

static void
Arcadia_Engine_Demo_SceneManager_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManagerDispatch* self
  );

static void
Arcadia_Engine_Demo_SceneManager_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManager* self
  );

static void
Arcadia_Engine_Demo_SceneManager_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManager* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Demo_SceneManager_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Demo_SceneManager_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Demo_SceneManager_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Demo_SceneManager_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Demo.SceneManager", Arcadia_Engine_Demo_SceneManager,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Engine_Demo_SceneManager_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManager* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Demo_SceneManager);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->engine = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_getType(thread));
  self->scene = NULL;
  self->sceneChangedEvent = Arcadia_Signal_create(thread);
  Arcadia_LeaveConstructor(Arcadia_Engine_Demo_SceneManager);
}

static void
Arcadia_Engine_Demo_SceneManager_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManagerDispatch* self
  )
{ }

static void
Arcadia_Engine_Demo_SceneManager_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManager* self
  )
{ }

static void
Arcadia_Engine_Demo_SceneManager_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManager* self
  )
{
  if (self->sceneChangedEvent) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->sceneChangedEvent);
  }
  if (self->engine) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->engine);
  }
  if (self->scene) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->scene);
  }
}

Arcadia_Engine_Demo_Scene*
Arcadia_Engine_Demo_SceneManager_getScene
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManager* self
  )
{ return self->scene; }

void
Arcadia_Engine_Demo_SceneManager_setScene
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManager* self,
    Arcadia_Engine_Demo_Scene* scene
  )
{
  if (self->scene != scene) {
    Arcadia_Object* oldValue = (Arcadia_Object*)self->scene;
    Arcadia_Object* newValue = (Arcadia_Object*)scene;
    self->scene = scene;
    if (oldValue) Arcadia_ValueStack_pushObjectReferenceValue(thread, oldValue); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
    if (newValue) Arcadia_ValueStack_pushObjectReferenceValue(thread, newValue); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_Signal_emit(thread, self->sceneChangedEvent, (Arcadia_Object*)self);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_ValueStack_popValues(thread, 2 + 1);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_ValueStack_popValues(thread, 2 + 1);
      Arcadia_Thread_jump(thread);
    }
  }
}

Arcadia_Engine_Demo_SceneManager*
Arcadia_Engine_Demo_SceneManager_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Demo_SceneManager);
  if (engine) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)engine);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Engine_Demo_SceneManager);
}
