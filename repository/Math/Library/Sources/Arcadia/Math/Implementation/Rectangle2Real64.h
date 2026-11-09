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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_RECTANGLE2REAL64_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_RECTANGLE2REAL64_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

/// A rectangle with an offset and a signed extent on each axis.
/// Negative extents are allowed. All edge getters return normalized bounds.
/// This is a native value type and does not require GC registration.
typedef struct Arcadia_Math_Rectangle2Real64 {
  Arcadia_Real64Value x;
  Arcadia_Real64Value y;
  Arcadia_Real64Value extentX;
  Arcadia_Real64Value extentY;
} Arcadia_Math_Rectangle2Real64;

/// Set a rectangle from its two corners, preserving the supplied edge order.
void
Arcadia_Math_Rectangle2Real64_setEdges
  (
    Arcadia_Math_Rectangle2Real64* self,
    Arcadia_Real64Value left,
    Arcadia_Real64Value bottom,
    Arcadia_Real64Value right,
    Arcadia_Real64Value top
  );

/// Set the offset and signed extent directly.
void
Arcadia_Math_Rectangle2Real64_setOffsetAndExtent
  (
    Arcadia_Math_Rectangle2Real64* self,
    Arcadia_Real64Value x,
    Arcadia_Real64Value y,
    Arcadia_Real64Value extentX,
    Arcadia_Real64Value extentY
  );

Arcadia_Real64Value
Arcadia_Math_Rectangle2Real64_getLeft
  (
    const Arcadia_Math_Rectangle2Real64* self
  );

Arcadia_Real64Value
Arcadia_Math_Rectangle2Real64_getRight
  (
    const Arcadia_Math_Rectangle2Real64* self
  );

Arcadia_Real64Value
Arcadia_Math_Rectangle2Real64_getBottom
  (
    const Arcadia_Math_Rectangle2Real64* self
  );

Arcadia_Real64Value
Arcadia_Math_Rectangle2Real64_getTop
  (
    const Arcadia_Math_Rectangle2Real64* self
  );

/// Compute the smallest axis-aligned rectangle containing both operands.
/// The result always has nonnegative extents. result may alias either operand.
void
Arcadia_Math_Rectangle2Real64_union
  (
    Arcadia_Math_Rectangle2Real64* result,
    const Arcadia_Math_Rectangle2Real64* a,
    const Arcadia_Math_Rectangle2Real64* b
  );

/// Translate the offset without changing the signed extents.
void
Arcadia_Math_Rectangle2Real64_translate
  (
    Arcadia_Math_Rectangle2Real64* self,
    Arcadia_Real64Value dx,
    Arcadia_Real64Value dy
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_RECTANGLE2REAL64_H_INCLUDED
