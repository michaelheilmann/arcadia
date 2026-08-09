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

#define ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_PRIVATE (1)
#include "Arcadia/Audials/Implementation/OpenAL/BackendContext.h"

#include "Arcadia/Audials/Implementation/OpenAL/Resources/SoundSourceResource.h"
#include <assert.h>

static Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* g_instance = NULL;

// @brief Update resources.
// @remmarks
// If a resource has a reference count of 0
// - unload the resource
// - unlink the resource
// - remove the resource from the list
// @return The number of removed resources
static Arcadia_SizeValue
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_updateResourcesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self,
    Arcadia_BooleanValue force
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_updateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self
  );

static Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource*
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_createSoundSourceResouceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self,
    Arcadia_Media_SampleBuffer* sampleBuffer
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContextDispatch* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.Implementation.OpenAL.BackendContext", Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext,
                         u8"Arcadia.Engine.Audials.BackendContext", Arcadia_Engine_Audials_BackendContext,
                         &_typeOperations);

static Arcadia_SizeValue
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_updateResourcesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self,
    Arcadia_BooleanValue force
  )
{
  Arcadia_SizeValue removed = 0;
  Arcadia_SizeValue removedNow = 0;
  do {
    removedNow = 0;
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext*)self)->resources); i < n;) {
      Arcadia_Engine_Audials_Implementation_Resource* resource = (Arcadia_Engine_Audials_Implementation_Resource*)Arcadia_List_getObjectReferenceValueAt(thread, ((Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext*)self)->resources, i);
      if (!resource->referenceCount || force) {
        Arcadia_Engine_Audials_Implementation_Resource_unload(thread, resource);
        Arcadia_Engine_Audials_Implementation_Resource_unlink(thread, resource);
        Arcadia_List_removeAt(thread, ((Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext*)self)->resources, i, 1);
        n--;
        removedNow++;
      } else {
        i++;
      }
    }
    removed += removedNow;
  } while (removedNow);
  return removed;
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_updateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self
  )
{
  // (1) Update the resources.
  Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_updateResourcesImpl(thread, self, Arcadia_BooleanValue_False);
}

static Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource*
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_createSoundSourceResouceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self,
    Arcadia_Media_SampleBuffer* sampleBuffer
  )
{
  Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource* resource = Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_create(thread, (Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext*)self, sampleBuffer);
  assert(((Arcadia_Engine_Audials_Implementation_Resource*)resource)->referenceCount == 0);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->resources, (Arcadia_Object*)resource);
  return resource;
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  // Create and lock the list of resources.
  self->resources = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)self->resources);

  self->alcDevice = NULL;
  self->alcContext = NULL;

  // Select the default audio device on the system.
  self->alcDevice = alcOpenDevice(NULL);
  if (!self->alcDevice) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  // Create a context on the device.
  self->alcContext = alcCreateContext(self->alcDevice, NULL);
  if (!self->alcContext) {
    alcCloseDevice(self->alcDevice);
    self->alcDevice = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  alcMakeContextCurrent(self->alcContext);
  if (alGetError()) {
    alcCloseDevice(self->alcDevice);
    self->alcDevice = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext);
}

void
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContextDispatch* self
  )
{
  ((Arcadia_Engine_Audials_BackendContextDispatch*)self)->createSoundSourceResource = (Arcadia_Engine_Audials_Implementation_SoundSourceResource * (*)(Arcadia_Thread*, Arcadia_Engine_Audials_BackendContext*,Arcadia_Media_SampleBuffer*)) & Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_createSoundSourceResouceImpl;
  ((Arcadia_Engine_BackendContextDispatch*)self)->update = (void(*)(Arcadia_Thread*, Arcadia_Engine_BackendContext*)) & Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_updateImpl;
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self
  )
{
  // (1) Update resources until no resources are left.
  while (Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_updateResourcesImpl(thread, self, Arcadia_BooleanValue_True)) {
    /*Intentionally empty.*/
  }

  // Unlock the list of resources.
  Arcadia_Object_unlock(thread, (Arcadia_Object*)self->resources);
  self->resources = NULL;

  if (self->alcContext) {
    if (alcGetCurrentContext() == self->alcContext) {
      alcMakeContextCurrent(NULL);
    }
    if (self->alcContext) {
      alcDestroyContext(self->alcContext);
      self->alcContext = NULL;
    }
  }
  if (self->alcDevice) {
    alcCloseDevice(self->alcDevice);
    self->alcDevice = NULL;
  }
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* self
  )
{/*Intentionally empty.*/}

static void
destroyCallback
  (
    void* argument1,
    void* argument2
  )
{
  g_instance = NULL;
}

Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext*
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext);
}

Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext*
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_getOrCreate
  (
    Arcadia_Thread* thread
  )
{
  if (!g_instance) {
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* instance = Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_create(thread);
    Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)instance, NULL, (void (*)(void*, Arcadia_Object*))  &destroyCallback);
    g_instance = instance;
  }
  return g_instance;
}
