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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_TRANSFORMPOINT_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_TRANSFORMPOINT_H_INCLUDED

#include "Arcadia/Math/Implementation/Matrix4Real32.h"
#include "Arcadia/Math/Implementation/Matrix4Real64.h"
#include "Arcadia/Math/Implementation/Vector3Real32.h"
#include "Arcadia/Math/Implementation/Vector3Real64.h"

/// @brief Transform a point in place by self * (x, y, z, 1).
/// @remarks self is an affine matrix. There is no perspective divide.
void
Arcadia_Math_Matrix4x4Real32_transformPoint
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Math_Vector3Real32* point
  );

/// @brief Transform a point in place by self * (x, y, z, 1).
/// @remarks self is an affine matrix. There is no perspective divide.
void
Arcadia_Math_Matrix4x4Real64_transformPoint
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real64* self,
    Arcadia_Math_Vector3Real64* point
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_TRANSFORMPOINT_H_INCLUDED
