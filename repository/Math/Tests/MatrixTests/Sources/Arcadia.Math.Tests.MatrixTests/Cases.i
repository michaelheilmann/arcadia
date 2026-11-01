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

// Included for each precision so identical mathematical assertions cover both APIs.
static void
Test(assertNear)
  (
    Arcadia_Thread* thread,
    Scalar actual,
    Scalar expected
  )
{
  Arcadia_Tests_assertTrue(thread, fabs((double)actual - expected) <= Tolerance * (1. + fabs((double)expected)));
}

static void
Test(expectInvalidLookAt)
  (
    Arcadia_Thread* thread,
    Matrix* matrix,
    Vector* eye,
    Vector* target,
    Vector* up
  )
{
  M(setIdentity)(thread, matrix);
  Arcadia_JumpTarget jumpTarget;
  volatile Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    G(setLookAt)(thread, matrix, eye, target, up);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_setStatus(thread, 0);
    raised = Arcadia_BooleanValue_True;
  }
  Arcadia_Tests_assertTrue(thread, raised);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    for (Arcadia_SizeValue j = 0; j < 4; ++j) {
      Test(assertNear)(thread, matrix->elements[i][j], i == j ? 1 : 0);
    }
  }
}

static void
Test(run)
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Matrix* a = M(create)(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Matrix* b = M(create)(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);
  Vector* v = V(create)(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)v);
  Vector* eye = V(create)(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)eye);
  Vector* target = V(create)(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)target);
  Vector* up = V(create)(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)up);

  Scalar values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
  M(setWithValues)(thread, a, values);
  M(transpose)(thread, b, a);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    for (Arcadia_SizeValue j = 0; j < 4; ++j) {
      Test(assertNear)(thread, b->elements[i][j], values[j * 4 + i]);
      Test(assertNear)(thread, a->elements[i][j], values[i * 4 + j]);
    }
  }
  M(transpose)(thread, b, b);
  M(transpose)(thread, a, a);
  M(transpose)(thread, a, a);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    for (Arcadia_SizeValue j = 0; j < 4; ++j) {
      Test(assertNear)(thread, a->elements[i][j], values[i * 4 + j]);
      Test(assertNear)(thread, b->elements[i][j], values[i * 4 + j]);
    }
  }

  M(setIdentity)(thread, a);
  Test(assertNear)(thread, M(getDeterminant)(thread, a), 1);
  M(setZero)(thread, a);
  Test(assertNear)(thread, M(getDeterminant)(thread, a), 0);
  Scalar triangular[] = { 2, 1, 3, 5, 0, -3, 7, 9, 0, 0, 4, 11, 0, 0, 0, 5 };
  M(setWithValues)(thread, a, triangular);
  Test(assertNear)(thread, M(getDeterminant)(thread, a), -120);
  // Expected determinant independently checked using all 24 signed permutations.
  Scalar dense[] = { 3, 2, -1, 4, 2, 1, 5, 7, 0, 5, 2, -6, -1, 2, 1, 0 };
  M(setWithValues)(thread, a, dense);
  Test(assertNear)(thread, M(getDeterminant)(thread, a), -418);
  for (Arcadia_SizeValue j = 0; j < 4; ++j) {
    Test(assertNear)(thread, a->elements[0][j], dense[j]);
    Scalar temporary = a->elements[0][j];
    a->elements[0][j] = a->elements[1][j];
    a->elements[1][j] = temporary;
  }
  Test(assertNear)(thread, M(getDeterminant)(thread, a), 418);
  for (Arcadia_SizeValue j = 0; j < 4; ++j) {
    a->elements[1][j] = a->elements[0][j];
  }
  Test(assertNear)(thread, M(getDeterminant)(thread, a), 0);
  M(setZero)(thread, a);
  a->elements[0][0] = 2; a->elements[1][1] = -3; a->elements[2][2] = 4; a->elements[3][3] = 5;
  Test(assertNear)(thread, M(getDeterminant)(thread, a), -120);

  M(setIdentity)(thread, a);
  v->elements[0] = 2; v->elements[1] = 3; v->elements[2] = 5;
  G(transformPoint)(thread, a, v);
  G(transformDirection)(thread, a, v);
  Test(assertNear)(thread, v->elements[0], 2);
  Test(assertNear)(thread, v->elements[1], 3);
  Test(assertNear)(thread, v->elements[2], 5);
  G(setTranslation)(thread, a, 7, 11, 13);
  G(transformDirection)(thread, a, v);
  Test(assertNear)(thread, v->elements[0], 2);
  Test(assertNear)(thread, v->elements[1], 3);
  Test(assertNear)(thread, v->elements[2], 5);
  G(transformPoint)(thread, a, v);
  Test(assertNear)(thread, v->elements[0], 9);
  Test(assertNear)(thread, v->elements[1], 14);
  Test(assertNear)(thread, v->elements[2], 18);

  Scalar affine[] = { 1, 2, 3, 7, 4, 5, 6, 11, 7, 8, 10, 13, 0, 0, 0, 1 };
  M(setWithValues)(thread, a, affine);
  v->elements[0] = 2; v->elements[1] = 3; v->elements[2] = 5;
  G(transformPoint)(thread, a, v);
  Test(assertNear)(thread, v->elements[0], 30);
  Test(assertNear)(thread, v->elements[1], 64);
  Test(assertNear)(thread, v->elements[2], 101);
  v->elements[0] = 2; v->elements[1] = 3; v->elements[2] = 5;
  G(transformDirection)(thread, a, v);
  Test(assertNear)(thread, v->elements[0], 23);
  Test(assertNear)(thread, v->elements[1], 53);
  Test(assertNear)(thread, v->elements[2], 88);
  // A quarter turn around Z combined with nonuniform scaling.
  Scalar rotationScale[] = { 0, -3, 0, 0, 2, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1 };
  M(setWithValues)(thread, a, rotationScale);
  v->elements[0] = 2; v->elements[1] = 3; v->elements[2] = 5;
  G(transformDirection)(thread, a, v);
  Test(assertNear)(thread, v->elements[0], -9);
  Test(assertNear)(thread, v->elements[1], 4);
  Test(assertNear)(thread, v->elements[2], 20);

  target->elements[2] = -1;
  up->elements[1] = 1;
  G(setLookAt)(thread, a, eye, target, up);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    for (Arcadia_SizeValue j = 0; j < 4; ++j) {
      Test(assertNear)(thread, a->elements[i][j], i == j ? 1 : 0);
    }
  }
  eye->elements[0] = 2; eye->elements[1] = 3; eye->elements[2] = 5;
  target->elements[0] = -1; target->elements[1] = 1; target->elements[2] = -2;
  up->elements[0] = 1; up->elements[1] = 4; up->elements[2] = 2;
  G(setLookAt)(thread, a, eye, target, up);
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    for (Arcadia_SizeValue j = 0; j < 3; ++j) {
      Scalar dot = 0;
      for (Arcadia_SizeValue k = 0; k < 3; ++k) {
        dot += a->elements[i][k] * a->elements[j][k];
      }
      Test(assertNear)(thread, dot, i == j ? 1 : 0);
    }
  }
  Test(assertNear)(thread, M(getDeterminant)(thread, a), 1);
  V(assign)(thread, v, eye);
  G(transformPoint)(thread, a, v);
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Test(assertNear)(thread, v->elements[i], 0);
  }
  V(assign)(thread, v, target);
  G(transformPoint)(thread, a, v);
  Test(assertNear)(thread, v->elements[0], 0);
  Test(assertNear)(thread, v->elements[1], 0);
  Test(assertNear)(thread, v->elements[2], (Scalar)-sqrt(62.));

  V(assign)(thread, target, eye);
  Test(expectInvalidLookAt)(thread, a, eye, target, up);
  target->elements[2] -= 1;
  up->elements[0] = 0; up->elements[1] = 0; up->elements[2] = 0;
  Test(expectInvalidLookAt)(thread, a, eye, target, up);
  up->elements[2] = 2;
  Test(expectInvalidLookAt)(thread, a, eye, target, up);
  up->elements[1] = (Scalar)NAN;
  Test(expectInvalidLookAt)(thread, a, eye, target, up);
  up->elements[1] = 1;
  target->elements[0] = (Scalar)INFINITY;
  Test(expectInvalidLookAt)(thread, a, eye, target, up);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}
