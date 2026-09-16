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

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/DSP.h"

/// @brief A source DSP generating white noise.
///
/// The node uses deterministic xorshift pseudo-random generation. The generator
/// state is stored on the node and preserved across render calls. Samples are in
/// the range [-amplitude, +amplitude].
Arcadia_declareObjectType(u8"Arcadia.Media.DSP.WhiteNoise", Arcadia_Media_DSP_WhiteNoise,
                          u8"Arcadia.Media.DSP");

struct Arcadia_Media_DSP_WhiteNoiseDispatch {
  Arcadia_Media_DSPDispatch parent;
};

struct Arcadia_Media_DSP_WhiteNoise {
  Arcadia_Media_DSP parent;
  Arcadia_Natural32Value initialSeed;
  Arcadia_Natural32Value randomState;
  Arcadia_Real32Value amplitude;
};

/// @brief Create a white-noise source.
/// @param thread A pointer to this thread.
/// @param seed Initial pseudo-random generator seed. Zero is remapped internally.
/// @param amplitude Output amplitude multiplier.
/// @return The created white-noise node.
Arcadia_Media_DSP_WhiteNoise*
Arcadia_Media_DSP_WhiteNoise_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value seed,
    Arcadia_Real32Value amplitude
  );

#endif // ARCADIA_MEDIA_DSP_WHITENOISE_H_INCLUDED
