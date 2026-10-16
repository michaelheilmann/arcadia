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

#if !defined(ARCADIA_ENGINE_AUDIALS_BACKENDCONTEXT_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_BACKENDCONTEXT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Audials/BackendContext.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/BackendContext.h"

typedef struct Arcadia_Engine_Audials_AudioStreamProvider Arcadia_Engine_Audials_AudioStreamProvider;
typedef struct Arcadia_Engine_Audials_BackendContext Arcadia_Engine_Audials_BackendContext;
typedef struct Arcadia_Engine_Audials_Implementation_SoundSourceResource Arcadia_Engine_Audials_Implementation_SoundSourceResource;
typedef struct Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource;
typedef struct Arcadia_Media_SampleBuffer Arcadia_Media_SampleBuffer;

Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.BackendContext", Arcadia_Engine_Audials_BackendContext,
                          u8"Arcadia.Engine.BackendContext");

struct Arcadia_Engine_Audials_BackendContextDispatch {
  Arcadia_Engine_BackendContextDispatch _parent;

  Arcadia_Engine_Audials_Implementation_SoundSourceResource* (*createSoundSourceResource)(Arcadia_Thread*, Arcadia_Engine_Audials_BackendContext*, Arcadia_Media_SampleBuffer*);

  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* (*createStreamingSoundSourceResource)(Arcadia_Thread*, Arcadia_Engine_Audials_BackendContext*, Arcadia_Engine_Audials_AudioStreamProvider*);

  void (*setMasterVolume)(Arcadia_Thread*, Arcadia_Engine_Audials_BackendContext*, Arcadia_Real64Value);
};

struct Arcadia_Engine_Audials_BackendContext {
  Arcadia_Engine_BackendContext _parent;
};

Arcadia_Engine_Audials_Implementation_SoundSourceResource*
Arcadia_Engine_Audials_BackendContext_createSoundSourceResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_BackendContext* self,
    Arcadia_Media_SampleBuffer* sampleBuffer
  );

Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*
Arcadia_Engine_Audials_BackendContext_createStreamingSoundSourceResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_BackendContext* self,
    Arcadia_Engine_Audials_AudioStreamProvider* provider
  );

void
Arcadia_Engine_Audials_BackendContext_setMasterVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_BackendContext* self,
    Arcadia_Real64Value volume
  );

#endif // ARCADIA_ENGINE_AUDIALS_BACKENDCONTEXT_H_INCLUDED
