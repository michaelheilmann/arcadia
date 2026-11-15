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

#include "Vector2.h"

#include "Arcadia/Math/Include.h"
#include "Arcadia/Testing/Include.h"

#include <stdlib.h>

// Each test owns its value-stack roots; none of the created vectors is a
// process-global fixture.  These cases exercise the generated classes.
static void
testVector2Real32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  const Arcadia_Real32Value first[] = { 3, 4 };
  const Arcadia_Real32Value second[] = { 1, 2 };
  Arcadia_Math_Vector2Real32* a = Arcadia_Math_Vector2Real32_createWithValues(thread, first);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Arcadia_Math_Vector2Real32* b = Arcadia_Math_Vector2Real32_createWithValues(thread, second);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 3 && a->elements[1] == 4);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector2Real32_getSquaredLength(thread, a) == 25);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector2Real32_getLength(thread, a) == 5);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector2Real32_dotProduct(thread, a, b) == 11);

  Arcadia_Math_Vector2Real32_multiplyScalar(thread, a, 2.f);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 6 && a->elements[1] == 8);
  Arcadia_Math_Vector2Real32_subtract(thread, a, b);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 5 && a->elements[1] == 6);
  Arcadia_Math_Vector2Real32_add(thread, a, b);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 6 && a->elements[1] == 8);
  Arcadia_Math_Vector2Real32_invert(thread, a, b);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == -1 && a->elements[1] == -2);
  Arcadia_Math_Vector2Real32_assign(thread, a, b);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 1 && a->elements[1] == 2);
  Arcadia_Math_Vector2Real32_assign(thread, a, a);
  Arcadia_Math_Vector2Real32_add(thread, a, a);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 2 && a->elements[1] == 4);

  a->elements[0] = 3;
  a->elements[1] = 4;
  Arcadia_Math_Vector2Real32_lerp(thread, a, b, 0.25f);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 2.5f && a->elements[1] == 3.5f);
  Arcadia_Math_Vector2Real32_lerp(thread, a, a, 0.5f);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 2.5f && a->elements[1] == 3.5f);
  Arcadia_Math_Vector2Real32_lerp(thread, a, b, 1.f);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 1 && a->elements[1] == 2);

  Arcadia_Math_Vector2Real32* clone = (Arcadia_Math_Vector2Real32*)Arcadia_Object_clone(thread, (Arcadia_Object*)a);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)clone);
  Arcadia_Value value = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)clone);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)a, &value));
  Arcadia_Math_Vector2Real32_invert(thread, clone, clone);
  Arcadia_Tests_assertTrue(thread, clone->elements[0] == -1 && clone->elements[1] == -2);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 1 && a->elements[1] == 2);

  Arcadia_Math_Vector2Real32* empty = Arcadia_Math_Vector2Real32_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)empty);
  Arcadia_Tests_assertTrue(thread, empty->elements[0] == 0.f && empty->elements[1] == 0.f);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_Vector2_real32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testVector2Real32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testVector2Real64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  const Arcadia_Real64Value first[] = { 3, 4 };
  const Arcadia_Real64Value second[] = { 1, 2 };
  Arcadia_Math_Vector2Real64* a = Arcadia_Math_Vector2Real64_createWithValues(thread, first);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Arcadia_Math_Vector2Real64* b = Arcadia_Math_Vector2Real64_createWithValues(thread, second);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 3 && a->elements[1] == 4);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector2Real64_getSquaredLength(thread, a) == 25);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector2Real64_getLength(thread, a) == 5);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector2Real64_dotProduct(thread, a, b) == 11);

  Arcadia_Math_Vector2Real64_multiplyScalar(thread, a, 2.);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 6 && a->elements[1] == 8);
  Arcadia_Math_Vector2Real64_subtract(thread, a, b);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 5 && a->elements[1] == 6);
  Arcadia_Math_Vector2Real64_add(thread, a, b);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 6 && a->elements[1] == 8);
  Arcadia_Math_Vector2Real64_invert(thread, a, b);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == -1 && a->elements[1] == -2);
  Arcadia_Math_Vector2Real64_assign(thread, a, b);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 1 && a->elements[1] == 2);
  Arcadia_Math_Vector2Real64_assign(thread, a, a);
  Arcadia_Math_Vector2Real64_add(thread, a, a);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 2 && a->elements[1] == 4);

  a->elements[0] = 3;
  a->elements[1] = 4;
  Arcadia_Math_Vector2Real64_lerp(thread, a, b, 0.25);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 2.5 && a->elements[1] == 3.5);
  Arcadia_Math_Vector2Real64_lerp(thread, a, a, 0.5);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 2.5 && a->elements[1] == 3.5);
  Arcadia_Math_Vector2Real64_lerp(thread, a, b, 1.);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 1 && a->elements[1] == 2);

  Arcadia_Math_Vector2Real64* clone = (Arcadia_Math_Vector2Real64*)Arcadia_Object_clone(thread, (Arcadia_Object*)a);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)clone);
  Arcadia_Value value = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)clone);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)a, &value));
  Arcadia_Math_Vector2Real64_invert(thread, clone, clone);
  Arcadia_Tests_assertTrue(thread, clone->elements[0] == -1 && clone->elements[1] == -2);
  Arcadia_Tests_assertTrue(thread, a->elements[0] == 1 && a->elements[1] == 2);

  Arcadia_Math_Vector2Real64* empty = Arcadia_Math_Vector2Real64_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)empty);
  Arcadia_Tests_assertTrue(thread, empty->elements[0] == 0. && empty->elements[1] == 0.);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
