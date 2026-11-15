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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_RAYINTERSECTSSPHERE_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_RAYINTERSECTSSPHERE_H_INCLUDED

#include "Arcadia/Math/Implementation/Vector3Real32.h"
#include "Arcadia/Math/Implementation/Vector3Real64.h"

// Return true if the ray origin + t * direction, t >= 0, intersects the sphere.
// The direction need not be normalized. A zero direction or negative radius
// is considered invalid and produces false. No allocations are performed.
Arcadia_BooleanValue
Arcadia_Math_Vector3Real32_rayIntersectsSphere
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real32* origin,
    Arcadia_Math_Vector3Real32* direction,
    Arcadia_Math_Vector3Real32* center,
    Arcadia_Real32Value radius
  );

Arcadia_BooleanValue
Arcadia_Math_Vector3Real64_rayIntersectsSphere
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real64* origin,
    Arcadia_Math_Vector3Real64* direction,
    Arcadia_Math_Vector3Real64* center,
    Arcadia_Real64Value radius
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_RAYINTERSECTSSPHERE_H_INCLUDED
