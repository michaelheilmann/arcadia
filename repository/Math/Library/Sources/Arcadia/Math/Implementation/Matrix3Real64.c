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

#include "Arcadia/Math/Implementation/Matrix3Real64.h"

void
Arcadia_Math_Matrix3Real64_setIdentity(Arcadia_Math_Matrix3Real64* self)
{
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      self->elements[i][j] = i == j ? 1 : 0;
    }
  }
}

void
Arcadia_Math_Matrix3Real64_setScaling(Arcadia_Math_Matrix3Real64* self, Arcadia_Real64Value x, Arcadia_Real64Value y, Arcadia_Real64Value z)
{
  Arcadia_Math_Matrix3Real64_setIdentity(self);
  self->elements[0][0] = x;
  self->elements[1][1] = y;
  self->elements[2][2] = z;
}

void
Arcadia_Math_Matrix3Real64_setValues(Arcadia_Math_Matrix3Real64* self, const Arcadia_Real64Value values[9])
{
  // Read all nine values before writing: values may point into self.
  Arcadia_Real64Value saved[9];
  for (Arcadia_SizeValue k = 0; k < 9; ++k) {
    saved[k] = values[k];
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      self->elements[i][j] = saved[3 * i + j];
    }
  }
}

void
Arcadia_Math_Matrix3Real64_copyValues(const Arcadia_Math_Matrix3Real64* self, Arcadia_Real64Value result[9])
{
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      result[3 * i + j] = self->elements[i][j];
    }
  }
}

void
Arcadia_Math_Matrix3Real64_multiply(Arcadia_Math_Matrix3Real64* result, const Arcadia_Math_Matrix3Real64* a, const Arcadia_Math_Matrix3Real64* b)
{
  Arcadia_Math_Matrix3Real64 temporary;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      Arcadia_Real64Value sum = 0;
      for (Arcadia_SizeValue k = 0; k < 3; ++k) {
        sum += a->elements[i][k] * b->elements[k][j];
      }
      temporary.elements[i][j] = sum;
    }
  }
  *result = temporary;
}

void
Arcadia_Math_Matrix3Real64_add(Arcadia_Math_Matrix3Real64* result, const Arcadia_Math_Matrix3Real64* a, const Arcadia_Math_Matrix3Real64* b)
{
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      result->elements[i][j] = a->elements[i][j] + b->elements[i][j];
    }
  }
}

void
Arcadia_Math_Matrix3Real64_subtract(Arcadia_Math_Matrix3Real64* result, const Arcadia_Math_Matrix3Real64* a, const Arcadia_Math_Matrix3Real64* b)
{
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      result->elements[i][j] = a->elements[i][j] - b->elements[i][j];
    }
  }
}

void
Arcadia_Math_Matrix3Real64_transpose(Arcadia_Math_Matrix3Real64* result, const Arcadia_Math_Matrix3Real64* value)
{
  Arcadia_Math_Matrix3Real64 temporary;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      temporary.elements[i][j] = value->elements[j][i];
    }
  }
  *result = temporary;
}

void
Arcadia_Math_Matrix3Real64_transformVector(const Arcadia_Math_Matrix3Real64* matrix, const Arcadia_Real64Value input[3], Arcadia_Real64Value result[3])
{
  Arcadia_Real64Value temporary[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    temporary[i] = matrix->elements[i][0] * input[0]
                 + matrix->elements[i][1] * input[1]
                 + matrix->elements[i][2] * input[2];
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    result[i] = temporary[i];
  }
}
