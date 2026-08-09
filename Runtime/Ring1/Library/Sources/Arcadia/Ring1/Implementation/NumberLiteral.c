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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Implementation/NumberLiteral.h"

static inline Arcadia_NumberLiteral_Sign
Arcadia_NumberLiteral_Sign_empty
  (
    Arcadia_SizeValue start
  )
{
  Arcadia_NumberLiteral_Sign instance = { .start = start, .length = 0 };
  return instance;
}

static inline Arcadia_NumberLiteral_Fractional
Arcadia_NumberLiteral_Fractional_empty
  (
    Arcadia_SizeValue start
  )
{
  Arcadia_NumberLiteral_Fractional instance = { .start = start, .length = 0, .trailingZeroes.start = start, .trailingZeroes.length = 0 };
  return instance;
}

static inline Arcadia_NumberLiteral_Integral
Arcadia_NumberLiteral_Integral_empty
  (
    Arcadia_SizeValue start
  )
{
  Arcadia_NumberLiteral_Integral instance = { .start = start, .length = 0, .leadingZeroes.start = start, .leadingZeroes.length = 0 };
  return instance;
}

static inline Arcadia_NumberLiteral_Exponent
Arcadia_NumberLiteral_Exponent_empty
  (
    Arcadia_SizeValue start
  )
{
  Arcadia_NumberLiteral_Exponent instance = {
    .start = start,
    .length = 0,
    .integral = Arcadia_NumberLiteral_Integral_empty(start),
    .powerSymbol.start = start,
    .powerSymbol.length = 0,
    .sign = Arcadia_NumberLiteral_Sign_empty(start)
  };
  return instance;
}

static inline Arcadia_NumberLiteral
Arcadia_NumberLiteral_empty
  (
    Arcadia_SizeValue start
  )
{
  Arcadia_NumberLiteral instance = {
    .significand.sign = Arcadia_NumberLiteral_Sign_empty(start),
    .significand.integral = Arcadia_NumberLiteral_Integral_empty(start),
    .significand.fractional = Arcadia_NumberLiteral_Fractional_empty(start),
    .significand.radixPoint.start = start,
    .significand.radixPoint.length = 0,
    .exponent = Arcadia_NumberLiteral_Exponent_empty(start),
  };
  return instance;
}

Arcadia_BooleanValue
Arcadia_parseNumberLiteral
  (
    Arcadia_Thread* thread,
    Arcadia_NumberLiteral* result,
    const Arcadia_Natural8Value* source,
    Arcadia_SizeValue sourceLength
  )
{
  Arcadia_SizeValue i = 0;
  Arcadia_SizeValue n = sourceLength;

  Arcadia_NumberLiteral number = Arcadia_NumberLiteral_empty(0);

  // Parse "significand.sign".
  if (i == n) {
    return Arcadia_BooleanValue_False;
  }
  number.significand.sign.start = i;
  switch (source[i]) {
    case '+':
    case '-': {
      number.significand.sign.start = i;
      number.significand.sign.length = 1;
      i++;
    } break;
  };

  // Parse "significand.integral".
  number.significand.integral.start = i;
  number.significand.integral.length = 0;
  number.significand.integral.leadingZeroes.start = i;
  number.significand.integral.leadingZeroes.length = 0;
  // Count leading zeroes of "significand.integral".
  while (i < n && source[i] == '0') {
    number.significand.integral.leadingZeroes.length++;
    number.significand.integral.length++;
    i++;
  }
  while (i < n && '0' <= source[i] && source[i] <= '9') {
    number.significand.integral.length++;
    i++;
  }
  // Parse "significand.radixPoint".
  if (i == n) {
    number.exponent = Arcadia_NumberLiteral_Exponent_empty(i);
    number.significand.radixPoint.start = i;
    number.significand.radixPoint.length = 0;
    number.significand.fractional = Arcadia_NumberLiteral_Fractional_empty(i);
    *result = number;
    if (number.significand.integral.length == 0) {
      return Arcadia_BooleanValue_False;
    } else {
      return Arcadia_BooleanValue_True;
    }
  }
  if (source[i] == '.') {
    number.significand.radixPoint.start = i;
    number.significand.radixPoint.length = 1;
    i++;
    if (i == n) {
      number.exponent = Arcadia_NumberLiteral_Exponent_empty(i);
      number.significand.fractional = Arcadia_NumberLiteral_Fractional_empty(i);
      *result = number;
      if (number.significand.integral.length == 0) {
        return Arcadia_BooleanValue_True;
      } else {
        return Arcadia_BooleanValue_True;
      }
    }
  } else {
    number.significand.radixPoint.start = i;
    number.significand.radixPoint.length = 0;
  }
  // Parse "significand.fractional".
  number.significand.fractional.start = i;
  while (i < n && '0' <= source[i] && source[i] <= '9') {
    number.significand.fractional.length++;
    i++;
  }
  // Count trailing zeroes of "significand.fractional".
  number.significand.fractional.trailingZeroes.start = number.significand.fractional.start + number.significand.fractional.length;
  number.significand.fractional.trailingZeroes.length = 0;
  for (Arcadia_SizeValue i = number.significand.fractional.start + number.significand.fractional.length; i > number.significand.fractional.start; --i) {
    if (source[i - 1] != '0') {
      break;
    }
    number.significand.fractional.trailingZeroes.start--;
    number.significand.fractional.trailingZeroes.length++;

  }
  if (i == n) {
    number.exponent = Arcadia_NumberLiteral_Exponent_empty(i);
    *result = number;
    if (number.significand.integral.length == 0 && number.significand.fractional.length == 0) {
      return Arcadia_BooleanValue_False;
    } else {
      return Arcadia_BooleanValue_True;
    }
  }
  // Parse "exponent".
  number.exponent.start = i;
  switch (source[i]) {
    case 'e':
    case 'E':
    case '^': {
      number.exponent.powerSymbol.start = i;
      number.exponent.powerSymbol.length = 1;
      number.exponent.length++;
      i++;
    } break;
    default: {
      number.exponent.length = 1;
      number.exponent.sign = Arcadia_NumberLiteral_Sign_empty(i);
      number.exponent.integral = Arcadia_NumberLiteral_Integral_empty(i);
      *result = number;
      return Arcadia_BooleanValue_False;
    } break;
  }
  if (i == n) {
    number.exponent.length = 1;
    number.exponent.sign = Arcadia_NumberLiteral_Sign_empty(i);
    number.exponent.integral = Arcadia_NumberLiteral_Integral_empty(i);
    *result = number;
    return Arcadia_BooleanValue_False;
  }
  switch (source[i]) {
    case '+':
    case '-': {
      number.exponent.sign.start = i;
      number.exponent.sign.length = 1;
      number.exponent.length++;
      i++;
    } break;
    default: {
      number.exponent.sign.start = i;
      number.exponent.sign.length = 0;
    } break;
  }
  number.exponent.integral.start = i;
  number.exponent.integral.leadingZeroes.start = i;
  // Count leading zeroes of "exponent.integral".
  while (i < n && source[i] == '0') {
    number.exponent.integral.leadingZeroes.length++;
    number.exponent.integral.length++;
    number.exponent.length++;
    i++;
  }
  while (i < n && '0' <= source[i] && source[i] <= '9') {
    number.exponent.integral.length++;
    number.exponent.length++;
    i++;
  }
  *result = number;
  if (i != n || number.exponent.integral.length == 0) {
    return Arcadia_BooleanValue_False;
  }
  return Arcadia_BooleanValue_True;
}
