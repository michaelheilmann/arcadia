// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
// for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#if !defined(ARCADIA_MATH_IMPLEMENTATION_MATRIX3REAL32_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_MATRIX3REAL32_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// Native, non-GC 3x3 matrix in row-major order: elements[row][column].
// Vectors are column vectors, so result = matrix * vector.
typedef struct Arcadia_Math_Matrix3Real32 {
  Arcadia_Real32Value elements[3][3];
} Arcadia_Math_Matrix3Real32;

void
Arcadia_Math_Matrix3Real32_setIdentity
  (
    Arcadia_Math_Matrix3Real32* self
  );

void
Arcadia_Math_Matrix3Real32_setScaling
  (
    Arcadia_Math_Matrix3Real32* self,
    Arcadia_Real32Value x,
    Arcadia_Real32Value y,
    Arcadia_Real32Value z
  );

void
Arcadia_Math_Matrix3Real32_setValues
  (
    Arcadia_Math_Matrix3Real32* self,
    const Arcadia_Real32Value values[9]
  );

void
Arcadia_Math_Matrix3Real32_copyValues
  (
    const Arcadia_Math_Matrix3Real32* self,
    Arcadia_Real32Value result[9]
  );

// All output functions permit aliasing of either operand (or both operands).
void
Arcadia_Math_Matrix3Real32_multiply
  (
    Arcadia_Math_Matrix3Real32* result,
    const Arcadia_Math_Matrix3Real32* a,
    const Arcadia_Math_Matrix3Real32* b
  );

void
Arcadia_Math_Matrix3Real32_add
  (
    Arcadia_Math_Matrix3Real32* result,
    const Arcadia_Math_Matrix3Real32* a,
    const Arcadia_Math_Matrix3Real32* b
  );

void
Arcadia_Math_Matrix3Real32_subtract
  (
    Arcadia_Math_Matrix3Real32* result,
    const Arcadia_Math_Matrix3Real32* a,
    const Arcadia_Math_Matrix3Real32* b
  );

void
Arcadia_Math_Matrix3Real32_transpose
  (
    Arcadia_Math_Matrix3Real32* result,
    const Arcadia_Math_Matrix3Real32* value
  );

// input and result may designate the same array.
void
Arcadia_Math_Matrix3Real32_transformVector
  (
    const Arcadia_Math_Matrix3Real32* matrix,
    const Arcadia_Real32Value input[3],
    Arcadia_Real32Value result[3]
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_MATRIX3REAL32_H_INCLUDED
