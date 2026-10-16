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
#include "Arcadia/Engine/Audials/Providers/AudioStreamProvider.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProviderDispatch* self
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

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.AudioStreamProvider", Arcadia_Engine_Audials_AudioStreamProvider,
                         u8"Arcadia.Object", Arcadia_Object, &typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_AudioStreamProvider);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_AudioStreamProvider);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProviderDispatch* self
  )
{
}

Arcadia_BooleanValue
Arcadia_Engine_Audials_AudioStreamProvider_read
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self,
    Arcadia_Media_DSP_Buffer* target,
    Arcadia_SizeValue* numberOfFramesWritten
  )
{
  Arcadia_VirtualCallWithReturn(Arcadia_Engine_Audials_AudioStreamProvider, read, self, target, numberOfFramesWritten);
}

Arcadia_Natural32Value
Arcadia_Engine_Audials_AudioStreamProvider_getSampleRate
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self
  )
{
  return self->sampleRate;
}
