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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_DECASTELJAU_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_DECASTELJAU_H_INCLUDED

#include "Arcadia/Math/Implementation/Vector3Real32.h"
#include "Arcadia/Math/Implementation/Vector3Real64.h"

// Evaluate a Bezier curve of arbitrary degree using De Casteljau's algorithm.
// controlPoints must contain numberOfControlPoints managed vectors; the caller
// must keep them rooted. workspace is a caller-owned array of at least
// 3 * numberOfControlPoints scalar elements. No allocations are performed.
// Result may alias any control point, but workspace must not overlap any
// input or output vector storage. t outside [0, 1] extrapolates.
// For zero points or an insufficient workspace, return false without writing
// result. No exceptions are raised by the numerical calculation.
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
  );

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
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_DECASTELJAU_H_INCLUDED
