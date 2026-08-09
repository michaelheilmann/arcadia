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

#include "Arcadia/Media/SampleBuffer.h"

Arcadia_declareObjectType(u8"Arcadia.Media.DSP", Arcadia_Media_DSP,
                          u8"Arcadia.Object");

struct Arcadia_Media_DSPDispatch {
  Arcadia_ObjectDispatch parent;
#if defined(Arcadia_Media_Configuration_DSP_withOutputPins) && 1 == Arcadia_Media_Configuration_DSP_withOutputPins
  Arcadia_SizeValue (*getNumberOfOutputPins)(Arcadia_Thread* thread, Arcadia_Media_DSP* self);
#endif
#if defined(Arcadia_Media_Configuration_DSP_withInputPins) && 1 == Arcadia_Media_Configuration_DSP_withInputPins
  Arcadia_SizeValue (*getNumberOfInputPins)(Arcadia_Thread* thread, Arcadia_Media_DSP* self);
#endif
  void (*generate)(Arcadia_Thread* thread, Arcadia_Media_DSP* self, Arcadia_Natural32Value samples, Arcadia_Natural32Value numberOfSamples, Arcadia_ByteArrayBuilder* target);
};

struct Arcadia_Media_DSP {
  Arcadia_Object parent;
};

/// @brief Generate a number of samples.
/// @param thread A pointer to this thread.
/// @param self A pointer to this DSP.
/// @param sampleRate The sample rate.
/// @param numberOfSamples The number of samples.
/// @param buffer A pointer to a buffer of @a numberOfSamples Arcadia_Real32Value.
void
Arcadia_Media_DSP_generate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP* self,
    Arcadia_Natural32Value sampleRate,
    Arcadia_Natural32Value numberOfSamples,
    Arcadia_ByteArrayBuilder* target
  );

#if defined(Arcadia_Media_Configuration_DSP_withOutputPins) && 1 == Arcadia_Media_Configuration_DSP_withOutputPins

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

#endif

#if defined(Arcadia_Media_Configuration_DSP_withInputPins) && 1 == Arcadia_Media_Configuration_DSP_withInputPins

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

#endif

#endif // ARCADIA_MEDIA_SAMPLEBUFFEROPERATION_H_INCLUDED
