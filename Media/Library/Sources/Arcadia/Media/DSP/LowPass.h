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

#if !defined(ARCADIA_MEDIA_DSP_LOWPASS_H_INCLUDED)
#define ARCADIA_MEDIA_DSP_LOWPASS_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/DSP.h"

/// @brief A unary one-pole low-pass filter DSP node.
///
/// The filter attenuates high-frequency content from its input. Its filter state
/// is preserved across render calls, making it suitable for chunked rendering as
/// well as offline rendering.
Arcadia_declareObjectType(u8"Arcadia.Media.DSP.LowPass", Arcadia_Media_DSP_LowPass,
                          u8"Arcadia.Media.DSP");

struct Arcadia_Media_DSP_LowPassDispatch {
  Arcadia_Media_DSPDispatch parent;
};

struct Arcadia_Media_DSP_LowPass {
  Arcadia_Media_DSP parent;
  Arcadia_Media_DSP* input;
  Arcadia_Real32Value cutoff;
  Arcadia_Real32Value state;
};

/// @brief Create a low-pass filter node.
/// @param thread A pointer to this thread.
/// @param input The input DSP. The node keeps this object reachable through its visit callback.
/// @param cutoff The cutoff frequency in Hz. Must be greater than zero.
/// @return The created low-pass node.
Arcadia_Media_DSP_LowPass*
Arcadia_Media_DSP_LowPass_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* input,
    Arcadia_Real32Value cutoff
  );

#endif // ARCADIA_MEDIA_DSP_LOWPASS_H_INCLUDED
