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

#if !defined(ARCADIA_ENGINE_AUDIALS_PROVIDERS_AUDIOSTREAMPROVIDER_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_PROVIDERS_AUDIOSTREAMPROVIDER_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Audials/Providers/AudioStreamProvider.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_Engine_Audials_AudioStreamProvider Arcadia_Engine_Audials_AudioStreamProvider;
typedef struct Arcadia_Media_DSP_Buffer Arcadia_Media_DSP_Buffer;

Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.AudioStreamProvider", Arcadia_Engine_Audials_AudioStreamProvider,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Audials_AudioStreamProviderDispatch {
  Arcadia_ObjectDispatch _parent;

  Arcadia_BooleanValue (*read)(Arcadia_Thread*, Arcadia_Engine_Audials_AudioStreamProvider*, Arcadia_Media_DSP_Buffer*, Arcadia_SizeValue*);
};

struct Arcadia_Engine_Audials_AudioStreamProvider {
  Arcadia_Object _parent;
  Arcadia_Natural32Value sampleRate;
};

Arcadia_BooleanValue
Arcadia_Engine_Audials_AudioStreamProvider_read
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self,
    Arcadia_Media_DSP_Buffer* target,
    Arcadia_SizeValue* numberOfFramesWritten
  );

Arcadia_Natural32Value
Arcadia_Engine_Audials_AudioStreamProvider_getSampleRate
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self
  );

#endif // ARCADIA_ENGINE_AUDIALS_PROVIDERS_AUDIOSTREAMPROVIDER_H_INCLUDED
