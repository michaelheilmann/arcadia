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

#if !defined(ARCADIA_MEDIA_COLORPALETTENATURAL8_H_INCLUDED)
#define ARCADIA_MEDIA_COLORPALETTENATURAL8_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include `Arcadia/Media/ColorPaletteNatural8.h` directly, include `Arcadia/Media/Include.h` instead")
#endif

#include "Arcadia/Media/PixelValuesNatural8.h"

// Named RGB8 colors salvaged from the legacy dx/core/math.h declarations.
// They are plain three-byte values, not heap-allocated objects. Return-by-value
// avoids shared mutable color storage and does not require library exports.

/// Capri: RGB (0, 191, 255).
static inline Arcadia_Media_PixelRedGreenBlueNatural8
Arcadia_Media_ColorPaletteNatural8_getCapri
  (
    void
  )
{
  Arcadia_Media_PixelRedGreenBlueNatural8 value = { 0, 191, 255 };
  return value;
}

/// Malachite: RGB (0, 255, 64).
static inline Arcadia_Media_PixelRedGreenBlueNatural8
Arcadia_Media_ColorPaletteNatural8_getMalachite
  (
    void
  )
{
  Arcadia_Media_PixelRedGreenBlueNatural8 value = { 0, 255, 64 };
  return value;
}

/// Amber: RGB (255, 192, 0).
static inline Arcadia_Media_PixelRedGreenBlueNatural8
Arcadia_Media_ColorPaletteNatural8_getAmber
  (
    void
  )
{
  Arcadia_Media_PixelRedGreenBlueNatural8 value = { 255, 192, 0 };
  return value;
}

#endif // ARCADIA_MEDIA_COLORPALETTENATURAL8_H_INCLUDED
