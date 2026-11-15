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

#include "Arcadia/Testing/Include.h"

#include <stdlib.h>

#include "Arcadia/Math/Include.h"
#include "Vector2.h"
#include "ComponentMultiply.h"
#include "NormalizeOrZero.h"
#include "NativeValues.h"

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Math_Vector3Real32 *u, *v, *w;
  Arcadia_Value t;

  // u must be equal to itself.
  u = Arcadia_Math_Vector3Real32_create(thread);
  t = Arcadia_Value_makeObjectReferenceValue(u);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)u, &t));

  // u and v are created with the same component values => u and v must be equal..
  v = Arcadia_Math_Vector3Real32_create(thread);
  t = Arcadia_Value_makeObjectReferenceValue(v);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)u, &t));

  // w is clone of u => w and u must be equal.
  w = (Arcadia_Math_Vector3Real32*)Arcadia_Object_clone(thread, (Arcadia_Object*)u);
  t = Arcadia_Value_makeObjectReferenceValue(w);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)u, &t));
}

static void
testCubicBezierReal32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real32Value a[] = { 0.f, 0.f, 0.f };
  Arcadia_Real32Value b[] = { 0.f, 2.f, 4.f };
  Arcadia_Real32Value c[] = { 2.f, 2.f, 8.f };
  Arcadia_Real32Value d[] = { 2.f, 0.f, 12.f };

  Arcadia_Math_Vector3Real32* p0 = Arcadia_Math_Vector3Real32_createWithValues(thread, a);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p0);
  Arcadia_Math_Vector3Real32* p1 = Arcadia_Math_Vector3Real32_createWithValues(thread, b);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p1);
  Arcadia_Math_Vector3Real32* p2 = Arcadia_Math_Vector3Real32_createWithValues(thread, c);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p2);
  Arcadia_Math_Vector3Real32* p3 = Arcadia_Math_Vector3Real32_createWithValues(thread, d);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p3);
  Arcadia_Math_Vector3Real32* result = Arcadia_Math_Vector3Real32_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)result);

  Arcadia_Math_Vector3Real32_cubicBezier(thread, p0, p1, p2, p3, 0.f, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0.f &&
                                  result->elements[1] == 0.f && result->elements[2] == 0.f);
  Arcadia_Math_Vector3Real32_cubicBezier(thread, p0, p1, p2, p3, 1.f, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 2.f &&
                                  result->elements[1] == 0.f && result->elements[2] == 12.f);
  // Quarter point distinguishes the inner control points (unlike the midpoint).
  Arcadia_Math_Vector3Real32_cubicBezier(thread, p0, p1, p2, p3, 0.25f, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0.3125f &&
                                  result->elements[1] == 1.125f && result->elements[2] == 3.f);
  Arcadia_Math_Vector3Real32_cubicBezier(thread, p0, p1, p2, p3, 0.5f, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 1.f &&
                                  result->elements[1] == 1.5f && result->elements[2] == 6.f);
  // In-place evaluation: writing the result over a control point must work.
  Arcadia_Math_Vector3Real32_cubicBezier(thread, p0, p1, p2, p3, 0.5f, p0);
  Arcadia_Tests_assertTrue(thread, p0->elements[0] == 1.f &&
                                  p0->elements[1] == 1.5f && p0->elements[2] == 6.f);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
