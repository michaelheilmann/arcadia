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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_SEGMENTINTERSECTION2_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_SEGMENTINTERSECTION2_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include <stddef.h>

// Native, non-GC values. Segment endpoints may be supplied in either order.
// All coordinates must be finite; invalid inputs yield Invalid and leave
// output unchanged. Endpoint touches count as Point; a positive-length
// collinear overlap is represented by its two lexicographically ordered ends.
typedef enum Arcadia_Math_SegmentIntersection2Kind {
  Arcadia_Math_SegmentIntersection2Kind_Invalid = -1,
  Arcadia_Math_SegmentIntersection2Kind_None = 0,
  Arcadia_Math_SegmentIntersection2Kind_Point = 1,
  Arcadia_Math_SegmentIntersection2Kind_Overlap = 2
} Arcadia_Math_SegmentIntersection2Kind;

#define ARCADIA_MATH_DECLARE_SEGMENT2(S) \
typedef struct Arcadia_Math_Segment2Real##S { \
  Arcadia_Real##S##Value a[2]; \
  Arcadia_Real##S##Value b[2]; \
} Arcadia_Math_Segment2Real##S; \
typedef struct Arcadia_Math_SegmentIntersection2Real##S { \
  Arcadia_Math_SegmentIntersection2Kind kind; \
  Arcadia_Real##S##Value a[2]; \
  Arcadia_Real##S##Value b[2]; \
} Arcadia_Math_SegmentIntersection2Real##S; \
Arcadia_Math_SegmentIntersection2Kind \
Arcadia_Math_Segment2Real##S##_intersect \
  ( \
    const Arcadia_Math_Segment2Real##S* first, \
    const Arcadia_Math_Segment2Real##S* second, \
    Arcadia_Math_SegmentIntersection2Real##S* result \
  );

ARCADIA_MATH_DECLARE_SEGMENT2(32)
ARCADIA_MATH_DECLARE_SEGMENT2(64)
#undef ARCADIA_MATH_DECLARE_SEGMENT2

#endif // ARCADIA_MATH_IMPLEMENTATION_SEGMENTINTERSECTION2_H_INCLUDED
