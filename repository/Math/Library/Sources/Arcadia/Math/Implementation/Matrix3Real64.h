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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_MATRIX3REAL64_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_MATRIX3REAL64_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// Native, non-GC 3x3 matrix in row-major order: elements[row][column].
// Vectors are column vectors, so result = matrix * vector.
typedef struct Arcadia_Math_Matrix3Real64 {
  Arcadia_Real64Value elements[3][3];
} Arcadia_Math_Matrix3Real64;

void Arcadia_Math_Matrix3Real64_setIdentity(Arcadia_Math_Matrix3Real64* self);
void Arcadia_Math_Matrix3Real64_setScaling(Arcadia_Math_Matrix3Real64* self, Arcadia_Real64Value x, Arcadia_Real64Value y, Arcadia_Real64Value z);
void Arcadia_Math_Matrix3Real64_setValues(Arcadia_Math_Matrix3Real64* self, const Arcadia_Real64Value values[9]);
void Arcadia_Math_Matrix3Real64_copyValues(const Arcadia_Math_Matrix3Real64* self, Arcadia_Real64Value result[9]);
// All output functions permit aliasing of either operand (or both operands).
void Arcadia_Math_Matrix3Real64_multiply(Arcadia_Math_Matrix3Real64* result, const Arcadia_Math_Matrix3Real64* a, const Arcadia_Math_Matrix3Real64* b);
void Arcadia_Math_Matrix3Real64_add(Arcadia_Math_Matrix3Real64* result, const Arcadia_Math_Matrix3Real64* a, const Arcadia_Math_Matrix3Real64* b);
void Arcadia_Math_Matrix3Real64_subtract(Arcadia_Math_Matrix3Real64* result, const Arcadia_Math_Matrix3Real64* a, const Arcadia_Math_Matrix3Real64* b);
void Arcadia_Math_Matrix3Real64_transpose(Arcadia_Math_Matrix3Real64* result, const Arcadia_Math_Matrix3Real64* value);
// input and result may designate the same array.
void Arcadia_Math_Matrix3Real64_transformVector(const Arcadia_Math_Matrix3Real64* matrix, const Arcadia_Real64Value input[3], Arcadia_Real64Value result[3]);

#endif // ARCADIA_MATH_IMPLEMENTATION_MATRIX3REAL64_H_INCLUDED
