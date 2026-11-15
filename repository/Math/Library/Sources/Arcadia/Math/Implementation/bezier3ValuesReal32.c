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

#include "Arcadia/Math/Implementation/Bezier3Values.h"

Arcadia_BooleanValue
Arcadia_Math_Bezier3Real32_evaluate(const Arcadia_Real32Value (*points)[3], Arcadia_SizeValue count, Arcadia_Real32Value t,
                          Arcadia_Real32Value* workspace, Arcadia_SizeValue workspaceElements, Arcadia_Real32Value result[3])
{
  if (!points || !workspace || !result || count == 0 || workspaceElements / 3 < count)
    return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    for (Arcadia_SizeValue axis = 0; axis < 3; ++axis)
      workspace[3 * i + axis] = points[i][axis];
  }
  Arcadia_Real32Value s = (Arcadia_Real32Value)1 - t;
  for (Arcadia_SizeValue remaining = count; remaining > 1; --remaining) {
    for (Arcadia_SizeValue i = 0; i + 1 < remaining; ++i) {
      for (Arcadia_SizeValue axis = 0; axis < 3; ++axis) {
        workspace[3 * i + axis] = s * workspace[3 * i + axis] +
                                   t * workspace[3 * (i + 1) + axis];
      }
    }
  }
  result[0] = workspace[0]; result[1] = workspace[1]; result[2] = workspace[2];
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Bezier3Real32_evaluateDerivative(const Arcadia_Real32Value (*points)[3], Arcadia_SizeValue count, Arcadia_Real32Value t,
                                    Arcadia_Real32Value* workspace, Arcadia_SizeValue workspaceElements, Arcadia_Real32Value result[3])
{
  if (!points || !workspace || !result || count == 0 || workspaceElements / 3 < count)
    return Arcadia_BooleanValue_False;
  if (count == 1) {
    result[0] = 0; result[1] = 0; result[2] = 0;
    return Arcadia_BooleanValue_True;
  }
  // The derivative is a Bezier of degree count - 2 with control values
  // (count - 1) * (P[i + 1] - P[i]). All reads happen before result writes.
  for (Arcadia_SizeValue i = 0; i + 1 < count; ++i) {
    for (Arcadia_SizeValue axis = 0; axis < 3; ++axis) {
      workspace[3 * i + axis] = (Arcadia_Real32Value)(count - 1) *
        (points[i + 1][axis] - points[i][axis]);
    }
  }
  Arcadia_Real32Value s = (Arcadia_Real32Value)1 - t;
  for (Arcadia_SizeValue remaining = count - 1; remaining > 1; --remaining) {
    for (Arcadia_SizeValue i = 0; i + 1 < remaining; ++i) {
      for (Arcadia_SizeValue axis = 0; axis < 3; ++axis) {
        workspace[3 * i + axis] = s * workspace[3 * i + axis] +
                                   t * workspace[3 * (i + 1) + axis];
      }
    }
  }
  result[0] = workspace[0]; result[1] = workspace[1]; result[2] = workspace[2];
  return Arcadia_BooleanValue_True;
}
