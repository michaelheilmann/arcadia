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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_RAYINTERSECTSPLANE_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_RAYINTERSECTSPLANE_H_INCLUDED

#include "Arcadia/Math/Implementation/Vector3Real32.h"
#include "Arcadia/Math/Implementation/Vector3Real64.h"

// Intersect origin + t * direction (t >= 0) with normal dot point + offset = 0.
// The direction and normal need not be normalized. Parallel and coplanar rays
// return false. On success, write the ray parameter t; on failure, leave it
// unchanged. No allocations are performed.
Arcadia_BooleanValue
Arcadia_Math_Vector3Real32_rayIntersectsPlane
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real32* origin,
    Arcadia_Math_Vector3Real32* direction,
    Arcadia_Math_Vector3Real32* normal,
    Arcadia_Real32Value offset,
    Arcadia_Real32Value* parameter
  );

Arcadia_BooleanValue
Arcadia_Math_Vector3Real64_rayIntersectsPlane
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real64* origin,
    Arcadia_Math_Vector3Real64* direction,
    Arcadia_Math_Vector3Real64* normal,
    Arcadia_Real64Value offset,
    Arcadia_Real64Value* parameter
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_RAYINTERSECTSPLANE_H_INCLUDED
