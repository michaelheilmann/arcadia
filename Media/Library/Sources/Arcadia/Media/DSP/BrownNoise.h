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

#if !defined(ARCADIA_MEDIA_DSP_BROWNNOISE_H_INCLUDED)
#define ARCADIA_MEDIA_DSP_BROWNNOISE_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/DSP.h"

/// @brief A source DSP generating low-frequency random motion.
///
/// Brown noise integrates deterministic pseudo-random steps with a small leak and
/// clamps the internal state to [-1, +1]. The output is the internal noise state
/// multiplied by the configured amplitude. Random and noise state are preserved
/// across render calls.
Arcadia_declareObjectType(u8"Arcadia.Media.DSP.BrownNoise", Arcadia_Media_DSP_BrownNoise,
                          u8"Arcadia.Media.DSP");

struct Arcadia_Media_DSP_BrownNoiseDispatch {
  Arcadia_Media_DSPDispatch parent;
};

struct Arcadia_Media_DSP_BrownNoise {
  Arcadia_Media_DSP parent;
  Arcadia_Natural32Value initialSeed;
  Arcadia_Natural32Value randomState;
  Arcadia_Real32Value noiseState;
  Arcadia_Real32Value amplitude;
};

/// @brief Create a brown-noise source.
/// @param thread A pointer to this thread.
/// @param seed Initial pseudo-random generator seed. Zero is remapped internally.
/// @param amplitude Output amplitude multiplier.
/// @return The created brown-noise node.
Arcadia_Media_DSP_BrownNoise*
Arcadia_Media_DSP_BrownNoise_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value seed,
    Arcadia_Real32Value amplitude
  );

#endif // ARCADIA_MEDIA_DSP_BROWNNOISE_H_INCLUDED
