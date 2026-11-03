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

#include "Arcadia/Math/Implementation/Lagrange3.h"

#include <math.h>

Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real64_setUniformKnots(Arcadia_SizeValue count, Arcadia_Real64Value* knots)
{
  if (!knots || count == 0) return Arcadia_BooleanValue_False;
  if (count == 1) {
    knots[0] = 0;
    return Arcadia_BooleanValue_True;
  }
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    knots[i] = (Arcadia_Real64Value)((double)i / (double)(count - 1));
  }
  knots[count - 1] = 1;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real64_setChordLengthKnots(const Arcadia_Real64Value (*points)[3], Arcadia_SizeValue count, Arcadia_Real64Value* knots)
{
  if (!points || !knots || count == 0) return Arcadia_BooleanValue_False;
  if (count == 1) return Arcadia_Math_Lagrange3Real64_setUniformKnots(count, knots);
  // Use double intermediates for Real32 inputs, avoiding length truncation.
  double total = 0.;
  for (Arcadia_SizeValue i = 1; i < count; ++i) {
    double dx = (double)points[i][0] - (double)points[i - 1][0];
    double dy = (double)points[i][1] - (double)points[i - 1][1];
    double dz = (double)points[i][2] - (double)points[i - 1][2];
    double segment = hypot(hypot(dx, dy), dz);
    if (!(segment > 0.) || !isfinite(segment))
      return Arcadia_Math_Lagrange3Real64_setUniformKnots(count, knots);
    total += segment;
  }
  if (!(total > 0.) || !isfinite(total))
    return Arcadia_Math_Lagrange3Real64_setUniformKnots(count, knots);
  double accumulated = 0.;
  knots[0] = 0;
  for (Arcadia_SizeValue i = 1; i + 1 < count; ++i) {
    double dx = (double)points[i][0] - (double)points[i - 1][0];
    double dy = (double)points[i][1] - (double)points[i - 1][1];
    double dz = (double)points[i][2] - (double)points[i - 1][2];
    accumulated += hypot(hypot(dx, dy), dz);
    Arcadia_Real64Value t = (Arcadia_Real64Value)(accumulated / total);
    // If rounding would produce duplicate knots, choose safe spacing.
    if (!(t > knots[i - 1]) || !(t < (Arcadia_Real64Value)1))
      return Arcadia_Math_Lagrange3Real64_setUniformKnots(count, knots);
    knots[i] = t;
  }
  knots[count - 1] = 1;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real64_computeWeights(const Arcadia_Real64Value* knots, Arcadia_SizeValue count, Arcadia_Real64Value* weights)
{
  if (!knots || !weights || count == 0) return Arcadia_BooleanValue_False;
  // Validate before writing, so repeated nodes cannot damage old weights.
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    for (Arcadia_SizeValue j = i + 1; j < count; ++j) {
      if (!(knots[i] < knots[j] || knots[i] > knots[j]))
        return Arcadia_BooleanValue_False;
    }
  }
  // Global scaling is immaterial to barycentric interpolation. For the
  // modest-degree curves used by the legacy editor, reciprocals are adequate.
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    double weight = 1.;
    for (Arcadia_SizeValue j = 0; j < count; ++j) {
      if (i != j) weight /= (double)knots[i] - (double)knots[j];
    }
    if (!isfinite(weight) || (weight == 0.)) return Arcadia_BooleanValue_False;
    weights[i] = (Arcadia_Real64Value)weight;
    if (!isfinite((double)weights[i]) || weights[i] == (Arcadia_Real64Value)0)
      return Arcadia_BooleanValue_False;
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real64_evaluate(const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* knots, const Arcadia_Real64Value* weights,
                    Arcadia_SizeValue count, Arcadia_Real64Value t, Arcadia_Real64Value result[3])
{
  if (!points || !knots || !weights || !result || count == 0)
    return Arcadia_BooleanValue_False;
  if (!isfinite((double)t)) return Arcadia_BooleanValue_False;
  // Exact knot interpolation avoids division by zero and round-off.
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    if (t == knots[i]) {
      Arcadia_Real64Value x = points[i][0], y = points[i][1], z = points[i][2];
      result[0] = x; result[1] = y; result[2] = z;
      return Arcadia_BooleanValue_True;
    }
  }
  double denominator = 0., x = 0., y = 0., z = 0.;
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    double coefficient = (double)weights[i] / ((double)t - (double)knots[i]);
    denominator += coefficient;
    x += coefficient * (double)points[i][0];
    y += coefficient * (double)points[i][1];
    z += coefficient * (double)points[i][2];
  }
  if (denominator == 0. || !isfinite(denominator))
    return Arcadia_BooleanValue_False;
  x /= denominator; y /= denominator; z /= denominator;
  if (!isfinite(x) || !isfinite(y) || !isfinite(z))
    return Arcadia_BooleanValue_False;
  result[0] = (Arcadia_Real64Value)x;
  result[1] = (Arcadia_Real64Value)y;
  result[2] = (Arcadia_Real64Value)z;
  return Arcadia_BooleanValue_True;
}
