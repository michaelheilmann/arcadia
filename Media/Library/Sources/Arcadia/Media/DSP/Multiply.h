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

#if !defined(ARCADIA_MEDIA_DSP_MULTIPLY_H_INCLUDED)
#define ARCADIA_MEDIA_DSP_MULTIPLY_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/DSP.h"

/// @brief A binary DSP node multiplying two input signals sample-by-sample.
///
/// Multiply is primarily useful for amplitude modulation and other control-rate
/// or audio-rate signal interactions.
Arcadia_declareObjectType(u8"Arcadia.Media.DSP.Multiply", Arcadia_Media_DSP_Multiply,
                          u8"Arcadia.Media.DSP");

struct Arcadia_Media_DSP_MultiplyDispatch {
  Arcadia_Media_DSPDispatch parent;
};

struct Arcadia_Media_DSP_Multiply {
  Arcadia_Media_DSP parent;
  Arcadia_Media_DSP* left;
  Arcadia_Media_DSP* right;
};

/// @brief Create a multiply node.
/// @param thread A pointer to this thread.
/// @param left The left input DSP. The node keeps this object reachable through its visit callback.
/// @param right The right input DSP. The node keeps this object reachable through its visit callback.
/// @return The created multiply node.
Arcadia_Media_DSP_Multiply*
Arcadia_Media_DSP_Multiply_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* left,
    Arcadia_Media_DSP* right
  );

#endif // ARCADIA_MEDIA_DSP_MULTIPLY_H_INCLUDED
