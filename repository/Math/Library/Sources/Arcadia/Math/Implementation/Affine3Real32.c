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

#include "Arcadia/Math/Implementation/Affine3Real32.h"

#include <assert.h>
#include <math.h>

static Arcadia_BooleanValue
finiteMatrix(const Arcadia_Math_Affine3Real32* matrix)
{
  if (!matrix) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < 3; ++i)
    for (Arcadia_SizeValue j = 0; j < 4; ++j)
      if (!isfinite(matrix->elements[i][j])) return Arcadia_BooleanValue_False;
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
store3(const double values[3], Arcadia_Real32Value output[3])
{
  Arcadia_Real32Value converted[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    if (!isfinite(values[i])) return Arcadia_BooleanValue_False;
    converted[i] = (Arcadia_Real32Value)values[i];
    if (!isfinite(converted[i])) return Arcadia_BooleanValue_False;
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) output[i] = converted[i];
  return Arcadia_BooleanValue_True;
}

void
Arcadia_Math_Affine3Real32_setIdentity(Arcadia_Math_Affine3Real32* self)
{
  assert(self);
  for (Arcadia_SizeValue i = 0; i < 3; ++i)
    for (Arcadia_SizeValue j = 0; j < 4; ++j)
      self->elements[i][j] = i == j ? (Arcadia_Real32Value)1 : (Arcadia_Real32Value)0;
}

Arcadia_BooleanValue
Arcadia_Math_Affine3Real32_setValues(Arcadia_Math_Affine3Real32* self, const Arcadia_Real32Value values[12])
{
  if (!self || !values) return Arcadia_BooleanValue_False;
  Arcadia_Math_Affine3Real32 temporary;
  for (Arcadia_SizeValue i = 0; i < 3; ++i)
    for (Arcadia_SizeValue j = 0; j < 4; ++j) {
      Arcadia_Real32Value value = values[i * 4 + j];
      if (!isfinite(value)) return Arcadia_BooleanValue_False;
      temporary.elements[i][j] = value;
    }
  *self = temporary;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Affine3Real32_multiply(Arcadia_Math_Affine3Real32* result, const Arcadia_Math_Affine3Real32* left, const Arcadia_Math_Affine3Real32* right)
{
  if (!result || !finiteMatrix(left) || !finiteMatrix(right)) return Arcadia_BooleanValue_False;
  Arcadia_Math_Affine3Real32 temporary;
  for (Arcadia_SizeValue i = 0; i < 3; ++i)
    for (Arcadia_SizeValue j = 0; j < 4; ++j) {
      double sum = j == 3 ? (double)left->elements[i][3] : 0.;
      for (Arcadia_SizeValue k = 0; k < 3; ++k)
        sum += (double)left->elements[i][k] * (double)right->elements[k][j];
      if (!isfinite(sum)) return Arcadia_BooleanValue_False;
      temporary.elements[i][j] = (Arcadia_Real32Value)sum;
      if (!isfinite(temporary.elements[i][j])) return Arcadia_BooleanValue_False;
    }
  *result = temporary;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Affine3Real32_inverse(Arcadia_Math_Affine3Real32* result, const Arcadia_Math_Affine3Real32* matrix)
{
  if (!result || !finiteMatrix(matrix)) return Arcadia_BooleanValue_False;
  // Gauss-Jordan elimination with partial pivoting on [A | I], plus translation.
  double a[3][6] = { {0} };
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    for (Arcadia_SizeValue j = 0; j < 3; ++j) a[i][j] = matrix->elements[i][j];
    a[i][i + 3] = 1.;
  }
  for (Arcadia_SizeValue pivot = 0; pivot < 3; ++pivot) {
    Arcadia_SizeValue best = pivot;
    for (Arcadia_SizeValue r = pivot + 1; r < 3; ++r)
      if (fabs(a[r][pivot]) > fabs(a[best][pivot])) best = r;
    if (!(fabs(a[best][pivot]) > 0.) || !isfinite(a[best][pivot])) return Arcadia_BooleanValue_False;
    if (best != pivot)
      for (Arcadia_SizeValue j = 0; j < 6; ++j) {
        double value = a[pivot][j]; a[pivot][j] = a[best][j]; a[best][j] = value;
      }
    double divisor = a[pivot][pivot];
    for (Arcadia_SizeValue j = 0; j < 6; ++j) a[pivot][j] /= divisor;
    for (Arcadia_SizeValue r = 0; r < 3; ++r) {
      if (r == pivot) continue;
      double factor = a[r][pivot];
      for (Arcadia_SizeValue j = 0; j < 6; ++j) a[r][j] -= factor * a[pivot][j];
    }
  }
  Arcadia_Math_Affine3Real32 temporary;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    double shift = 0.;
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      double value = a[i][j + 3];
      if (!isfinite(value)) return Arcadia_BooleanValue_False;
      temporary.elements[i][j] = (Arcadia_Real32Value)value;
      if (!isfinite(temporary.elements[i][j])) return Arcadia_BooleanValue_False;
      shift -= value * (double)matrix->elements[j][3];
    }
    if (!isfinite(shift)) return Arcadia_BooleanValue_False;
    temporary.elements[i][3] = (Arcadia_Real32Value)shift;
    if (!isfinite(temporary.elements[i][3])) return Arcadia_BooleanValue_False;
  }
  *result = temporary;
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
transform3(const Arcadia_Math_Affine3Real32* matrix, const Arcadia_Real32Value input[3], Arcadia_Real32Value result[3], Arcadia_BooleanValue includeTranslation)
{
  if (!input || !result || !finiteMatrix(matrix)) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < 3; ++i)
    if (!isfinite(input[i])) return Arcadia_BooleanValue_False;
  double next[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    double sum = includeTranslation ? (double)matrix->elements[i][3] : 0.;
    for (Arcadia_SizeValue j = 0; j < 3; ++j)
      sum += (double)matrix->elements[i][j] * (double)input[j];
    next[i] = sum;
  }
  return store3(next, result);
}

