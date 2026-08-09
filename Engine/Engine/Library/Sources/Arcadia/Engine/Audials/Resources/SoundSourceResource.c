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
#include "Arcadia/Engine/Audials/Resources/SoundSourceResource.h"

static void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch* self
  );

static void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  );

static void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Audials_Implementation_SoundSourceResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Audials_Implementation_SoundSourceResource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Audials_Implementation_SoundSourceResource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Audials_Implementation_SoundSourceResource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.Implementation.SoundSourceResource", Arcadia_Engine_Audials_Implementation_SoundSourceResource,
                         u8"Arcadia.Engine.Audials.Implementation.Resource", Arcadia_Engine_Audials_Implementation_Resource,
                         &_typeOperations);

static void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_Implementation_SoundSourceResource);
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value t;
    t = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushValue(thread, &t);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_Implementation_SoundSourceResource);
}

static void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  )
{/*Intentionally empty.*/}

Arcadia_BooleanValue
Arcadia_Engine_Audials_Implementation_SoundSourceResource_isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Audials_Implementation_SoundSourceResource, isPlaying, self); }

void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_pause
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_SoundSourceResource, pause, self); }

void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_play
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_SoundSourceResource, play, self); }

void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_stop
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_SoundSourceResource, stop, self); }

void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_setIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self,
    Arcadia_BooleanValue isLooping
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_SoundSourceResource, setIsLooping, self, isLooping); }

Arcadia_BooleanValue
Arcadia_Engine_Audials_Implementation_SoundSourceResource_getIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Audials_Implementation_SoundSourceResource, getIsLooping, self); }

void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self,
    Arcadia_Real32Value volume
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_SoundSourceResource, setVolume, self, volume); }

Arcadia_Real32Value
Arcadia_Engine_Audials_Implementation_SoundSourceResource_getVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Audials_Implementation_SoundSourceResource, getVolume, self); }
