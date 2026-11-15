// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
// for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "NormalizeOrZero.h"
#include "Arcadia/Math/Include.h"
#include "Arcadia/Testing/Include.h"

#include <stdlib.h>

static void
testNormalizeOrZeroVector2Real32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Math_Vector2Real32* zero = Arcadia_Math_Vector2Real32_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)zero);

  // The legacy operation accepts zero length and leaves the zero vector intact.
  Arcadia_Math_Vector2Real32_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, zero->elements[0] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[1] == 0);
  const Arcadia_Real32Value values[2] = { 3, 4 };
  Arcadia_Math_Vector2Real32* vector = Arcadia_Math_Vector2Real32_createWithValues(thread, values);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)vector);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector2Real32_getLength(thread, vector) == 5);
  Arcadia_Math_Vector2Real32_normalizeOrZero(thread, vector);
  Arcadia_Tests_assertTrue(thread, vector->elements[0] > 0.599999f && vector->elements[0] < 0.600001f);
  Arcadia_Tests_assertTrue(thread, vector->elements[1] > 0.799999f && vector->elements[1] < 0.800001f);
  // Already normalized vectors should remain unit length under a second call.
  Arcadia_Math_Vector2Real32_normalizeOrZero(thread, vector);
  Arcadia_Math_Vector2Real32_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector2Real32_getLength(thread, vector) > 0.99999f &&
                                  Arcadia_Math_Vector2Real32_getLength(thread, vector) < 1.00001f);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_normalizeOrZeroVector2Real32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNormalizeOrZeroVector2Real32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testNormalizeOrZeroVector2Real64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Math_Vector2Real64* zero = Arcadia_Math_Vector2Real64_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)zero);

  // The legacy operation accepts zero length and leaves the zero vector intact.
  Arcadia_Math_Vector2Real64_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, zero->elements[0] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[1] == 0);
  const Arcadia_Real64Value values[2] = { 3, 4 };
  Arcadia_Math_Vector2Real64* vector = Arcadia_Math_Vector2Real64_createWithValues(thread, values);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)vector);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector2Real64_getLength(thread, vector) == 5);
  Arcadia_Math_Vector2Real64_normalizeOrZero(thread, vector);
  Arcadia_Tests_assertTrue(thread, vector->elements[0] > 0.599999999999 && vector->elements[0] < 0.600000000001);
  Arcadia_Tests_assertTrue(thread, vector->elements[1] > 0.799999999999 && vector->elements[1] < 0.800000000001);
  // Already normalized vectors should remain unit length under a second call.
  Arcadia_Math_Vector2Real64_normalizeOrZero(thread, vector);
  Arcadia_Math_Vector2Real64_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector2Real64_getLength(thread, vector) > 0.999999999999 &&
                                  Arcadia_Math_Vector2Real64_getLength(thread, vector) < 1.000000000001);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_normalizeOrZeroVector2Real64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNormalizeOrZeroVector2Real64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testNormalizeOrZeroVector3Real32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Math_Vector3Real32* zero = Arcadia_Math_Vector3Real32_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)zero);

  // The legacy operation accepts zero length and leaves the zero vector intact.
  Arcadia_Math_Vector3Real32_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, zero->elements[0] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[1] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[2] == 0);
  const Arcadia_Real32Value values[3] = { 0, 3, 4 };
  Arcadia_Math_Vector3Real32* vector = Arcadia_Math_Vector3Real32_createWithValues(thread, values);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)vector);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_getLength(thread, vector) == 5);
  Arcadia_Math_Vector3Real32_normalizeOrZero(thread, vector);
  Arcadia_Tests_assertTrue(thread, vector->elements[0] == 0);
  Arcadia_Tests_assertTrue(thread, vector->elements[1] > 0.599999f && vector->elements[1] < 0.600001f);
  Arcadia_Tests_assertTrue(thread, vector->elements[2] > 0.799999f && vector->elements[2] < 0.800001f);
  // Already normalized vectors should remain unit length under a second call.
  Arcadia_Math_Vector3Real32_normalizeOrZero(thread, vector);
  Arcadia_Math_Vector3Real32_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real32_getLength(thread, vector) > 0.99999f &&
                                  Arcadia_Math_Vector3Real32_getLength(thread, vector) < 1.00001f);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_normalizeOrZeroVector3Real32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNormalizeOrZeroVector3Real32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testNormalizeOrZeroVector3Real64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Math_Vector3Real64* zero = Arcadia_Math_Vector3Real64_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)zero);

  // The legacy operation accepts zero length and leaves the zero vector intact.
  Arcadia_Math_Vector3Real64_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, zero->elements[0] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[1] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[2] == 0);
  const Arcadia_Real64Value values[3] = { 0, 3, 4 };
  Arcadia_Math_Vector3Real64* vector = Arcadia_Math_Vector3Real64_createWithValues(thread, values);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)vector);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_getLength(thread, vector) == 5);
  Arcadia_Math_Vector3Real64_normalizeOrZero(thread, vector);
  Arcadia_Tests_assertTrue(thread, vector->elements[0] == 0);
  Arcadia_Tests_assertTrue(thread, vector->elements[1] > 0.599999999999 && vector->elements[1] < 0.600000000001);
  Arcadia_Tests_assertTrue(thread, vector->elements[2] > 0.799999999999 && vector->elements[2] < 0.800000000001);
  // Already normalized vectors should remain unit length under a second call.
  Arcadia_Math_Vector3Real64_normalizeOrZero(thread, vector);
  Arcadia_Math_Vector3Real64_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector3Real64_getLength(thread, vector) > 0.999999999999 &&
                                  Arcadia_Math_Vector3Real64_getLength(thread, vector) < 1.000000000001);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_normalizeOrZeroVector3Real64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNormalizeOrZeroVector3Real64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testNormalizeOrZeroVector4Real32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Math_Vector4Real32* zero = Arcadia_Math_Vector4Real32_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)zero);

  // The legacy operation accepts zero length and leaves the zero vector intact.
  Arcadia_Math_Vector4Real32_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, zero->elements[0] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[1] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[2] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[3] == 0);
  const Arcadia_Real32Value values[4] = { 0, 0, 3, 4 };
  Arcadia_Math_Vector4Real32* vector = Arcadia_Math_Vector4Real32_createWithValues(thread, values);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)vector);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector4Real32_getLength(thread, vector) == 5);
  Arcadia_Math_Vector4Real32_normalizeOrZero(thread, vector);
  Arcadia_Tests_assertTrue(thread, vector->elements[0] == 0);
  Arcadia_Tests_assertTrue(thread, vector->elements[1] == 0);
  Arcadia_Tests_assertTrue(thread, vector->elements[2] > 0.599999f && vector->elements[2] < 0.600001f);
  Arcadia_Tests_assertTrue(thread, vector->elements[3] > 0.799999f && vector->elements[3] < 0.800001f);
  // Already normalized vectors should remain unit length under a second call.
  Arcadia_Math_Vector4Real32_normalizeOrZero(thread, vector);
  Arcadia_Math_Vector4Real32_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector4Real32_getLength(thread, vector) > 0.99999f &&
                                  Arcadia_Math_Vector4Real32_getLength(thread, vector) < 1.00001f);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_normalizeOrZeroVector4Real32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNormalizeOrZeroVector4Real32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testNormalizeOrZeroVector4Real64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Math_Vector4Real64* zero = Arcadia_Math_Vector4Real64_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)zero);

  // The legacy operation accepts zero length and leaves the zero vector intact.
  Arcadia_Math_Vector4Real64_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, zero->elements[0] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[1] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[2] == 0);
  Arcadia_Tests_assertTrue(thread, zero->elements[3] == 0);
  const Arcadia_Real64Value values[4] = { 0, 0, 3, 4 };
  Arcadia_Math_Vector4Real64* vector = Arcadia_Math_Vector4Real64_createWithValues(thread, values);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)vector);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector4Real64_getLength(thread, vector) == 5);
  Arcadia_Math_Vector4Real64_normalizeOrZero(thread, vector);
  Arcadia_Tests_assertTrue(thread, vector->elements[0] == 0);
  Arcadia_Tests_assertTrue(thread, vector->elements[1] == 0);
  Arcadia_Tests_assertTrue(thread, vector->elements[2] > 0.599999999999 && vector->elements[2] < 0.600000000001);
  Arcadia_Tests_assertTrue(thread, vector->elements[3] > 0.799999999999 && vector->elements[3] < 0.800000000001);
  // Already normalized vectors should remain unit length under a second call.
  Arcadia_Math_Vector4Real64_normalizeOrZero(thread, vector);
  Arcadia_Math_Vector4Real64_normalizeOrZero(thread, zero);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_Vector4Real64_getLength(thread, vector) > 0.999999999999 &&
                                  Arcadia_Math_Vector4Real64_getLength(thread, vector) < 1.000000000001);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_normalizeOrZeroVector4Real64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNormalizeOrZeroVector4Real64) ? EXIT_SUCCESS : EXIT_FAILURE;
}
