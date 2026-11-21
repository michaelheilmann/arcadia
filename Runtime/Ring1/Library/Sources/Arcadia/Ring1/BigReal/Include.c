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
#include "Arcadia/Ring1/BigReal/Include.h"

#include "Arcadia/Ring1/Include.h"

#define TypeName u8"Arcadia.BigReal"

static Arcadia_BooleanValue g_registered = Arcadia_BooleanValue_False;

static void
onTypeRemoved
  (
    Arcadia_Process* process,
    const uint8_t* bytes,
    size_t numberOfBytes
  )
{
  g_registered = Arcadia_BooleanValue_False;
}

static void
onVisit
  (
    Arcadia_Process* process,
    Arcadia_BigReal* self
  )
{
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  if (self->significand) {
    Arcadia_BigInteger_visit(thread, self->significand);
  }
  if (self->exponent) {
    Arcadia_BigInteger_visit(thread, self->exponent);
  }
}

Arcadia_BigReal*
Arcadia_BigReal_create
  (
    Arcadia_Thread* thread
  )
{
  if (!g_registered) {
    Arcadia_Process_registerType(Arcadia_Thread_getProcess(thread),
                                 TypeName, sizeof(TypeName) - 1,
                                 Arcadia_Thread_getProcess(thread),
                                 (Arcadia_Process_TypeRemovedCallback*)&onTypeRemoved,
                                 (Arcadia_Process_VisitCallback*)&onVisit,
                                 NULL);
    g_registered = Arcadia_BooleanValue_True;
  }
  Arcadia_BigReal* self = NULL;
  Arcadia_Process_allocate(Arcadia_Thread_getProcess(thread), (void**)&self,
                           TypeName, sizeof(TypeName) - 1, sizeof(Arcadia_BigReal));
  self->significand = NULL;
  self->exponent = NULL;
  self->significand = Arcadia_BigInteger_create(thread);
  self->exponent = Arcadia_BigInteger_create(thread);
  return self;
}

static void
invalidText
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

// Normalize positive magnitude, then apply its sign.
// Use only bounded small-divisor operations; the exponent itself is unbounded.
static void
normalize
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* significand,
    Arcadia_BigInteger* exponent
  )
{
  if (Arcadia_BigInteger_isZero(thread, significand)) {
    Arcadia_BigInteger_setZero(thread, exponent);
    return;
  }
  Arcadia_BooleanValue negative = Arcadia_BigInteger_isNegative(thread, significand);
  if (negative) {
    Arcadia_BigInteger_multiplyInteger8(thread, significand, Arcadia_Integer8Value_Literal(-1));
  }
  Arcadia_BigInteger* ten = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, ten, 10);
  Arcadia_BigInteger* quotient = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger* remainder = Arcadia_BigInteger_create(thread);
  while (!Arcadia_BigInteger_isZero(thread, significand)) {
    Arcadia_BigInteger_divide3(thread, significand, ten, quotient, remainder);
    if (!Arcadia_BigInteger_isZero(thread, remainder)) {
      break;
    }
    Arcadia_BigInteger_copy(thread, significand, quotient);
    Arcadia_BigInteger_addNatural8(thread, exponent, 1);
  }
  if (negative) {
    Arcadia_BigInteger_multiplyInteger8(thread, significand, Arcadia_Integer8Value_Literal(-1));
  }
}

void
Arcadia_BigReal_fromDecimalString
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (!self || !bytes || !numberOfBytes) {
    invalidText(thread);
  }
  Arcadia_SizeValue i = 0;
  Arcadia_BooleanValue negative = false;
  if (bytes[i] == '+' || bytes[i] == '-') {
    negative = bytes[i] == '-';
    ++i;
  }
  Arcadia_BigInteger* significand = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger* exponent = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger* ten = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, ten, 10);
  Arcadia_BooleanValue sawDigit = false;
  Arcadia_BooleanValue sawPoint = false;
  for (; i < numberOfBytes && bytes[i] != 'e' && bytes[i] != 'E'; ++i) {
    if (bytes[i] == '.') {
      if (sawPoint) {
        invalidText(thread);
      }
      sawPoint = true;
    } else if (bytes[i] >= '0' && bytes[i] <= '9') {
      sawDigit = true;
      Arcadia_BigInteger_multiply(thread, significand, ten);
      Arcadia_BigInteger_addNatural8(thread, significand, bytes[i] - '0');
      if (sawPoint) {
        Arcadia_BigInteger_subtractNatural8(thread, exponent, 1);
      }
    } else {
      invalidText(thread);
    }
  }
  if (!sawDigit) {
    invalidText(thread);
  }
  if (i < numberOfBytes) {
    ++i;
    if (i == numberOfBytes) {
      invalidText(thread);
    }
    Arcadia_BigInteger* explicitExponent = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_fromDecimalString(thread, explicitExponent,
                                         bytes + i, numberOfBytes - i);
    Arcadia_BigInteger_add(thread, exponent, explicitExponent);
  }
  if (negative) {
    Arcadia_BigInteger_multiplyInteger8(thread, significand, Arcadia_Integer8Value_Literal(-1));
  }
  normalize(thread, significand, exponent);
  Arcadia_BigInteger_copy(thread, self->significand, significand);
  Arcadia_BigInteger_copy(thread, self->exponent, exponent);
}

