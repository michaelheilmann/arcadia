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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/FPSCounter.h"

// The width of the frames-per-second window, in milliseconds.
#define Arcadia_Engine_FPSCounter_WindowTicks (500)
// The number of frames-per-second window samples retained for smoothing.
#define Arcadia_Engine_FPSCounter_SampleCount (8)
// The width, in frames per second, of a histogram bucket.
#define Arcadia_Engine_FPSCounter_BucketSize (100)

static Arcadia_Integer32Value
computeHistogramFps
  (
    Arcadia_Engine_FPSCounter const* self
  );

void
Arcadia_Engine_FPSCounter_initialize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FPSCounter* self
  )
{
  self->accumulatedTicks = 0.0;
  self->numberOfFrames = 0.0;
  self->samples = Arcadia_Memory_allocateUnmanaged(thread, Arcadia_Engine_FPSCounter_SampleCount * sizeof(Arcadia_Integer32Value));
  for (Arcadia_SizeValue i = 0; i < Arcadia_Engine_FPSCounter_SampleCount; ++i) {
    self->samples[i] = 0;
  }
  self->sampleCount = 0;
  self->sampleIndex = 0;
  self->currentFps = 0;
}

void
Arcadia_Engine_FPSCounter_uninitialize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FPSCounter* self
  )
{
  if (self->samples) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->samples);
  }
  self->accumulatedTicks = 0.0;
  self->numberOfFrames = 0.0;
  self->samples = NULL;
  self->sampleCount = 0;
  self->sampleIndex = 0;
  self->currentFps = 0;
}

void
Arcadia_Engine_FPSCounter_update
  (
    Arcadia_Engine_FPSCounter* self,
    Arcadia_Real64Value tick
  )
{
  self->accumulatedTicks += tick;
  self->numberOfFrames += 1.0;
  if (self->accumulatedTicks >= Arcadia_Engine_FPSCounter_WindowTicks) {
    Arcadia_Integer32Value const fps = (Arcadia_Integer32Value)(0.5 + self->numberOfFrames * 1000.0 / self->accumulatedTicks);
    self->samples[self->sampleIndex] = fps;
    self->sampleIndex = (self->sampleIndex + 1) % Arcadia_Engine_FPSCounter_SampleCount;
    if (self->sampleCount < Arcadia_Engine_FPSCounter_SampleCount) {
      ++self->sampleCount;
    }
    self->currentFps = computeHistogramFps(self);
    self->accumulatedTicks = 0.0;
    self->numberOfFrames = 0.0;
  }
}

Arcadia_SizeValue
Arcadia_Engine_FPSCounter_format
  (
    Arcadia_Engine_FPSCounter const* self,
    char* buffer
  )
{
  Arcadia_Integer32Value fps = self->currentFps;
  if (fps < 0) {
    fps = 0;
  }
  if (fps > 9999) {
    fps = 9999;
  }
  buffer[0] = 'F';
  buffer[1] = 'P';
  buffer[2] = 'S';
  buffer[3] = ':';
  buffer[4] = ' ';
  Arcadia_SizeValue numberOfCharacters = 5;
  if (0 == fps) {
    buffer[numberOfCharacters++] = '0';
  } else {
    char digits[Arcadia_Engine_FPSCounter_MaximumTextLength];
    Arcadia_SizeValue numberOfDigits = 0;
    while (0 < fps) {
      digits[numberOfDigits++] = (char)('0' + fps % 10);
      fps = fps / 10;
    }
    while (0 < numberOfDigits) {
      buffer[numberOfCharacters++] = digits[--numberOfDigits];
    }
  }
  buffer[numberOfCharacters] = '\0';
  return numberOfCharacters;
}

char const*
Arcadia_Engine_FPSCounter_getGlyphSource
  (
    void
  )
{
  return "FPS: 0123456789 ";
}

static Arcadia_Integer32Value
computeHistogramFps
  (
    Arcadia_Engine_FPSCounter const* self
  )
{
  if (0 == self->sampleCount) {
    return 0;
  }
  // Determine the value range of the samples to bound the buckets.
  Arcadia_Integer32Value maxSample = 0;
  for (Arcadia_SizeValue i = 0; i < self->sampleCount; ++i) {
    if (self->samples[i] > maxSample) {
      maxSample = self->samples[i];
    }
  }
  Arcadia_SizeValue const maxBucketStart = (Arcadia_SizeValue)(maxSample / Arcadia_Engine_FPSCounter_BucketSize);
  // Find the bucket containing the most samples.
  Arcadia_Integer32Value bestBucketCount = -1;
  Arcadia_SizeValue bestBucketStart = 0;
  for (Arcadia_SizeValue bucketStart = 0; bucketStart <= maxBucketStart; ++bucketStart) {
    Arcadia_Integer32Value count = 0;
    for (Arcadia_SizeValue i = 0; i < self->sampleCount; ++i) {
      Arcadia_SizeValue const sampleBucketStart = (Arcadia_SizeValue)(self->samples[i] / Arcadia_Engine_FPSCounter_BucketSize);
      if (sampleBucketStart == bucketStart) {
        ++count;
      }
    }
    if (count > bestBucketCount) {
      bestBucketCount = count;
      bestBucketStart = bucketStart;
    }
  }
  // Average the samples in the winning bucket.
  Arcadia_SizeValue count = 0;
  Arcadia_Natural64Value sum = 0;
  for (Arcadia_SizeValue i = 0; i < self->sampleCount; ++i) {
    Arcadia_SizeValue const sampleBucketStart = (Arcadia_SizeValue)(self->samples[i] / Arcadia_Engine_FPSCounter_BucketSize);
    if (sampleBucketStart == bestBucketStart) {
      ++count;
      sum += (Arcadia_Natural64Value)self->samples[i];
    }
  }
  return (Arcadia_Integer32Value)((sum + count / 2) / count);
}

#undef Arcadia_Engine_FPSCounter_WindowTicks
#undef Arcadia_Engine_FPSCounter_SampleCount
#undef Arcadia_Engine_FPSCounter_BucketSize
