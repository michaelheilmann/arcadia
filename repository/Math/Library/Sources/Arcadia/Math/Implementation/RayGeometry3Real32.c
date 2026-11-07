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

#include "Arcadia/Math/Implementation/RayGeometry3Real32.h"

#include <assert.h>
#include <float.h>
#include <math.h>

static Arcadia_Real32Value
dot3(const Arcadia_Real32Value a[3], const Arcadia_Real32Value b[3])
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static void
cross3(const Arcadia_Real32Value a[3], const Arcadia_Real32Value b[3], Arcadia_Real32Value result[3])
{
  result[0] = a[1] * b[2] - a[2] * b[1];
  result[1] = a[2] * b[0] - a[0] * b[2];
  result[2] = a[0] * b[1] - a[1] * b[0];
}

Arcadia_BooleanValue
Arcadia_Math_Ray3Real32_set(Arcadia_Math_Ray3Real32* self, const Arcadia_Real32Value origin[3], const Arcadia_Real32Value direction[3])
{
  assert(self && origin && direction);
  Arcadia_Real32Value d[3] = { direction[0], direction[1], direction[2] };
  Arcadia_Real32Value o[3] = { origin[0], origin[1], origin[2] };
  if (dot3(d, d) == 0.f) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    self->origin[i] = o[i];
    self->direction[i] = d[i];
  }
  return Arcadia_BooleanValue_True;
}

void
Arcadia_Math_Ray3Real32_pointAt(const Arcadia_Math_Ray3Real32* self, Arcadia_Real32Value t, Arcadia_Real32Value result[3])
{
  assert(self && result);
  Arcadia_Real32Value point[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i)
    point[i] = self->origin[i] + t * self->direction[i];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) result[i] = point[i];
}

void
Arcadia_Math_Ray3Real32_translate(Arcadia_Math_Ray3Real32* self, const Arcadia_Real32Value delta[3])
{
  assert(self && delta);
  for (Arcadia_SizeValue i = 0; i < 3; ++i) self->origin[i] += delta[i];
}

