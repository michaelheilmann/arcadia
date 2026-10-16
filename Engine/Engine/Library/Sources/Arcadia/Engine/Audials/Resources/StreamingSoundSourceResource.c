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
#include "Arcadia/Engine/Audials/Resources/StreamingSoundSourceResource.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch* self
  );

static const Arcadia_ObjectType_Operations objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.Implementation.StreamingSoundSourceResource", Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource,
                         u8"Arcadia.Engine.Audials.Implementation.Resource", Arcadia_Engine_Audials_Implementation_Resource,
                         &typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource);
  {
    Arcadia_Value value = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushValue(thread, &value);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch* self
  )
{
}

Arcadia_BooleanValue
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  )
{
  Arcadia_VirtualCallWithReturn(Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource, isPlaying, self);
}

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_play
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  )
{
  Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource, play, self);
}

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_pause
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  )
{
  Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource, pause, self);
}

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_stop
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  )
{
  Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource, stop, self);
}

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self,
    Arcadia_Real32Value volume
  )
{
  Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource, setVolume, self, volume);
}

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  )
{
  Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource, update, self);
}
