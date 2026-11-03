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

#include "Arcadia/Math/Implementation/NURBS3ControlNetReal32.h"
#include <math.h>

// Math-side count preflight is shared by both the indexed selection metadata
// and the eventual render-ready line stream.
Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlNetReal32_countCurve(Arcadia_SizeValue count,
    Arcadia_SizeValue* lineCount, Arcadia_SizeValue* vertexCount)
{
  Arcadia_SizeValue limit = (Arcadia_SizeValue)-1;
  if (!lineCount || !vertexCount || lineCount == vertexCount || count < 2 ||
      count - 1 > limit / 2) return Arcadia_BooleanValue_False;
  Arcadia_SizeValue lines = count - 1;
  *lineCount = lines;
  *vertexCount = lines * 2;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlNetReal32_countSurface(Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    Arcadia_SizeValue* lineCount, Arcadia_SizeValue* vertexCount)
{
  Arcadia_SizeValue limit = (Arcadia_SizeValue)-1;
  if (!lineCount || !vertexCount || lineCount == vertexCount ||
      countU < 2 || countV < 2 || countU > limit / countV) return Arcadia_BooleanValue_False;
  Arcadia_SizeValue edgesU = (countU - 1) * countV;
  Arcadia_SizeValue edgesV = countU * (countV - 1);
  if (edgesU > limit - edgesV) return Arcadia_BooleanValue_False;
  Arcadia_SizeValue lines = edgesU + edgesV;
  if (lines > limit / 2) return Arcadia_BooleanValue_False;
  *lineCount = lines;
  *vertexCount = lines * 2;
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
validPoints(const Arcadia_Real32Value (*points)[3], Arcadia_SizeValue count)
{
  if (!points) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < count; ++i)
    for (Arcadia_SizeValue j = 0; j < 3; ++j)
      if (!isfinite(points[i][j])) return Arcadia_BooleanValue_False;
  return Arcadia_BooleanValue_True;
}

static void
writeEdge(const Arcadia_Real32Value (*points)[3], Arcadia_SizeValue a,
    Arcadia_SizeValue b, Arcadia_SizeValue edge,
    Arcadia_Real32Value (*vertices)[3], Arcadia_SizeValue (*sourcePairs)[2])
{
  for (Arcadia_SizeValue k = 0; k < 3; ++k) {
    vertices[2 * edge][k] = points[a][k];
    vertices[2 * edge + 1][k] = points[b][k];
  }
  if (sourcePairs) {
    sourcePairs[edge][0] = a;
    sourcePairs[edge][1] = b;
  }
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlNetReal32_emitCurve(const Arcadia_Real32Value (*points)[3], Arcadia_SizeValue count,
    Arcadia_Real32Value (*vertices)[3], Arcadia_SizeValue vertexCapacity,
    Arcadia_SizeValue (*sourcePairs)[2], Arcadia_SizeValue pairCapacity)
{
  Arcadia_SizeValue lines, needed;
  if (!vertices || !Arcadia_Math_NURBS3ControlNetReal32_countCurve(count, &lines, &needed) ||
      vertexCapacity < needed || (sourcePairs && pairCapacity < lines) ||
      !validPoints(points, count)) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < lines; ++i)
    writeEdge(points, i, i + 1, i, vertices, sourcePairs);
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlNetReal32_emitSurface(const Arcadia_Real32Value (*points)[3], Arcadia_SizeValue countU,
    Arcadia_SizeValue countV, Arcadia_Real32Value (*vertices)[3], Arcadia_SizeValue vertexCapacity,
    Arcadia_SizeValue (*sourcePairs)[2], Arcadia_SizeValue pairCapacity)
{
  Arcadia_SizeValue lines, needed;
  if (!vertices || !Arcadia_Math_NURBS3ControlNetReal32_countSurface(countU, countV, &lines, &needed) ||
      vertexCapacity < needed || (sourcePairs && pairCapacity < lines) ||
      !validPoints(points, countU * countV)) return Arcadia_BooleanValue_False;
  Arcadia_SizeValue edge = 0;
  for (Arcadia_SizeValue u = 0; u < countU - 1; ++u)
    for (Arcadia_SizeValue v = 0; v < countV; ++v) {
      Arcadia_SizeValue a = u * countV + v;
      writeEdge(points, a, a + countV, edge++, vertices, sourcePairs);
    }
  for (Arcadia_SizeValue u = 0; u < countU; ++u)
    for (Arcadia_SizeValue v = 0; v < countV - 1; ++v) {
      Arcadia_SizeValue a = u * countV + v;
      writeEdge(points, a, a + 1, edge++, vertices, sourcePairs);
    }
  return Arcadia_BooleanValue_True;
}
