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

#define ARCADIA_MEDIA_PRIVATE (1)
#include "Arcadia/Media/PixelBufferComposite.h"

#include <stdint.h>

static void
failInvalidArgument
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

static Arcadia_BooleanValue
isSupportedFormat
  (
    Arcadia_Media_PixelFormat format
  )
{
  switch (format) {
    case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8:
    case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8:
    case Arcadia_Media_PixelFormat_BlueGreenRedNatural8:
    case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8:
    case Arcadia_Media_PixelFormat_RedGreenBlueNatural8:
    case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8:
      return Arcadia_BooleanValue_True;
    default:
      return Arcadia_BooleanValue_False;
  }
}

static int64_t
maximumInteger64
  (
    int64_t a,
    int64_t b
  )
{
  return a > b ? a : b;
}

static int64_t
minimumInteger64
  (
    int64_t a,
    int64_t b
  )
{
  return a < b ? a : b;
}

static Arcadia_Natural8Value
blendComponent
  (
    Arcadia_Natural8Value sourceComponent,
    Arcadia_Natural8Value destinationComponent,
    uint32_t sourceWeight,
    uint32_t destinationWeight
  )
{
  uint32_t denominator = sourceWeight + destinationWeight;
  if (0 == denominator) {
    return 0;
  }
  uint32_t numerator = (uint32_t)sourceComponent * sourceWeight + (uint32_t)destinationComponent * destinationWeight;
  return (Arcadia_Natural8Value)((numerator + denominator / 2u) / denominator);
}

void
Arcadia_Media_PixelBuffer_compositeRectangle
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* destination,
    Arcadia_Integer32Value destinationLeft,
    Arcadia_Integer32Value destinationTop,
    Arcadia_Media_PixelBuffer* source,
    Arcadia_Integer32Value sourceLeft,
    Arcadia_Integer32Value sourceTop,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_Media_PixelBufferCompositeMode mode
  )
{
  if (NULL == destination || NULL == source || width < 0 || height < 0 ||
      (mode != Arcadia_Media_PixelBufferCompositeMode_Copy && mode != Arcadia_Media_PixelBufferCompositeMode_SourceOver)) {
    failInvalidArgument(thread);
  }
  if (isSupportedFormat(destination->pixelFormat) != Arcadia_BooleanValue_True ||
      isSupportedFormat(source->pixelFormat) != Arcadia_BooleanValue_True) {
    failInvalidArgument(thread);
  }
  if (destination->width < 0 || destination->height < 0 || source->width < 0 || source->height < 0) {
    failInvalidArgument(thread);
  }
  if (0 == width || 0 == height) {
    return;
  }

  // The destination coordinate (x, y) corresponds to source coordinate
  // (sourceLeft + x - destinationLeft, sourceTop + y - destinationTop).
  // Signed 64-bit arithmetic avoids overflowing 32-bit rectangle endpoints.
  int64_t x0 = maximumInteger64((int64_t)destinationLeft, 0);
  int64_t y0 = maximumInteger64((int64_t)destinationTop, 0);
  int64_t x1 = minimumInteger64((int64_t)destinationLeft + width, destination->width);
  int64_t y1 = minimumInteger64((int64_t)destinationTop + height, destination->height);
  x0 = maximumInteger64(x0, (int64_t)destinationLeft - sourceLeft);
  y0 = maximumInteger64(y0, (int64_t)destinationTop - sourceTop);
  x1 = minimumInteger64(x1, (int64_t)destinationLeft - sourceLeft + source->width);
  y1 = minimumInteger64(y1, (int64_t)destinationTop - sourceTop + source->height);
  if (x0 >= x1 || y0 >= y1) {
    return;
  }

  // A translated self-copy can be traversed like memmove: when moving down,
  // visit rows bottom-up; when staying on a row but moving right, visit right-to-left.
  // The source pixel is then read before any earlier write could overwrite it.
  Arcadia_BooleanValue reverseRows = source == destination && destinationTop > sourceTop;
  Arcadia_BooleanValue reverseColumns = source == destination && destinationTop == sourceTop && destinationLeft > sourceLeft;
  for (int64_t row = 0; row < y1 - y0; ++row) {
    int64_t y = reverseRows ? y1 - 1 - row : y0 + row;
    Arcadia_Integer32Value sourceY = (Arcadia_Integer32Value)((int64_t)sourceTop + y - destinationTop);
    for (int64_t column = 0; column < x1 - x0; ++column) {
      int64_t x = reverseColumns ? x1 - 1 - column : x0 + column;
      Arcadia_Integer32Value sourceX = (Arcadia_Integer32Value)((int64_t)sourceLeft + x - destinationLeft);
      Arcadia_Natural8Value sr, sg, sb, sa;
      Arcadia_Media_PixelBuffer_getPixelRGBA(thread, source, sourceX, sourceY, &sr, &sg, &sb, &sa);
      if (mode == Arcadia_Media_PixelBufferCompositeMode_Copy) {
        Arcadia_Media_PixelBuffer_setPixelRGBA(thread, destination, (Arcadia_Integer32Value)x, (Arcadia_Integer32Value)y, sr, sg, sb, sa);
      } else {
        Arcadia_Natural8Value dr, dg, db, da;
        Arcadia_Media_PixelBuffer_getPixelRGBA(thread, destination, (Arcadia_Integer32Value)x, (Arcadia_Integer32Value)y, &dr, &dg, &db, &da);
        uint32_t sourceWeight = 255u * sa;
        uint32_t destinationWeight = (uint32_t)da * (255u - sa);
        uint32_t alphaNumerator = sourceWeight + destinationWeight;
        Arcadia_Natural8Value oa = (Arcadia_Natural8Value)((alphaNumerator + 127u) / 255u);
        Arcadia_Natural8Value or_ = blendComponent(sr, dr, sourceWeight, destinationWeight);
        Arcadia_Natural8Value og = blendComponent(sg, dg, sourceWeight, destinationWeight);
        Arcadia_Natural8Value ob = blendComponent(sb, db, sourceWeight, destinationWeight);
        Arcadia_Media_PixelBuffer_setPixelRGBA(thread, destination, (Arcadia_Integer32Value)x, (Arcadia_Integer32Value)y, or_, og, ob, oa);
      }
    }
  }
}
