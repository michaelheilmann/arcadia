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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_LERP_H_INCLUDE)
#define ARCADIA_MATH_IMPLEMENTATION_LERP_H_INCLUDE

#include "Arcadia/Math/Implementation/Color4Real32.h"
#include "Arcadia/Math/Implementation/Color4Real64.h"

#include "Arcadia/Math/Implementation/Vector3Real32.h"
#include "Arcadia/Math/Implementation/Vector3Real64.h"
#include "Arcadia/Math/Implementation/Vector4Real32.h"
#include "Arcadia/Math/Implementation/Vector4Real64.h"

// @brief Lerp between two colors.
// @param start The start position.
// @param end The end position.
// @param t The interpolation factor. If outside [0,1] the function extrapolates beyond the start point or end point.
// Extrapolations outside of [0,1] are clamped to [0,1].
void
Arcadia_Math_Color4Real32Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Color4Real32* self,
    Arcadia_Math_Color4Real32* other,
    Arcadia_Real32Value t,
    Arcadia_Math_Color4Real32* result
  );

// @brief Lerp between two colors.
// @param start The start position.
// @param end The end position.
// @param t The interpolation factor. If outside [0,1] the function extrapolates beyond the start point or end point.
// Extrapolations outside of [0,1] are clamped to [0,1].
void
Arcadia_Math_Color4Real64Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Color4Real64* self,
    Arcadia_Math_Color4Real64* other,
    Arcadia_Real64Value t,
    Arcadia_Math_Color4Real64* result
  );

void 
Arcadia_Math_Vector3Real32Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real32* self,
    Arcadia_Math_Vector3Real32* other,
    Arcadia_Real32Value t,
    Arcadia_Math_Vector3Real32* result
  );

void 
Arcadia_Math_Vector4Real32Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector4Real32* self,
    Arcadia_Math_Vector4Real32* other,
    Arcadia_Real32Value t,
    Arcadia_Math_Vector4Real32* result
  );

void
Arcadia_Math_Vector3Real64Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real64* self,
    Arcadia_Math_Vector3Real64* other,
    Arcadia_Real64Value t,
    Arcadia_Math_Vector3Real64* result
  );

void
Arcadia_Math_Vector4Real64Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector4Real64* self,
    Arcadia_Math_Vector4Real64* other,
    Arcadia_Real64Value t,
    Arcadia_Math_Vector4Real64* result
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_LERP_H_INCLUDE
