// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#if !defined(ARCADIA_MATH_IMPLEMENTATION_NURBS3ADAPTIVEREAL32_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_NURBS3ADAPTIVEREAL32_H_INCLUDED

#include "Arcadia/Math/Implementation/NURBS3Real32.h"

// Adaptive subdivision of a native 3D NURBS curve. Each emitted line has
// two independent endpoints; consecutive segments share their positions.
// The midpoint-to-chord error is measured in the same units as control points.
// A midpoint criterion is not a global maximum-error guarantee for an
// oscillatory curve: maxDepth is an explicit additional resolution limit.
// maxDepth must be between 0 and 20 inclusive. All output is caller-owned.
// On invalid inputs, insufficient capacity, or evaluation error no vertices
// or count are modified. This requires two evaluation passes, no allocations.
// Workspace needs at least 3 * (degree + 1) scalar values.
Arcadia_BooleanValue
Arcadia_Math_NURBS3AdaptiveReal32_sampleLines(
    const Arcadia_Real32Value (*points)[3], const Arcadia_Real32Value* weights,
    Arcadia_SizeValue count, Arcadia_SizeValue degree,
    const Arcadia_Real32Value* knots, Arcadia_SizeValue knotCount,
    Arcadia_Real32Value tolerance, Arcadia_SizeValue maxDepth,
    Arcadia_Real32Value (*positions)[3], Arcadia_SizeValue vertexCapacity,
    Arcadia_SizeValue* vertexCount,
    Arcadia_Real32Value* workspace, Arcadia_SizeValue workspaceElements);

#endif // ARCADIA_MATH_IMPLEMENTATION_NURBS3ADAPTIVEREAL32_H_INCLUDED