Arcadia_RuntimeUTF8String*
Arcadia_BigReal_toScientificString
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self
  )
{
  if (!self) {
    invalidText(thread);
  }
  Arcadia_RuntimeUTF8String* m = Arcadia_BigInteger_toDecimalString(thread, self->significand);
  if (Arcadia_BigInteger_isZero(thread, self->exponent)) {
    return m;
  }
  Arcadia_RuntimeUTF8String* e = Arcadia_BigInteger_toDecimalString(thread, self->exponent);
  if (m->numberOfBytes > SIZE_MAX - e->numberOfBytes - 1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue n = m->numberOfBytes + 1 + e->numberOfBytes;
  // The managed result is created from a temporary unmanaged byte buffer.
  Arcadia_Natural8Value* volatile bytes = Arcadia_Memory_allocateUnmanaged(thread, n);
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_Memory_copy(thread, (void*)bytes, m->bytes, m->numberOfBytes);
    bytes[m->numberOfBytes] = 'e';
    Arcadia_Memory_copy(thread, (void*)(bytes + m->numberOfBytes + 1),
                        e->bytes, e->numberOfBytes);
    Arcadia_RuntimeUTF8String* result = Arcadia_RuntimeUTF8String_create(thread, (void*)bytes, n);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Memory_deallocateUnmanaged(thread, (void*)bytes);
    return result;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Memory_deallocateUnmanaged(thread, (void*)bytes);
    Arcadia_Thread_jump(thread);
  }
  return NULL;
}

Arcadia_BooleanValue
Arcadia_BigReal_isEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  )
{
  if (!self || !other) {
    invalidText(thread);
  }
  return Arcadia_BigInteger_isEqualTo(thread, self->significand, other->significand) &&
         Arcadia_BigInteger_isEqualTo(thread, self->exponent, other->exponent);
}


// Compare magnitudes by decimal order, then by significant digits.
// Virtual trailing zeros avoid exponent-dependent allocation.
static Arcadia_Integer8Value
compareMagnitude
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* a,
    Arcadia_BigReal* b
  )
{
  Arcadia_RuntimeUTF8String* aText = Arcadia_BigInteger_toDecimalString(thread, a->significand);
  Arcadia_RuntimeUTF8String* bText = Arcadia_BigInteger_toDecimalString(thread, b->significand);
  Arcadia_SizeValue aStart = aText->bytes[0] == '-' ? 1 : 0;
  Arcadia_SizeValue bStart = bText->bytes[0] == '-' ? 1 : 0;
  Arcadia_SizeValue aDigits = aText->numberOfBytes - aStart;
  Arcadia_SizeValue bDigits = bText->numberOfBytes - bStart;

  Arcadia_BigInteger* aOrder = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger* bOrder = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_copy(thread, aOrder, a->exponent);
  Arcadia_BigInteger_copy(thread, bOrder, b->exponent);
  Arcadia_BigInteger_addNatural64(thread, aOrder, aDigits);
  Arcadia_BigInteger_addNatural64(thread, bOrder, bDigits);
  Arcadia_Integer8Value order = Arcadia_BigInteger_compareTo(thread, aOrder, bOrder);
  if (order) {
    return order;
  }

  Arcadia_SizeValue length = aDigits > bDigits ? aDigits : bDigits;
  for (Arcadia_SizeValue i = 0; i < length; ++i) {
    Arcadia_Natural8Value x = i < aDigits ? aText->bytes[aStart + i] : '0';
    Arcadia_Natural8Value y = i < bDigits ? bText->bytes[bStart + i] : '0';
    if (x < y) {
      return -1;
    }
    if (x > y) {
      return +1;
    }
  }
  return 0;
}

Arcadia_Integer8Value
Arcadia_BigReal_compareTo
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  )
{
  if (!self || !other) {
    invalidText(thread);
  }
  Arcadia_Integer8Value x = self->significand->sign;
  Arcadia_Integer8Value y = other->significand->sign;
  if (x < y) {
    return -1;
  }
  if (x > y) {
    return +1;
  }
  if (!x) {
    return 0;
  }
  Arcadia_Integer8Value magnitude = compareMagnitude(thread, self, other);
  return x > 0 ? magnitude : -magnitude;
}

Arcadia_BooleanValue
Arcadia_BigReal_isLowerThan
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  )
{
  return Arcadia_BigReal_compareTo(thread, self, other) < 0;
}

Arcadia_BooleanValue
Arcadia_BigReal_isLowerThanOrEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  )
{
  return Arcadia_BigReal_compareTo(thread, self, other) <= 0;
}

Arcadia_BooleanValue
Arcadia_BigReal_isGreaterThan
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  )
{
  return Arcadia_BigReal_compareTo(thread, self, other) > 0;
}

Arcadia_BooleanValue
Arcadia_BigReal_isGreaterThanOrEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  )
{
  return Arcadia_BigReal_compareTo(thread, self, other) >= 0;
}

void
Arcadia_BigReal_multiply3
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* result,
    Arcadia_BigReal* a,
    Arcadia_BigReal* b
  )
{
  Arcadia_BigInteger* significand = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger* exponent = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_multiply3(thread, significand, a->significand, b->significand);
  Arcadia_BigInteger_add3(thread, exponent, a->exponent, b->exponent);
  normalize(thread, significand, exponent);
  Arcadia_BigInteger_copy(thread, result->significand, significand);
  Arcadia_BigInteger_copy(thread, result->exponent, exponent);
}
