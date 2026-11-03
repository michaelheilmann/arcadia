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

#include "Arcadia/Math/Implementation/NURBS3MeshReal64.h"

#include <math.h>

Arcadia_BooleanValue
Arcadia_Math_NURBS3MeshReal64_getVertexCount(
    Arcadia_SizeValue samplesU, Arcadia_SizeValue samplesV,
    Arcadia_SizeValue* vertexCount)
{
  Arcadia_SizeValue gridVertices, triangles;
  if (!vertexCount ||
      !Arcadia_Math_NURBS3Grid_getCounts(samplesU, samplesV,
                                         &gridVertices, &triangles) ||
      triangles > ((Arcadia_SizeValue)-1) / 3) {
    return Arcadia_BooleanValue_False;
  }
  *vertexCount = triangles * 3;
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
validKnots(const Arcadia_Real64Value* knots, Arcadia_SizeValue count,
           Arcadia_SizeValue degree, Arcadia_SizeValue knotCount)
{
  if (!knots || !count || degree >= count || knotCount <= count ||
      knotCount - count - 1 != degree) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < knotCount; ++i) {
    if (!isfinite(knots[i]) || (i && knots[i] < knots[i - 1]))
      return Arcadia_BooleanValue_False;
  }
  return knots[degree] < knots[count] ?
    Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
sampleVertex(
    const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* weights,
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    Arcadia_SizeValue degreeU, Arcadia_SizeValue degreeV,
    const Arcadia_Real64Value* knotsU, Arcadia_SizeValue knotCountU,
    const Arcadia_Real64Value* knotsV, Arcadia_SizeValue knotCountV,
    Arcadia_SizeValue samplesU, Arcadia_SizeValue samplesV,
    Arcadia_SizeValue i, Arcadia_SizeValue j,
    Arcadia_Real64Value (*positions)[3], Arcadia_Real64Value (*normals)[3], Arcadia_Real64Value (*uv)[2],
    Arcadia_SizeValue outputIndex, Arcadia_Real64Value* workspace,
    Arcadia_SizeValue workspaceElements)
{
  const double fu = (double)i / (double)(samplesU - 1);
  const double fv = (double)j / (double)(samplesV - 1);
  const Arcadia_Real64Value u = i == samplesU - 1 ? knotsU[countU] :
    (Arcadia_Real64Value)((double)knotsU[degreeU] +
      ((double)knotsU[countU] - (double)knotsU[degreeU]) * fu);
  const Arcadia_Real64Value v = j == samplesV - 1 ? knotsV[countV] :
    (Arcadia_Real64Value)((double)knotsV[degreeV] +
      ((double)knotsV[countV] - (double)knotsV[degreeV]) * fv);
  Arcadia_Real64Value position[3], du[3], dv[3], normal[3];
  if (!Arcadia_Math_NURBS3Real64_evaluateSurfaceDifferentials(
        points, weights, countU, countV, degreeU, degreeV,
        knotsU, knotCountU, knotsV, knotCountV, u, v,
        workspace, workspaceElements, position, du, dv))
    return Arcadia_BooleanValue_False;
  if (normals) {
    double cross[3] = {
      (double)du[1] * dv[2] - (double)du[2] * dv[1],
      (double)du[2] * dv[0] - (double)du[0] * dv[2],
      (double)du[0] * dv[1] - (double)du[1] * dv[0]
    };
    double length = hypot(hypot(cross[0], cross[1]), cross[2]);
    if (!(length > 0.) || !isfinite(length))
      return Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue k = 0; k < 3; ++k) {
      double value = cross[k] / length;
      normal[k] = (Arcadia_Real64Value)value;
      if (!isfinite(normal[k])) return Arcadia_BooleanValue_False;
    }
  }
  Arcadia_Real64Value tu = i == samplesU - 1 ? (Arcadia_Real64Value)1 : (Arcadia_Real64Value)fu;
  Arcadia_Real64Value tv = j == samplesV - 1 ? (Arcadia_Real64Value)1 : (Arcadia_Real64Value)fv;
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    positions[outputIndex][k] = position[k];
    if (normals) normals[outputIndex][k] = normal[k];
  }
  uv[outputIndex][0] = tu;
  uv[outputIndex][1] = tv;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3MeshReal64_sampleTriangles(
    const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* weights,
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    Arcadia_SizeValue degreeU, Arcadia_SizeValue degreeV,
    const Arcadia_Real64Value* knotsU, Arcadia_SizeValue knotCountU,
    const Arcadia_Real64Value* knotsV, Arcadia_SizeValue knotCountV,
    Arcadia_SizeValue samplesU, Arcadia_SizeValue samplesV,
    Arcadia_Real64Value (*positions)[3], Arcadia_Real64Value (*normals)[3],
    Arcadia_Real64Value (*uv)[2], Arcadia_SizeValue vertexCapacity,
    Arcadia_Real64Value* workspace, Arcadia_SizeValue workspaceElements)
{
  Arcadia_SizeValue vertices;
  if (!points || !positions || !uv || !workspace ||
      !Arcadia_Math_NURBS3MeshReal64_getVertexCount(samplesU, samplesV, &vertices) ||
      vertexCapacity < vertices ||
      !validKnots(knotsU, countU, degreeU, knotCountU) ||
      !validKnots(knotsV, countV, degreeV, knotCountV) ||
      countU > ((Arcadia_SizeValue)-1) / countV ||
      workspaceElements / 4 < degreeU + 1 ||
      workspaceElements / 4 - (degreeU + 1) < degreeV + 1)
    return Arcadia_BooleanValue_False;
  // Validate the complete lattice, not only points active in the first cell.
  for (Arcadia_SizeValue i = 0, n = countU * countV; i < n; ++i) {
    if (weights && (!isfinite(weights[i]) || weights[i] <= (Arcadia_Real64Value)0))
      return Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue k = 0; k < 3; ++k)
      if (!isfinite(points[i][k])) return Arcadia_BooleanValue_False;
  }
  Arcadia_SizeValue out = 0;
  for (Arcadia_SizeValue i = 0; i < samplesU - 1; ++i) {
    for (Arcadia_SizeValue j = 0; j < samplesV - 1; ++j) {
      // Matches triangles (a,c,b), (b,c,d) from NURBS3Grid.
      const Arcadia_SizeValue gi[6] = {i, i + 1, i, i, i + 1, i + 1};
      const Arcadia_SizeValue gj[6] = {j, j, j + 1, j + 1, j, j + 1};
      for (Arcadia_SizeValue k = 0; k < 6; ++k) {
        if (!sampleVertex(points, weights, countU, countV, degreeU, degreeV,
                          knotsU, knotCountU, knotsV, knotCountV,
                          samplesU, samplesV, gi[k], gj[k], positions, normals,
                          uv, out, workspace, workspaceElements))
          return Arcadia_BooleanValue_False;
        ++out;
      }
    }
  }
  return Arcadia_BooleanValue_True;
}
