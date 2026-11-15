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

// This file is included with Scalar, Matrix, Box, M, B and Test defined.
static int
Test(matrixArithmetic)
  (
    void
  )
{
  Matrix a, b, result, expected;
  M(setIdentity)(&a);
  M(setScaling)(&b, 2, 3, 4);
  M(multiply)(&result, &a, &b);
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (result.elements[i][j] != b.elements[i][j]) return EXIT_FAILURE;
    }
  }
  const Scalar left[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  const Scalar right[9] = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
  const Scalar product[9] = { 30, 24, 18, 84, 69, 54, 138, 114, 90 };
  M(setValues)(&a, left);
  M(setValues)(&b, right);
  M(multiply)(&expected, &a, &b);
  Scalar values[9];
  M(copyValues)(&expected, values);
  for (int i = 0; i < 9; ++i) {
    if (values[i] != product[i]) return EXIT_FAILURE;
  }
  // In-place multiplication must work with either operand, unlike the
  // legacy matrix33 implementation (which also indexed a fourth row).
  result = a;
  M(multiply)(&result, &result, &b);
  M(copyValues)(&result, values);
  for (int i = 0; i < 9; ++i) if (values[i] != product[i]) return EXIT_FAILURE;
  result = b;
  M(multiply)(&result, &a, &result);
  M(copyValues)(&result, values);
  for (int i = 0; i < 9; ++i) if (values[i] != product[i]) return EXIT_FAILURE;
  result = a;
  M(multiply)(&result, &result, &result);
  if (result.elements[0][0] != 30 || result.elements[2][2] != 150) return EXIT_FAILURE;
  M(add)(&result, &a, &b);
  for (int i = 0; i < 9; ++i) if (result.elements[i / 3][i % 3] != 10) return EXIT_FAILURE;
  M(subtract)(&result, &result, &b);
  for (int i = 0; i < 9; ++i) if (result.elements[i / 3][i % 3] != left[i]) return EXIT_FAILURE;
  M(subtract)(&b, &b, &b);
  for (int i = 0; i < 9; ++i) if (b.elements[i / 3][i % 3] != 0) return EXIT_FAILURE;
  M(setValues)(&result, left);
  M(transpose)(&result, &result);
  if (result.elements[0][2] != 7 || result.elements[2][0] != 3) return EXIT_FAILURE;
  M(transpose)(&result, &result);
  M(copyValues)(&result, values);
  for (int i = 0; i < 9; ++i) if (values[i] != left[i]) return EXIT_FAILURE;
  values[0] = -900;
  if (result.elements[0][0] != 1) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

static int
Test(matrixTransform)
  (
    void
  )
{
  Matrix matrix;
  M(setScaling)(&matrix, 2, 3, 4);
  Scalar point[3] = { 5, -2, 7 };
  M(transformVector)(&matrix, point, point);
  if (point[0] != 10 || point[1] != -6 || point[2] != 28) return EXIT_FAILURE;
  const Scalar values[9] = { 0, 1, 0, 0, 0, 1, 1, 0, 0 };
  M(setValues)(&matrix, values);
  M(transformVector)(&matrix, point, point);
  if (point[0] != -6 || point[1] != 28 || point[2] != 10) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

static int
Test(aabbUnion)
  (
    void
  )
{
  Box a, b, result, empty;
  B(setEmpty)(&a);
  B(setEmpty)(&empty);
  Scalar p[3] = { 3, -2, 5 };
  B(includePoint)(&a, p);
  if (a.empty || a.minimum[0] != 3 || a.maximum[2] != 5) return EXIT_FAILURE;
  p[0] = -4; p[1] = 8; p[2] = 1;
  B(includePoint)(&a, p);
  if (a.minimum[0] != -4 || a.maximum[1] != 8 || a.minimum[2] != 1) return EXIT_FAILURE;
  const Scalar min[3] = { 10, 20, 30 };
  const Scalar max[3] = { 5, 25, 8 };
  B(setMinMax)(&b, min, max);
  if (b.minimum[0] != 5 || b.maximum[0] != 10 || b.minimum[2] != 8 || b.maximum[2] != 30) return EXIT_FAILURE;
  B(union)(&result, &a, &b);
  if (result.minimum[0] != -4 || result.maximum[0] != 10 ||
      result.minimum[1] != -2 || result.maximum[1] != 25 ||
      result.minimum[2] != 1 || result.maximum[2] != 30) return EXIT_FAILURE;
  B(union)(&a, &a, &b);
  if (a.minimum[0] != -4 || a.maximum[2] != 30) return EXIT_FAILURE;
  B(union)(&b, &empty, &b);
  if (b.minimum[0] != 5 || b.maximum[0] != 10) return EXIT_FAILURE;
  B(union)(&empty, &empty, &empty);
  if (!empty.empty) return EXIT_FAILURE;
  const Scalar inside[3] = { 0, 0, 2 };
  if (!B(containsPoint)(&a, inside)) return EXIT_FAILURE;
  const Scalar outside[3] = { 0, 0, 50 };
  if (B(containsPoint)(&a, outside)) return EXIT_FAILURE;
  B(translate)(&a, 2, 3, -1);
  if (a.minimum[0] != -2 || a.maximum[1] != 28 || a.minimum[2] != 0) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

static int
Test(aabbIntersection)
  (
    void
  )
{
  Box a, b, result;
  const Scalar aMin[3] = { 0, 0, 0 };
  const Scalar aMax[3] = { 10, 10, 10 };
  const Scalar bMin[3] = { 5, 8, 10 };
  const Scalar bMax[3] = { 12, 12, 12 };
  B(setMinMax)(&a, aMin, aMax);
  B(setMinMax)(&b, bMin, bMax);
  B(intersection)(&result, &a, &b);
  if (result.empty || result.minimum[0] != 5 || result.maximum[0] != 10 ||
      result.minimum[1] != 8 || result.maximum[1] != 10 ||
      result.minimum[2] != 10 || result.maximum[2] != 10) return EXIT_FAILURE;
  B(intersection)(&a, &a, &b);
  if (a.empty || a.minimum[0] != 5 || a.maximum[2] != 10) return EXIT_FAILURE;
  B(intersection)(&b, &a, &b);
  if (b.empty || b.maximum[1] != 10) return EXIT_FAILURE;
  const Scalar farMin[3] = { 20, 20, 20 };
  const Scalar farMax[3] = { 30, 30, 30 };
  B(setMinMax)(&b, farMin, farMax);
  B(intersection)(&a, &a, &b);
  if (!a.empty || a.minimum[0] != 0 || a.maximum[2] != 0) return EXIT_FAILURE;
  B(translate)(&a, 5, 5, 5);
  if (!a.empty) return EXIT_FAILURE;
  const Scalar p[3] = { 5, 5, 5 };
  if (B(containsPoint)(&a, p)) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
