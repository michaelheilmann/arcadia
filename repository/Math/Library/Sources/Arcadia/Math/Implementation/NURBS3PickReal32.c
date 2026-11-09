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

#include "Arcadia/Math/Implementation/NURBS3PickReal32.h"
#include <math.h>

// A candidate is kept private until the closest valid triangle is found.
typedef struct PickCandidate {
  Arcadia_SizeValue triangle;
  Arcadia_Real32Value t;
  Arcadia_Real32Value uv[2];
  Arcadia_Real32Value point[3];
  Arcadia_BooleanValue found;
} PickCandidate;

static Arcadia_BooleanValue
validRay(const Arcadia_Math_Ray3Real32* ray, Arcadia_Real32Value maxT)
{
  if (!ray || !isfinite(maxT) || maxT < (Arcadia_Real32Value)0)
    return Arcadia_BooleanValue_False;
  Arcadia_BooleanValue nonzero = Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    if (!isfinite(ray->origin[k]) || !isfinite(ray->direction[k]))
      return Arcadia_BooleanValue_False;
    if (ray->direction[k] != (Arcadia_Real32Value)0)
      nonzero = Arcadia_BooleanValue_True;
  }
  return nonzero;
}

static Arcadia_BooleanValue
validPositions(const Arcadia_Real32Value (*positions)[3], Arcadia_SizeValue count)
{
  if (!positions) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < count; ++i)
    for (Arcadia_SizeValue k = 0; k < 3; ++k)
      if (!isfinite(positions[i][k])) return Arcadia_BooleanValue_False;
  return Arcadia_BooleanValue_True;
}

// Return false for a miss and for arithmetic which is not representable.
// A failed candidate never changes the best candidate.
static void
consider(const Arcadia_Math_Ray3Real32* ray,
    const Arcadia_Real32Value aPoint[3],
    const Arcadia_Real32Value bPoint[3], const Arcadia_Real32Value cPoint[3],
    Arcadia_Real32Value uv[3][2], Arcadia_SizeValue triangle,
    Arcadia_Real32Value maxT, PickCandidate* best)
{
  Arcadia_Real32Value t, b, c;
  if (!Arcadia_Math_Ray3Real32_intersectTriangle(ray,
        aPoint, bPoint, cPoint, &t, &b, &c)) return;
  if (!isfinite(t) || !isfinite(b) || !isfinite(c) || t < 0 || t > maxT ||
      b < 0 || c < 0 || b + c > 1) return;
  if (best->found && !(t < best->t)) return;
  Arcadia_Real32Value a = (Arcadia_Real32Value)1 - b - c;
  const Arcadia_Real32Value* positions[3] = {aPoint,bPoint,cPoint};
  PickCandidate candidate;
  candidate.triangle = triangle;
  candidate.t = t;
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    candidate.point[k] = a * positions[0][k] + b * positions[1][k] + c * positions[2][k];
    if (!isfinite(candidate.point[k])) return;
  }
  for (Arcadia_SizeValue k = 0; k < 2; ++k) {
    candidate.uv[k] = a * uv[0][k] + b * uv[1][k] + c * uv[2][k];
    if (!isfinite(candidate.uv[k])) return;
  }
  candidate.found = Arcadia_BooleanValue_True;
  *best = candidate;
}

