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

#if !defined(ARCADIA_ENGINE_AUDIALS_RESOURCES_STREAMINGSOUNDSOURCERESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_RESOURCES_STREAMINGSOUNDSOURCERESOURCE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Audials/Resources/StreamingSoundSourceResource.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/Audials/Resource.h"

typedef struct Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource;

Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.Implementation.StreamingSoundSourceResource", Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource,
                          u8"Arcadia.Engine.Audials.Implementation.Resource");

struct Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch {
  Arcadia_Engine_Audials_Implementation_ResourceDispatch _parent;

  Arcadia_BooleanValue (*isPlaying)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*);

  void (*play)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*);

  void (*pause)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*);

  void (*stop)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*);

  void (*setVolume)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*, Arcadia_Real32Value);

  void (*update)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*);
};

struct Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource {
  Arcadia_Engine_Audials_Implementation_Resource _parent;
};

Arcadia_BooleanValue
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  );

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_play
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  );

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_pause
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  );

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_stop
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  );

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self,
    Arcadia_Real32Value volume
  );

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  );

#endif // ARCADIA_ENGINE_AUDIALS_RESOURCES_STREAMINGSOUNDSOURCERESOURCE_H_INCLUDED
