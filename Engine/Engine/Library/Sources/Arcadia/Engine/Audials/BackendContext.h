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
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Engine/BackendContext.h"
#include "Arcadia/Media/Include.h"
typedef struct Arcadia_Engine_Audials_Implementation_SoundSourceResource Arcadia_Engine_Audials_Implementation_SoundSourceResource;

Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.BackendContext", Arcadia_Engine_Audials_BackendContext,
                          u8"Arcadia.Engine.BackendContext");

struct Arcadia_Engine_Audials_BackendContextDispatch {
  Arcadia_Engine_BackendContextDispatch _parent;

  Arcadia_Engine_Audials_Implementation_SoundSourceResource*
  (*createSoundSourceResource)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Audials_BackendContext* self,
      Arcadia_Media_SampleBuffer* sampleBuffer
    );
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

#endif // ARCADIA_ENGINE_AUDIALS_BACKENDCONTEXT_H_INCLUDED
