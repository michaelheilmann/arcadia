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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_RECTANGLE2REAL32_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_RECTANGLE2REAL32_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

/// A rectangle with an offset and a signed extent on each axis.
/// Negative extents are allowed. All edge getters return normalized bounds.
/// This is a native value type and does not require GC registration.
typedef struct Arcadia_Math_Rectangle2Real32 {
  Arcadia_Real32Value x;
  Arcadia_Real32Value y;
  Arcadia_Real32Value extentX;
  Arcadia_Real32Value extentY;
} Arcadia_Math_Rectangle2Real32;

/// Set a rectangle from its two corners, preserving the supplied edge order.
void
Arcadia_Math_Rectangle2Real32_setEdges
  (
    Arcadia_Math_Rectangle2Real32* self,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top
  );

/// Set the offset and signed extent directly.
void
Arcadia_Math_Rectangle2Real32_setOffsetAndExtent
  (
    Arcadia_Math_Rectangle2Real32* self,
    Arcadia_Real32Value x,
    Arcadia_Real32Value y,
    Arcadia_Real32Value extentX,
    Arcadia_Real32Value extentY
  );

Arcadia_Real32Value
Arcadia_Math_Rectangle2Real32_getLeft
  (
    const Arcadia_Math_Rectangle2Real32* self
  );

Arcadia_Real32Value
Arcadia_Math_Rectangle2Real32_getRight
  (
    const Arcadia_Math_Rectangle2Real32* self
  );

Arcadia_Real32Value
Arcadia_Math_Rectangle2Real32_getBottom
  (
    const Arcadia_Math_Rectangle2Real32* self
  );

Arcadia_Real32Value
Arcadia_Math_Rectangle2Real32_getTop
  (
    const Arcadia_Math_Rectangle2Real32* self
  );

/// Compute the smallest axis-aligned rectangle containing both operands.
/// The result always has nonnegative extents. result may alias either operand.
void
Arcadia_Math_Rectangle2Real32_union
  (
    Arcadia_Math_Rectangle2Real32* result,
    const Arcadia_Math_Rectangle2Real32* a,
    const Arcadia_Math_Rectangle2Real32* b
  );

/// Translate the offset without changing the signed extents.
void
Arcadia_Math_Rectangle2Real32_translate
  (
    Arcadia_Math_Rectangle2Real32* self,
    Arcadia_Real32Value dx,
    Arcadia_Real32Value dy
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_RECTANGLE2REAL32_H_INCLUDED