static void
commit(const PickCandidate* best, Arcadia_SizeValue* triangle,
    Arcadia_Real32Value* t, Arcadia_Real32Value uv[2], Arcadia_Real32Value point[3])
{
  *triangle = best->triangle;
  if (t) *t = best->t;
  if (uv) for (Arcadia_SizeValue k = 0; k < 2; ++k) uv[k] = best->uv[k];
  if (point) for (Arcadia_SizeValue k = 0; k < 3; ++k) point[k] = best->point[k];
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3PickReal32_pickTriangles(
    const Arcadia_Math_Ray3Real32* ray,
    const Arcadia_Real32Value (*positions)[3],
    const Arcadia_Real32Value (*sourceUV)[2],
    Arcadia_SizeValue vertexCount, Arcadia_Real32Value maxT,
    Arcadia_SizeValue* triangleIndex, Arcadia_Real32Value* hitT,
    Arcadia_Real32Value hitUV[2], Arcadia_Real32Value hitPosition[3])
{
  if (!triangleIndex || vertexCount < 3 || vertexCount % 3 ||
      !validRay(ray, maxT) || !validPositions(positions, vertexCount) ||
      (hitUV && !sourceUV)) return Arcadia_BooleanValue_False;
  if (sourceUV) for (Arcadia_SizeValue i = 0; i < vertexCount; ++i)
    for (Arcadia_SizeValue k = 0; k < 2; ++k)
      if (!isfinite(sourceUV[i][k])) return Arcadia_BooleanValue_False;

  PickCandidate best = {0};
  for (Arcadia_SizeValue i = 0; i < vertexCount / 3; ++i) {
    const Arcadia_Real32Value (*face)[3] = positions + i * 3;
    Arcadia_Real32Value uv[3][2] = {{0,0},{0,0},{0,0}};
    if (sourceUV) for (Arcadia_SizeValue j = 0; j < 3; ++j)
      for (Arcadia_SizeValue k = 0; k < 2; ++k)
        uv[j][k] = sourceUV[i * 3 + j][k];
    consider(ray, face[0], face[1], face[2], uv, i, maxT, &best);
  }
  if (!best.found) return Arcadia_BooleanValue_False;
  commit(&best, triangleIndex, hitT, hitUV, hitPosition);
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3PickReal32_pickGrid(
    const Arcadia_Math_Ray3Real32* ray,
    const Arcadia_Real32Value (*positions)[3],
    Arcadia_SizeValue samplesU, Arcadia_SizeValue samplesV,
    Arcadia_Real32Value maxT, Arcadia_SizeValue* triangleIndex,
    Arcadia_Real32Value* hitT, Arcadia_Real32Value hitUV[2],
    Arcadia_Real32Value hitPosition[3])
{
  Arcadia_SizeValue vertices, triangles;
  if (!triangleIndex || !validRay(ray,maxT) ||
      !Arcadia_Math_NURBS3Grid_getCounts(samplesU, samplesV, &vertices, &triangles) ||
      !validPositions(positions, vertices)) return Arcadia_BooleanValue_False;

  PickCandidate best = {0};
  Arcadia_SizeValue triangle = 0;
  for (Arcadia_SizeValue i = 0; i < samplesU - 1; ++i) {
    for (Arcadia_SizeValue j = 0; j < samplesV - 1; ++j) {
      Arcadia_SizeValue a = i * samplesV + j, b = a + 1;
      Arcadia_SizeValue c = a + samplesV, d = c + 1;
      // Identical ordering to NURBS3Grid_makeTriangles.
      const Arcadia_SizeValue indices[2][3] = {{a,c,b},{b,c,d}};
      for (Arcadia_SizeValue face = 0; face < 2; ++face) {
        Arcadia_Real32Value xyz[3][3], uv[3][2];
        for (Arcadia_SizeValue vertex = 0; vertex < 3; ++vertex) {
          Arcadia_SizeValue index = indices[face][vertex];
          for (Arcadia_SizeValue k = 0; k < 3; ++k)
            xyz[vertex][k] = positions[index][k];
          Arcadia_SizeValue uIndex = index / samplesV;
          Arcadia_SizeValue vIndex = index % samplesV;
          uv[vertex][0] = uIndex == samplesU - 1 ? (Arcadia_Real32Value)1 :
            (Arcadia_Real32Value)((double)uIndex / (double)(samplesU - 1));
          uv[vertex][1] = vIndex == samplesV - 1 ? (Arcadia_Real32Value)1 :
            (Arcadia_Real32Value)((double)vIndex / (double)(samplesV - 1));
        }
        consider(ray, xyz[0], xyz[1], xyz[2], uv, triangle++, maxT, &best);
      }
    }
  }
  if (!best.found) return Arcadia_BooleanValue_False;
  commit(&best, triangleIndex, hitT, hitUV, hitPosition);
  return Arcadia_BooleanValue_True;
}
