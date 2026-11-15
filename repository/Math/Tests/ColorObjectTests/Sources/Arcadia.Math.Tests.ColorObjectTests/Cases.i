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

// Included twice so both precision-specific generated color APIs are tested.
static void
Test(near)
  (
    Arcadia_Thread* thread,
    Scalar actual,
    Scalar expected
  )
{
  Arcadia_Tests_assertTrue(thread, fabs((double)actual - (double)expected) <= Tolerance);
}

static void
Test(construction)
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  // The fractional component distinguishes the Real64 value-stack getter
  // from the incorrect Real32 getter in the old template.
  const Scalar values[4] = { (Scalar)0.123456789012345, (Scalar)0.25, (Scalar)0.5, (Scalar)0.875 };
  Color* arrayColor = C(create4v)(thread, values);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)arrayColor);
  Color* scalarColor = C(create4)(thread, values[0], values[1], values[2], values[3]);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)scalarColor);
  Color* zero = C(create)(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)zero);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    Arcadia_Tests_assertTrue(thread, arrayColor->components[i] == values[i]);
    Arcadia_Tests_assertTrue(thread, scalarColor->components[i] == values[i]);
    Arcadia_Tests_assertTrue(thread, zero->components[i] == (Scalar)0);
  }
  Arcadia_Value reference = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)scalarColor);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)arrayColor, &reference));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
Test(arithmetic)
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  const Scalar left[4] = { (Scalar)0.2, (Scalar)0.4, (Scalar)-0.2, (Scalar)0.8 };
  const Scalar right[4] = { (Scalar)0.5, (Scalar)0.7, (Scalar)0.1, (Scalar)0.5 };
  Color* a = C(create4v)(thread, left);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Color* b = C(create4v)(thread, right);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);
  C(add)(thread, a, b);
  const Scalar sum[4] = { (Scalar)0.7, (Scalar)1, (Scalar)0, (Scalar)1 };
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    Test(near)(thread, a->components[i], sum[i]);
  }
  // Invert must read other, not the original value of a.
  C(invert)(thread, a, b);
  const Scalar inverse[4] = { (Scalar)0.5, (Scalar)0.3, (Scalar)0.9, (Scalar)0.5 };
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    Test(near)(thread, a->components[i], inverse[i]);
  }
  C(invert)(thread, a, a);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    Test(near)(thread, a->components[i], right[i]);
  }

  const Scalar start[4] = { (Scalar)0, (Scalar)0.2, (Scalar)0.4, (Scalar)0.6 };
  const Scalar end[4] = { (Scalar)1, (Scalar)0.8, (Scalar)0.6, (Scalar)0.4 };
  C(assign)(thread, a, b);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    a->components[i] = start[i];
    b->components[i] = end[i];
  }
  C(lerp)(thread, a, b, (Scalar)-1);
  Test(near)(thread, a->components[0], (Scalar)0);
  C(lerp)(thread, a, b, (Scalar)0.25);
  const Scalar interpolated[4] = { (Scalar)0.25, (Scalar)0.35, (Scalar)0.45, (Scalar)0.55 };
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    Test(near)(thread, a->components[i], interpolated[i]);
  }
  C(lerp)(thread, a, a, (Scalar)0.5);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    Test(near)(thread, a->components[i], interpolated[i]);
  }
  C(lerp)(thread, a, b, (Scalar)2);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    Test(near)(thread, a->components[i], end[i]);
  }
  // Interior results saturate in the channel domain, independent of t.
  a->components[0] = (Scalar)-2;
  b->components[0] = (Scalar)2;
  C(lerp)(thread, a, b, (Scalar)0.5);
  Test(near)(thread, a->components[0], (Scalar)0);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
Test(nativeValuesAndClone)
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  const Scalar values[4] = { (Scalar)0.125, (Scalar)0.25, (Scalar)0.5, (Scalar)1 };
  Color* original = C(create4v)(thread, values);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)original);
  Scalar copied[4] = { (Scalar)-1, (Scalar)-1, (Scalar)-1, (Scalar)-1 };
  C(copyValues)(thread, original, copied);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    Arcadia_Tests_assertTrue(thread, copied[i] == values[i]);
  }
  copied[0] = (Scalar)-123;
  Arcadia_Tests_assertTrue(thread, original->components[0] == values[0]);
  Color* clone = (Color*)Arcadia_Object_clone(thread, (Arcadia_Object*)original);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)clone);
  Arcadia_Tests_assertTrue(thread, clone != original);
  Arcadia_Value value = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)clone);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)original, &value));
  clone->components[2] = (Scalar)0.75;
  Arcadia_Tests_assertTrue(thread, original->components[2] == values[2]);
  Arcadia_Tests_assertFalse(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)original, &value));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}
