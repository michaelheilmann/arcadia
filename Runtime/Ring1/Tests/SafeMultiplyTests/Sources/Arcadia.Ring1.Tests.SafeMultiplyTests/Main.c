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

#include <stdlib.h>

#include "Arcadia/Ring1/Include.h"

// Test a*b and b*a using all available functions.
static void
testFixtureNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value multiplier,
    Arcadia_Natural16Value multiplicand,
    Arcadia_Natural16Value expectedUpperProduct,
    Arcadia_Natural16Value expectedLowerProduct
  )
{
  Arcadia_Natural16Value receivedUpperProduct, receivedLowerProduct;

  Arcadia_safeMultiplyNatural16Value(thread, multiplier, multiplicand, &receivedUpperProduct, &receivedLowerProduct);
  Arcadia_Tests_assertTrue (thread, receivedUpperProduct == expectedUpperProduct);
  Arcadia_Tests_assertTrue (thread, receivedLowerProduct == expectedLowerProduct);

  Arcadia_safeMultiplyNatural16Value(thread, multiplicand, multiplier, &receivedUpperProduct, &receivedLowerProduct);
  Arcadia_Tests_assertTrue (thread, receivedUpperProduct == expectedUpperProduct);
  Arcadia_Tests_assertTrue (thread, receivedLowerProduct == expectedLowerProduct);
}

// Test a*b and b*a using all available functions.
static void
testFixtureNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value multiplier,
    Arcadia_Natural32Value multiplicand,
    Arcadia_Natural32Value expectedUpperProduct,
    Arcadia_Natural32Value expectedLowerProduct
  )
{
  Arcadia_Natural32Value receivedUpperProduct, receivedLowerProduct;

  Arcadia_safeMultiplyNatural32Value(thread, multiplier, multiplicand, &receivedUpperProduct, &receivedLowerProduct);
  Arcadia_Tests_assertTrue (thread, receivedUpperProduct == expectedUpperProduct);
  Arcadia_Tests_assertTrue (thread, receivedLowerProduct == expectedLowerProduct);

  Arcadia_safeMultiplyNatural32Value(thread, multiplicand, multiplier, &receivedUpperProduct, &receivedLowerProduct);
  Arcadia_Tests_assertTrue (thread, receivedUpperProduct == expectedUpperProduct);
  Arcadia_Tests_assertTrue (thread, receivedLowerProduct == expectedLowerProduct);
}

// Test a*b and b*a using all available functions.
static void
testFixtureNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value multiplier,
    Arcadia_Natural64Value multiplicand,
    Arcadia_Natural64Value expectedUpperProduct,
    Arcadia_Natural64Value expectedLowerProduct
  )
{
  Arcadia_Natural64Value receivedUpperProduct, receivedLowerProduct;

  Arcadia_safeMultiplyNatural64Value(thread, multiplier, multiplicand, &receivedUpperProduct, &receivedLowerProduct);
  Arcadia_Tests_assertTrue (thread, receivedUpperProduct == expectedUpperProduct);
  Arcadia_Tests_assertTrue (thread, receivedLowerProduct == expectedLowerProduct);

  Arcadia_safeMultiplyNatural64Value(thread, multiplicand, multiplier, &receivedUpperProduct, &receivedLowerProduct);
  Arcadia_Tests_assertTrue (thread, receivedUpperProduct == expectedUpperProduct);
  Arcadia_Tests_assertTrue (thread, receivedLowerProduct == expectedLowerProduct);

  Arcadia_safeMultiplyNatural64Value(thread, multiplier, multiplicand, &receivedUpperProduct, &receivedLowerProduct);
  Arcadia_Tests_assertTrue (thread, receivedUpperProduct == expectedUpperProduct);
  Arcadia_Tests_assertTrue (thread, receivedLowerProduct == expectedLowerProduct);

  Arcadia_safeMultiplyNatural64Value(thread, multiplicand, multiplier, &receivedUpperProduct, &receivedLowerProduct);
  Arcadia_Tests_assertTrue (thread, receivedUpperProduct == expectedUpperProduct);
  Arcadia_Tests_assertTrue (thread, receivedLowerProduct == expectedLowerProduct);
}

