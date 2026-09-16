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

#if !defined(ARCADIA_MEDIA_SAMPLEBUFFEROPERATIONS_SAWTOOTHWAVE_H_INCLUDED)
#define ARCADIA_MEDIA_SAMPLEBUFFEROPERATIONS_SAWTOOTHWAVE_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/DSP.h"

/// @brief A source DSP generating a mono sawtooth waveform.
///
/// The oscillator frequency is stored as Real32. The output is approximately
/// [-0.5, +0.5), and phase is preserved across render calls.
Arcadia_declareObjectType(u8"Arcadia.Media.DSP.SawtoothWave", Arcadia_Media_DSP_SawtoothWave,
                          u8"Arcadia.Media.DSP");

struct Arcadia_Media_DSP_SawtoothWaveDispatch {
  Arcadia_Media_DSPDispatch parent;
};

struct Arcadia_Media_DSP_SawtoothWave {
  Arcadia_Media_DSP parent;
  Arcadia_Real32Value frequency;
  Arcadia_Real32Value phase;
};

/// @brief Create a sawtooth oscillator source.
/// @param thread A pointer to this thread.
/// @param frequency The oscillator frequency in Hz. Must be greater than zero.
/// @return The created sawtooth wave node.
Arcadia_Media_DSP_SawtoothWave*
Arcadia_Media_DSP_SawtoothWave_create
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value frequency
  );

#endif // ARCADIA_MEDIA_SAMPLEBUFFEROPERATIONS_SAWTOOTHWAVE_H_INCLUDED