Arcadia_BooleanValue
Arcadia_Math_Plane3Real32_setFromPointNormal(Arcadia_Math_Plane3Real32* self, const Arcadia_Real32Value point[3], const Arcadia_Real32Value normal[3])
{
  assert(self && point && normal);
  Arcadia_Real32Value n[3] = { normal[0], normal[1], normal[2] };
  if (dot3(n, n) == 0.f) return Arcadia_BooleanValue_False;
  Arcadia_Real32Value offset = -dot3(n, point);
  for (Arcadia_SizeValue i = 0; i < 3; ++i) self->normal[i] = n[i];
  self->offset = offset;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Plane3Real32_setFromPoints(Arcadia_Math_Plane3Real32* self, const Arcadia_Real32Value a[3], const Arcadia_Real32Value b[3], const Arcadia_Real32Value c[3])
{
  assert(self && a && b && c);
  Arcadia_Real32Value ab[3], ac[3], normal[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    ab[i] = b[i] - a[i];
    ac[i] = c[i] - a[i];
  }
  cross3(ab, ac, normal);
  return Arcadia_Math_Plane3Real32_setFromPointNormal(self, a, normal);
}

Arcadia_Real32Value
Arcadia_Math_Plane3Real32_signedOffset(const Arcadia_Math_Plane3Real32* self, const Arcadia_Real32Value point[3])
{
  assert(self && point);
  return dot3(self->normal, point) + self->offset;
}

Arcadia_BooleanValue
Arcadia_Math_Sphere3Real32_set(Arcadia_Math_Sphere3Real32* self, const Arcadia_Real32Value center[3], Arcadia_Real32Value radius)
{
  assert(self && center);
  if (!(radius >= 0.f)) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) self->center[i] = center[i];
  self->radius = radius;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Sphere3Real32_containsPoint(const Arcadia_Math_Sphere3Real32* self, const Arcadia_Real32Value point[3])
{
  assert(self && point);
  Arcadia_Real32Value diff[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) diff[i] = point[i] - self->center[i];
  return dot3(diff, diff) <= self->radius * self->radius ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}

Arcadia_BooleanValue
Arcadia_Math_Sphere3Real32_intersectsSphere(const Arcadia_Math_Sphere3Real32* self, const Arcadia_Math_Sphere3Real32* other)
{
  assert(self && other);
  Arcadia_Real32Value diff[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) diff[i] = self->center[i] - other->center[i];
  Arcadia_Real32Value radiusSum = self->radius + other->radius;
  return dot3(diff, diff) <= radiusSum * radiusSum ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}

Arcadia_BooleanValue
Arcadia_Math_Ray3Real32_intersectPlane(const Arcadia_Math_Ray3Real32* ray, const Arcadia_Math_Plane3Real32* plane, Arcadia_Real32Value* t)
{
  assert(ray && plane);
  Arcadia_Real32Value denominator = dot3(plane->normal, ray->direction);
  if (denominator == 0.f) {
    // A coplanar ray intersects immediately; a parallel offset ray misses.
    if (Arcadia_Math_Plane3Real32_signedOffset(plane, ray->origin) != 0.f) return Arcadia_BooleanValue_False;
    if (t) *t = 0.f;
    return Arcadia_BooleanValue_True;
  }
  Arcadia_Real32Value hit = -Arcadia_Math_Plane3Real32_signedOffset(plane, ray->origin) / denominator;
  if (hit < 0.f) return Arcadia_BooleanValue_False;
  if (t) *t = hit;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Ray3Real32_intersectSphere(const Arcadia_Math_Ray3Real32* ray, const Arcadia_Math_Sphere3Real32* sphere, Arcadia_Real32Value* t)
{
  assert(ray && sphere);
  Arcadia_Real32Value m[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) m[i] = ray->origin[i] - sphere->center[i];
  Arcadia_Real32Value c = dot3(m, m) - sphere->radius * sphere->radius;
  if (c <= 0.f) {
    if (t) *t = 0.f;
    return Arcadia_BooleanValue_True;
  }
  Arcadia_Real32Value a = dot3(ray->direction, ray->direction);
  if (a == 0.f) return Arcadia_BooleanValue_False;
  Arcadia_Real32Value b = dot3(m, ray->direction);
  if (b >= 0.f) return Arcadia_BooleanValue_False;
  Arcadia_Real32Value discriminant = b * b - a * c;
  if (discriminant < 0.f) return Arcadia_BooleanValue_False;
  Arcadia_Real32Value hit = (-b - sqrtf(discriminant)) / a;
  if (hit < 0.f) return Arcadia_BooleanValue_False;
  if (t) *t = hit;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Ray3Real32_intersectAABB(const Arcadia_Math_Ray3Real32* ray, const Arcadia_Math_AABB3Real32* bounds, Arcadia_Real32Value* tEnter, Arcadia_Real32Value* tExit)
{
  assert(ray && bounds);
  if (bounds->empty) return Arcadia_BooleanValue_False;
  Arcadia_Real32Value nearValue = 0.f;
  Arcadia_Real32Value farValue = FLT_MAX;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_Real32Value d = ray->direction[i];
    Arcadia_Real32Value o = ray->origin[i];
    if (d == 0.f) {
      if (o < bounds->minimum[i] || o > bounds->maximum[i]) return Arcadia_BooleanValue_False;
      continue;
    }
    Arcadia_Real32Value a = (bounds->minimum[i] - o) / d;
    Arcadia_Real32Value b = (bounds->maximum[i] - o) / d;
    if (a > b) { Arcadia_Real32Value temp = a; a = b; b = temp; }
    if (a > nearValue) nearValue = a;
    if (b < farValue) farValue = b;
    if (nearValue > farValue) return Arcadia_BooleanValue_False;
  }
  if (farValue < 0.f) return Arcadia_BooleanValue_False;
  if (tEnter) *tEnter = nearValue;
  if (tExit) *tExit = farValue;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Ray3Real32_intersectTriangle(const Arcadia_Math_Ray3Real32* ray, const Arcadia_Real32Value a[3], const Arcadia_Real32Value b[3], const Arcadia_Real32Value c[3], Arcadia_Real32Value* t, Arcadia_Real32Value* u, Arcadia_Real32Value* v)
{
  assert(ray && a && b && c);
  Arcadia_Real32Value edge1[3], edge2[3], p[3], q[3], originOffset[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    edge1[i] = b[i] - a[i];
    edge2[i] = c[i] - a[i];
    originOffset[i] = ray->origin[i] - a[i];
  }
  cross3(ray->direction, edge2, p);
  Arcadia_Real32Value det = dot3(edge1, p);
  if (det == 0.f) return Arcadia_BooleanValue_False;
  Arcadia_Real32Value inverse = 1.f / det;
  Arcadia_Real32Value baryU = dot3(originOffset, p) * inverse;
  if (baryU < 0.f || baryU > 1.f) return Arcadia_BooleanValue_False;
  cross3(originOffset, edge1, q);
  Arcadia_Real32Value baryV = dot3(ray->direction, q) * inverse;
  if (baryV < 0.f || baryU + baryV > 1.f) return Arcadia_BooleanValue_False;
  Arcadia_Real32Value hit = dot3(edge2, q) * inverse;
  if (hit < 0.f) return Arcadia_BooleanValue_False;
  if (t) *t = hit;
  if (u) *u = baryU;
  if (v) *v = baryV;
  return Arcadia_BooleanValue_True;
}
