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

#include "Arcadia/Ring1/Include.h"
#include <string.h>

static void
literalTests
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_NumberLiteral number;
  Arcadia_Tests_assertTrue(thread, Arcadia_parseNumberLiteral(thread, &number, u8"-0.0e10", strlen(u8"-0.0e10")));
  Arcadia_Tests_assertTrue(thread, number.significand.sign.start == 0 && number.significand.sign.length == 1);
  Arcadia_Tests_assertTrue(thread, number.significand.integral.start == 1 && number.significand.integral.length == 1);
  Arcadia_Tests_assertTrue(thread, number.significand.integral.leadingZeroes.length == 1);
  Arcadia_Tests_assertTrue(thread, number.significand.radixPoint.start == 2 && number.significand.radixPoint.length == 1);
  Arcadia_Tests_assertTrue(thread, number.significand.fractional.start == 3 && number.significand.fractional.length == 1);
  Arcadia_Tests_assertTrue(thread, number.significand.fractional.trailingZeroes.start == 3 && number.significand.fractional.trailingZeroes.length == 1);
  Arcadia_Tests_assertTrue(thread, number.exponent.start == 4 && number.exponent.length == 3);
  Arcadia_Tests_assertTrue(thread, number.exponent.powerSymbol.start == 4 && number.exponent.powerSymbol.length == 1);
  Arcadia_Tests_assertTrue(thread, number.exponent.sign.length == 0);
  Arcadia_Tests_assertTrue(thread, number.exponent.integral.start == 5 && number.exponent.integral.length == 2);
  Arcadia_Tests_assertFalse(thread, Arcadia_parseNumberLiteral(thread, &number, u8"1e", strlen(u8"1e")));
  Arcadia_Tests_assertFalse(thread, Arcadia_parseNumberLiteral(thread, &number, u8"1x", strlen(u8"1x")));
}

static int
runTests
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(&literalTests)) {
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
    { "LiteralTests", &runTests },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
