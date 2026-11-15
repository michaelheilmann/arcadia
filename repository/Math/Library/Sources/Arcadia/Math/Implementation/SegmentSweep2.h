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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_SEGMENTSWEEP2_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_SEGMENTSWEEP2_H_INCLUDED

#include "Arcadia/Math/Implementation/SegmentIntersection2.h"
#include <stddef.h>

// Enumerate every intersecting pair as one deterministic event.
// Pair identity is its two ordered input indices; duplicate segments are distinct.
// Collinear overlaps report one overlap interval rather than infinite point events.
// Intersection events are ordered by (first point x, first point y, pair indices).
// The synchronous callback may return false to stop after the current event.
// NULL callback counts all intersections. Invalid inputs and allocation failure
// never invoke callbacks and leave the caller's count unchanged.
// The sweep uses an immutable-order AVL interval index, augmented with subtree
// high-Y bounds, and an X-expiration min-heap. It prunes disjoint candidate
// boxes and never mutates a live tree comparator. Pair candidates can still
// be quadratic in adversarial cases; this is not the optimal neighbor-only
// Bentley-Ottmann event/status algorithm.
typedef enum Arcadia_Math_SegmentSweep2Status {
  Arcadia_Math_SegmentSweep2Status_Invalid = -2,
  Arcadia_Math_SegmentSweep2Status_NoMemory = -1,
  Arcadia_Math_SegmentSweep2Status_Completed = 0,
  Arcadia_Math_SegmentSweep2Status_Stopped = 1
} Arcadia_Math_SegmentSweep2Status;

#define DECLARE_SWEEP(S) \
typedef struct Arcadia_Math_SegmentSweep2EventReal##S { \
  size_t firstIndex; \
  size_t secondIndex; \
  Arcadia_Math_SegmentIntersection2Real##S intersection; \
} Arcadia_Math_SegmentSweep2EventReal##S; \
typedef Arcadia_BooleanValue (*Arcadia_Math_SegmentSweep2CallbackReal##S) \
  (void* context, const Arcadia_Math_SegmentSweep2EventReal##S* event); \
Arcadia_Math_SegmentSweep2Status \
Arcadia_Math_Segment2Real##S##_enumerateIntersections \
  ( \
    const Arcadia_Math_Segment2Real##S* segments, \
    size_t numberOfSegments, \
    Arcadia_Math_SegmentSweep2CallbackReal##S callback, \
    void* context, \
    size_t* numberOfIntersections \
  );

DECLARE_SWEEP(32)
DECLARE_SWEEP(64)
#undef DECLARE_SWEEP

#endif // ARCADIA_MATH_IMPLEMENTATION_SEGMENTSWEEP2_H_INCLUDED
