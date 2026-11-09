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

#include "Arcadia/Math/Implementation/NURBS3RefineReal32.h"
#include <math.h>

// Right-hand span on an interior knot; binary search avoids linear scans
// through long knot vectors while handling repeated knots deterministically.
static Arcadia_SizeValue
findSpan(const Arcadia_Real32Value* knots, Arcadia_SizeValue count,
    Arcadia_SizeValue degree, Arcadia_Real32Value parameter)
{
  Arcadia_SizeValue low = degree, high = count;
  while (low < high) {
    Arcadia_SizeValue middle = low + (high - low) / 2;
    if (parameter < knots[middle]) high = middle;
    else if (parameter >= knots[middle + 1]) low = middle + 1;
    else return middle;
  }
  return count - 1;
}

// Compute the rational homogeneous blend, then divide back to 3D.
// Evaluate into a stack temporary to avoid partially storing a point.
static Arcadia_BooleanValue
blend(const Arcadia_Real32Value a[3], Arcadia_Real32Value wa,
    const Arcadia_Real32Value b[3], Arcadia_Real32Value wb,
    double alpha, Arcadia_Real32Value result[3], Arcadia_Real32Value* weight)
{
  double w = (1. - alpha) * (double)wa + alpha * (double)wb;
  if (!(w > 0.) || !isfinite(w)) return Arcadia_BooleanValue_False;
  Arcadia_Real32Value convertedWeight = (Arcadia_Real32Value)w;
  if (!(convertedWeight > 0) || !isfinite(convertedWeight))
    return Arcadia_BooleanValue_False;
  Arcadia_Real32Value temporary[3];
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    double h = (1. - alpha) * (double)wa * (double)a[k] +
               alpha * (double)wb * (double)b[k];
    double q = h / w;
    temporary[k] = (Arcadia_Real32Value)q;
    if (!isfinite(h) || !isfinite(q) || !isfinite(temporary[k]))
      return Arcadia_BooleanValue_False;
  }
  for (Arcadia_SizeValue k = 0; k < 3; ++k) result[k] = temporary[k];
  *weight = convertedWeight;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3RefineReal32_insertKnot(
    const Arcadia_Real32Value (*points)[3], const Arcadia_Real32Value* weights,
    Arcadia_SizeValue count, Arcadia_SizeValue degree,
    const Arcadia_Real32Value* knots, Arcadia_SizeValue knotCount,
    Arcadia_Real32Value parameter,
    Arcadia_Real32Value (*newPoints)[3], Arcadia_Real32Value* newWeights,
    Arcadia_SizeValue pointCapacity,
    Arcadia_Real32Value* newKnots, Arcadia_SizeValue knotCapacity)
{
  const Arcadia_SizeValue maximum = (Arcadia_SizeValue)-1;
  if (!points || !knots || !newPoints || !newKnots ||
      (weights && !newWeights) || !isfinite(parameter) ||
      count == 0 || count == maximum || degree >= count ||
      knotCount <= count || knotCount == maximum ||
      knotCount - count - 1 != degree ||
      pointCapacity < count + 1 || knotCapacity < knotCount + 1)
    return Arcadia_BooleanValue_False;

  for (Arcadia_SizeValue i = 0; i < knotCount; ++i)
    if (!isfinite(knots[i]) || (i && knots[i] < knots[i-1]))
      return Arcadia_BooleanValue_False;
  if (!(parameter > knots[degree] && parameter < knots[count]))
    return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    if (weights && (!isfinite(weights[i]) || !(weights[i] > 0)))
      return Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue j = 0; j < 3; ++j)
      if (!isfinite(points[i][j])) return Arcadia_BooleanValue_False;
  }

  Arcadia_SizeValue span = findSpan(knots, count, degree, parameter);
  Arcadia_SizeValue multiplicity = 0;
  for (Arcadia_SizeValue i = 0; i < knotCount; ++i)
    if (knots[i] == parameter) ++multiplicity;
  if (multiplicity > degree) return Arcadia_BooleanValue_False;

  // The de Boor knot-insertion convex blend is defined for i in
  // (span-degree, span-multiplicity]. It leaves other controls unchanged.
  // First pass calculates all possible output changes without modifying any
  // caller buffers. It also catches overflow/underflow in rational weights.
  for (Arcadia_SizeValue i = span - degree + 1; i <= span - multiplicity; ++i) {
    double denominator = (double)knots[i + degree] - (double)knots[i];
    double alpha = ((double)parameter - (double)knots[i]) / denominator;
    if (!(denominator > 0.) || !isfinite(alpha) || alpha < 0. || alpha > 1.)
      return Arcadia_BooleanValue_False;
    Arcadia_Real32Value candidate[3], w;
    Arcadia_Real32Value wa = weights ? weights[i - 1] : (Arcadia_Real32Value)1;
    Arcadia_Real32Value wb = weights ? weights[i] : (Arcadia_Real32Value)1;
    if (!blend(points[i-1], wa, points[i], wb, alpha, candidate, &w))
      return Arcadia_BooleanValue_False;
  }

  for (Arcadia_SizeValue i = 0; i <= span; ++i) newKnots[i] = knots[i];
  newKnots[span+1] = parameter;
  for (Arcadia_SizeValue i = span+1; i < knotCount; ++i)
    newKnots[i+1] = knots[i];

  // The two copy ranges are disjoint for degree > 0, but for degree zero
  // their common boundary intentionally duplicates the same control point.
  for (Arcadia_SizeValue i = 0; i <= span-degree; ++i) {
    for (Arcadia_SizeValue j = 0; j < 3; ++j) newPoints[i][j] = points[i][j];
    if (newWeights) newWeights[i] = weights ? weights[i] : (Arcadia_Real32Value)1;
  }
  for (Arcadia_SizeValue i = span - multiplicity; i < count; ++i) {
    for (Arcadia_SizeValue j = 0; j < 3; ++j) newPoints[i+1][j] = points[i][j];
    if (newWeights) newWeights[i+1] = weights ? weights[i] : (Arcadia_Real32Value)1;
  }
  for (Arcadia_SizeValue i = span-degree+1; i <= span-multiplicity; ++i) {
    double alpha = ((double)parameter - (double)knots[i]) /
      ((double)knots[i+degree] - (double)knots[i]);
    Arcadia_Real32Value value[3], w;
    Arcadia_Real32Value wa = weights ? weights[i-1] : (Arcadia_Real32Value)1;
    Arcadia_Real32Value wb = weights ? weights[i] : (Arcadia_Real32Value)1;
    // Already validated: these local operations cannot fail on the second pass.
    if (!blend(points[i-1], wa, points[i], wb, alpha, value, &w))
      return Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue j = 0; j < 3; ++j) newPoints[i][j] = value[j];
    if (newWeights) newWeights[i] = w;
  }
  return Arcadia_BooleanValue_True;
}
