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

#if !defined(ARCADIA_ENGINE_AUDIALS_PROVIDERS_DSPSTREAMPROVIDER_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_PROVIDERS_DSPSTREAMPROVIDER_H_INCLUDED

#include "Arcadia/Engine/Audials/Providers/AudioStreamProvider.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.DSPStreamProvider", Arcadia_Engine_Audials_DSPStreamProvider,
                          u8"Arcadia.Engine.Audials.AudioStreamProvider");

struct Arcadia_Engine_Audials_DSPStreamProviderDispatch {
  Arcadia_Engine_Audials_AudioStreamProviderDispatch parent;
};

struct Arcadia_Engine_Audials_DSPStreamProvider {
  Arcadia_Engine_Audials_AudioStreamProvider parent;
  Arcadia_Media_DSP* dsp;
  Arcadia_Natural32Value sampleRate;
  Arcadia_SizeValue framesRemaining;
  Arcadia_BooleanValue isInfinite;
};

Arcadia_Engine_Audials_DSPStreamProvider*
Arcadia_Engine_Audials_DSPStreamProvider_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* dsp,
    Arcadia_Natural32Value sampleRate,
    Arcadia_SizeValue numberOfFrames
  );

Arcadia_Engine_Audials_DSPStreamProvider*
Arcadia_Engine_Audials_DSPStreamProvider_createInfinite
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* dsp,
    Arcadia_Natural32Value sampleRate
  );

#endif