Arcadia_BooleanValue
Arcadia_Math_Affine3Real32_transformPoint(const Arcadia_Math_Affine3Real32* matrix, const Arcadia_Real32Value point[3], Arcadia_Real32Value result[3])
{
  return transform3(matrix, point, result, Arcadia_BooleanValue_True);
}

Arcadia_BooleanValue
Arcadia_Math_Affine3Real32_transformDirection(const Arcadia_Math_Affine3Real32* matrix, const Arcadia_Real32Value direction[3], Arcadia_Real32Value result[3])
{
  return transform3(matrix, direction, result, Arcadia_BooleanValue_False);
}

Arcadia_BooleanValue
Arcadia_Math_Affine3Real32_transformPlane(const Arcadia_Math_Affine3Real32* matrix, const Arcadia_Math_Plane3Real32* plane, Arcadia_Math_Plane3Real32* result)
{
  if (!plane || !result) return Arcadia_BooleanValue_False;
  Arcadia_Math_Affine3Real32 inverse;
  if (!Arcadia_Math_Affine3Real32_inverse(&inverse, matrix) || !isfinite(plane->offset)) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue k = 0; k < 3; ++k)
    if (!isfinite(plane->normal[k])) return Arcadia_BooleanValue_False;
  double normal[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    normal[i] = 0.;
    for (Arcadia_SizeValue j = 0; j < 3; ++j)
      normal[i] += (double)inverse.elements[j][i] * (double)plane->normal[j];
  }
  double offset = (double)plane->offset;
  for (Arcadia_SizeValue i = 0; i < 3; ++i)
    offset += (double)inverse.elements[i][3] * (double)plane->normal[i];
  Arcadia_Math_Plane3Real32 temporary;
  if (!store3(normal, temporary.normal) || !isfinite(offset)) return Arcadia_BooleanValue_False;
  temporary.offset = (Arcadia_Real32Value)offset;
  if (!isfinite(temporary.offset)) return Arcadia_BooleanValue_False;
  // Singular affine transformations are rejected by inverse().
  *result = temporary;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Affine3Real32_transformRay(const Arcadia_Math_Affine3Real32* matrix, const Arcadia_Math_Ray3Real32* ray, Arcadia_Math_Ray3Real32* result)
{
  if (!ray || !result) return Arcadia_BooleanValue_False;
  Arcadia_Math_Ray3Real32 temporary;
  if (!Arcadia_Math_Affine3Real32_transformPoint(matrix, ray->origin, temporary.origin) ||
      !Arcadia_Math_Affine3Real32_transformDirection(matrix, ray->direction, temporary.direction)) return Arcadia_BooleanValue_False;
  double squaredLength = 0.;
  for (Arcadia_SizeValue i = 0; i < 3; ++i)
    squaredLength += (double)temporary.direction[i] * (double)temporary.direction[i];
  if (!(squaredLength > 0.) || !isfinite(squaredLength)) return Arcadia_BooleanValue_False;
  *result = temporary;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Affine3Real32_transformAABB(const Arcadia_Math_Affine3Real32* matrix, const Arcadia_Math_AABB3Real32* bounds, Arcadia_Math_AABB3Real32* result)
{
  if (!bounds || !result || !finiteMatrix(matrix)) return Arcadia_BooleanValue_False;
  Arcadia_Math_AABB3Real32 next;
  if (bounds->empty) {
    Arcadia_Math_AABB3Real32_setEmpty(&next);
    *result = next;
    return Arcadia_BooleanValue_True;
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i)
    if (!isfinite(bounds->minimum[i]) || !isfinite(bounds->maximum[i]) ||
        bounds->minimum[i] > bounds->maximum[i]) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue corner = 0; corner < 8; ++corner) {
    Arcadia_Real32Value input[3], point[3];
    for (Arcadia_SizeValue i = 0; i < 3; ++i)
      input[i] = corner & (1u << i) ? bounds->maximum[i] : bounds->minimum[i];
    if (!Arcadia_Math_Affine3Real32_transformPoint(matrix, input, point)) return Arcadia_BooleanValue_False;
    if (corner == 0) Arcadia_Math_AABB3Real32_setMinMax(&next, point, point);
    else Arcadia_Math_AABB3Real32_includePoint(&next, point);
  }
  *result = next;
  return Arcadia_BooleanValue_True;
}
