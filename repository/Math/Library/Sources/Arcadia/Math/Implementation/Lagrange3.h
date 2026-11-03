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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_LAGRANGE3_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_LAGRANGE3_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// Evaluate Lagrange interpolation in three dimensions from native points.
// Points are packed as [n][3], and knots/weights are caller-owned scalar
// arrays with n entries. No managed objects or allocations are involved.
//
// Repeated knot values are rejected; in that case computeWeights leaves the
// output array untouched. Chord-length knots fall back to uniform spacing
// for coincident adjacent points (or an entirely coincident point set).
//
// The evaluation is exact at a knot. Values outside the knot interval
// extrapolate. result may alias a point in the input array, but knots and
// weights must not overlap the points or output storage.
// A return value of false leaves result unchanged.

Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real32_setUniformKnots(Arcadia_SizeValue count, Arcadia_Real32Value* knots);
Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real32_setChordLengthKnots(const Arcadia_Real32Value (*points)[3], Arcadia_SizeValue count, Arcadia_Real32Value* knots);
Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real32_computeWeights(const Arcadia_Real32Value* knots, Arcadia_SizeValue count, Arcadia_Real32Value* weights);
Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real32_evaluate(const Arcadia_Real32Value (*points)[3], const Arcadia_Real32Value* knots, const Arcadia_Real32Value* weights,
             Arcadia_SizeValue count, Arcadia_Real32Value t, Arcadia_Real32Value result[3]);

Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real64_setUniformKnots(Arcadia_SizeValue count, Arcadia_Real64Value* knots);
Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real64_setChordLengthKnots(const Arcadia_Real64Value (*points)[3], Arcadia_SizeValue count, Arcadia_Real64Value* knots);
Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real64_computeWeights(const Arcadia_Real64Value* knots, Arcadia_SizeValue count, Arcadia_Real64Value* weights);
Arcadia_BooleanValue
Arcadia_Math_Lagrange3Real64_evaluate(const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* knots, const Arcadia_Real64Value* weights,
             Arcadia_SizeValue count, Arcadia_Real64Value t, Arcadia_Real64Value result[3]);

#endif // ARCADIA_MATH_IMPLEMENTATION_LAGRANGE3_H_INCLUDED