testCubicBezierReal64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real64Value a[] = { 0, 0, 0 };
  Arcadia_Real64Value b[] = { 0, 2, 4 };
  Arcadia_Real64Value c[] = { 2, 2, 8 };
  Arcadia_Real64Value d[] = { 2, 0, 12 };

  Arcadia_Math_Vector3Real64* p0 = Arcadia_Math_Vector3Real64_createWithValues(thread, a);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p0);
  Arcadia_Math_Vector3Real64* p1 = Arcadia_Math_Vector3Real64_createWithValues(thread, b);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p1);
  Arcadia_Math_Vector3Real64* p2 = Arcadia_Math_Vector3Real64_createWithValues(thread, c);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p2);
  Arcadia_Math_Vector3Real64* p3 = Arcadia_Math_Vector3Real64_createWithValues(thread, d);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p3);
  Arcadia_Math_Vector3Real64* result = Arcadia_Math_Vector3Real64_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)result);

  Arcadia_Math_Vector3Real64_cubicBezier(thread, p0, p1, p2, p3, 0, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0 &&
                                  result->elements[1] == 0 && result->elements[2] == 0);
  Arcadia_Math_Vector3Real64_cubicBezier(thread, p0, p1, p2, p3, 1, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 2 &&
                                  result->elements[1] == 0 && result->elements[2] == 12);
  // Quarter point distinguishes the inner control points (unlike the midpoint).
  Arcadia_Math_Vector3Real64_cubicBezier(thread, p0, p1, p2, p3, 0.25, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0.3125 &&
                                  result->elements[1] == 1.125 && result->elements[2] == 3);
  Arcadia_Math_Vector3Real64_cubicBezier(thread, p0, p1, p2, p3, 0.5, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 1 &&
                                  result->elements[1] == 1.5 && result->elements[2] == 6);
  // In-place evaluation: writing the result over a control point must work.
  Arcadia_Math_Vector3Real64_cubicBezier(thread, p0, p1, p2, p3, 0.5, p0);
  Arcadia_Tests_assertTrue(thread, p0->elements[0] == 1 &&
                                  p0->elements[1] == 1.5 && p0->elements[2] == 6);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runCubicBezierReal32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testCubicBezierReal32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runCubicBezierReal64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testCubicBezierReal64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testRayIntersectsSphereReal32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real32Value a[] = { 0.f, 0.f, 0.f };
  Arcadia_Real32Value b[] = { 0.f, 0.f, 2.f };
  Arcadia_Real32Value c[] = { 0.f, 0.f, 5.f };
  Arcadia_Math_Vector3Real32* origin = Arcadia_Math_Vector3Real32_createWithValues(thread, a);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)origin);
  Arcadia_Math_Vector3Real32* direction = Arcadia_Math_Vector3Real32_createWithValues(thread, b);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)direction);
  Arcadia_Math_Vector3Real32* center = Arcadia_Math_Vector3Real32_createWithValues(thread, c);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)center);

  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_rayIntersectsSphere(thread, origin, direction, center, 1.f));
  center->elements[0] = 1.f;
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_rayIntersectsSphere(thread, origin, direction, center, 1.f)); // tangent
  center->elements[0] = 2.f;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real32_rayIntersectsSphere(thread, origin, direction, center, 1.f));
  center->elements[0] = 0.f;
  direction->elements[2] = -2.f;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real32_rayIntersectsSphere(thread, origin, direction, center, 1.f));
  origin->elements[2] = 5.f;
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_rayIntersectsSphere(thread, origin, direction, center, 1.f)); // origin inside
  direction->elements[2] = 0.f;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real32_rayIntersectsSphere(thread, origin, direction, center, 1.f));
  direction->elements[2] = 1.f;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real32_rayIntersectsSphere(thread, origin, direction, center, -1.f));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
