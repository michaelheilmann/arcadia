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

#include "Arcadia/Math/Implementation/NURBS3Grid.h"

Arcadia_BooleanValue
Arcadia_Math_NURBS3Grid_getCounts
  (
    Arcadia_SizeValue samplesU,
    Arcadia_SizeValue samplesV,
    Arcadia_SizeValue* vertexCount,
    Arcadia_SizeValue* triangleCount
  )
{
  const Arcadia_SizeValue maximum = (Arcadia_SizeValue)-1;
  if (!vertexCount || !triangleCount || vertexCount == triangleCount ||
      samplesU < 2 || samplesV < 2 || samplesU > maximum / samplesV) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_SizeValue quadU = samplesU - 1;
  Arcadia_SizeValue quadV = samplesV - 1;
  if (quadU > maximum / quadV || quadU * quadV > maximum / 2) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_SizeValue vertices = samplesU * samplesV;
  Arcadia_SizeValue triangles = 2 * (quadU * quadV);
  *vertexCount = vertices;
  *triangleCount = triangles;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3Grid_makeTriangles
  (
    Arcadia_SizeValue samplesU,
    Arcadia_SizeValue samplesV,
    Arcadia_SizeValue (*triangles)[3],
    Arcadia_SizeValue triangleCapacity
  )
{
  Arcadia_SizeValue vertices, count;
  if (!triangles ||
      !Arcadia_Math_NURBS3Grid_getCounts(samplesU, samplesV, &vertices, &count) ||
      triangleCapacity < count) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_SizeValue triangle = 0;
  for (Arcadia_SizeValue i = 0; i < samplesU - 1; ++i) {
    for (Arcadia_SizeValue j = 0; j < samplesV - 1; ++j) {
      Arcadia_SizeValue a = i * samplesV + j;
      Arcadia_SizeValue b = a + 1;
      Arcadia_SizeValue c = a + samplesV;
      Arcadia_SizeValue d = c + 1;
      // a--b, c--d, with u increasing downward and v to the right.
      // (a,c,b) and (b,c,d) both follow du cross dv.
      triangles[triangle][0] = a;
      triangles[triangle][1] = c;
      triangles[triangle][2] = b;
      ++triangle;
      triangles[triangle][0] = b;
      triangles[triangle][1] = c;
      triangles[triangle][2] = d;
      ++triangle;
    }
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3Grid_makeUVReal32
  (
    Arcadia_SizeValue samplesU,
    Arcadia_SizeValue samplesV,
    Arcadia_Real32Value (*uv)[2],
    Arcadia_SizeValue vertexCapacity
  )
{
  Arcadia_SizeValue vertices, triangles;
  if (!uv ||
      !Arcadia_Math_NURBS3Grid_getCounts(samplesU, samplesV, &vertices, &triangles) ||
      vertexCapacity < vertices) {
    return Arcadia_BooleanValue_False;
  }
  for (Arcadia_SizeValue i = 0; i < samplesU; ++i) {
    Arcadia_Real32Value u = i == samplesU - 1 ? (Arcadia_Real32Value)1 :
      (Arcadia_Real32Value)((double)i / (double)(samplesU - 1));
    for (Arcadia_SizeValue j = 0; j < samplesV; ++j) {
      Arcadia_SizeValue index = i * samplesV + j;
      uv[index][0] = u;
      uv[index][1] = j == samplesV - 1 ? (Arcadia_Real32Value)1 :
        (Arcadia_Real32Value)((double)j / (double)(samplesV - 1));
    }
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3Grid_makeUVReal64
  (
    Arcadia_SizeValue samplesU,
    Arcadia_SizeValue samplesV,
    Arcadia_Real64Value (*uv)[2],
    Arcadia_SizeValue vertexCapacity
  )
{
  Arcadia_SizeValue vertices, triangles;
  if (!uv ||
      !Arcadia_Math_NURBS3Grid_getCounts(samplesU, samplesV, &vertices, &triangles) ||
      vertexCapacity < vertices) {
    return Arcadia_BooleanValue_False;
  }
  for (Arcadia_SizeValue i = 0; i < samplesU; ++i) {
    Arcadia_Real64Value u = i == samplesU - 1 ? (Arcadia_Real64Value)1 :
      (Arcadia_Real64Value)((double)i / (double)(samplesU - 1));
    for (Arcadia_SizeValue j = 0; j < samplesV; ++j) {
      Arcadia_SizeValue index = i * samplesV + j;
      uv[index][0] = u;
      uv[index][1] = j == samplesV - 1 ? (Arcadia_Real64Value)1 :
        (Arcadia_Real64Value)((double)j / (double)(samplesV - 1));
    }
  }
  return Arcadia_BooleanValue_True;
}
