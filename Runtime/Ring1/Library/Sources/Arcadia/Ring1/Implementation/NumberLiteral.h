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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_NUMBERLITERAL_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_NUMBERLITERAL_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Boolean.h"
#include "Arcadia/Ring1/Implementation/Natural8.h"
#include "Arcadia/Ring1/Implementation/Size.h"

typedef struct Arcadia_NumberLiteral_Sign {
  Arcadia_SizeValue start;
  Arcadia_SizeValue length;
} Arcadia_NumberLiteral_Sign;

typedef struct Arcadia_NumberLiteral_Fractional {
  Arcadia_SizeValue start;
  Arcadia_SizeValue length;
  struct {
    Arcadia_SizeValue start;
    Arcadia_SizeValue length;
  } trailingZeroes;
} Arcadia_NumberLiteral_Fractional;

typedef struct Arcadia_NumberLiteral_Integral {
  Arcadia_SizeValue start;
  Arcadia_SizeValue length;
  struct {
    Arcadia_SizeValue start;
    Arcadia_SizeValue length;
  } leadingZeroes;
} Arcadia_NumberLiteral_Integral;

typedef struct Arcadia_NumberLiteral_Exponent {
  Arcadia_SizeValue start;
  Arcadia_SizeValue length;
  Arcadia_NumberLiteral_Sign sign;
  struct {
    Arcadia_SizeValue start;
    Arcadia_SizeValue length;
  } powerSymbol;
  Arcadia_NumberLiteral_Integral integral;
} Arcadia_NumberLiteral_Exponent;

typedef struct Arcadia_NumberLiteral {
  struct {
    Arcadia_NumberLiteral_Sign sign;
    Arcadia_NumberLiteral_Integral integral;
    Arcadia_NumberLiteral_Fractional fractional;
    struct {
      Arcadia_SizeValue start;
      Arcadia_SizeValue length;
    } radixPoint;
  } significand;
  Arcadia_NumberLiteral_Exponent exponent;
} Arcadia_NumberLiteral;


Arcadia_BooleanValue
Arcadia_parseNumberLiteral
  (
    Arcadia_Thread* thread,
    Arcadia_NumberLiteral* result,
    const Arcadia_Natural8Value* bytes,
    Arcadia_SizeValue numberOfBytes
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_NUMBERLITERAL_H_INCLUDED