testRayIntersectsSphereReal64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real64Value a[] = { 0., 0., 0. };
  Arcadia_Real64Value b[] = { 0., 0., 2. };
  Arcadia_Real64Value c[] = { 0., 0., 5. };
  Arcadia_Math_Vector3Real64* origin = Arcadia_Math_Vector3Real64_createWithValues(thread, a);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)origin);
  Arcadia_Math_Vector3Real64* direction = Arcadia_Math_Vector3Real64_createWithValues(thread, b);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)direction);
  Arcadia_Math_Vector3Real64* center = Arcadia_Math_Vector3Real64_createWithValues(thread, c);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)center);

  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_rayIntersectsSphere(thread, origin, direction, center, 1.));
  center->elements[0] = 1.;
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_rayIntersectsSphere(thread, origin, direction, center, 1.)); // tangent
  center->elements[0] = 2.;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real64_rayIntersectsSphere(thread, origin, direction, center, 1.));
  center->elements[0] = 0.;
  direction->elements[2] = -2.;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real64_rayIntersectsSphere(thread, origin, direction, center, 1.));
  origin->elements[2] = 5.;
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_rayIntersectsSphere(thread, origin, direction, center, 1.)); // origin inside
  direction->elements[2] = 0.;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real64_rayIntersectsSphere(thread, origin, direction, center, 1.));
  direction->elements[2] = 1.;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real64_rayIntersectsSphere(thread, origin, direction, center, -1.));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runRayIntersectsSphereReal32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRayIntersectsSphereReal32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRayIntersectsSphereReal64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRayIntersectsSphereReal64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testRayIntersectsPlaneReal32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real32Value originValues[] = { 0.f, 0.f, 0.f };
  Arcadia_Real32Value directionValues[] = { 0.f, 0.f, 2.f };
  Arcadia_Real32Value normalValues[] = { 0.f, 0.f, 1.f };
  Arcadia_Math_Vector3Real32* origin = Arcadia_Math_Vector3Real32_createWithValues(thread, originValues);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)origin);
  Arcadia_Math_Vector3Real32* direction = Arcadia_Math_Vector3Real32_createWithValues(thread, directionValues);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)direction);
  Arcadia_Math_Vector3Real32* normal = Arcadia_Math_Vector3Real32_createWithValues(thread, normalValues);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)normal);
  Arcadia_Real32Value t = 99.f;

  // Plane z = 5; non-unit ray direction gives parameter 2.5, not distance 5.
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_rayIntersectsPlane(thread, origin, direction, normal, -5.f, &t));
  Arcadia_Tests_assertTrue(thread, t == 2.5f);
  // Reverse direction: the entire plane lies behind the ray, and t is unchanged.
  direction->elements[2] = -2.f;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real32_rayIntersectsPlane(thread, origin, direction, normal, -5.f, &t));
  Arcadia_Tests_assertTrue(thread, t == 2.5f);
  // Parallel ray, including the coplanar case, has no unique intersection.
  direction->elements[2] = 0.f;
  direction->elements[0] = 1.f;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real32_rayIntersectsPlane(thread, origin, direction, normal, -5.f, &t));
  origin->elements[2] = 5.f;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real32_rayIntersectsPlane(thread, origin, direction, normal, -5.f, &t));
  Arcadia_Tests_assertTrue(thread, t == 2.5f);
  // Starting on the plane with nonparallel direction is a hit at t = 0.
  direction->elements[0] = 0.f;
  direction->elements[2] = 2.f;
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_rayIntersectsPlane(thread, origin, direction, normal, -5.f, &t));
  Arcadia_Tests_assertTrue(thread, t == 0.f);
  // Scaling the plane equation does not affect the intersection parameter.
  origin->elements[2] = 0.f;
  normal->elements[2] = 3.f;
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_rayIntersectsPlane(thread, origin, direction, normal, -15.f, &t));
  Arcadia_Tests_assertTrue(thread, t == 2.5f);
  // Degenerate normal yields no unique plane/hit.
  normal->elements[2] = 0.f;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real32_rayIntersectsPlane(thread, origin, direction, normal, -5.f, &t));
  Arcadia_Tests_assertTrue(thread, t == 2.5f);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runRayIntersectsPlaneReal32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRayIntersectsPlaneReal32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testRayIntersectsPlaneReal64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real64Value originValues[] = { 0., 0., 0. };
  Arcadia_Real64Value directionValues[] = { 0., 0., 2. };
  Arcadia_Real64Value normalValues[] = { 0., 0., 1. };
  Arcadia_Math_Vector3Real64* origin = Arcadia_Math_Vector3Real64_createWithValues(thread, originValues);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)origin);
  Arcadia_Math_Vector3Real64* direction = Arcadia_Math_Vector3Real64_createWithValues(thread, directionValues);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)direction);
  Arcadia_Math_Vector3Real64* normal = Arcadia_Math_Vector3Real64_createWithValues(thread, normalValues);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)normal);
  Arcadia_Real64Value t = 99.;

  // Plane z = 5; non-unit ray direction gives parameter 2.5, not distance 5.
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_rayIntersectsPlane(thread, origin, direction, normal, -5., &t));
  Arcadia_Tests_assertTrue(thread, t == 2.5);
  // Reverse direction: the entire plane lies behind the ray, and t is unchanged.
  direction->elements[2] = -2.;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real64_rayIntersectsPlane(thread, origin, direction, normal, -5., &t));
  Arcadia_Tests_assertTrue(thread, t == 2.5);
  // Parallel ray, including the coplanar case, has no unique intersection.
  direction->elements[2] = 0.;
  direction->elements[0] = 1.;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real64_rayIntersectsPlane(thread, origin, direction, normal, -5., &t));
  origin->elements[2] = 5.;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real64_rayIntersectsPlane(thread, origin, direction, normal, -5., &t));
  Arcadia_Tests_assertTrue(thread, t == 2.5);
  // Starting on the plane with nonparallel direction is a hit at t = 0.
  direction->elements[0] = 0.;
  direction->elements[2] = 2.;
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_rayIntersectsPlane(thread, origin, direction, normal, -5., &t));
  Arcadia_Tests_assertTrue(thread, t == 0.);
  // Scaling the plane equation does not affect the intersection parameter.
  origin->elements[2] = 0.;
  normal->elements[2] = 3.;
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_rayIntersectsPlane(thread, origin, direction, normal, -15., &t));
  Arcadia_Tests_assertTrue(thread, t == 2.5);
  // Degenerate normal yields no unique plane/hit.
  normal->elements[2] = 0.;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real64_rayIntersectsPlane(thread, origin, direction, normal, -5., &t));
  Arcadia_Tests_assertTrue(thread, t == 2.5);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runRayIntersectsPlaneReal64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRayIntersectsPlaneReal64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testDeCasteljauReal32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real32Value a[] = { 0.f, 0.f, 0.f };
  Arcadia_Real32Value b[] = { 0.f, 2.f, 4.f };
  Arcadia_Real32Value c[] = { 2.f, 2.f, 8.f };
  Arcadia_Real32Value d[] = { 2.f, 0.f, 12.f };
  Arcadia_Math_Vector3Real32* p0 = Arcadia_Math_Vector3Real32_createWithValues(thread, a);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p0);
  Arcadia_Math_Vector3Real32* p1 = Arcadia_Math_Vector3Real32_createWithValues(thread, b);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p1);
  Arcadia_Math_Vector3Real32* p2 = Arcadia_Math_Vector3Real32_createWithValues(thread, c);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p2);
  Arcadia_Math_Vector3Real32* p3 = Arcadia_Math_Vector3Real32_createWithValues(thread, d);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p3);
  Arcadia_Math_Vector3Real32* result = Arcadia_Math_Vector3Real32_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)result);
  Arcadia_Math_Vector3Real32* controlPoints[] = { p0, p1, p2, p3 };
  Arcadia_Real32Value workspace[12];

  // Arbitrary-degree evaluator agrees with the already-integrated cubic Bezier.
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_deCasteljau(thread, controlPoints, 4, 0.25f, workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0.3125f &&
                                  result->elements[1] == 1.125f && result->elements[2] == 3.f);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_deCasteljau(thread, controlPoints, 4, 0.f, workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0.f &&
                                  result->elements[1] == 0.f && result->elements[2] == 0.f);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_deCasteljau(thread, controlPoints, 4, 1.f, workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 2.f &&
                                  result->elements[1] == 0.f && result->elements[2] == 12.f);
  // Degree zero and degree one behave correctly.
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_deCasteljau(thread, controlPoints, 1, 0.25f, workspace, 3, result));
  Arcadia_Tests_assertTrue(thread, result->elements[2] == 0.f);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_deCasteljau(thread, controlPoints, 2, 0.5f, workspace, 6, result));
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0.f &&
                                  result->elements[1] == 1.f && result->elements[2] == 2.f);
  // Invalid count/capacity leave result untouched.
  result->elements[2] = 99.f;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real32_deCasteljau(thread, controlPoints, 4, 0.5f, workspace, 11, result));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real32_deCasteljau(thread, controlPoints, 0, 0.5f, workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, result->elements[2] == 99.f);
  // In-place update must work for all three coordinates, including Z.
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_deCasteljau(thread, controlPoints, 4, 0.5f, workspace, 12, p0));
  Arcadia_Tests_assertTrue(thread, p0->elements[0] == 1.f &&
                                  p0->elements[1] == 1.5f && p0->elements[2] == 6.f);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runDeCasteljauReal32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testDeCasteljauReal32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testDeCasteljauReal64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real64Value a[] = { 0., 0., 0. };
  Arcadia_Real64Value b[] = { 0., 2., 4. };
  Arcadia_Real64Value c[] = { 2., 2., 8. };
  Arcadia_Real64Value d[] = { 2., 0., 12. };
  Arcadia_Math_Vector3Real64* p0 = Arcadia_Math_Vector3Real64_createWithValues(thread, a);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p0);
  Arcadia_Math_Vector3Real64* p1 = Arcadia_Math_Vector3Real64_createWithValues(thread, b);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p1);
  Arcadia_Math_Vector3Real64* p2 = Arcadia_Math_Vector3Real64_createWithValues(thread, c);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p2);
  Arcadia_Math_Vector3Real64* p3 = Arcadia_Math_Vector3Real64_createWithValues(thread, d);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p3);
  Arcadia_Math_Vector3Real64* result = Arcadia_Math_Vector3Real64_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)result);
  Arcadia_Math_Vector3Real64* controlPoints[] = { p0, p1, p2, p3 };
  Arcadia_Real64Value workspace[12];

  // Arbitrary-degree evaluator agrees with the already-integrated cubic Bezier.
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_deCasteljau(thread, controlPoints, 4, 0.25, workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0.3125 &&
                                  result->elements[1] == 1.125 && result->elements[2] == 3.);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_deCasteljau(thread, controlPoints, 4, 0., workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0. &&
                                  result->elements[1] == 0. && result->elements[2] == 0.);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_deCasteljau(thread, controlPoints, 4, 1., workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 2. &&
                                  result->elements[1] == 0. && result->elements[2] == 12.);
  // Degree zero and degree one behave correctly.
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_deCasteljau(thread, controlPoints, 1, 0.25, workspace, 3, result));
  Arcadia_Tests_assertTrue(thread, result->elements[2] == 0.);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_deCasteljau(thread, controlPoints, 2, 0.5, workspace, 6, result));
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0. &&
                                  result->elements[1] == 1. && result->elements[2] == 2.);
  // Invalid count/capacity leave result untouched.
  result->elements[2] = 99.;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real64_deCasteljau(thread, controlPoints, 4, 0.5, workspace, 11, result));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Math_Vector3Real64_deCasteljau(thread, controlPoints, 0, 0.5, workspace, 12, result));
  Arcadia_Tests_assertTrue(thread, result->elements[2] == 99.);
  // In-place update must work for all three coordinates, including Z.
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_deCasteljau(thread, controlPoints, 4, 0.5, workspace, 12, p0));
  Arcadia_Tests_assertTrue(thread, p0->elements[0] == 1. &&
                                  p0->elements[1] == 1.5 && p0->elements[2] == 6.);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runDeCasteljauReal64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testDeCasteljauReal64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testQuadraticBSplineSegmentReal32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real32Value a[] = { 0.f, 0.f, 0.f };
  Arcadia_Real32Value b[] = { 0.f, 2.f, 4.f };
  Arcadia_Real32Value c[] = { 2.f, 0.f, 8.f };
  Arcadia_Math_Vector3Real32* p0 = Arcadia_Math_Vector3Real32_createWithValues(thread, a);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p0);
  Arcadia_Math_Vector3Real32* p1 = Arcadia_Math_Vector3Real32_createWithValues(thread, b);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p1);
  Arcadia_Math_Vector3Real32* p2 = Arcadia_Math_Vector3Real32_createWithValues(thread, c);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p2);
  Arcadia_Math_Vector3Real32* result = Arcadia_Math_Vector3Real32_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)result);

  // The segment endpoints are midpoints of adjacent controls, not p0 and p2.
  Arcadia_Math_Vector3Real32_quadraticBSplineSegment(thread, p0, p1, p2, 0.f, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0.f &&
                                  result->elements[1] == 1.f && result->elements[2] == 2.f);
  Arcadia_Math_Vector3Real32_quadraticBSplineSegment(thread, p0, p1, p2, 1.f, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 1.f &&
                                  result->elements[1] == 1.f && result->elements[2] == 6.f);
  // Basis weights (1/8, 3/4, 1/8) differ from quadratic Bezier weights.
  Arcadia_Math_Vector3Real32_quadraticBSplineSegment(thread, p0, p1, p2, 0.5f, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0.25f &&
                                  result->elements[1] == 1.5f && result->elements[2] == 4.f);
  Arcadia_Math_Vector3Real32_quadraticBSplineSegment(thread, p0, p1, p2, 0.5f, p0);
  Arcadia_Tests_assertTrue(thread, p0->elements[0] == 0.25f &&
                                  p0->elements[1] == 1.5f && p0->elements[2] == 4.f);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runQuadraticBSplineSegmentReal32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testQuadraticBSplineSegmentReal32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testQuadraticBSplineSegmentReal64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real64Value a[] = { 0., 0., 0. };
  Arcadia_Real64Value b[] = { 0., 2., 4. };
  Arcadia_Real64Value c[] = { 2., 0., 8. };
  Arcadia_Math_Vector3Real64* p0 = Arcadia_Math_Vector3Real64_createWithValues(thread, a);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p0);
  Arcadia_Math_Vector3Real64* p1 = Arcadia_Math_Vector3Real64_createWithValues(thread, b);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p1);
  Arcadia_Math_Vector3Real64* p2 = Arcadia_Math_Vector3Real64_createWithValues(thread, c);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)p2);
  Arcadia_Math_Vector3Real64* result = Arcadia_Math_Vector3Real64_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)result);

  // The segment endpoints are midpoints of adjacent controls, not p0 and p2.
  Arcadia_Math_Vector3Real64_quadraticBSplineSegment(thread, p0, p1, p2, 0., result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0. &&
                                  result->elements[1] == 1. && result->elements[2] == 2.);
  Arcadia_Math_Vector3Real64_quadraticBSplineSegment(thread, p0, p1, p2, 1., result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 1. &&
                                  result->elements[1] == 1. && result->elements[2] == 6.);
  // Basis weights (1/8, 3/4, 1/8) differ from quadratic Bezier weights.
  Arcadia_Math_Vector3Real64_quadraticBSplineSegment(thread, p0, p1, p2, 0.5, result);
  Arcadia_Tests_assertTrue(thread, result->elements[0] == 0.25 &&
                                  result->elements[1] == 1.5 && result->elements[2] == 4.);
  Arcadia_Math_Vector3Real64_quadraticBSplineSegment(thread, p0, p1, p2, 0.5, p0);
  Arcadia_Tests_assertTrue(thread, p0->elements[0] == 0.25 &&
                                  p0->elements[1] == 1.5 && p0->elements[2] == 4.);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runQuadraticBSplineSegmentReal64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testQuadraticBSplineSegmentReal64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runTests
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Vector2Real32", &Arcadia_Math_Tests_Vector2_real32 },
    { "Vector2Real64", &Arcadia_Math_Tests_Vector2_real64 },
    { "VectorTemplateReal32", &Arcadia_Math_Tests_Vector2_templateReal32 },
    { "VectorTemplateReal64", &Arcadia_Math_Tests_Vector2_templateReal64 },
    { "NativeValuesVector2Real32", &Arcadia_Math_Tests_nativeVectorValues2Real32_run },
    { "NativeValuesVector2Real64", &Arcadia_Math_Tests_nativeVectorValues2Real64_run },
    { "NativeValuesVector3Real32", &Arcadia_Math_Tests_nativeVectorValues3Real32_run },
    { "NativeValuesVector3Real64", &Arcadia_Math_Tests_nativeVectorValues3Real64_run },
    { "NativeValuesVector4Real32", &Arcadia_Math_Tests_nativeVectorValues4Real32_run },
    { "NativeValuesVector4Real64", &Arcadia_Math_Tests_nativeVectorValues4Real64_run },
    { "MultiplyComponentsVector2Real32", &Arcadia_Math_Tests_multiplyComponentsVector2Real32 },
    { "MultiplyComponentsVector2Real64", &Arcadia_Math_Tests_multiplyComponentsVector2Real64 },
    { "MultiplyComponentsVector3Real32", &Arcadia_Math_Tests_multiplyComponentsVector3Real32 },
    { "MultiplyComponentsVector3Real64", &Arcadia_Math_Tests_multiplyComponentsVector3Real64 },
    { "MultiplyComponentsVector4Real32", &Arcadia_Math_Tests_multiplyComponentsVector4Real32 },
    { "MultiplyComponentsVector4Real64", &Arcadia_Math_Tests_multiplyComponentsVector4Real64 },
    { "NormalizeOrZeroVector2Real32", &Arcadia_Math_Tests_normalizeOrZeroVector2Real32 },
    { "NormalizeOrZeroVector2Real64", &Arcadia_Math_Tests_normalizeOrZeroVector2Real64 },
    { "NormalizeOrZeroVector3Real32", &Arcadia_Math_Tests_normalizeOrZeroVector3Real32 },
    { "NormalizeOrZeroVector3Real64", &Arcadia_Math_Tests_normalizeOrZeroVector3Real64 },
    { "NormalizeOrZeroVector4Real32", &Arcadia_Math_Tests_normalizeOrZeroVector4Real32 },
    { "NormalizeOrZeroVector4Real64", &Arcadia_Math_Tests_normalizeOrZeroVector4Real64 },
    { "VectorTests", &runTests },
    { "CubicBezierReal32", &runCubicBezierReal32 },
    { "CubicBezierReal64", &runCubicBezierReal64 },
    { "RayIntersectsSphereReal32", &runRayIntersectsSphereReal32 },
    { "RayIntersectsSphereReal64", &runRayIntersectsSphereReal64 },
    { "RayIntersectsPlaneReal32", &runRayIntersectsPlaneReal32 },
    { "RayIntersectsPlaneReal64", &runRayIntersectsPlaneReal64 },
    { "DeCasteljauReal32", &runDeCasteljauReal32 },
    { "DeCasteljauReal64", &runDeCasteljauReal64 },
    { "QuadraticBSplineSegmentReal32", &runQuadraticBSplineSegmentReal32 },
    { "QuadraticBSplineSegmentReal64", &runQuadraticBSplineSegmentReal64 },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
