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

#if !defined(ARCADIA_MEDIA_PIXELBUFFERCOMPOSITE_H_INCLUDED)
#define ARCADIA_MEDIA_PIXELBUFFERCOMPOSITE_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif

#include "Arcadia/Media/PixelBuffer.h"

typedef enum Arcadia_Media_PixelBufferCompositeMode {
  Arcadia_Media_PixelBufferCompositeMode_Copy,
  Arcadia_Media_PixelBufferCompositeMode_SourceOver,
} Arcadia_Media_PixelBufferCompositeMode;

/// @brief Composite a source pixel rectangle onto a destination pixel rectangle.
/// @param destination The destination buffer (modified in place).
/// @param destinationLeft The destination rectangle's left coordinate.
/// @param destinationTop The destination rectangle's top coordinate.
/// @param source The source buffer, which may equal @a destination.
/// @param sourceLeft The source rectangle's left coordinate.
/// @param sourceTop The source rectangle's top coordinate.
/// @param width The common rectangle width; must be nonnegative.
/// @param height The common rectangle height; must be nonnegative.
/// @param mode Copy replaces destination pixels; SourceOver uses straight-alpha
/// Porter-Duff source-over compositing with nearest-integer rounding.
/// @remarks
/// Both rectangles are clipped together to their corresponding buffer bounds.
/// Empty intersections succeed without modifying the destination. Self-overlap
/// behaves as if source pixels were snapshotted before any destination writes.
/// Six 8-bit RGB/RGBA formats are supported: ABGR, ARGB, BGR, BGRA, RGB, RGBA.
/// Formats without alpha are treated as opaque; their destination alpha is
/// discarded. Color channels are composited in their stored color space.
/// @error Arcadia_Status_ArgumentValueInvalid Invalid pointer, rectangle size,
/// composite mode, or unsupported pixel format.
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
  );

#endif // ARCADIA_MEDIA_PIXELBUFFERCOMPOSITE_H_INCLUDED
