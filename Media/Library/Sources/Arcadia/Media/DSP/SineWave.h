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

#if !defined(ARCADIA_MEDIA_SAMPLEBUFFEROPERATIONS_SINEWAVE_H_INCLUDED)
#define ARCADIA_MEDIA_SAMPLEBUFFEROPERATIONS_SINEWAVE_H_INCLUDED

#if !defined(ARCADIA_MEDIA_MODULE) || 1 != ARCADIA_MEDIA_MODULE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Media/DSP.h"

Arcadia_declareObjectType(u8"Arcadia.Media.DSP.SineWave", Arcadia_Media_DSP_SineWave,
                          u8"Arcadia.Media.DSP");

struct Arcadia_Media_DSP_SineWaveDispatch {
  Arcadia_Media_DSPDispatch parent;
};

struct Arcadia_Media_DSP_SineWave {
  Arcadia_Media_DSP parent;
  Arcadia_Integer32Value frequency;
  Arcadia_ADL_SineWaveDefinition* definition;
};

Arcadia_Media_DSP_SineWave*
Arcadia_Media_DSP_SineWave_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value frequency
  );

#endif // ARCADIA_MEDIA_SAMPLEBUFFEROPERATIONS_SINEWAVE_H_INCLUDED
