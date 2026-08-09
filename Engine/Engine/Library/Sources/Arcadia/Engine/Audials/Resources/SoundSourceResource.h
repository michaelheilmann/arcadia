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

#if !defined(ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_RESOURCES_SOUNDSOURCERESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_RESOURCES_SOUNDSOURCERESOURCE_H_INCLUDED

#include "Arcadia/Engine/Audials/Resource.h"
#include "Arcadia/Math/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.Implementation.SoundSourceResource", Arcadia_Engine_Audials_Implementation_SoundSourceResource,
                          u8"Arcadia.Engine.Audials.Implementation.Resource")

struct Arcadia_Engine_Audials_Implementation_SoundSourceResourceDispatch {
  Arcadia_Engine_Audials_Implementation_ResourceDispatch _parent;

  Arcadia_BooleanValue(*isPlaying)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*);
  void (*pause)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*);
  void (*play)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*);
  void (*stop)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*);
  void (*setIsLooping)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*, Arcadia_BooleanValue);
  Arcadia_BooleanValue (*getIsLooping)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*);
  void (*setVolume)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*, Arcadia_Real32Value);
  Arcadia_Real32Value(*getVolume)(Arcadia_Thread*, Arcadia_Engine_Audials_Implementation_SoundSourceResource*);
};

struct Arcadia_Engine_Audials_Implementation_SoundSourceResource {
  Arcadia_Engine_Audials_Implementation_Resource _parent;
};

Arcadia_BooleanValue
Arcadia_Engine_Audials_Implementation_SoundSourceResource_isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  );

void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_pause
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  );

void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_play
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  );

void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_stop
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  );

void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_setIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self,
    Arcadia_BooleanValue isLooping
  );

Arcadia_BooleanValue
Arcadia_Engine_Audials_Implementation_SoundSourceResource_getIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  );

void
Arcadia_Engine_Audials_Implementation_SoundSourceResource_setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self,
    Arcadia_Real32Value volume
  );

Arcadia_Real32Value
Arcadia_Engine_Audials_Implementation_SoundSourceResource_getVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* self
  );

#endif // ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_RESOURCES_SOUNDSOURCERESOURCE_H_INCLUDED
