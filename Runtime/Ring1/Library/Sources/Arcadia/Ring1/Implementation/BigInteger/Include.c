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
#include "Arcadia/Ring1/Implementation/BigInteger/Include.h"

#include "Arcadia/Ring1/Include.h"

#define TypeName u8"Arcadia.BigInteger"

static Arcadia_BooleanValue g_registered = Arcadia_BooleanValue_False;

static void
onFinalize
  (
    Arcadia_Process* process,
    Arcadia_BigInteger* self
  )
{
  if (self->limps) {
    Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), self->limps);
    self->limps = NULL;
  }
}

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

Arcadia_BigInteger*
Arcadia_BigInteger_create
  (
    Arcadia_Thread* thread
  )
{
  if (!g_registered) {
    Arcadia_Process_registerType(Arcadia_Thread_getProcess(thread),
                                 TypeName, sizeof(TypeName) - 1,
                                 Arcadia_Thread_getProcess(thread), 
                                 (Arcadia_Process_TypeRemovedCallback*)&onTypeRemoved,
                                 NULL,
                                 (Arcadia_Process_FinalizeCallback*)&onFinalize);
    g_registered = Arcadia_BooleanValue_True;
  }
  Arcadia_BigInteger* self = NULL;
  Arcadia_Process_allocate(Arcadia_Thread_getProcess(thread), (void**)&self, TypeName, sizeof(TypeName) - 1, sizeof(Arcadia_BigInteger));
  self->numberOfLimps = 0;
  self->limps = NULL;
  self->sign = 0;
  self->limps = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_BigInteger_Limp) * 0);
  return self;
}

void
Arcadia_BigInteger_swap
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger* other
  )
{
  if (self != other) {
    Arcadia_swapPointer(thread, (void**)&self->limps, (void**)&other->limps);
    Arcadia_swapSize(thread, &self->numberOfLimps, &other->numberOfLimps);
    Arcadia_swapInteger8(thread, &self->sign, &other->sign);
  }
}

void
Arcadia_BigInteger_copy
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger* other
  )
{
  if (self != other) {
    if (self->numberOfLimps < other->numberOfLimps) {
      Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->limps, sizeof(Arcadia_BigInteger_Limp) * other->numberOfLimps);
    }
    Arcadia_Memory_copy(thread, self->limps, other->limps, sizeof(Arcadia_BigInteger_Limp) * other->numberOfLimps);
    self->numberOfLimps = other->numberOfLimps;
    self->sign = other->sign;
  }
}

void
Arcadia_BigInteger_setZero
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  )
{
  self->sign = 0;
  self->numberOfLimps = 0;
}

Arcadia_BooleanValue
Arcadia_BigInteger_isZero
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  )
{ return 0 == self->sign; }

Arcadia_BooleanValue
Arcadia_BigInteger_isPositive
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  )
{ return +1 == self->sign; }

Arcadia_BooleanValue
Arcadia_BigInteger_isNegative
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  )
{ return -1 == self->sign; }

void
Arcadia_BigInteger_visit
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self
  )
{
  Arcadia_Process_visitObject(Arcadia_Thread_getProcess(thread), self);
}

static void
isEqualTo
  (
    Arcadia_Thread* thread
  );

static void
isGreaterThan
  (
    Arcadia_Thread* thread
  );

static void
isGreaterThanOrEqualTo
  (
    Arcadia_Thread* thread
  );

static void
getHash
  (
    Arcadia_Thread* thread
  );

static void
isLowerThan
  (
    Arcadia_Thread* thread
  );

static void
isLowerThanOrEqualTo
  (
    Arcadia_Thread* thread
  );

static void
isNotEqualTo
  (
    Arcadia_Thread* thread
  );

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .isEqualTo = &isEqualTo,
  .isGreaterThan = &isGreaterThan,
  .isGreaterThanOrEqualTo = &isGreaterThanOrEqualTo,
  .getHash = &getHash,
  .isLowerThan = &isLowerThan,
  .isLowerThanOrEqualTo = &isLowerThanOrEqualTo,
  .isNotEqualTo = &isNotEqualTo,
};

#define BINARY_OPERATION() \
  if (Arcadia_ValueStack_getSize(thread) < 3) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  if (2 != Arcadia_ValueStack_getNatural8Value(thread, 0)) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 2); \
  Arcadia_Value y = Arcadia_ValueStack_getValue(thread, 1); \
  Arcadia_ValueStack_popValues(thread, 3);

