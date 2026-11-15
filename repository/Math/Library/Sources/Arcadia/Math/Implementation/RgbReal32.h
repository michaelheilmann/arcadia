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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_RGBREAL32_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_RGBREAL32_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// Plain RGB floating-point value: not an Arcadia.Object and not GC-managed.
// Components are stored in r, g, b order; values are not implicitly clamped.
typedef struct Arcadia_Math_RgbReal32 {
  Arcadia_Real32Value r;
  Arcadia_Real32Value g;
  Arcadia_Real32Value b;
} Arcadia_Math_RgbReal32;

static inline void
Arcadia_Math_RgbReal32_set
  (
    Arcadia_Math_RgbReal32* self,
    Arcadia_Real32Value r,
    Arcadia_Real32Value g,
    Arcadia_Real32Value b
  )
{
  self->r = r;
  self->g = g;
  self->b = b;
}

// Exact component equality, including the ordinary IEEE-754 NaN semantics.
static inline Arcadia_BooleanValue
Arcadia_Math_RgbReal32_isEqualTo
  (
    const Arcadia_Math_RgbReal32* self,
    const Arcadia_Math_RgbReal32* other
  )
{
  return self->r == other->r
      && self->g == other->g
      && self->b == other->b;
}

// Interpolate each channel. The interpolation factor is clamped to [0, 1],
// while the color components themselves are not clamped. The destination
// may alias either (or both) input colors.
static inline void
Arcadia_Math_RgbReal32_lerp
  (
    Arcadia_Math_RgbReal32* result,
    const Arcadia_Math_RgbReal32* first,
    const Arcadia_Math_RgbReal32* second,
    Arcadia_Real32Value t
  )
{
  // Repeated inputs must retain their exact values, even for fractional t.
  if (first == second) {
    Arcadia_Math_RgbReal32_set(result, first->r, first->g, first->b);
    return;
  }
  if (t <= 0.f) {
    Arcadia_Real32Value r = first->r;
    Arcadia_Real32Value g = first->g;
    Arcadia_Real32Value b = first->b;
    Arcadia_Math_RgbReal32_set(result, r, g, b);
  } else if (t >= 1.f) {
    Arcadia_Real32Value r = second->r;
    Arcadia_Real32Value g = second->g;
    Arcadia_Real32Value b = second->b;
    Arcadia_Math_RgbReal32_set(result, r, g, b);
  } else {
    Arcadia_Real32Value s = 1.f - t;
    Arcadia_Real32Value r = s * first->r + t * second->r;
    Arcadia_Real32Value g = s * first->g + t * second->g;
    Arcadia_Real32Value b = s * first->b + t * second->b;
    Arcadia_Math_RgbReal32_set(result, r, g, b);
  }
}

#endif // ARCADIA_MATH_IMPLEMENTATION_RGBREAL32_H_INCLUDED