// Test a*b and b*a using all available functions.
static void
testFixtureNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value multiplier,
    Arcadia_Natural8Value multiplicand,
    Arcadia_Natural8Value expectedUpperProduct,
    Arcadia_Natural8Value expectedLowerProduct
  )
{
  Arcadia_Natural8Value receivedUpperProduct, receivedLowerProduct;

  Arcadia_safeMultiplyNatural8Value(thread, multiplier, multiplicand, &receivedUpperProduct, &receivedLowerProduct);
  Arcadia_Tests_assertTrue (thread, receivedUpperProduct == expectedUpperProduct);
  Arcadia_Tests_assertTrue (thread, receivedLowerProduct == expectedLowerProduct);

  Arcadia_safeMultiplyNatural8Value(thread, multiplicand, multiplier, &receivedUpperProduct, &receivedLowerProduct);
  Arcadia_Tests_assertTrue (thread, receivedUpperProduct == expectedUpperProduct);
  Arcadia_Tests_assertTrue (thread, receivedLowerProduct == expectedLowerProduct);
}

#define Define(Type, A, B, H, L) \
{ \
  testFixture##Type(thread, A,B, H, L); \
}

static void
safeMultiplyNatural8Tests
  (
    Arcadia_Thread* thread
  )
{
  // 0 * 0 = (0, 0)
  Define(Natural8, 0, 0, 0, 0);
  // MAX * 0 = 0 * MAX = (0, 0)
  Define(Natural8, Arcadia_Natural8Value_Maximum, 0, 0, 0);
  // MAX * 1 = 1 * MAX = (MAX, 0)
  Define(Natural8, Arcadia_Natural8Value_Maximum, 1, 0, Arcadia_Natural8Value_Maximum);
  static const Arcadia_SizeValue bits = 8;
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Arcadia_Natural8Value lower, upper;
    Arcadia_Natural8Value u, v;
    // The lower Arcadia_Natural8 value.
    // All bits are one except for the LSB.
    lower = Arcadia_Natural8Value_Maximum;
    Arcadia_Natural8Value mask = Arcadia_makeBitmaskN8(thread, bits - 1, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Arcadia_Natural8 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // MAX * 2 = 2 * MAX = (lower, upper)
    Define(Natural8, Arcadia_Natural8Value_Maximum, 2, upper, lower);
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Arcadia_Natural8Value upper, lower;
    Arcadia_Natural8Value u, v;
    // The lower Arcadia_Natural8 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Arcadia_Natural8Value_Maximum;
    Arcadia_Natural8Value mask = Arcadia_makeBitmaskN8(thread, bits - 2, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Arcadia_Natural16 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // MAX * 4 = 4 * MAX = (lower, upper)
    Define(Natural8, Arcadia_Natural8Value_Maximum, 4, upper, lower);
  }
}

static void
safeMultiplyNatural16Tests
  (
    Arcadia_Thread* thread
  )
{
  // 0 * 0 = (0, 0)
  Define(Natural16, 0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  Define(Natural16, Arcadia_Natural16Value_Maximum, 0, 0, 0);
  // 0 * MAX = (0, 0)
  Define(Natural16, 0, Arcadia_Natural16Value_Maximum, 0, 0);
  // MAX * 1 = (MAX, 0)
  Define(Natural16, Arcadia_Natural16Value_Maximum, 1, 0, Arcadia_Natural16Value_Maximum);
  // 1 * MAX = (MAX, 0)
  Define(Natural16, 1, Arcadia_Natural16Value_Maximum, 0, Arcadia_Natural16Value_Maximum);
  static const Arcadia_SizeValue bits = 16;
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Arcadia_Natural16Value lower, upper;
    Arcadia_Natural16Value u, v;
    // The lower Arcadia_Natural8 value.
    // All bits are one except for the LSB.
    lower = Arcadia_Natural16Value_Maximum;
    Arcadia_Natural16Value mask = Arcadia_makeBitmaskN16(thread, bits - 1, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Arcadia_Natural8 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // MAX * 2 = 2 * MAX = (lower, upper)
    Define(Natural16, Arcadia_Natural16Value_Maximum, 2, upper, lower);
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Arcadia_Natural16Value upper, lower;
    Arcadia_Natural16Value u, v;
    // The lower Arcadia_Natural16 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Arcadia_Natural16Value_Maximum;
    Arcadia_Natural16Value mask = Arcadia_makeBitmaskN16(thread, bits - 2, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Arcadia_Natural16 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // MAX * 4 = 4 * MAX = (lower, upper)
    Define(Natural16, Arcadia_Natural16Value_Maximum, 4, upper, lower);
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
}

static void
safeMultiplyNatural32Tests
  (
    Arcadia_Thread* thread
  )
{
  // 0 * 0 = (0, 0)
  Define(Natural32, 0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  Define(Natural32, Arcadia_Natural32Value_Maximum, 0, 0, 0);
  // 0 * MAX = (0, 0)
  Define(Natural32, 0, Arcadia_Natural32Value_Maximum, 0, 0);
  // MAX * 1 = (MAX, 0)
  Define(Natural32, Arcadia_Natural32Value_Maximum, 1, 0, Arcadia_Natural32Value_Maximum);
  // 1 * MAX = (MAX, 0)
  Define(Natural32, 1, Arcadia_Natural32Value_Maximum, 0, Arcadia_Natural32Value_Maximum);
  static const Arcadia_SizeValue bits = 32;
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Arcadia_Natural32Value lower, upper;
    Arcadia_Natural32Value u, v;
    // The lower Arcadia_Natural32 value.
    // All bits are one except for the LSB.
    lower = Arcadia_Natural32Value_Maximum;
    Arcadia_Natural32Value mask = Arcadia_makeBitmaskN32(thread, bits - 1, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Arcadia_Natural32 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // MAX * 2 = 2 * MAX = (lower, upper)
    Define(Natural32, Arcadia_Natural32Value_Maximum, 2, upper, lower);
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Arcadia_Natural32Value upper, lower;
    Arcadia_Natural32Value u, v;
    // The lower Arcadia_Natural32 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Arcadia_Natural32Value_Maximum;
    Arcadia_Natural32Value mask = Arcadia_makeBitmaskN32(thread, bits - 2, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Arcadia_Natural32 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // MAX * 4 = 4 * MAX = (lower, upper)
    Define(Natural32, Arcadia_Natural32Value_Maximum, 4, upper, lower);
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
}

static void
safeMultiplyNatural64Tests
  (
    Arcadia_Thread* thread
  )
{
  // 0 * 0 = (0, 0)
  Define(Natural64, 0, 0, 0, 0);
  // MAX * 0 = (0, 0)
  Define(Natural64, Arcadia_Natural64Value_Maximum, 0, 0, 0);
  // 0 * MAX = (0, 0)
  Define(Natural64, 0, Arcadia_Natural64Value_Maximum, 0, 0);
  // MAX * 1 = (MAX, 0)
  Define(Natural64, Arcadia_Natural64Value_Maximum, 1, 0, Arcadia_Natural64Value_Maximum);
  // 1 * MAX = (MAX, 0)
  Define(Natural64, 1, Arcadia_Natural64Value_Maximum, 0, Arcadia_Natural64Value_Maximum);
  static const Arcadia_SizeValue bits = 64;
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Arcadia_Natural64Value lower, upper;
    Arcadia_Natural64Value u, v;
    // The lower Arcadia_Natural64 value.
    // All bits are one except for the LSB.
    lower = Arcadia_Natural64Value_Maximum;
    Arcadia_Natural64Value mask = Arcadia_makeBitmaskN64(thread, bits - 1, 1);
    mask = ~mask;
    lower &= mask;
    // The upper Arcadia_Natural64 value.
    // All bits are zero except for the LSB.
    upper = 1;
    // MAX * 2 = 2 * MAX = (lower, upper)
    Define(Natural64, Arcadia_Natural64Value_Maximum, 2, upper, lower);
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  {
    Arcadia_Natural64Value upper, lower;
    Arcadia_Natural64Value u, v;
    // The lower Arcadia_Natural64 value.
    // All bits are one except for LSB + 0 and LSB + 1.
    lower = Arcadia_Natural64Value_Maximum;
    Arcadia_Natural64Value mask = Arcadia_makeBitmaskN64(thread, bits - 2, 2);
    mask = ~mask;
    lower &= mask;
    // The upper Arcadia_Natural64 value.
    // All bits are zero except for LSB + 0 and LSB + 1.
    upper = 3;
    // MAX * 4 = 4 * MAX = (lower, upper)
    Define(Natural64, Arcadia_Natural64Value_Maximum, 4, upper, lower);
  }
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
}

#undef Define

#define Define(type, multiplier, multiplicand, expectedOverflow, expectedProductHigh, expectedProductLow) \
  { \
    Arcadia_BooleanValue receivedOverflow; \
    Arcadia_##type##Value receivedProductHigh, receivedProductLow; \
\
    receivedOverflow = Arcadia_safeMultiply##type##Value(thread, multiplier, multiplicand, &receivedProductHigh, &receivedProductLow); \
    Arcadia_Tests_assertTrue(thread, receivedOverflow == expectedOverflow); \
    Arcadia_Tests_assertTrue(thread, receivedProductHigh == expectedProductHigh); \
    Arcadia_Tests_assertTrue(thread, receivedProductLow == expectedProductLow); \
\
    receivedOverflow = Arcadia_safeMultiply##type##Value(thread, multiplicand, multiplier, &receivedProductHigh, &receivedProductLow); \
    Arcadia_Tests_assertTrue(thread, receivedOverflow == expectedOverflow); \
    Arcadia_Tests_assertTrue(thread, receivedProductHigh == expectedProductHigh); \
    Arcadia_Tests_assertTrue(thread, receivedProductLow == expectedProductLow); \
  }

static void
safeMultiplyInteger16Tests
  (
    Arcadia_Thread* thread
  )
{
  Define(Integer16, Arcadia_Integer16Value_Literal(0),    Arcadia_Integer16Value_Literal(0),  false, Arcadia_Integer16Value_Literal(0), Arcadia_Integer16Value_Literal(0));
  Define(Integer16, Arcadia_Integer16Value_Maximum,       Arcadia_Integer16Value_Literal(0),  false, Arcadia_Integer16Value_Literal(0), Arcadia_Integer16Value_Literal(0));
  Define(Integer16, Arcadia_Integer16Value_Minimum,       Arcadia_Integer16Value_Literal(0),  false, Arcadia_Integer16Value_Literal(0), Arcadia_Integer16Value_Literal(0));

  Define(Integer16, Arcadia_Integer16Value_Literal(+1),   Arcadia_Integer16Value_Literal(+1), false, Arcadia_Integer16Value_Literal(0),  Arcadia_Integer16Value_Literal(+1));
  Define(Integer16, Arcadia_Integer16Value_Literal(+1),   Arcadia_Integer16Value_Literal(-1), false, Arcadia_Integer16Value_Literal(-1), Arcadia_Integer16Value_Literal(-1));
  Define(Integer16, Arcadia_Integer16Value_Literal(-1),   Arcadia_Integer16Value_Literal(+1), false, Arcadia_Integer16Value_Literal(-1), Arcadia_Integer16Value_Literal(-1));
  Define(Integer16, Arcadia_Integer16Value_Literal(-1),   Arcadia_Integer16Value_Literal(-1), false, Arcadia_Integer16Value_Literal(0),  Arcadia_Integer16Value_Literal(+1));
}

static void
safeMultiplyInteger32Tests
  (
    Arcadia_Thread* thread
  )
{
  Define(Integer32, Arcadia_Integer32Value_Literal(0),  Arcadia_Integer32Value_Literal(0),     false, Arcadia_Integer32Value_Literal(0),  Arcadia_Integer32Value_Literal(0));
  Define(Integer32, Arcadia_Integer32Value_Maximum,     Arcadia_Integer32Value_Literal(0),     false, Arcadia_Integer32Value_Literal(0),  Arcadia_Integer32Value_Literal(0));
  Define(Integer32, Arcadia_Integer32Value_Minimum,     Arcadia_Integer32Value_Literal(0),     false, Arcadia_Integer32Value_Literal(0),  Arcadia_Integer32Value_Literal(0));

  Define(Integer32, Arcadia_Integer32Value_Literal(+1), Arcadia_Integer32Value_Literal(+1),    false, Arcadia_Integer32Value_Literal(0),  Arcadia_Integer32Value_Literal(+1));
  Define(Integer32, Arcadia_Integer32Value_Literal(+1), Arcadia_Integer32Value_Literal(-1),    false, Arcadia_Integer32Value_Literal(-1), Arcadia_Integer32Value_Literal(-1));
  Define(Integer32, Arcadia_Integer32Value_Literal(-1), Arcadia_Integer32Value_Literal(+1),    false, Arcadia_Integer32Value_Literal(-1), Arcadia_Integer32Value_Literal(-1));
  Define(Integer32, Arcadia_Integer32Value_Literal(-1), Arcadia_Integer32Value_Literal(-1),    false, Arcadia_Integer32Value_Literal(0),  Arcadia_Integer32Value_Literal(+1));
}

static void
safeMultiplyInteger64Tests
  (
    Arcadia_Thread* thread
  )
{
  {
    // Regression.
    Arcadia_Integer64Value multiplier = -1, multiplicand = +1;
    Arcadia_Integer64Value upperProduct, lowerProduct;
    Arcadia_BooleanValue overflow = Arcadia_safeMultiplyInteger64Value(thread, multiplier, multiplicand, &upperProduct, &lowerProduct);
    Arcadia_Tests_assertTrue(thread, overflow == Arcadia_BooleanValue_False);
    Arcadia_Tests_assertTrue(thread, upperProduct == -1);
    Arcadia_Tests_assertTrue(thread, lowerProduct == -1);
  }
  Define(Integer64, Arcadia_Integer64Value_Literal(0),  Arcadia_Integer64Value_Literal(0),  false, Arcadia_Integer64Value_Literal(0),  Arcadia_Integer64Value_Literal(0));
  Define(Integer64, Arcadia_Integer64Value_Maximum,     Arcadia_Integer64Value_Literal(0),  false, Arcadia_Integer64Value_Literal(0),  Arcadia_Integer64Value_Literal(0));
  Define(Integer64, Arcadia_Integer64Value_Minimum,     Arcadia_Integer64Value_Literal(0),  false, Arcadia_Integer64Value_Literal(0),  Arcadia_Integer64Value_Literal(0));

  Define(Integer64, Arcadia_Integer64Value_Literal(+1), Arcadia_Integer64Value_Literal(+1), false, Arcadia_Integer64Value_Literal(0),  Arcadia_Integer64Value_Literal(+1));
  Define(Integer64, Arcadia_Integer64Value_Literal(+1), Arcadia_Integer64Value_Literal(-1), false, Arcadia_Integer64Value_Literal(-1), Arcadia_Integer64Value_Literal(-1));
  Define(Integer64, Arcadia_Integer64Value_Literal(-1), Arcadia_Integer64Value_Literal(+1), false, Arcadia_Integer64Value_Literal(-1), Arcadia_Integer64Value_Literal(-1));
  Define(Integer64, Arcadia_Integer64Value_Literal(-1), Arcadia_Integer64Value_Literal(-1), false, Arcadia_Integer64Value_Literal(0),  Arcadia_Integer64Value_Literal(+1));
}

static void
safeMultiplyInteger8Tests
  (
    Arcadia_Thread* thread
  )
{
  Define(Integer8, Arcadia_Integer8Value_Literal(0), Arcadia_Integer8Value_Literal(0), false, Arcadia_Integer8Value_Literal(0), Arcadia_Integer8Value_Literal(0));
  Define(Integer8, Arcadia_Integer8Value_Maximum,    Arcadia_Integer8Value_Literal(0), false, Arcadia_Integer8Value_Literal(0), Arcadia_Integer8Value_Literal(0));
  Define(Integer8, Arcadia_Integer8Value_Minimum,    Arcadia_Integer8Value_Literal(0), false, Arcadia_Integer8Value_Literal(0), Arcadia_Integer8Value_Literal(0));

  Define(Integer8, Arcadia_Integer8Value_Literal(+1), Arcadia_Integer8Value_Literal(+1), false, Arcadia_Integer8Value_Literal(0), Arcadia_Integer8Value_Literal(+1));
  Define(Integer8, Arcadia_Integer8Value_Literal(+1), Arcadia_Integer8Value_Literal(-1), false, Arcadia_Integer8Value_Literal(-1), Arcadia_Integer8Value_Literal(-1));
  Define(Integer8, Arcadia_Integer8Value_Literal(-1), Arcadia_Integer8Value_Literal(+1), false, Arcadia_Integer8Value_Literal(-1), Arcadia_Integer8Value_Literal(-1));
  Define(Integer8, Arcadia_Integer8Value_Literal(-1), Arcadia_Integer8Value_Literal(-1), false, Arcadia_Integer8Value_Literal(0), Arcadia_Integer8Value_Literal(+1));
}

#undef Define

int
main
  (
    int argc,
    char **argv
  )
{

  if (!Arcadia_Tests_safeExecute(&safeMultiplyNatural16Tests)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&safeMultiplyNatural32Tests)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&safeMultiplyNatural64Tests)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&safeMultiplyNatural8Tests)) {
    return EXIT_FAILURE;
  }

  if (!Arcadia_Tests_safeExecute(&safeMultiplyInteger16Tests)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&safeMultiplyInteger32Tests)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&safeMultiplyInteger64Tests)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&safeMultiplyInteger8Tests)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

}
