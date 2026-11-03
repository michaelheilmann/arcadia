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

#include "Arcadia/Math/Implementation/SegmentSweep2.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

// Each precision instantiates one sweep with its own interval AVL and expiry
// heap. The status ordering is immutable (low Y, then input index), unlike the
// mutable TreeSet comparator in the historical Java implementation.
#define SW_REAL Arcadia_Real32Value
#define SW_SEG Arcadia_Math_Segment2Real32
#define SW_HIT Arcadia_Math_SegmentIntersection2Real32
#define SW_EVENT Arcadia_Math_SegmentSweep2EventReal32
#define SW_CALLBACK Arcadia_Math_SegmentSweep2CallbackReal32
#define SW_INTERSECT Arcadia_Math_Segment2Real32_intersect
#define SW_ENUM Arcadia_Math_Segment2Real32_enumerateIntersections
#define SW_SUFFIX 32
#include "Arcadia/Math/Implementation/SegmentSweep2.i"
#undef SW_SUFFIX
#undef SW_ENUM
#undef SW_INTERSECT
#undef SW_CALLBACK
#undef SW_EVENT
#undef SW_HIT
#undef SW_SEG
#undef SW_REAL

#define SW_REAL Arcadia_Real64Value
#define SW_SEG Arcadia_Math_Segment2Real64
#define SW_HIT Arcadia_Math_SegmentIntersection2Real64
#define SW_EVENT Arcadia_Math_SegmentSweep2EventReal64
#define SW_CALLBACK Arcadia_Math_SegmentSweep2CallbackReal64
#define SW_INTERSECT Arcadia_Math_Segment2Real64_intersect
#define SW_ENUM Arcadia_Math_Segment2Real64_enumerateIntersections
#define SW_SUFFIX 64
#include "Arcadia/Math/Implementation/SegmentSweep2.i"
#undef SW_SUFFIX
#undef SW_ENUM
#undef SW_INTERSECT
#undef SW_CALLBACK
#undef SW_EVENT
#undef SW_HIT
#undef SW_SEG
#undef SW_REAL