#define UNARY_OPERATION() \
  if (Arcadia_ValueStack_getSize(thread) < 2) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  if (1 != Arcadia_ValueStack_getNatural8Value(thread, 0)) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 1); \
  Arcadia_ValueStack_popValues(thread, 2);

static void
isEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isBigIntegerValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BigInteger_isEqualTo(thread, Arcadia_Value_getBigIntegerValue(&x),  Arcadia_Value_getBigIntegerValue(&y)));
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
  }
}

static void
isGreaterThan
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isBigIntegerValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BigInteger_isGreaterThan(thread, Arcadia_Value_getBigIntegerValue(&x), Arcadia_Value_getBigIntegerValue(&y)));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
isGreaterThanOrEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isBigIntegerValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BigInteger_isGreaterThanOrEqualTo(thread, Arcadia_Value_getBigIntegerValue(&x), Arcadia_Value_getBigIntegerValue(&y)));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
getHash
  (
    Arcadia_Thread* thread
  )
{
  UNARY_OPERATION();
  Arcadia_ValueStack_pushSizeValue(thread, (Arcadia_SizeValue)(uintptr_t)Arcadia_Value_getBigIntegerValue(&x));
}

static void
isLowerThan
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isBigIntegerValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BigInteger_isLowerThan(thread, Arcadia_Value_getBigIntegerValue(&x), Arcadia_Value_getBigIntegerValue(&y)));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
isLowerThanOrEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isBigIntegerValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BigInteger_isLowerThanOrEqualTo(thread, Arcadia_Value_getBigIntegerValue(&x), Arcadia_Value_getBigIntegerValue(&y)));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
isNotEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isBigIntegerValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BigInteger_notEqualTo(thread, Arcadia_Value_getBigIntegerValue(&x), Arcadia_Value_getBigIntegerValue(&y)));
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_True);
  }
}

static Arcadia_TypeValue g_type = NULL;

static void
typeDestructing
  (
    void* context
  )
{
  g_type = NULL;
}

Arcadia_TypeValue
_Arcadia_BigIntegerValue_getType
  (
    Arcadia_Thread* thread
  )
{
  if (!g_type) {
    g_type = Arcadia_registerInternalType(thread, Arcadia_Names_getOrCreateName(thread, TypeName, sizeof(TypeName) - 1), &_typeOperations, &typeDestructing);
  }
  return g_type;
}

void
Arcadia_BigInteger_toStdoutDebug
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  )
{
  if (Arcadia_BigInteger_isZero(thread, self)) {
    Arcadia_logf(Arcadia_LogFlags_Trace, "0");
  } else {
    Arcadia_BigInteger* ten = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_setInteger16(thread, ten, 10);
    Arcadia_BigInteger* quotient = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_copy(thread, quotient, self);
    Arcadia_BigInteger* remainder = Arcadia_BigInteger_create(thread);
    if (Arcadia_BigInteger_isNegative(thread, self)) {
      Arcadia_logf(Arcadia_LogFlags_Trace, "-");
    }
    Arcadia_SizeValue i = 0, n = 1024;
    char* p = Arcadia_Memory_allocateUnmanaged(thread, 1024);
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      do {
        Arcadia_BigInteger_divide3(thread, quotient, ten, quotient, remainder);
        Arcadia_Natural8Value digit = Arcadia_BigInteger_toNatural8(thread, remainder);
        if (i == n) {
          Arcadia_SizeValue m = 2 * n;
          if (m < n) {
            Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
            Arcadia_Thread_jump(thread);
          }
          Arcadia_Memory_reallocateUnmanaged(thread, (void**)&p, m);
          n = m;
        }
        p[i++] = (char)(digit + '0');

      } while (!Arcadia_BigInteger_isZero(thread, quotient));
      for (Arcadia_SizeValue j = 0; j < i / 2; ++j) {
        char t = p[j];
        p[j] = p[i - j - 1];
        p[i - j - 1] = t;
      }
      p[i] = '\0';
      Arcadia_logf(Arcadia_LogFlags_Trace, "%s\n", p);
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Memory_deallocateUnmanaged(thread, p);
    p = NULL;
  }
}

void
_Arcadia_BigInteger_stripLeadingZeroes
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger_Limp** limps,
    Arcadia_SizeValue* numberOfLimps
  )
{
  while ((*numberOfLimps)> 0 && !(*limps)[(*numberOfLimps) - 1]) {
    (*numberOfLimps)--;
  }
}
