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

#include "Arcadia/Math/Implementation/RayGeometry3Real64.h"

#include <assert.h>
#include <float.h>
#include <math.h>

static Arcadia_Real64Value
dot3(const Arcadia_Real64Value a[3], const Arcadia_Real64Value b[3])
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static void
cross3(const Arcadia_Real64Value a[3], const Arcadia_Real64Value b[3], Arcadia_Real64Value result[3])
{
  result[0] = a[1] * b[2] - a[2] * b[1];
  result[1] = a[2] * b[0] - a[0] * b[2];
  result[2] = a[0] * b[1] - a[1] * b[0];
}

Arcadia_BooleanValue
Arcadia_Math_Ray3Real64_set(Arcadia_Math_Ray3Real64* self, const Arcadia_Real64Value origin[3], const Arcadia_Real64Value direction[3])
{
  assert(self && origin && direction);
  Arcadia_Real64Value d[3] = { direction[0], direction[1], direction[2] };
  Arcadia_Real64Value o[3] = { origin[0], origin[1], origin[2] };
  if (dot3(d, d) == 0.) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    self->origin[i] = o[i];
    self->direction[i] = d[i];
  }
  return Arcadia_BooleanValue_True;
}

void
Arcadia_Math_Ray3Real64_pointAt(const Arcadia_Math_Ray3Real64* self, Arcadia_Real64Value t, Arcadia_Real64Value result[3])
{
  assert(self && result);
  Arcadia_Real64Value point[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i)
    point[i] = self->origin[i] + t * self->direction[i];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) result[i] = point[i];
}

void
Arcadia_Math_Ray3Real64_translate(Arcadia_Math_Ray3Real64* self, const Arcadia_Real64Value delta[3])
{
  assert(self && delta);
  for (Arcadia_SizeValue i = 0; i < 3; ++i) self->origin[i] += delta[i];
}

