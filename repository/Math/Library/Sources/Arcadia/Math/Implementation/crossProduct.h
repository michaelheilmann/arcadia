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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_CROSSPRODUCT_H_INCLUDE)
#define ARCADIA_MATH_IMPLEMENTATION_CROSSPRODUCT_H_INCLUDE

#include "Arcadia/Math/Implementation/Vector3Real32.h"
#include "Arcadia/Math/Implementation/Vector3Real64.h"

/// @{

/// @brief Compute the cross product of this vector and another vector.
/// @param u The first vector.
/// @param v The other vector.
/// @return The cross product of the vector.
/// @return A corresponding angle in degrees.
Arcadia_Math_Vector3Real32*
Arcadia_Math_Vector3Real32_crossProduct
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real32* u,
    Arcadia_Math_Vector3Real32* v
  );

Arcadia_Math_Vector3Real64*
Arcadia_Math_Vector3Real64_crossProduct
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real64* u,
    Arcadia_Math_Vector3Real64* v
  );

/// @}

#endif // ARCADIA_MATH_IMPLEMENTATION_CROSSPRODUCT_H_INCLUDE
