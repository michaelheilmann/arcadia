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

#if !defined(ARCADIA_MEDIA_SAMPLEBUFFEROPERATION_H_INCLUDED)
#define ARCADIA_MEDIA_SAMPLEBUFFEROPERATION_H_INCLUDED

#include "Arcadia/Media/DSP/Buffer.h"

Arcadia_declareObjectType(u8"Arcadia.Media.DSP", Arcadia_Media_DSP,
                          u8"Arcadia.Object");

struct Arcadia_Media_DSPDispatch {
  Arcadia_ObjectDispatch parent;
  Arcadia_SizeValue (*getNumberOfOutputPins)(Arcadia_Thread* thread, Arcadia_Media_DSP* self);
  Arcadia_SizeValue (*getNumberOfInputPins)(Arcadia_Thread* thread, Arcadia_Media_DSP* self);
  void (*render)(Arcadia_Thread* thread, Arcadia_Media_DSP* self, Arcadia_Media_DSP_Buffer* target);
  void (*reset)(Arcadia_Thread* thread, Arcadia_Media_DSP* self);
  void (*reseed)(Arcadia_Thread* thread, Arcadia_Media_DSP* self, Arcadia_Natural32Value seed);
};

struct Arcadia_Media_DSP {
  Arcadia_Object parent;
  Arcadia_Object* ownerGraph;
};

/// @brief Render this DSP into an internal mono Real32 DSP buffer.
///
/// DSP nodes render mono samples as Arcadia_Real32Value values into caller-provided
/// Arcadia_Media_DSP_Buffer instances. Public sample formats, channel layout, and
/// quantization are handled at the Arcadia_Media_SampleBuffer boundary.
/// @param thread A pointer to this thread.
/// @param self A pointer to this DSP.
/// @param target The target DSP buffer.
void
Arcadia_Media_DSP_render
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self,
    Arcadia_Media_DSP_Buffer* target
  );

/// @brief Generate mono Real32 samples into a byte builder. Compatibility wrapper around @ref Arcadia_Media_DSP_render.
/// @param thread A pointer to this thread.
/// @param self A pointer to this DSP.
/// @param sampleRate The sample rate.
/// @param numberOfSamples The number of samples.
/// @param target The target byte builder.
void
Arcadia_Media_DSP_generate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self,
    Arcadia_Natural32Value sampleRate,
    Arcadia_Natural32Value numberOfSamples,
    Arcadia_ByteArrayBuilder* target
  );

/// @brief Get the number of output pins of this DSP.
/// @param thread A pointer to this thread.
/// @param self A pointer to this DSP.
/// @return The number of output pins.
Arcadia_SizeValue
Arcadia_Media_DSP_getNumberOfOutputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self
  );

/// @brief Get the number of input pins of this DSP.
/// @param thread A pointer to this thread.
/// @param self A pointer to this DSP.
/// @return The number of input pins.
Arcadia_SizeValue
Arcadia_Media_DSP_getNumberOfInputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self
  );

/// @brief Reset this DSP node's own render state.
///
/// This is an internal hook used by Arcadia_Media_DSP_Graph_reset. It resets only
/// the node's own state and does not recurse into input nodes.
void
_Arcadia_Media_DSP_reset
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self
  );

/// @brief Reseed this DSP node if it has pseudo-random render state.
///
/// This is an internal hook used by Arcadia_Media_DSP_Graph_reseed. It changes
/// only the node's own seed state and does not recurse into input nodes.
void
_Arcadia_Media_DSP_reseed
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self,
    Arcadia_Natural32Value seed
  );

#endif // ARCADIA_MEDIA_SAMPLEBUFFEROPERATION_H_INCLUDED
