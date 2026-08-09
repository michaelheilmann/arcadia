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
#include "Arcadia/Ring1/Implementation/Void.h"

#include "Arcadia/Ring1/Implementation/_defineScalarType.h"
#include "Arcadia/Ring1/Include.h"

static void
isEqualTo
  (
    Arcadia_Thread* thread
  );

static void
getHash
  (
    Arcadia_Thread* thread
  );

static void
isIdenticalTo
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
  .getHash = &getHash,
  .isIdenticalTo = &isIdenticalTo,
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
  if (Arcadia_Value_isVoidValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getVoidValue(&x) == Arcadia_Value_getVoidValue(&y));
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
  }
}

static void
getHash
  (
    Arcadia_Thread* thread
  )
{
  UNARY_OPERATION();
  Arcadia_ValueStack_pushSizeValue(thread, (Arcadia_SizeValue)71);
}

static void
isIdenticalTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isVoidValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_True);
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
  }
}

static void
isNotEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isVoidValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getVoidValue(&x) != Arcadia_Value_getVoidValue(&y));
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_True);
  }
}

Arcadia_defineScalarType(Arcadia_Void, u8"Arcadia.Void", &_typeOperations);
