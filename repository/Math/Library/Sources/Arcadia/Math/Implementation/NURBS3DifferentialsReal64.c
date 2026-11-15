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

#include "Arcadia/Math/Implementation/NURBS3DifferentialsReal64.h"

#include <math.h>
#include <stdint.h>

// The same right-hand span convention as the ordinary NURBS evaluator.
static Arcadia_SizeValue
findSpan(const Arcadia_Real64Value* knots, Arcadia_SizeValue count,
         Arcadia_SizeValue degree, Arcadia_Real64Value parameter)
{
  if (parameter == knots[count]) return count - 1;
  Arcadia_SizeValue low = degree;
  Arcadia_SizeValue high = count;
  while (low < high) {
    Arcadia_SizeValue mid = low + (high - low) / 2;
    if (parameter < knots[mid]) high = mid;
    else if (parameter >= knots[mid + 1]) low = mid + 1;
    else return mid;
  }
  return count - 1;
}

static Arcadia_BooleanValue
validKnots(Arcadia_SizeValue count, Arcadia_SizeValue degree,
           const Arcadia_Real64Value* knots, Arcadia_SizeValue knotCount, Arcadia_Real64Value parameter)
{
  if (!knots || !isfinite(parameter) || !count || degree >= count ||
      knotCount <= count || knotCount - count - 1 != degree) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < knotCount; ++i) {
    if (!isfinite(knots[i]) || (i && knots[i] < knots[i - 1])) return Arcadia_BooleanValue_False;
  }
  return (knots[degree] < knots[count] &&
          parameter >= knots[degree] && parameter <= knots[count])
      ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}

static void
basisFunctions(const Arcadia_Real64Value* knots, Arcadia_SizeValue degree,
               Arcadia_SizeValue span, Arcadia_Real64Value parameter,
               Arcadia_Real64Value* basis, Arcadia_Real64Value* left, Arcadia_Real64Value* right)
{
  basis[0] = (Arcadia_Real64Value)1;
  for (Arcadia_SizeValue j = 1; j <= degree; ++j) {
    left[j] = parameter - knots[span + 1 - j];
    right[j] = knots[span + j] - parameter;
    Arcadia_Real64Value saved = (Arcadia_Real64Value)0;
    for (Arcadia_SizeValue r = 0; r < j; ++r) {
      Arcadia_Real64Value divisor = right[r + 1] + left[j - r];
      Arcadia_Real64Value temp = divisor != (Arcadia_Real64Value)0 ? basis[r] / divisor : (Arcadia_Real64Value)0;
      basis[r] = saved + right[r + 1] * temp;
      saved = left[j - r] * temp;
    }
    basis[j] = saved;
  }
}

// Four blocks of degree+1 scalars:
// [degree basis, lower-degree basis, degree derivative, scratch].
// Only the active interval is evaluated; no recursion and no allocations.
static void
basisAndDerivative(const Arcadia_Real64Value* knots, Arcadia_SizeValue degree,
                   Arcadia_SizeValue span, Arcadia_Real64Value parameter, Arcadia_Real64Value* scratch)
{
  Arcadia_SizeValue width = degree + 1;
  Arcadia_Real64Value* basis = scratch;
  Arcadia_Real64Value* lower = scratch + width;
  Arcadia_Real64Value* derivative = scratch + width * 2;
  Arcadia_Real64Value* temporary = scratch + width * 3;
  basisFunctions(knots, degree, span, parameter, basis, derivative, temporary);
  if (!degree) {
    derivative[0] = (Arcadia_Real64Value)0;
    return;
  }
  basisFunctions(knots, degree - 1, span, parameter, lower, derivative, temporary);
  for (Arcadia_SizeValue j = 0; j < width; ++j) {
    Arcadia_SizeValue index = span - degree + j;
    Arcadia_Real64Value a = knots[index + degree] - knots[index];
    Arcadia_Real64Value b = knots[index + degree + 1] - knots[index + 1];
    double first = (j && a != (Arcadia_Real64Value)0)
      ? ((double)degree * (double)lower[j - 1]) / (double)a : 0.;
    double second = (j < degree && b != (Arcadia_Real64Value)0)
      ? ((double)degree * (double)lower[j]) / (double)b : 0.;
    derivative[j] = (Arcadia_Real64Value)(first - second);
  }
}

