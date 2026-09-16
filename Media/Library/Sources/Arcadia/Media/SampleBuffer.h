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

#if !defined(ARCADIA_MEDIA_SAMPLEBUFFER_H_INCLUDED)
#define ARCADIA_MEDIA_SAMPLEBUFFER_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Media/SampleFormat.h"
typedef struct Arcadia_Media_DSP Arcadia_Media_DSP;

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_SampleBuffer
Arcadia_declareObjectType(u8"Arcadia.Media.SampleBuffer", Arcadia_Media_SampleBuffer,
                          u8"Arcadia.Object");

struct Arcadia_Media_SampleBufferDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Media_SampleBuffer {
  Arcadia_Object _parent;
  /// A pointer to the Bytes of this sample buffer.
  Arcadia_Natural8Value* bytes;
  /// The sample format of this sample buffer.
  Arcadia_Media_SampleFormat sampleFormat;
  /// The length, in seconds, of this sample buffer.
  Arcadia_Integer32Value length;
  /// The sample rate, in Hz, of this sample buffer.
  Arcadia_Integer32Value sampleRate;
};

/// @brief Get the sample format of this sample buffer.
/// @param self A pointer to this sample buffer.
/// @return The sample format of this sample buffer.
Arcadia_Media_SampleFormat
Arcadia_Media_SampleBuffer_getSampleFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Media_SampleBuffer* self
  );

/// @brief Get the sample rate of this sample buffer.
/// @param self A pointer to this sample buffer.
/// @return The sample rate, in Hertz, of this sample buffer.
Arcadia_Integer32Value
Arcadia_Media_SampleBuffer_getSampleRate
  (
    Arcadia_Thread* thread,
    Arcadia_Media_SampleBuffer* self
  );

/// @brief Get the length, in seconds, of this sample buffer.
/// @param self A pointer to this sample buffer.
/// @return The length, in seconds, of this sample buffer.
Arcadia_Integer32Value
Arcadia_Media_SampleBuffer_getLength
  (
    Arcadia_Thread* thread,
    Arcadia_Media_SampleBuffer* self
  );

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_SampleBuffer_create
Arcadia_Media_SampleBuffer*
Arcadia_Media_SampleBuffer_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value length,
    Arcadia_Integer32Value sampleRate,
    Arcadia_Media_SampleFormat sampleFormat
  );

Arcadia_Media_SampleBuffer*
Arcadia_Media_SampleBuffer_createClone
  (
    Arcadia_Thread* thread,
    Arcadia_Media_SampleBuffer* other
  );

/// @brie Fill this sample buffer with a signal (converted to PCM data).
/// @param thread A pointer to this thread.
/// @param self A pointer to this sample buffer.
/// @param dsp A pointer to the DSP.
void
Arcadia_Media_SampleBuffer_fill
  (
    Arcadia_Thread* thread,
    Arcadia_Media_SampleBuffer* self,
    Arcadia_Media_DSP* dsp
  );

void
Arcadia_Media_sampleBuffer_setFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Media_SampleBuffer* self,
    Arcadia_Media_SampleFormat format
  );

/// @brief Swap the contents of this sample buffer with the contents of another sample buffer.
/// @param thread A pointer to this thread.
/// @param self A pointer to this sample buffer.
/// @param other A pointer to the other sample buffer.
void
Arcadia_Media_SampleBuffer_swap
  (
    Arcadia_Thread* thread,
    Arcadia_Media_SampleBuffer* self,
    Arcadia_Media_SampleBuffer* other
  );

#endif // ARCADIA_MEDIA_SAMPLEBUFFER_H_INCLUDED
