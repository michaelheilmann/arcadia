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

// Included for each vector dimension and floating-point precision.
static void
Test(test)
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Scalar original[Dimensions];
  Scalar copied[Dimensions];
  for (Arcadia_SizeValue i = 0; i < Dimensions; ++i) {
    original[i] = (Scalar)(i + 2);
    copied[i] = (Scalar)-1;
  }
  Vector* vector = V(createWithValues)(thread, original);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)vector);
  V(copyValues)(thread, vector, copied);
  for (Arcadia_SizeValue i = 0; i < Dimensions; ++i) {
    Arcadia_Tests_assertTrue(thread, copied[i] == (Scalar)(i + 2));
  }
  original[0] = (Scalar)-100;
  copied[1] = (Scalar)-200;
  Arcadia_Tests_assertTrue(thread, vector->elements[0] == (Scalar)2);
  Arcadia_Tests_assertTrue(thread, vector->elements[1] == (Scalar)3);
  vector->elements[Dimensions - 1] = (Scalar)123;
  V(copyValues)(thread, vector, copied);
  Arcadia_Tests_assertTrue(thread, copied[Dimensions - 1] == (Scalar)123);
  // Aliasing the destination with the vector's own storage is safe.
  V(copyValues)(thread, vector, vector->elements);
  Arcadia_Tests_assertTrue(thread, vector->elements[Dimensions - 1] == (Scalar)123);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Test(run)
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Test(test)) ? EXIT_SUCCESS : EXIT_FAILURE;
}
