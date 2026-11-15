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

#include "Arcadia/Math/Implementation/NURBS3Real64.h"

#include <math.h>

// Find the right-hand knot span, except that the final domain endpoint is
// assigned to the final span. This resolves repeated interior knots and avoids
// double-counting knot-boundary basis functions in the old M0 evaluator.
static Arcadia_SizeValue
findSpan(const Arcadia_Real64Value* knots, Arcadia_SizeValue count,
         Arcadia_SizeValue degree, Arcadia_Real64Value parameter)
{
  if (parameter == knots[count]) return count - 1;
  Arcadia_SizeValue low = degree;
  Arcadia_SizeValue high = count;
  while (low < high) {
    Arcadia_SizeValue mid = low + (high - low) / 2;
    if (parameter < knots[mid]) {
      high = mid;
    } else if (parameter >= knots[mid + 1]) {
      low = mid + 1;
    } else {
      return mid;
    }
  }
  return count - 1;
}

// Compute the nonzero B-spline basis functions iteratively (Cox-de Boor).
// Caller provides three adjacent scratch arrays, each degree+1 long.
static void
basisFunctions(const Arcadia_Real64Value* knots, Arcadia_SizeValue degree,
               Arcadia_SizeValue span, Arcadia_Real64Value parameter,
               Arcadia_Real64Value* basis,
               Arcadia_Real64Value* left,
               Arcadia_Real64Value* right)
{
  basis[0] = (Arcadia_Real64Value)1;
  for (Arcadia_SizeValue j = 1; j <= degree; ++j) {
    left[j] = parameter - knots[span + 1 - j];
    right[j] = knots[span + j] - parameter;
    Arcadia_Real64Value saved = (Arcadia_Real64Value)0;
    for (Arcadia_SizeValue r = 0; r < j; ++r) {
      Arcadia_Real64Value divisor = right[r + 1] + left[j - r];
      Arcadia_Real64Value temp = divisor != (Arcadia_Real64Value)0
                                   ? basis[r] / divisor : (Arcadia_Real64Value)0;
      basis[r] = saved + right[r + 1] * temp;
      saved = left[j - r] * temp;
    }
    basis[j] = saved;
  }
}

// Validate all knot intervals, including zero-width intervals from repeated
// knots, and reject zero-width domains and out-of-domain parameters.
static Arcadia_BooleanValue
validKnots(Arcadia_SizeValue count, Arcadia_SizeValue degree,
           const Arcadia_Real64Value* knots, Arcadia_SizeValue knotCount,
           Arcadia_Real64Value parameter)
{
  if (!knots || !isfinite(parameter) || !count || degree >= count ||
      knotCount <= count || knotCount - count - 1 != degree) {
    return Arcadia_BooleanValue_False;
  }
  for (Arcadia_SizeValue i = 0; i < knotCount; ++i) {
    if (!isfinite(knots[i]) || (i > 0 && knots[i] < knots[i - 1])) {
      return Arcadia_BooleanValue_False;
    }
  }
  if (knots[degree] >= knots[count] ||
      parameter < knots[degree] || parameter > knots[count]) {
    return Arcadia_BooleanValue_False;
  }
  return Arcadia_BooleanValue_True;
}

