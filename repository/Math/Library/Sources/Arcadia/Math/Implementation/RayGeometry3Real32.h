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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_RAYGEOMETRY3REAL32_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_RAYGEOMETRY3REAL32_H_INCLUDED

#include "Arcadia/Math/Implementation/AABB3Real32.h"

// Native, non-GC 3D primitives. Directions may have arbitrary nonzero length;
// intersection parameters t are relative to origin + t * direction.
// Input coordinates must be finite. Setters reject zero direction/normal or
// negative radius and leave the target untouched on failure.
typedef struct Arcadia_Math_Ray3Real32 {
  Arcadia_Real32Value origin[3];
  Arcadia_Real32Value direction[3];
} Arcadia_Math_Ray3Real32;

typedef struct Arcadia_Math_Plane3Real32 {
  Arcadia_Real32Value normal[3];
  Arcadia_Real32Value offset; // dot(normal, point) + offset == 0
} Arcadia_Math_Plane3Real32;

typedef struct Arcadia_Math_Sphere3Real32 {
  Arcadia_Real32Value center[3];
  Arcadia_Real32Value radius;
} Arcadia_Math_Sphere3Real32;

Arcadia_BooleanValue Arcadia_Math_Ray3Real32_set(Arcadia_Math_Ray3Real32* self, const Arcadia_Real32Value origin[3], const Arcadia_Real32Value direction[3]);
void Arcadia_Math_Ray3Real32_pointAt(const Arcadia_Math_Ray3Real32* self, Arcadia_Real32Value t, Arcadia_Real32Value result[3]);
void Arcadia_Math_Ray3Real32_translate(Arcadia_Math_Ray3Real32* self, const Arcadia_Real32Value delta[3]);

// Plane normals are deliberately not normalized. signedOffset is a signed
// plane-equation value, not Euclidean distance unless the normal is unit length.
Arcadia_BooleanValue Arcadia_Math_Plane3Real32_setFromPointNormal(Arcadia_Math_Plane3Real32* self, const Arcadia_Real32Value point[3], const Arcadia_Real32Value normal[3]);
Arcadia_BooleanValue Arcadia_Math_Plane3Real32_setFromPoints(Arcadia_Math_Plane3Real32* self, const Arcadia_Real32Value a[3], const Arcadia_Real32Value b[3], const Arcadia_Real32Value c[3]);
Arcadia_Real32Value Arcadia_Math_Plane3Real32_signedOffset(const Arcadia_Math_Plane3Real32* self, const Arcadia_Real32Value point[3]);

Arcadia_BooleanValue Arcadia_Math_Sphere3Real32_set(Arcadia_Math_Sphere3Real32* self, const Arcadia_Real32Value center[3], Arcadia_Real32Value radius);
Arcadia_BooleanValue Arcadia_Math_Sphere3Real32_containsPoint(const Arcadia_Math_Sphere3Real32* self, const Arcadia_Real32Value point[3]);
Arcadia_BooleanValue Arcadia_Math_Sphere3Real32_intersectsSphere(const Arcadia_Math_Sphere3Real32* self, const Arcadia_Math_Sphere3Real32* other);

// A hit always has t >= 0. A ray starting inside a solid hits at t == 0.
// Tangencies and AABB boundary contacts count as hits. On a miss, all output
// parameters remain unchanged. The optional output parameters may be NULL.
Arcadia_BooleanValue Arcadia_Math_Ray3Real32_intersectPlane(const Arcadia_Math_Ray3Real32* ray, const Arcadia_Math_Plane3Real32* plane, Arcadia_Real32Value* t);
Arcadia_BooleanValue Arcadia_Math_Ray3Real32_intersectSphere(const Arcadia_Math_Ray3Real32* ray, const Arcadia_Math_Sphere3Real32* sphere, Arcadia_Real32Value* t);
// tEnter is zero when the ray starts inside the bounds; tExit is the exit time.
Arcadia_BooleanValue Arcadia_Math_Ray3Real32_intersectAABB(const Arcadia_Math_Ray3Real32* ray, const Arcadia_Math_AABB3Real32* bounds, Arcadia_Real32Value* tEnter, Arcadia_Real32Value* tExit);
// Two-sided Moller-Trumbore. Degenerate and coplanar triangles do not hit.
// u and v are barycentric coordinates for vertices b and c; a has 1-u-v.
Arcadia_BooleanValue Arcadia_Math_Ray3Real32_intersectTriangle(const Arcadia_Math_Ray3Real32* ray, const Arcadia_Real32Value a[3], const Arcadia_Real32Value b[3], const Arcadia_Real32Value c[3], Arcadia_Real32Value* t, Arcadia_Real32Value* u, Arcadia_Real32Value* v);

#endif // ARCADIA_MATH_IMPLEMENTATION_RAYGEOMETRY3REAL32_H_INCLUDED
