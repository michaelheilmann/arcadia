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

#include "Arcadia/Math/Implementation/deCasteljau.h"

#include <assert.h>

Arcadia_BooleanValue
Arcadia_Math_Vector3Real32_deCasteljau
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real32* const* controlPoints,
    Arcadia_SizeValue numberOfControlPoints,
    Arcadia_Real32Value t,
    Arcadia_Real32Value* workspace,
    Arcadia_SizeValue workspaceElements,
    Arcadia_Math_Vector3Real32* result
  )
{
  assert(thread && controlPoints && workspace && result);
  (void)thread;
  if (numberOfControlPoints == 0 || workspaceElements / 3 < numberOfControlPoints) {
    return Arcadia_BooleanValue_False;
  }
  // Copy all inputs first, preserving correct results when result aliases a point.
  for (Arcadia_SizeValue i = 0; i < numberOfControlPoints; ++i) {
    assert(controlPoints[i]);
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      workspace[3 * i + j] = controlPoints[i]->elements[j];
    }
  }
  Arcadia_Real32Value s = 1.f - t;
  for (Arcadia_SizeValue remaining = numberOfControlPoints; remaining > 1; --remaining) {
    for (Arcadia_SizeValue i = 0; i < remaining - 1; ++i) {
      for (Arcadia_SizeValue j = 0; j < 3; ++j) {
        workspace[3 * i + j] = s * workspace[3 * i + j] +
                               t * workspace[3 * (i + 1) + j];
      }
    }
  }
  for (Arcadia_SizeValue j = 0; j < 3; ++j) {
    result->elements[j] = workspace[j];
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Vector3Real64_deCasteljau
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real64* const* controlPoints,
    Arcadia_SizeValue numberOfControlPoints,
    Arcadia_Real64Value t,
    Arcadia_Real64Value* workspace,
    Arcadia_SizeValue workspaceElements,
    Arcadia_Math_Vector3Real64* result
  )
{
  assert(thread && controlPoints && workspace && result);
  (void)thread;
  if (numberOfControlPoints == 0 || workspaceElements / 3 < numberOfControlPoints) {
    return Arcadia_BooleanValue_False;
  }
  // Copy all inputs first, preserving correct results when result aliases a point.
  for (Arcadia_SizeValue i = 0; i < numberOfControlPoints; ++i) {
    assert(controlPoints[i]);
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      workspace[3 * i + j] = controlPoints[i]->elements[j];
    }
  }
  Arcadia_Real64Value s = 1. - t;
  for (Arcadia_SizeValue remaining = numberOfControlPoints; remaining > 1; --remaining) {
    for (Arcadia_SizeValue i = 0; i < remaining - 1; ++i) {
      for (Arcadia_SizeValue j = 0; j < 3; ++j) {
        workspace[3 * i + j] = s * workspace[3 * i + j] +
                               t * workspace[3 * (i + 1) + j];
      }
    }
  }
  for (Arcadia_SizeValue j = 0; j < 3; ++j) {
    result->elements[j] = workspace[j];
  }
  return Arcadia_BooleanValue_True;
}
