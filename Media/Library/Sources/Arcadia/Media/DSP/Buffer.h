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

#if !defined(ARCADIA_MEDIA_DSP_BUFFER_H_INCLUDED)
#define ARCADIA_MEDIA_DSP_BUFFER_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

/// @brief Internal mono Real32 sample buffer used while rendering DSP graphs.
///
/// DSP buffers are not public audio assets. They are temporary render targets for
/// Arcadia_Media_DSP nodes. Conversion to public sample formats happens at the
/// Arcadia_Media_SampleBuffer boundary.
Arcadia_declareObjectType(u8"Arcadia.Media.DSP.Buffer", Arcadia_Media_DSP_Buffer,
                          u8"Arcadia.Object");

struct Arcadia_Media_DSP_BufferDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Media_DSP_Buffer {
  Arcadia_Object parent;
  Arcadia_Real32Value* samples;
  Arcadia_Natural32Value sampleRate;
  Arcadia_SizeValue numberOfSamples;
};

/// @brief Create a zero-initialized DSP buffer.
/// @param thread A pointer to this thread.
/// @param sampleRate The sample rate, in Hz, used by nodes that depend on time.
/// @param numberOfSamples The number of mono samples stored by the buffer.
/// @return The created buffer.
Arcadia_Media_DSP_Buffer*
Arcadia_Media_DSP_Buffer_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value sampleRate,
    Arcadia_SizeValue numberOfSamples
  );

/// @brief Get the writable sample storage of this buffer.
/// @param thread A pointer to this thread.
/// @param self A pointer to this buffer.
/// @return A pointer to @ref Arcadia_Media_DSP_Buffer_getNumberOfSamples Real32 samples.
Arcadia_Real32Value*
Arcadia_Media_DSP_Buffer_getSamples
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Buffer* self
  );

/// @brief Get this buffer's sample rate.
/// @param thread A pointer to this thread.
/// @param self A pointer to this buffer.
/// @return The sample rate in Hz.
Arcadia_Natural32Value
Arcadia_Media_DSP_Buffer_getSampleRate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Buffer* self
  );

/// @brief Get the number of mono samples in this buffer.
/// @param thread A pointer to this thread.
/// @param self A pointer to this buffer.
/// @return The number of samples.
Arcadia_SizeValue
Arcadia_Media_DSP_Buffer_getNumberOfSamples
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Buffer* self
  );

#endif // ARCADIA_MEDIA_DSP_BUFFER_H_INCLUDED