// Convert all outputs first to preserve the failure-atomic contract.
static Arcadia_BooleanValue
convertVector(const double from[3], Arcadia_Real64Value to[3])
{
  for (Arcadia_SizeValue j = 0; j < 3; ++j) {
    if (!isfinite(from[j])) return Arcadia_BooleanValue_False;
    to[j] = (Arcadia_Real64Value)from[j];
    if (!isfinite(to[j])) return Arcadia_BooleanValue_False;
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3Real64_evaluateCurveTangent
  (
    const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* weights,
    Arcadia_SizeValue count, Arcadia_SizeValue degree,
    const Arcadia_Real64Value* knots, Arcadia_SizeValue knotCount, Arcadia_Real64Value parameter,
    Arcadia_Real64Value* workspace, Arcadia_SizeValue workspaceElements,
    Arcadia_Real64Value tangent[3]
  )
{
  if (!points || !workspace || !tangent ||
      !validKnots(count, degree, knots, knotCount, parameter) ||
      workspaceElements / 4 < degree + 1) return Arcadia_BooleanValue_False;
  Arcadia_SizeValue span = findSpan(knots, count, degree, parameter);
  Arcadia_SizeValue width = degree + 1;
  basisAndDerivative(knots, degree, span, parameter, workspace);
  const Arcadia_Real64Value* basis = workspace;
  const Arcadia_Real64Value* derivative = workspace + 2 * width;
  double numerator[3] = { 0., 0., 0. };
  double derivativeNumerator[3] = { 0., 0., 0. };
  double denominator = 0.;
  double derivativeDenominator = 0.;
  for (Arcadia_SizeValue i = 0; i < width; ++i) {
    Arcadia_SizeValue index = span - degree + i;
    Arcadia_Real64Value w = weights ? weights[index] : (Arcadia_Real64Value)1;
    if (!isfinite(w) || !(w > (Arcadia_Real64Value)0)) return Arcadia_BooleanValue_False;
    double factor = (double)basis[i] * (double)w;
    double dfactor = (double)derivative[i] * (double)w;
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      if (!isfinite(points[index][j])) return Arcadia_BooleanValue_False;
      numerator[j] += factor * (double)points[index][j];
      derivativeNumerator[j] += dfactor * (double)points[index][j];
    }
    denominator += factor;
    derivativeDenominator += dfactor;
  }
  if (!(denominator > 0.) || !isfinite(denominator)) return Arcadia_BooleanValue_False;
  double derivativeValue[3];
  for (Arcadia_SizeValue j = 0; j < 3; ++j) {
    double position = numerator[j] / denominator;
    derivativeValue[j] = (derivativeNumerator[j] - position * derivativeDenominator) / denominator;
  }
  Arcadia_Real64Value next[3];
  if (!convertVector(derivativeValue, next)) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue j = 0; j < 3; ++j) tangent[j] = next[j];
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3Real64_evaluateSurfaceDifferentials
  (
    const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* weights,
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    Arcadia_SizeValue degreeU, Arcadia_SizeValue degreeV,
    const Arcadia_Real64Value* knotsU, Arcadia_SizeValue knotCountU,
    const Arcadia_Real64Value* knotsV, Arcadia_SizeValue knotCountV,
    Arcadia_Real64Value u, Arcadia_Real64Value v,
    Arcadia_Real64Value* workspace, Arcadia_SizeValue workspaceElements,
    Arcadia_Real64Value position[3], Arcadia_Real64Value tangentU[3], Arcadia_Real64Value tangentV[3]
  )
{
  if (!points || !workspace || !position || !tangentU || !tangentV ||
      !countU || !countV || countU > ((Arcadia_SizeValue)-1) / countV ||
      !validKnots(countU, degreeU, knotsU, knotCountU, u) ||
      !validKnots(countV, degreeV, knotsV, knotCountV, v)) return Arcadia_BooleanValue_False;
  Arcadia_SizeValue widthU = degreeU + 1;
  Arcadia_SizeValue widthV = degreeV + 1;
  if (workspaceElements / 4 < widthU || workspaceElements / 4 - widthU < widthV)
    return Arcadia_BooleanValue_False;
  Arcadia_SizeValue spanU = findSpan(knotsU, countU, degreeU, u);
  Arcadia_SizeValue spanV = findSpan(knotsV, countV, degreeV, v);
  Arcadia_Real64Value* tableU = workspace;
  Arcadia_Real64Value* tableV = workspace + 4 * widthU;
  basisAndDerivative(knotsU, degreeU, spanU, u, tableU);
  basisAndDerivative(knotsV, degreeV, spanV, v, tableV);
  const Arcadia_Real64Value* basisU = tableU;
  const Arcadia_Real64Value* derivU = tableU + 2 * widthU;
  const Arcadia_Real64Value* basisV = tableV;
  const Arcadia_Real64Value* derivV = tableV + 2 * widthV;
  double numerator[3] = { 0., 0., 0. };
  double numeratorU[3] = { 0., 0., 0. };
  double numeratorV[3] = { 0., 0., 0. };
  double denominator = 0., denominatorU = 0., denominatorV = 0.;
  for (Arcadia_SizeValue i = 0; i < widthU; ++i) {
    Arcadia_SizeValue row = spanU - degreeU + i;
    for (Arcadia_SizeValue j = 0; j < widthV; ++j) {
      Arcadia_SizeValue index = row * countV + spanV - degreeV + j;
      Arcadia_Real64Value w = weights ? weights[index] : (Arcadia_Real64Value)1;
      if (!isfinite(w) || !(w > (Arcadia_Real64Value)0)) return Arcadia_BooleanValue_False;
      double factor = (double)basisU[i] * (double)basisV[j] * (double)w;
      double factorU = (double)derivU[i] * (double)basisV[j] * (double)w;
      double factorV = (double)basisU[i] * (double)derivV[j] * (double)w;
      for (Arcadia_SizeValue k = 0; k < 3; ++k) {
        if (!isfinite(points[index][k])) return Arcadia_BooleanValue_False;
        numerator[k] += factor * (double)points[index][k];
        numeratorU[k] += factorU * (double)points[index][k];
        numeratorV[k] += factorV * (double)points[index][k];
      }
      denominator += factor;
      denominatorU += factorU;
      denominatorV += factorV;
    }
  }
  if (!(denominator > 0.) || !isfinite(denominator)) return Arcadia_BooleanValue_False;
  double posD[3], duD[3], dvD[3];
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    posD[k] = numerator[k] / denominator;
    duD[k] = (numeratorU[k] - posD[k] * denominatorU) / denominator;
    dvD[k] = (numeratorV[k] - posD[k] * denominatorV) / denominator;
  }
  Arcadia_Real64Value posNext[3], uNext[3], vNext[3];
  if (!convertVector(posD, posNext) || !convertVector(duD, uNext) ||
      !convertVector(dvD, vNext)) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    position[k] = posNext[k];
    tangentU[k] = uNext[k];
    tangentV[k] = vNext[k];
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3Real64_sampleSurface
  (
    const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* weights,
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    Arcadia_SizeValue degreeU, Arcadia_SizeValue degreeV,
    const Arcadia_Real64Value* knotsU, Arcadia_SizeValue knotCountU,
    const Arcadia_Real64Value* knotsV, Arcadia_SizeValue knotCountV,
    Arcadia_SizeValue samplesU, Arcadia_SizeValue samplesV,
    Arcadia_Real64Value (*positions)[3], Arcadia_Real64Value (*normals)[3],
    Arcadia_Real64Value* workspace, Arcadia_SizeValue workspaceElements
  )
{
  if (!points || !positions || !workspace || !knotsU || !knotsV ||
      samplesU < 2 || samplesV < 2 ||
      samplesU > ((Arcadia_SizeValue)-1) / samplesV ||
      !countU || !countV || degreeU >= countU || degreeV >= countV ||
      knotCountU <= countU || knotCountU - countU - 1 != degreeU ||
      knotCountV <= countV || knotCountV - countV - 1 != degreeV) return Arcadia_BooleanValue_False;
  Arcadia_Real64Value startU = knotsU[degreeU], endU = knotsU[countU];
  Arcadia_Real64Value startV = knotsV[degreeV], endV = knotsV[countV];
  if (!validKnots(countU, degreeU, knotsU, knotCountU, startU) ||
      !validKnots(countV, degreeV, knotsV, knotCountV, startV)) return Arcadia_BooleanValue_False;
  Arcadia_SizeValue widthU = degreeU + 1, widthV = degreeV + 1;
  if (workspaceElements / 4 < widthU || workspaceElements / 4 - widthU < widthV)
    return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < samplesU; ++i) {
    Arcadia_Real64Value u = i == samplesU - 1 ? endU :
      (Arcadia_Real64Value)((double)startU + ((double)endU - (double)startU) *
               ((double)i / (double)(samplesU - 1)));
    for (Arcadia_SizeValue j = 0; j < samplesV; ++j) {
      Arcadia_Real64Value v = j == samplesV - 1 ? endV :
        (Arcadia_Real64Value)((double)startV + ((double)endV - (double)startV) *
                 ((double)j / (double)(samplesV - 1)));
      Arcadia_Real64Value p[3], du[3], dv[3], normal[3];
      if (!Arcadia_Math_NURBS3Real64_evaluateSurfaceDifferentials(
            points, weights, countU, countV, degreeU, degreeV,
            knotsU, knotCountU, knotsV, knotCountV, u, v,
            workspace, workspaceElements, p, du, dv)) return Arcadia_BooleanValue_False;
      if (normals) {
        double cross[3] = {
          (double)du[1] * (double)dv[2] - (double)du[2] * (double)dv[1],
          (double)du[2] * (double)dv[0] - (double)du[0] * (double)dv[2],
          (double)du[0] * (double)dv[1] - (double)du[1] * (double)dv[0]
        };
        double length = sqrt(cross[0] * cross[0] + cross[1] * cross[1] + cross[2] * cross[2]);
        if (!(length > 0.) || !isfinite(length)) return Arcadia_BooleanValue_False;
        double unit[3] = { cross[0]/length, cross[1]/length, cross[2]/length };
        if (!convertVector(unit, normal)) return Arcadia_BooleanValue_False;
      }
      Arcadia_SizeValue index = i * samplesV + j;
      for (Arcadia_SizeValue k = 0; k < 3; ++k) {
        positions[index][k] = p[k];
        if (normals) normals[index][k] = normal[k];
      }
    }
  }
  return Arcadia_BooleanValue_True;
}
