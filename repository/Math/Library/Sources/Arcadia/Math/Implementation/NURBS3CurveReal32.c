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

#include "Arcadia/Math/Implementation/NURBS3CurveReal32.h"
#include <math.h>

Arcadia_BooleanValue
Arcadia_Math_NURBS3CurveReal32_getVertexCount(Arcadia_SizeValue samples,
    Arcadia_SizeValue* vertexCount)
{
  if (!vertexCount || samples < 2 || samples - 1 > ((Arcadia_SizeValue)-1) / 2)
    return Arcadia_BooleanValue_False;
  *vertexCount = 2 * (samples - 1);
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
validCurve(const Arcadia_Real32Value (*points)[3], const Arcadia_Real32Value* weights,
    Arcadia_SizeValue count, Arcadia_SizeValue degree,
    const Arcadia_Real32Value* knots, Arcadia_SizeValue knotCount)
{
  if (!points || !knots || !count || degree >= count ||
      knotCount <= count || knotCount - count - 1 != degree)
    return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < knotCount; ++i)
    if (!isfinite(knots[i]) || (i && knots[i] < knots[i - 1]))
      return Arcadia_BooleanValue_False;
  if (!(knots[degree] < knots[count])) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    if (weights && (!isfinite(weights[i]) || !(weights[i] > (Arcadia_Real32Value)0)))
      return Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue k = 0; k < 3; ++k)
      if (!isfinite(points[i][k])) return Arcadia_BooleanValue_False;
  }
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
sampleOne(const Arcadia_Real32Value (*points)[3], const Arcadia_Real32Value* weights,
    Arcadia_SizeValue count, Arcadia_SizeValue degree,
    const Arcadia_Real32Value* knots, Arcadia_SizeValue knotCount,
    Arcadia_SizeValue samples, Arcadia_SizeValue index,
    Arcadia_Real32Value position[3], Arcadia_Real32Value tangent[3],
    Arcadia_Real32Value* workspace, Arcadia_SizeValue workspaceElements)
{
  double fraction = (double)index / (double)(samples - 1);
  Arcadia_Real32Value parameter = index == samples - 1 ? knots[count] :
    (Arcadia_Real32Value)((double)knots[degree] +
      ((double)knots[count] - (double)knots[degree]) * fraction);
  if (!isfinite(parameter) || parameter < knots[degree] || parameter > knots[count])
    return Arcadia_BooleanValue_False;
  if (!Arcadia_Math_NURBS3Real32_evaluateCurve(points, weights, count, degree,
      knots, knotCount, parameter, workspace, workspaceElements, position))
    return Arcadia_BooleanValue_False;
  if (tangent && !Arcadia_Math_NURBS3Real32_evaluateCurveTangent(points, weights,
      count, degree, knots, knotCount, parameter,
      workspace, workspaceElements, tangent))
    return Arcadia_BooleanValue_False;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3CurveReal32_sampleLines(
    const Arcadia_Real32Value (*points)[3], const Arcadia_Real32Value* weights,
    Arcadia_SizeValue count, Arcadia_SizeValue degree,
    const Arcadia_Real32Value* knots, Arcadia_SizeValue knotCount,
    Arcadia_SizeValue samples,
    Arcadia_Real32Value (*positions)[3], Arcadia_Real32Value (*tangents)[3],
    Arcadia_SizeValue vertexCapacity,
    Arcadia_Real32Value* workspace, Arcadia_SizeValue workspaceElements)
{
  Arcadia_SizeValue needed;
  if (!positions || !workspace ||
      !Arcadia_Math_NURBS3CurveReal32_getVertexCount(samples, &needed) ||
      vertexCapacity < needed ||
      !validCurve(points, weights, count, degree, knots, knotCount) ||
      degree == (Arcadia_SizeValue)-1 ||
      workspaceElements / (tangents ? 4 : 3) < degree + 1)
    return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < samples - 1; ++i) {
    Arcadia_Real32Value p[3], q[3], dp[3], dq[3];
    if (!sampleOne(points, weights, count, degree, knots, knotCount,
          samples, i, p, tangents ? dp : NULL, workspace, workspaceElements) ||
        !sampleOne(points, weights, count, degree, knots, knotCount,
          samples, i + 1, q, tangents ? dq : NULL, workspace, workspaceElements))
      return Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue k = 0; k < 3; ++k) {
      positions[2 * i][k] = p[k];
      positions[2 * i + 1][k] = q[k];
      if (tangents) { tangents[2 * i][k] = dp[k]; tangents[2 * i + 1][k] = dq[k]; }
    }
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3CurveReal32_approximateLength(
    const Arcadia_Real32Value (*positions)[3], Arcadia_SizeValue samples,
    Arcadia_Real32Value* result)
{
  if (!positions || !result || samples < 2) return Arcadia_BooleanValue_False;
  double total = 0.;
  for (Arcadia_SizeValue i = 0; i < samples; ++i)
    for (Arcadia_SizeValue k = 0; k < 3; ++k)
      if (!isfinite(positions[i][k])) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 1; i < samples; ++i) {
    double dx = (double)positions[i][0] - (double)positions[i-1][0];
    double dy = (double)positions[i][1] - (double)positions[i-1][1];
    double dz = (double)positions[i][2] - (double)positions[i-1][2];
    total += hypot(hypot(dx,dy),dz);
    if (!isfinite(total)) return Arcadia_BooleanValue_False;
  }
  Arcadia_Real32Value converted = (Arcadia_Real32Value)total;
  if (!isfinite(converted)) return Arcadia_BooleanValue_False;
  *result = converted;
  return Arcadia_BooleanValue_True;
}