Arcadia_BooleanValue
Arcadia_Math_Plane3Real64_setFromPointNormal(Arcadia_Math_Plane3Real64* self, const Arcadia_Real64Value point[3], const Arcadia_Real64Value normal[3])
{
  assert(self && point && normal);
  Arcadia_Real64Value n[3] = { normal[0], normal[1], normal[2] };
  if (dot3(n, n) == 0.) return Arcadia_BooleanValue_False;
  Arcadia_Real64Value offset = -dot3(n, point);
  for (Arcadia_SizeValue i = 0; i < 3; ++i) self->normal[i] = n[i];
  self->offset = offset;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Plane3Real64_setFromPoints(Arcadia_Math_Plane3Real64* self, const Arcadia_Real64Value a[3], const Arcadia_Real64Value b[3], const Arcadia_Real64Value c[3])
{
  assert(self && a && b && c);
  Arcadia_Real64Value ab[3], ac[3], normal[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    ab[i] = b[i] - a[i];
    ac[i] = c[i] - a[i];
  }
  cross3(ab, ac, normal);
  return Arcadia_Math_Plane3Real64_setFromPointNormal(self, a, normal);
}

Arcadia_Real64Value
Arcadia_Math_Plane3Real64_signedOffset(const Arcadia_Math_Plane3Real64* self, const Arcadia_Real64Value point[3])
{
  assert(self && point);
  return dot3(self->normal, point) + self->offset;
}

Arcadia_BooleanValue
Arcadia_Math_Sphere3Real64_set(Arcadia_Math_Sphere3Real64* self, const Arcadia_Real64Value center[3], Arcadia_Real64Value radius)
{
  assert(self && center);
  if (!(radius >= 0.)) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) self->center[i] = center[i];
  self->radius = radius;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Sphere3Real64_containsPoint(const Arcadia_Math_Sphere3Real64* self, const Arcadia_Real64Value point[3])
{
  assert(self && point);
  Arcadia_Real64Value diff[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) diff[i] = point[i] - self->center[i];
  return dot3(diff, diff) <= self->radius * self->radius ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}

Arcadia_BooleanValue
Arcadia_Math_Sphere3Real64_intersectsSphere(const Arcadia_Math_Sphere3Real64* self, const Arcadia_Math_Sphere3Real64* other)
{
  assert(self && other);
  Arcadia_Real64Value diff[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) diff[i] = self->center[i] - other->center[i];
  Arcadia_Real64Value radiusSum = self->radius + other->radius;
  return dot3(diff, diff) <= radiusSum * radiusSum ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}

Arcadia_BooleanValue
Arcadia_Math_Ray3Real64_intersectPlane(const Arcadia_Math_Ray3Real64* ray, const Arcadia_Math_Plane3Real64* plane, Arcadia_Real64Value* t)
{
  assert(ray && plane);
  Arcadia_Real64Value denominator = dot3(plane->normal, ray->direction);
  if (denominator == 0.) {
    // A coplanar ray intersects immediately; a parallel offset ray misses.
    if (Arcadia_Math_Plane3Real64_signedOffset(plane, ray->origin) != 0.) return Arcadia_BooleanValue_False;
    if (t) *t = 0.;
    return Arcadia_BooleanValue_True;
  }
  Arcadia_Real64Value hit = -Arcadia_Math_Plane3Real64_signedOffset(plane, ray->origin) / denominator;
  if (hit < 0.) return Arcadia_BooleanValue_False;
  if (t) *t = hit;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Ray3Real64_intersectSphere(const Arcadia_Math_Ray3Real64* ray, const Arcadia_Math_Sphere3Real64* sphere, Arcadia_Real64Value* t)
{
  assert(ray && sphere);
  Arcadia_Real64Value m[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) m[i] = ray->origin[i] - sphere->center[i];
  Arcadia_Real64Value c = dot3(m, m) - sphere->radius * sphere->radius;
  if (c <= 0.) {
    if (t) *t = 0.;
    return Arcadia_BooleanValue_True;
  }
  Arcadia_Real64Value a = dot3(ray->direction, ray->direction);
  if (a == 0.) return Arcadia_BooleanValue_False;
  Arcadia_Real64Value b = dot3(m, ray->direction);
  if (b >= 0.) return Arcadia_BooleanValue_False;
  Arcadia_Real64Value discriminant = b * b - a * c;
  if (discriminant < 0.) return Arcadia_BooleanValue_False;
  Arcadia_Real64Value hit = (-b - sqrt(discriminant)) / a;
  if (hit < 0.) return Arcadia_BooleanValue_False;
  if (t) *t = hit;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Ray3Real64_intersectAABB(const Arcadia_Math_Ray3Real64* ray, const Arcadia_Math_AABB3Real64* bounds, Arcadia_Real64Value* tEnter, Arcadia_Real64Value* tExit)
{
  assert(ray && bounds);
  if (bounds->empty) return Arcadia_BooleanValue_False;
  Arcadia_Real64Value nearValue = 0.;
  Arcadia_Real64Value farValue = DBL_MAX;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_Real64Value d = ray->direction[i];
    Arcadia_Real64Value o = ray->origin[i];
    if (d == 0.) {
      if (o < bounds->minimum[i] || o > bounds->maximum[i]) return Arcadia_BooleanValue_False;
      continue;
    }
    Arcadia_Real64Value a = (bounds->minimum[i] - o) / d;
    Arcadia_Real64Value b = (bounds->maximum[i] - o) / d;
    if (a > b) { Arcadia_Real64Value temp = a; a = b; b = temp; }
    if (a > nearValue) nearValue = a;
    if (b < farValue) farValue = b;
    if (nearValue > farValue) return Arcadia_BooleanValue_False;
  }
  if (farValue < 0.) return Arcadia_BooleanValue_False;
  if (tEnter) *tEnter = nearValue;
  if (tExit) *tExit = farValue;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Ray3Real64_intersectTriangle(const Arcadia_Math_Ray3Real64* ray, const Arcadia_Real64Value a[3], const Arcadia_Real64Value b[3], const Arcadia_Real64Value c[3], Arcadia_Real64Value* t, Arcadia_Real64Value* u, Arcadia_Real64Value* v)
{
  assert(ray && a && b && c);
  Arcadia_Real64Value edge1[3], edge2[3], p[3], q[3], originOffset[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    edge1[i] = b[i] - a[i];
    edge2[i] = c[i] - a[i];
    originOffset[i] = ray->origin[i] - a[i];
  }
  cross3(ray->direction, edge2, p);
  Arcadia_Real64Value det = dot3(edge1, p);
  if (det == 0.) return Arcadia_BooleanValue_False;
  Arcadia_Real64Value inverse = 1. / det;
  Arcadia_Real64Value baryU = dot3(originOffset, p) * inverse;
  if (baryU < 0. || baryU > 1.) return Arcadia_BooleanValue_False;
  cross3(originOffset, edge1, q);
  Arcadia_Real64Value baryV = dot3(ray->direction, q) * inverse;
  if (baryV < 0. || baryU + baryV > 1.) return Arcadia_BooleanValue_False;
  Arcadia_Real64Value hit = dot3(edge2, q) * inverse;
  if (hit < 0.) return Arcadia_BooleanValue_False;
  if (t) *t = hit;
  if (u) *u = baryU;
  if (v) *v = baryV;
  return Arcadia_BooleanValue_True;
}
