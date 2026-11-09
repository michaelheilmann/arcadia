// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Arcadia/Math/Implementation/NURBS3ControlReal64.h"

#include <math.h>

static Arcadia_BooleanValue
validCount(Arcadia_SizeValue countU, Arcadia_SizeValue countV,
           Arcadia_SizeValue* count)
{
  if (!countU || !countV || countU > ((Arcadia_SizeValue)-1) / countV)
    return Arcadia_BooleanValue_False;
  *count = countU * countV;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlReal64_computeAABB(const Arcadia_Real64Value (*points)[3],
    Arcadia_SizeValue countU, Arcadia_SizeValue countV, Arcadia_Math_AABB3Real64* result)
{
  Arcadia_SizeValue count;
  if (!points || !result || !validCount(countU, countV, &count))
    return Arcadia_BooleanValue_False;
  Arcadia_Real64Value lo[3], hi[3];
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    if (!isfinite(points[0][k])) return Arcadia_BooleanValue_False;
    lo[k] = hi[k] = points[0][k];
  }
  for (Arcadia_SizeValue i = 1; i < count; ++i)
    for (Arcadia_SizeValue k = 0; k < 3; ++k) {
      Arcadia_Real64Value p = points[i][k];
      if (!isfinite(p)) return Arcadia_BooleanValue_False;
      if (p < lo[k]) lo[k] = p;
      if (p > hi[k]) hi[k] = p;
    }
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    result->minimum[k] = lo[k];
    result->maximum[k] = hi[k];
  }
  result->empty = Arcadia_BooleanValue_False;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlReal64_normalize(const Arcadia_Real64Value (*points)[3],
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    Arcadia_Real64Value (*result)[3], Arcadia_SizeValue resultCapacity)
{
  Arcadia_SizeValue count;
  Arcadia_Math_AABB3Real64 hull;
  if (!result || !validCount(countU, countV, &count) ||
      resultCapacity < count ||
      !Arcadia_Math_NURBS3ControlReal64_computeAABB(points, countU, countV, &hull))
    return Arcadia_BooleanValue_False;
  // Double intermediates are deliberate for Real32; Real64 coordinates with
  // unrepresentable extents are rejected before any result is written.
  double middle[3], extent[3], maximum = 0.;
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    double lo = (double)hull.minimum[k], hi = (double)hull.maximum[k];
    middle[k] = lo * 0.5 + hi * 0.5;
    extent[k] = hi - lo;
    if (!isfinite(middle[k]) || !isfinite(extent[k]))
      return Arcadia_BooleanValue_False;
    if (extent[k] > maximum) maximum = extent[k];
  }
  double factor = maximum > 0. ? 2. / maximum : 0.;
  if (!isfinite(factor)) return Arcadia_BooleanValue_False;
  // Preflight all values. This guarantees no partial writes, including when
  // the result array is the same as the input control lattice.
  for (Arcadia_SizeValue i = 0; i < count; ++i)
    for (Arcadia_SizeValue k = 0; k < 3; ++k) {
      double value = ((double)points[i][k] - middle[k]) * factor;
      if (!isfinite(value) || !isfinite((Arcadia_Real64Value)value))
        return Arcadia_BooleanValue_False;
    }
  for (Arcadia_SizeValue i = 0; i < count; ++i)
    for (Arcadia_SizeValue k = 0; k < 3; ++k)
      result[i][k] = (Arcadia_Real64Value)(((double)points[i][k] - middle[k]) * factor);
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlReal64_transform(const Arcadia_Real64Value (*points)[3],
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    const Arcadia_Math_Affine3Real64* transform, Arcadia_Real64Value (*result)[3],
    Arcadia_SizeValue resultCapacity)
{
  Arcadia_SizeValue count;
  if (!points || !transform || !result ||
      !validCount(countU, countV, &count) || resultCapacity < count)
    return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < 3; ++i)
    for (Arcadia_SizeValue j = 0; j < 4; ++j)
      if (!isfinite(transform->elements[i][j])) return Arcadia_BooleanValue_False;
  // Preflight conversion and inputs before any output writes.
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    for (Arcadia_SizeValue k = 0; k < 3; ++k)
      if (!isfinite(points[i][k])) return Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue row = 0; row < 3; ++row) {
      double value = (double)transform->elements[row][3];
      for (Arcadia_SizeValue k = 0; k < 3; ++k)
        value += (double)transform->elements[row][k] * (double)points[i][k];
      if (!isfinite(value) || !isfinite((Arcadia_Real64Value)value)) return Arcadia_BooleanValue_False;
    }
  }
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    Arcadia_Real64Value next[3];
    for (Arcadia_SizeValue row = 0; row < 3; ++row) {
      double value = (double)transform->elements[row][3];
      for (Arcadia_SizeValue k = 0; k < 3; ++k)
        value += (double)transform->elements[row][k] * (double)points[i][k];
      next[row] = (Arcadia_Real64Value)value;
    }
    for (Arcadia_SizeValue k = 0; k < 3; ++k) result[i][k] = next[k];
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlReal64_pickControl(const Arcadia_Real64Value (*points)[3],
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    const Arcadia_Math_Ray3Real64* ray, Arcadia_Real64Value radius,
    Arcadia_SizeValue* index, Arcadia_Real64Value* t)
{
  Arcadia_SizeValue count;
  if (!points || !ray || !index || !validCount(countU, countV, &count) ||
      !isfinite(radius) || !(radius > (Arcadia_Real64Value)0))
    return Arcadia_BooleanValue_False;
  double direction[3], origin[3], length2 = 0.;
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    if (!isfinite(ray->origin[k]) || !isfinite(ray->direction[k]))
      return Arcadia_BooleanValue_False;
    direction[k] = (double)ray->direction[k];
    origin[k] = (double)ray->origin[k];
    length2 += direction[k] * direction[k];
  }
  if (!(length2 > 0.) || !isfinite(length2)) return Arcadia_BooleanValue_False;
  double radius2 = (double)radius * (double)radius;
  if (!isfinite(radius2)) return Arcadia_BooleanValue_False;
  // Reject invalid control coordinates even if an earlier control is hit.
  for (Arcadia_SizeValue i = 0; i < count; ++i)
    for (Arcadia_SizeValue k = 0; k < 3; ++k)
      if (!isfinite(points[i][k])) return Arcadia_BooleanValue_False;
  Arcadia_BooleanValue found = Arcadia_BooleanValue_False;
  double bestT = 0.;
  Arcadia_SizeValue bestIndex = 0;
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    double delta[3], projection = 0.;
    for (Arcadia_SizeValue k = 0; k < 3; ++k) {
      delta[k] = (double)points[i][k] - origin[k];
      projection += delta[k] * direction[k];
    }
    if (!isfinite(projection)) return Arcadia_BooleanValue_False;
    double candidate = projection > 0. ? projection / length2 : 0.;
    if (!isfinite(candidate)) return Arcadia_BooleanValue_False;
    double distance2 = 0.;
    for (Arcadia_SizeValue k = 0; k < 3; ++k) {
      double remainder = delta[k] - candidate * direction[k];
      distance2 += remainder * remainder;
    }
    if (!isfinite(distance2)) return Arcadia_BooleanValue_False;
    if (distance2 <= radius2 && (!found || candidate < bestT)) {
      bestT = candidate;
      bestIndex = i;
      found = Arcadia_BooleanValue_True;
    }
  }
  if (!found || !isfinite((Arcadia_Real64Value)bestT)) return Arcadia_BooleanValue_False;
  *index = bestIndex;
  if (t) *t = (Arcadia_Real64Value)bestT;
  return Arcadia_BooleanValue_True;
}
