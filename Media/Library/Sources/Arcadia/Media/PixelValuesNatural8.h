// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
// for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#if !defined(ARCADIA_MEDIA_PIXELVALUESNATURAL8_H_INCLUDED)
#define ARCADIA_MEDIA_PIXELVALUESNATURAL8_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include `Arcadia/Media/PixelValuesNatural8.h` directly, include `Arcadia/Media/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include <stdint.h>

// Plain, non-GC-managed pixel values. Component order in every struct is the
// memory order specified by the matching Arcadia.Media.PixelFormat value.
// A value never owns storage; it is safe to use on the stack or in arrays.
// These types do not imply that PixelBuffer currently supports every format.

typedef struct Arcadia_Media_PixelAlphaBlueGreenRedNatural8 {
  Arcadia_Natural8Value a;
  Arcadia_Natural8Value b;
  Arcadia_Natural8Value g;
  Arcadia_Natural8Value r;
} Arcadia_Media_PixelAlphaBlueGreenRedNatural8;

static inline void
Arcadia_Media_PixelAlphaBlueGreenRedNatural8_set
  (
    Arcadia_Media_PixelAlphaBlueGreenRedNatural8* target,
    Arcadia_Natural8Value a,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value r
  )
{
  target->a = a;
  target->b = b;
  target->g = g;
  target->r = r;
}

static inline Arcadia_BooleanValue
Arcadia_Media_PixelAlphaBlueGreenRedNatural8_isEqualTo
  (
    const Arcadia_Media_PixelAlphaBlueGreenRedNatural8* self,
    const Arcadia_Media_PixelAlphaBlueGreenRedNatural8* other
  )
{
  return self->a == other->a
      && self->b == other->b
      && self->g == other->g
      && self->r == other->r;
}

typedef struct Arcadia_Media_PixelAlphaGrayNatural8 {
  Arcadia_Natural8Value a;
  Arcadia_Natural8Value g;
} Arcadia_Media_PixelAlphaGrayNatural8;

static inline void
Arcadia_Media_PixelAlphaGrayNatural8_set
  (
    Arcadia_Media_PixelAlphaGrayNatural8* target,
    Arcadia_Natural8Value a,
    Arcadia_Natural8Value g
  )
{
  target->a = a;
  target->g = g;
}

static inline Arcadia_BooleanValue
Arcadia_Media_PixelAlphaGrayNatural8_isEqualTo
  (
    const Arcadia_Media_PixelAlphaGrayNatural8* self,
    const Arcadia_Media_PixelAlphaGrayNatural8* other
  )
{
  return self->a == other->a
      && self->g == other->g;
}

typedef struct Arcadia_Media_PixelAlphaRedGreenBlueNatural8 {
  Arcadia_Natural8Value a;
  Arcadia_Natural8Value r;
  Arcadia_Natural8Value g;
  Arcadia_Natural8Value b;
} Arcadia_Media_PixelAlphaRedGreenBlueNatural8;

static inline void
Arcadia_Media_PixelAlphaRedGreenBlueNatural8_set
  (
    Arcadia_Media_PixelAlphaRedGreenBlueNatural8* target,
    Arcadia_Natural8Value a,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b
  )
{
  target->a = a;
  target->r = r;
  target->g = g;
  target->b = b;
}

static inline Arcadia_BooleanValue
Arcadia_Media_PixelAlphaRedGreenBlueNatural8_isEqualTo
  (
    const Arcadia_Media_PixelAlphaRedGreenBlueNatural8* self,
    const Arcadia_Media_PixelAlphaRedGreenBlueNatural8* other
  )
{
  return self->a == other->a
      && self->r == other->r
      && self->g == other->g
      && self->b == other->b;
}

typedef struct Arcadia_Media_PixelBlueGreenRedNatural8 {
  Arcadia_Natural8Value b;
  Arcadia_Natural8Value g;
  Arcadia_Natural8Value r;
} Arcadia_Media_PixelBlueGreenRedNatural8;

static inline void
Arcadia_Media_PixelBlueGreenRedNatural8_set
  (
    Arcadia_Media_PixelBlueGreenRedNatural8* target,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value r
  )
{
  target->b = b;
  target->g = g;
  target->r = r;
}

static inline Arcadia_BooleanValue
Arcadia_Media_PixelBlueGreenRedNatural8_isEqualTo
  (
    const Arcadia_Media_PixelBlueGreenRedNatural8* self,
    const Arcadia_Media_PixelBlueGreenRedNatural8* other
  )
{
  return self->b == other->b
      && self->g == other->g
      && self->r == other->r;
}

typedef struct Arcadia_Media_PixelBlueGreenRedAlphaNatural8 {
  Arcadia_Natural8Value b;
  Arcadia_Natural8Value g;
  Arcadia_Natural8Value r;
  Arcadia_Natural8Value a;
} Arcadia_Media_PixelBlueGreenRedAlphaNatural8;

