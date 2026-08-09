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
#include "Arcadia/Ring1/Implementation/NaturalToString/Include.h"

#include "Arcadia/Ring1/Include.h"

void
Arcadia_Natural16Value_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value natural16Value,
    void* context,
    void (*function)(Arcadia_Thread* thread, void* context, const Arcadia_Natural8Value* bytes, Arcadia_SizeValue numberOfBytes)
  )
{ Arcadia_Natural64Value_toUTF8String(thread, natural16Value, context, function); }

void
Arcadia_Natural32Value_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value natural32Value,
    void* context,
    void (*function)(Arcadia_Thread* thread, void* context, const Arcadia_Natural8Value* bytes, Arcadia_SizeValue numberOfBytes)
  )
{ Arcadia_Natural64Value_toUTF8String(thread, natural32Value, context, function); }

void
Arcadia_Natural64Value_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value natural64Value,
    void* context,
    void (*function)(Arcadia_Thread* thread, void* context, const Arcadia_Natural8Value* bytes, Arcadia_SizeValue numberOfBytes)
  )
{
  // The maximum value of a uint64_t is 2^64-1 = 18,446,744,073,709,551,615.
  // These are 20 decimal digit symbol.
  // Plus one more symbol for a sign, these are 21 symbols.
  // We allocate 32 symbols in paranoia mode.
  // Note we do not use a static buffer to be thread-safe.
  Arcadia_Natural8Value bytes[32];

  Arcadia_Natural64Value valueTemporary;
  Arcadia_SizeValue numberOfDigits = 0;
  Arcadia_SizeValue numberOfBytes = 0;

  valueTemporary = natural64Value;
  if (!valueTemporary) {
    numberOfBytes++;
    numberOfDigits++;
  } else {
    while (valueTemporary > 0) {
      valueTemporary /= 10;
      numberOfBytes++;
      numberOfDigits++;
    }
  }

  valueTemporary = natural64Value;
  char* p = &(bytes[0]);
  if (!valueTemporary) {
    *p = (char)'0';
    ++p;
  } else {
    char* q = p + numberOfDigits;
    while (valueTemporary > 0) {
      Arcadia_Natural64Value digit = valueTemporary % 10;
      *(--q) = (char)digit + '0';
      valueTemporary /= 10;
    }
    p += numberOfDigits;
  }

  (*function)(thread, context, bytes, numberOfBytes);
}

void
Arcadia_Natural8Value_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value natural8Value,
    void* context,
    void (*function)(Arcadia_Thread* thread, void* context, const Arcadia_Natural8Value* bytes, Arcadia_SizeValue numberOfBytes)
  )
{ Arcadia_Natural64Value_toUTF8String(thread, natural8Value, context, function); }
