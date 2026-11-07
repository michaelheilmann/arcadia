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

#include "Arcadia/Math/Include.h"
#include "Arcadia/Testing/Include.h"
#include "ComponentMultiply.h"

#include <stdlib.h>

static void
testMultiplyComponentsVector2Real32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real32Value inputA[2] = { 2, 3 };
  Arcadia_Real32Value inputB[2] = { 5, 7 };
  Arcadia_Math_Vector2Real32* a = Arcadia_Math_Vector2Real32_createWithValues(thread, inputA);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Arcadia_Math_Vector2Real32* b = Arcadia_Math_Vector2Real32_createWithValues(thread, inputB);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);

  // Distinct operands: match the old component-wise vector multiplication.
  Arcadia_Math_Vector2Real32_multiplyComponents(thread, a, b);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 10 &&
      a->elements[1] == 21);
  // The RHS must not have been modified.
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 5 &&
      b->elements[1] == 7);

  // Aliased operands: every component must be squared in place.
  Arcadia_Math_Vector2Real32_multiplyComponents(thread, a, a);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 100 &&
      a->elements[1] == 441);

  // Using a previously updated RHS must read that RHS, not a cached input.
  Arcadia_Math_Vector2Real32_multiplyComponents(thread, b, a);
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 500 &&
      b->elements[1] == 3087);

  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_multiplyComponentsVector2Real32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMultiplyComponentsVector2Real32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testMultiplyComponentsVector2Real64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real64Value inputA[2] = { 2, 3 };
  Arcadia_Real64Value inputB[2] = { 5, 7 };
  Arcadia_Math_Vector2Real64* a = Arcadia_Math_Vector2Real64_createWithValues(thread, inputA);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Arcadia_Math_Vector2Real64* b = Arcadia_Math_Vector2Real64_createWithValues(thread, inputB);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);

  // Distinct operands: match the old component-wise vector multiplication.
  Arcadia_Math_Vector2Real64_multiplyComponents(thread, a, b);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 10 &&
      a->elements[1] == 21);
  // The RHS must not have been modified.
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 5 &&
      b->elements[1] == 7);

  // Aliased operands: every component must be squared in place.
  Arcadia_Math_Vector2Real64_multiplyComponents(thread, a, a);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 100 &&
      a->elements[1] == 441);

  // Using a previously updated RHS must read that RHS, not a cached input.
  Arcadia_Math_Vector2Real64_multiplyComponents(thread, b, a);
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 500 &&
      b->elements[1] == 3087);

  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_multiplyComponentsVector2Real64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMultiplyComponentsVector2Real64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testMultiplyComponentsVector3Real32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real32Value inputA[3] = { 2, 3, 4 };
  Arcadia_Real32Value inputB[3] = { 5, 7, 11 };
  Arcadia_Math_Vector3Real32* a = Arcadia_Math_Vector3Real32_createWithValues(thread, inputA);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Arcadia_Math_Vector3Real32* b = Arcadia_Math_Vector3Real32_createWithValues(thread, inputB);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);

  // Distinct operands: match the old component-wise vector multiplication.
  Arcadia_Math_Vector3Real32_multiplyComponents(thread, a, b);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 10 &&
      a->elements[1] == 21 &&
      a->elements[2] == 44);
  // The RHS must not have been modified.
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 5 &&
      b->elements[1] == 7 &&
      b->elements[2] == 11);

  // Aliased operands: every component must be squared in place.
  Arcadia_Math_Vector3Real32_multiplyComponents(thread, a, a);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 100 &&
      a->elements[1] == 441 &&
      a->elements[2] == 1936);

  // Using a previously updated RHS must read that RHS, not a cached input.
  Arcadia_Math_Vector3Real32_multiplyComponents(thread, b, a);
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 500 &&
      b->elements[1] == 3087 &&
      b->elements[2] == 21296);

  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_multiplyComponentsVector3Real32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMultiplyComponentsVector3Real32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testMultiplyComponentsVector3Real64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real64Value inputA[3] = { 2, 3, 4 };
  Arcadia_Real64Value inputB[3] = { 5, 7, 11 };
  Arcadia_Math_Vector3Real64* a = Arcadia_Math_Vector3Real64_createWithValues(thread, inputA);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Arcadia_Math_Vector3Real64* b = Arcadia_Math_Vector3Real64_createWithValues(thread, inputB);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);

  // Distinct operands: match the old component-wise vector multiplication.
  Arcadia_Math_Vector3Real64_multiplyComponents(thread, a, b);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 10 &&
      a->elements[1] == 21 &&
      a->elements[2] == 44);
  // The RHS must not have been modified.
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 5 &&
      b->elements[1] == 7 &&
      b->elements[2] == 11);

  // Aliased operands: every component must be squared in place.
  Arcadia_Math_Vector3Real64_multiplyComponents(thread, a, a);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 100 &&
      a->elements[1] == 441 &&
      a->elements[2] == 1936);

  // Using a previously updated RHS must read that RHS, not a cached input.
  Arcadia_Math_Vector3Real64_multiplyComponents(thread, b, a);
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 500 &&
      b->elements[1] == 3087 &&
      b->elements[2] == 21296);

  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_multiplyComponentsVector3Real64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMultiplyComponentsVector3Real64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testMultiplyComponentsVector4Real32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real32Value inputA[4] = { 2, 3, 4, 5 };
  Arcadia_Real32Value inputB[4] = { 5, 7, 11, 13 };
  Arcadia_Math_Vector4Real32* a = Arcadia_Math_Vector4Real32_createWithValues(thread, inputA);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Arcadia_Math_Vector4Real32* b = Arcadia_Math_Vector4Real32_createWithValues(thread, inputB);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);

  // Distinct operands: match the old component-wise vector multiplication.
  Arcadia_Math_Vector4Real32_multiplyComponents(thread, a, b);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 10 &&
      a->elements[1] == 21 &&
      a->elements[2] == 44 &&
      a->elements[3] == 65);
  // The RHS must not have been modified.
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 5 &&
      b->elements[1] == 7 &&
      b->elements[2] == 11 &&
      b->elements[3] == 13);

  // Aliased operands: every component must be squared in place.
  Arcadia_Math_Vector4Real32_multiplyComponents(thread, a, a);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 100 &&
      a->elements[1] == 441 &&
      a->elements[2] == 1936 &&
      a->elements[3] == 4225);

  // Using a previously updated RHS must read that RHS, not a cached input.
  Arcadia_Math_Vector4Real32_multiplyComponents(thread, b, a);
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 500 &&
      b->elements[1] == 3087 &&
      b->elements[2] == 21296 &&
      b->elements[3] == 54925);

  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_multiplyComponentsVector4Real32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMultiplyComponentsVector4Real32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testMultiplyComponentsVector4Real64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Real64Value inputA[4] = { 2, 3, 4, 5 };
  Arcadia_Real64Value inputB[4] = { 5, 7, 11, 13 };
  Arcadia_Math_Vector4Real64* a = Arcadia_Math_Vector4Real64_createWithValues(thread, inputA);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Arcadia_Math_Vector4Real64* b = Arcadia_Math_Vector4Real64_createWithValues(thread, inputB);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);

  // Distinct operands: match the old component-wise vector multiplication.
  Arcadia_Math_Vector4Real64_multiplyComponents(thread, a, b);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 10 &&
      a->elements[1] == 21 &&
      a->elements[2] == 44 &&
      a->elements[3] == 65);
  // The RHS must not have been modified.
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 5 &&
      b->elements[1] == 7 &&
      b->elements[2] == 11 &&
      b->elements[3] == 13);

  // Aliased operands: every component must be squared in place.
  Arcadia_Math_Vector4Real64_multiplyComponents(thread, a, a);
  Arcadia_Tests_assertTrue(thread,
      a->elements[0] == 100 &&
      a->elements[1] == 441 &&
      a->elements[2] == 1936 &&
      a->elements[3] == 4225);

  // Using a previously updated RHS must read that RHS, not a cached input.
  Arcadia_Math_Vector4Real64_multiplyComponents(thread, b, a);
  Arcadia_Tests_assertTrue(thread,
      b->elements[0] == 500 &&
      b->elements[1] == 3087 &&
      b->elements[2] == 21296 &&
      b->elements[3] == 54925);

  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Math_Tests_multiplyComponentsVector4Real64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMultiplyComponentsVector4Real64) ? EXIT_SUCCESS : EXIT_FAILURE;
}
