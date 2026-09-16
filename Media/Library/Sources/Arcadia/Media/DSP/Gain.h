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

#if !defined(ARCADIA_MEDIA_DSP_GAIN_H_INCLUDED)
#define ARCADIA_MEDIA_DSP_GAIN_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/DSP.h"

/// @brief A unary DSP node multiplying every input sample by a scalar gain.
Arcadia_declareObjectType(u8"Arcadia.Media.DSP.Gain", Arcadia_Media_DSP_Gain,
                          u8"Arcadia.Media.DSP");

struct Arcadia_Media_DSP_GainDispatch {
  Arcadia_Media_DSPDispatch parent;
};

struct Arcadia_Media_DSP_Gain {
  Arcadia_Media_DSP parent;
  Arcadia_Media_DSP* input;
  Arcadia_Real32Value gain;
};

/// @brief Create a gain node.
/// @param thread A pointer to this thread.
/// @param input The input DSP. The node keeps this object reachable through its visit callback.
/// @param gain The linear gain multiplier applied to each sample.
/// @return The created gain node.
Arcadia_Media_DSP_Gain*
Arcadia_Media_DSP_Gain_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* input,
    Arcadia_Real32Value gain
  );

#endif // ARCADIA_MEDIA_DSP_GAIN_H_INCLUDED
