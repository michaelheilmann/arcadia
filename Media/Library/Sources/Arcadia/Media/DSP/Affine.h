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

#if !defined(ARCADIA_MEDIA_DSP_AFFINE_H_INCLUDED)
#define ARCADIA_MEDIA_DSP_AFFINE_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/DSP.h"

/// @brief A unary DSP node applying output = input * scale + bias.
///
/// This is useful for converting oscillator outputs into control signals, for
/// example mapping a sine wave in [-1, +1] into a gain range.
Arcadia_declareObjectType(u8"Arcadia.Media.DSP.Affine", Arcadia_Media_DSP_Affine,
                          u8"Arcadia.Media.DSP");

struct Arcadia_Media_DSP_AffineDispatch {
  Arcadia_Media_DSPDispatch parent;
};

struct Arcadia_Media_DSP_Affine {
  Arcadia_Media_DSP parent;
  Arcadia_Media_DSP* input;
  Arcadia_Real32Value scale;
  Arcadia_Real32Value bias;
};

/// @brief Create an affine transform node.
/// @param thread A pointer to this thread.
/// @param input The input DSP. The node keeps this object reachable through its visit callback.
/// @param scale The multiplier applied to each input sample.
/// @param bias The value added after scaling each input sample.
/// @return The created affine node.
Arcadia_Media_DSP_Affine*
Arcadia_Media_DSP_Affine_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* input,
    Arcadia_Real32Value scale,
    Arcadia_Real32Value bias
  );

#endif // ARCADIA_MEDIA_DSP_AFFINE_H_INCLUDED
