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

#if !defined(ARCADIA_MEDIA_DSP_WHITENOISE_H_INCLUDED)
#define ARCADIA_MEDIA_DSP_WHITENOISE_H_INCLUDED

#if !defined(ARCADIA_MEDIA_MODULE) || 1 != ARCADIA_MEDIA_MODULE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Media/DSP.h"

/// @brief A DSP generating white noise.
/// @todo We use rand() here. We must not use rand() as it is in general not thread safe.
Arcadia_declareObjectType(u8"Arcadia.Media.DSP.WhiteNoise", Arcadia_Media_DSP_WhiteNoise,
                          u8"Arcadia.Media.DSP");

struct Arcadia_Media_DSP_WhiteNoiseDispatch {
  Arcadia_Media_DSPDispatch parent;
};

struct Arcadia_Media_DSP_WhiteNoise {
  Arcadia_Media_DSP parent;
  Arcadia_ADL_WhiteNoiseDefinition* definition;
};

Arcadia_Media_DSP_WhiteNoise*
Arcadia_Media_DSP_WhiteNoise_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_MEDIA_DSP_WHITENOISE_H_INCLUDED