static inline void
Arcadia_Media_PixelBlueGreenRedAlphaNatural8_set
  (
    Arcadia_Media_PixelBlueGreenRedAlphaNatural8* target,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value a
  )
{
  target->b = b;
  target->g = g;
  target->r = r;
  target->a = a;
}

static inline Arcadia_BooleanValue
Arcadia_Media_PixelBlueGreenRedAlphaNatural8_isEqualTo
  (
    const Arcadia_Media_PixelBlueGreenRedAlphaNatural8* self,
    const Arcadia_Media_PixelBlueGreenRedAlphaNatural8* other
  )
{
  return self->b == other->b
      && self->g == other->g
      && self->r == other->r
      && self->a == other->a;
}

typedef struct Arcadia_Media_PixelGrayNatural8 {
  Arcadia_Natural8Value g;
} Arcadia_Media_PixelGrayNatural8;

static inline void
Arcadia_Media_PixelGrayNatural8_set
  (
    Arcadia_Media_PixelGrayNatural8* target,
    Arcadia_Natural8Value g
  )
{
  target->g = g;
}

static inline Arcadia_BooleanValue
Arcadia_Media_PixelGrayNatural8_isEqualTo
  (
    const Arcadia_Media_PixelGrayNatural8* self,
    const Arcadia_Media_PixelGrayNatural8* other
  )
{
  return self->g == other->g;
}

typedef struct Arcadia_Media_PixelGrayAlphaNatural8 {
  Arcadia_Natural8Value g;
  Arcadia_Natural8Value a;
} Arcadia_Media_PixelGrayAlphaNatural8;

static inline void
Arcadia_Media_PixelGrayAlphaNatural8_set
  (
    Arcadia_Media_PixelGrayAlphaNatural8* target,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value a
  )
{
  target->g = g;
  target->a = a;
}

static inline Arcadia_BooleanValue
Arcadia_Media_PixelGrayAlphaNatural8_isEqualTo
  (
    const Arcadia_Media_PixelGrayAlphaNatural8* self,
    const Arcadia_Media_PixelGrayAlphaNatural8* other
  )
{
  return self->g == other->g
      && self->a == other->a;
}

typedef struct Arcadia_Media_PixelRedGreenBlueNatural8 {
  Arcadia_Natural8Value r;
  Arcadia_Natural8Value g;
  Arcadia_Natural8Value b;
} Arcadia_Media_PixelRedGreenBlueNatural8;

static inline void
Arcadia_Media_PixelRedGreenBlueNatural8_set
  (
    Arcadia_Media_PixelRedGreenBlueNatural8* target,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b
  )
{
  target->r = r;
  target->g = g;
  target->b = b;
}

static inline Arcadia_BooleanValue
Arcadia_Media_PixelRedGreenBlueNatural8_isEqualTo
  (
    const Arcadia_Media_PixelRedGreenBlueNatural8* self,
    const Arcadia_Media_PixelRedGreenBlueNatural8* other
  )
{
  return self->r == other->r
      && self->g == other->g
      && self->b == other->b;
}

typedef struct Arcadia_Media_PixelRedGreenBlueAlphaNatural8 {
  Arcadia_Natural8Value r;
  Arcadia_Natural8Value g;
  Arcadia_Natural8Value b;
  Arcadia_Natural8Value a;
} Arcadia_Media_PixelRedGreenBlueAlphaNatural8;

static inline void
Arcadia_Media_PixelRedGreenBlueAlphaNatural8_set
  (
    Arcadia_Media_PixelRedGreenBlueAlphaNatural8* target,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value a
  )
{
  target->r = r;
  target->g = g;
  target->b = b;
  target->a = a;
}

static inline Arcadia_BooleanValue
Arcadia_Media_PixelRedGreenBlueAlphaNatural8_isEqualTo
  (
    const Arcadia_Media_PixelRedGreenBlueAlphaNatural8* self,
    const Arcadia_Media_PixelRedGreenBlueAlphaNatural8* other
  )
{
  return self->r == other->r
      && self->g == other->g
      && self->b == other->b
      && self->a == other->a;
}

// Linear interpolation of single-byte components with half-up rounding.
// t=0 returns a, t=255 returns b. The widened unsigned arithmetic avoids the
// legacy negative-division rounding error when b < a. Maximum sum: 65025+127.
static inline Arcadia_Natural8Value
Arcadia_Media_PixelNatural8_lerp
  (
    Arcadia_Natural8Value a,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value t
  )
{
  uint32_t numerator = (uint32_t)a * (255u - (uint32_t)t) + (uint32_t)b * (uint32_t)t;
  return (Arcadia_Natural8Value)((numerator + 127u) / 255u);
}

#endif // ARCADIA_MEDIA_PIXELVALUESNATURAL8_H_INCLUDED