testNormalize64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  const Arcadia_Real64Value xy[] = { 1., 1. };
  Arcadia_Math_Vector2Real64* vector = Arcadia_Math_Vector2Real64_createWithValues(thread, xy);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)vector);
  Arcadia_Math_Vector2Real64_normalize(thread, vector);
  // Real32 intermediates lose ~1e-8 here; Real64 should retain ~1e-16.
  Arcadia_Tests_assertTrue(thread, vector->elements[0] > 0.70710678118654 && vector->elements[0] < 0.70710678118656);
  Arcadia_Tests_assertTrue(thread, vector->elements[1] > 0.70710678118654 && vector->elements[1] < 0.70710678118656);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_Vector2_real64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testVector2Real64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testTemplateReal32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  const Arcadia_Real32Value a3[] = { 0, 0, 0 };
  const Arcadia_Real32Value b3[] = { 4, 8, 12 };
  Arcadia_Math_Vector3Real32* left3 = Arcadia_Math_Vector3Real32_createWithValues(thread, a3);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)left3);
  Arcadia_Math_Vector3Real32* right3 = Arcadia_Math_Vector3Real32_createWithValues(thread, b3);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)right3);
  Arcadia_Math_Vector3Real32_lerp(thread, left3, right3, 0.25f);
  Arcadia_Tests_assertTrue(thread, left3->elements[0] == 1);
  Arcadia_Tests_assertTrue(thread, left3->elements[1] == 2);
  Arcadia_Tests_assertTrue(thread, left3->elements[2] == 3);
  Arcadia_Math_Vector3Real32_lerp(thread, right3, right3, 0.5f);
  Arcadia_Tests_assertTrue(thread, right3->elements[0] == 4);
  const Arcadia_Real32Value a4[] = { 0, 0, 0, 0 };
  const Arcadia_Real32Value b4[] = { 4, 8, 12, 16 };
  Arcadia_Math_Vector4Real32* left4 = Arcadia_Math_Vector4Real32_createWithValues(thread, a4);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)left4);
  Arcadia_Math_Vector4Real32* right4 = Arcadia_Math_Vector4Real32_createWithValues(thread, b4);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)right4);
  Arcadia_Math_Vector4Real32_lerp(thread, left4, right4, 0.25f);
  Arcadia_Tests_assertTrue(thread, left4->elements[0] == 1);
  Arcadia_Tests_assertTrue(thread, left4->elements[1] == 2);
  Arcadia_Tests_assertTrue(thread, left4->elements[2] == 3);
  Arcadia_Tests_assertTrue(thread, left4->elements[3] == 4);
  Arcadia_Math_Vector4Real32_lerp(thread, right4, right4, 0.5f);
  Arcadia_Tests_assertTrue(thread, right4->elements[0] == 4);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_Vector2_templateReal32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testTemplateReal32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testTemplateReal64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  const Arcadia_Real64Value a3[] = { 0, 0, 0 };
  const Arcadia_Real64Value b3[] = { 4, 8, 12 };
  Arcadia_Math_Vector3Real64* left3 = Arcadia_Math_Vector3Real64_createWithValues(thread, a3);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)left3);
  Arcadia_Math_Vector3Real64* right3 = Arcadia_Math_Vector3Real64_createWithValues(thread, b3);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)right3);
  Arcadia_Math_Vector3Real64_lerp(thread, left3, right3, 0.25);
  Arcadia_Tests_assertTrue(thread, left3->elements[0] == 1);
  Arcadia_Tests_assertTrue(thread, left3->elements[1] == 2);
  Arcadia_Tests_assertTrue(thread, left3->elements[2] == 3);
  Arcadia_Math_Vector3Real64_lerp(thread, right3, right3, 0.5);
  Arcadia_Tests_assertTrue(thread, right3->elements[0] == 4);
  const Arcadia_Real64Value diag3[] = { 1., 1., 1. };
  Arcadia_Math_Vector3Real64* unit3 = Arcadia_Math_Vector3Real64_createWithValues(thread, diag3);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)unit3);
  Arcadia_Math_Vector3Real64_normalize(thread, unit3);
  Arcadia_Tests_assertTrue(thread, unit3->elements[0] > 0.57735026918962 && unit3->elements[0] < 0.57735026918963);
  const Arcadia_Real64Value a4[] = { 0, 0, 0, 0 };
  const Arcadia_Real64Value b4[] = { 4, 8, 12, 16 };
  Arcadia_Math_Vector4Real64* left4 = Arcadia_Math_Vector4Real64_createWithValues(thread, a4);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)left4);
  Arcadia_Math_Vector4Real64* right4 = Arcadia_Math_Vector4Real64_createWithValues(thread, b4);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)right4);
  Arcadia_Math_Vector4Real64_lerp(thread, left4, right4, 0.25);
  Arcadia_Tests_assertTrue(thread, left4->elements[0] == 1);
  Arcadia_Tests_assertTrue(thread, left4->elements[1] == 2);
  Arcadia_Tests_assertTrue(thread, left4->elements[2] == 3);
  Arcadia_Tests_assertTrue(thread, left4->elements[3] == 4);
  Arcadia_Math_Vector4Real64_lerp(thread, right4, right4, 0.5);
  Arcadia_Tests_assertTrue(thread, right4->elements[0] == 4);
  const Arcadia_Real64Value diag4[] = { 1., 1., 1., 1. };
  Arcadia_Math_Vector4Real64* unit4 = Arcadia_Math_Vector4Real64_createWithValues(thread, diag4);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)unit4);
  Arcadia_Math_Vector4Real64_normalize(thread, unit4);
  Arcadia_Tests_assertTrue(thread, unit4->elements[0] > 0.49999999999999 && unit4->elements[0] < 0.50000000000001);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_Vector2_templateReal64
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(&testNormalize64)) return EXIT_FAILURE;
  return Arcadia_Tests_safeExecute(&testTemplateReal64) ? EXIT_SUCCESS : EXIT_FAILURE;
}
