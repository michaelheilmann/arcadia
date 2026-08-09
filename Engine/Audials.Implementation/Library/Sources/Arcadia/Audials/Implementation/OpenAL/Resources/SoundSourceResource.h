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

#if !defined(ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_OPENAL_RESOURCES_SOUNDSOURCERESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_OPENAL_RESOURCES_SOUNDSOURCERESOURCE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
typedef struct Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext;

#define AL_LIBTYPE_STATIC
#include <AL/al.h>

Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.Implementation.OpenAL.SoundSourceResource", Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource,
                          u8"Arcadia.Engine.Audials.Implementation.SoundSourceResource");

struct Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResourceDispatch {
  Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch _parent;
};

struct Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource {
  Arcadia_Engine_Audials_Implementation_SoundSourceResource _parent;
  Arcadia_Media_SampleBuffer* sampleBuffer;
  Arcadia_BooleanValue isLooping;
  Arcadia_Real32Value volume;
  Arcadia_Natural8Value dirtyBits;
  Arcadia_ByteArrayBuilder* byteBuffer;
  ALuint alBufferID;
  ALuint alSourceID;
};

Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource*
Arcadia_Engine_Audials_Implementation_OpenAL_SoundSourceResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* backendContext,
    Arcadia_Media_SampleBuffer* sampleBuffer
  );

#endif // ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_OPENAL_RESOURCES_SOUNDSOURCERESOURCE_H_INCLUDED
