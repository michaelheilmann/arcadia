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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_BEZIER3VALUES_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_BEZIER3VALUES_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// De Casteljau evaluation of an arbitrary-degree Bezier curve from native
// three-dimensional control points. No allocation and no GC references.
// The caller supplies at least 3 * n scalar elements of workspace. The
// workspace must not overlap the points or result. result may alias an input
// point because all input values are copied into workspace before evaluation.
// Parameters outside [0,1] extrapolate. The derivative is with respect to t,
// not arc length. Zero points or insufficient workspace fail without writing
// result. One point has zero derivative.

Arcadia_BooleanValue
Arcadia_Math_Bezier3Real32_evaluate(const Arcadia_Real32Value (*points)[3], Arcadia_SizeValue count, Arcadia_Real32Value t,
             Arcadia_Real32Value* workspace, Arcadia_SizeValue workspaceElements, Arcadia_Real32Value result[3]);
Arcadia_BooleanValue
Arcadia_Math_Bezier3Real32_evaluateDerivative(const Arcadia_Real32Value (*points)[3], Arcadia_SizeValue count, Arcadia_Real32Value t,
                       Arcadia_Real32Value* workspace, Arcadia_SizeValue workspaceElements, Arcadia_Real32Value result[3]);

Arcadia_BooleanValue
Arcadia_Math_Bezier3Real64_evaluate(const Arcadia_Real64Value (*points)[3], Arcadia_SizeValue count, Arcadia_Real64Value t,
             Arcadia_Real64Value* workspace, Arcadia_SizeValue workspaceElements, Arcadia_Real64Value result[3]);
Arcadia_BooleanValue
Arcadia_Math_Bezier3Real64_evaluateDerivative(const Arcadia_Real64Value (*points)[3], Arcadia_SizeValue count, Arcadia_Real64Value t,
                       Arcadia_Real64Value* workspace, Arcadia_SizeValue workspaceElements, Arcadia_Real64Value result[3]);

#endif // ARCADIA_MATH_IMPLEMENTATION_BEZIER3VALUES_H_INCLUDED
