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

#if !defined(ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_OPENAL_RESOURCES_STREAMINGSOUNDSOURCERESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_OPENAL_RESOURCES_STREAMINGSOUNDSOURCERESOURCE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Audials/Implementation/OpenAL/BackendContext.h"

#define AL_LIBTYPE_STATIC
#include <AL/al.h>

#define ARCADIA_OPENAL_STREAM_BUFFER_COUNT (4)
#define ARCADIA_OPENAL_STREAM_CHUNK_FRAMES (2048)

Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.Implementation.OpenAL.StreamingSoundSourceResource", Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource,
                          u8"Arcadia.Engine.Audials.Implementation.StreamingSoundSourceResource");

struct Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResourceDispatch {
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch parent;
};

struct Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource {
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource parent;
  Arcadia_Engine_Audials_AudioStreamProvider* provider;
  Arcadia_Media_DSP_Buffer* renderBuffer;
  Arcadia_Integer16Value* pcmBuffer;
  Arcadia_Natural32Value sampleRate;
  Arcadia_BooleanValue endOfStream;
  Arcadia_BooleanValue stopped;
  Arcadia_Real32Value volume;
  ALuint alSourceID;
  ALuint alBufferIDs[ARCADIA_OPENAL_STREAM_BUFFER_COUNT];
};

Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource*
Arcadia_Engine_Audials_Implementation_OpenAL_StreamingSoundSourceResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext* backendContext,
    Arcadia_Engine_Audials_AudioStreamProvider* provider
  );

#endif