// Accumulates active homogeneous coordinates using double intermediates.
// Converts only at the very end so even a failure cannot partially overwrite
// a control point if result aliases it.
static Arcadia_BooleanValue
storeResult(const double numerator[3], double denominator,
            Arcadia_Real64Value result[3])
{
  Arcadia_Real64Value values[3];
  if (!(denominator > 0.) || !isfinite(denominator)) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    double value = numerator[k] / denominator;
    if (!isfinite(value)) return Arcadia_BooleanValue_False;
    values[k] = (Arcadia_Real64Value)value;
    if (!isfinite(values[k])) return Arcadia_BooleanValue_False;
  }
  for (Arcadia_SizeValue k = 0; k < 3; ++k) result[k] = values[k];
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3Real64_makeOpenUniformKnots
  (
    Arcadia_SizeValue count,
    Arcadia_SizeValue degree,
    Arcadia_Real64Value* knots,
    Arcadia_SizeValue knotCount
  )
{
  if (!knots || !count || degree >= count ||
      knotCount <= count || knotCount - count - 1 != degree) {
    return Arcadia_BooleanValue_False;
  }
  for (Arcadia_SizeValue i = 0; i < knotCount; ++i) {
    if (i <= degree) {
      knots[i] = (Arcadia_Real64Value)0;
    } else if (i >= count) {
      knots[i] = (Arcadia_Real64Value)1;
    } else {
      knots[i] = (Arcadia_Real64Value)(i - degree) /
                 (Arcadia_Real64Value)(count - degree);
    }
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3Real64_evaluateCurve
  (
    const Arcadia_Real64Value (*points)[3],
    const Arcadia_Real64Value* weights,
    Arcadia_SizeValue count,
    Arcadia_SizeValue degree,
    const Arcadia_Real64Value* knots,
    Arcadia_SizeValue knotCount,
    Arcadia_Real64Value parameter,
    Arcadia_Real64Value* workspace,
    Arcadia_SizeValue workspaceElements,
    Arcadia_Real64Value result[3]
  )
{
  if (!points || !workspace || !result ||
      !validKnots(count, degree, knots, knotCount, parameter) ||
      workspaceElements / 3 < degree + 1) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_SizeValue span = findSpan(knots, count, degree, parameter);
  Arcadia_SizeValue width = degree + 1;
  Arcadia_Real64Value* basis = workspace;
  Arcadia_Real64Value* left = workspace + width;
  Arcadia_Real64Value* right = workspace + 2 * width;
  basisFunctions(knots, degree, span, parameter, basis, left, right);
  double numerator[3] = { 0., 0., 0. };
  double denominator = 0.;
  for (Arcadia_SizeValue j = 0; j < width; ++j) {
    Arcadia_SizeValue index = span - degree + j;
    Arcadia_Real64Value weight = weights ? weights[index] : (Arcadia_Real64Value)1;
    if (!isfinite(weight) || weight <= (Arcadia_Real64Value)0) {
      return Arcadia_BooleanValue_False;
    }
    double factor = (double)basis[j] * (double)weight;
    for (Arcadia_SizeValue k = 0; k < 3; ++k) {
      if (!isfinite(points[index][k])) return Arcadia_BooleanValue_False;
      numerator[k] += factor * (double)points[index][k];
    }
    denominator += factor;
  }
  return storeResult(numerator, denominator, result);
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3Real64_evaluateSurface
  (
    const Arcadia_Real64Value (*points)[3],
    const Arcadia_Real64Value* weights,
    Arcadia_SizeValue countU,
    Arcadia_SizeValue countV,
    Arcadia_SizeValue degreeU,
    Arcadia_SizeValue degreeV,
    const Arcadia_Real64Value* knotsU,
    Arcadia_SizeValue knotCountU,
    const Arcadia_Real64Value* knotsV,
    Arcadia_SizeValue knotCountV,
    Arcadia_Real64Value u,
    Arcadia_Real64Value v,
    Arcadia_Real64Value* workspace,
    Arcadia_SizeValue workspaceElements,
    Arcadia_Real64Value result[3]
  )
{
  if (!points || !workspace || !result || !countU || !countV ||
      countU > ((Arcadia_SizeValue)-1) / countV ||
      !validKnots(countU, degreeU, knotsU, knotCountU, u) ||
      !validKnots(countV, degreeV, knotsV, knotCountV, v)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_SizeValue widthU = degreeU + 1;
  Arcadia_SizeValue widthV = degreeV + 1;
  Arcadia_SizeValue groups = workspaceElements / 3;
  if (groups < widthU || groups - widthU < widthV) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_SizeValue spanU = findSpan(knotsU, countU, degreeU, u);
  Arcadia_SizeValue spanV = findSpan(knotsV, countV, degreeV, v);
  Arcadia_Real64Value* basisU = workspace;
  Arcadia_Real64Value* basisV = workspace + 3 * widthU;
  basisFunctions(knotsU, degreeU, spanU, u, basisU, basisU + widthU,
                 basisU + 2 * widthU);
  basisFunctions(knotsV, degreeV, spanV, v, basisV, basisV + widthV,
                 basisV + 2 * widthV);
  double numerator[3] = { 0., 0., 0. };
  double denominator = 0.;
  for (Arcadia_SizeValue i = 0; i < widthU; ++i) {
    Arcadia_SizeValue row = spanU - degreeU + i;
    for (Arcadia_SizeValue j = 0; j < widthV; ++j) {
      Arcadia_SizeValue column = spanV - degreeV + j;
      Arcadia_SizeValue index = row * countV + column;
      Arcadia_Real64Value weight = weights ? weights[index] : (Arcadia_Real64Value)1;
      if (!isfinite(weight) || weight <= (Arcadia_Real64Value)0) {
        return Arcadia_BooleanValue_False;
      }
      double factor = (double)basisU[i] * (double)basisV[j] * (double)weight;
      for (Arcadia_SizeValue k = 0; k < 3; ++k) {
        if (!isfinite(points[index][k])) return Arcadia_BooleanValue_False;
        numerator[k] += factor * (double)points[index][k];
      }
      denominator += factor;
    }
  }
  return storeResult(numerator, denominator, result);
}
