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
#include "Arcadia/Ring1/Implementation/Integer16.h"

#include "Arcadia/Ring1/Implementation/_defineScalarType.h"
#include "Arcadia/Ring1/Include.h"

static void
add
  (
    Arcadia_Thread* thread
  );

static void
divide
  (
    Arcadia_Thread* thread
  );

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
isIdenticalTo
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
multiply
  (
    Arcadia_Thread* thread
  );

static void
negate
  (
    Arcadia_Thread* thread
  );

static void
isNotEqualTo
  (
    Arcadia_Thread* thread
  );

static void
subtract
  (
    Arcadia_Thread* thread
  );

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .add = &add,
  .divide = &divide,
  .isEqualTo = &isEqualTo,
  .isGreaterThan = &isGreaterThan,
  .isGreaterThanOrEqualTo = &isGreaterThanOrEqualTo,
  .getHash = &getHash,
  .isIdenticalTo = &isIdenticalTo,
  .isLowerThan = &isLowerThan,
  .isLowerThanOrEqualTo = &isLowerThanOrEqualTo,
  .multiply = &multiply,
  .negate = &negate,
  .isNotEqualTo = &isNotEqualTo,
  .subtract = &subtract,
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
add
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isInteger8Value(&y)) {
    Arcadia_ValueStack_pushInteger16Value(thread, Arcadia_Value_getInteger16Value(&x) + (Arcadia_Integer16Value)Arcadia_Value_getInteger8Value(&y));
  } else if (Arcadia_Value_isInteger16Value(&y)) {
    Arcadia_ValueStack_pushInteger16Value(thread, Arcadia_Value_getInteger16Value(&x) + Arcadia_Value_getInteger16Value(&y));
  } else if (Arcadia_Value_isInteger32Value(&y)) {
    Arcadia_ValueStack_pushInteger32Value(thread, (Arcadia_Integer32Value)Arcadia_Value_getInteger16Value(&x) + Arcadia_Value_getInteger32Value(&y));
  } else if (Arcadia_Value_isInteger64Value(&y)) {
    Arcadia_ValueStack_pushInteger64Value(thread, (Arcadia_Integer64Value)Arcadia_Value_getInteger16Value(&x) + Arcadia_Value_getInteger64Value(&y));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
divide
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isInteger8Value(&y)) {
    if (!Arcadia_Value_getInteger8Value(&y)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_DivisionByZero);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ValueStack_pushInteger16Value(thread, Arcadia_Value_getInteger16Value(&x) / (Arcadia_Integer16Value)Arcadia_Value_getInteger8Value(&y));
  } else if (Arcadia_Value_isInteger16Value(&y)) {
    if (!Arcadia_Value_getInteger16Value(&y)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_DivisionByZero);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ValueStack_pushInteger16Value(thread, Arcadia_Value_getInteger16Value(&x) / Arcadia_Value_getInteger16Value(&y));
  } else if (Arcadia_Value_isInteger32Value(&y)) {
    if (!Arcadia_Value_getInteger32Value(&y)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_DivisionByZero);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ValueStack_pushInteger32Value(thread, (Arcadia_Integer32Value)Arcadia_Value_getInteger16Value(&x) / Arcadia_Value_getInteger32Value(&y));
  } else if (Arcadia_Value_isInteger64Value(&y)) {
    if (!Arcadia_Value_getInteger64Value(&y)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_DivisionByZero);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ValueStack_pushInteger64Value(thread, (Arcadia_Integer64Value)Arcadia_Value_getInteger16Value(&x) / Arcadia_Value_getInteger64Value(&y));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
isEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isInteger8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) == (Arcadia_Integer16Value)Arcadia_Value_getInteger8Value(&y));
  } else if (Arcadia_Value_isInteger16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) == Arcadia_Value_getInteger16Value(&y));
  } else if (Arcadia_Value_isInteger32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer32Value)Arcadia_Value_getInteger16Value(&x) == Arcadia_Value_getInteger32Value(&y));
  } else if (Arcadia_Value_isInteger64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer64Value)Arcadia_Value_getInteger16Value(&x) == Arcadia_Value_getInteger64Value(&y));
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
  if (Arcadia_Value_isInteger8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) > (Arcadia_Integer16Value)Arcadia_Value_getInteger8Value(&y));
  } else if (Arcadia_Value_isInteger16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) > Arcadia_Value_getInteger16Value(&y));
  } else if (Arcadia_Value_isInteger32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer32Value)Arcadia_Value_getInteger16Value(&x) > Arcadia_Value_getInteger32Value(&y));
  } else if (Arcadia_Value_isInteger64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer64Value)Arcadia_Value_getInteger16Value(&x) > Arcadia_Value_getInteger64Value(&y));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
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
  if (Arcadia_Value_isInteger8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) >= (Arcadia_Integer16Value)Arcadia_Value_getInteger8Value(&y));
  } else if (Arcadia_Value_isInteger16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) >= Arcadia_Value_getInteger16Value(&y));
  } else if (Arcadia_Value_isInteger32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer32Value)Arcadia_Value_getInteger16Value(&x) >= Arcadia_Value_getInteger32Value(&y));
  } else if (Arcadia_Value_isInteger64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer64Value)Arcadia_Value_getInteger16Value(&x) >= Arcadia_Value_getInteger64Value(&y));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
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
  Arcadia_ValueStack_pushSizeValue(thread, (Arcadia_SizeValue)Arcadia_Value_getInteger16Value(&x));
}

static void
isIdenticalTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isInteger16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) == Arcadia_Value_getInteger16Value(&y));
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
  }
}

static void
isLowerThan
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isInteger8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) < (Arcadia_Integer16Value)Arcadia_Value_getInteger8Value(&y));
  } else if (Arcadia_Value_isInteger16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) < Arcadia_Value_getInteger16Value(&y));
  } else if (Arcadia_Value_isInteger32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer32Value)Arcadia_Value_getInteger16Value(&x) < Arcadia_Value_getInteger32Value(&y));
  } else if (Arcadia_Value_isInteger64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer64Value)Arcadia_Value_getInteger16Value(&x) < Arcadia_Value_getInteger64Value(&y));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
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
  if (Arcadia_Value_isInteger8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) <= (Arcadia_Integer16Value)Arcadia_Value_getInteger8Value(&y));
  } else if (Arcadia_Value_isInteger16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) <= Arcadia_Value_getInteger16Value(&y));
  } else if (Arcadia_Value_isInteger32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer32Value)Arcadia_Value_getInteger16Value(&x) <= Arcadia_Value_getInteger32Value(&y));
  } else if (Arcadia_Value_isInteger64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer64Value)Arcadia_Value_getInteger16Value(&x) <= Arcadia_Value_getInteger64Value(&y));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
multiply
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isInteger8Value(&y)) {
    Arcadia_ValueStack_pushInteger16Value(thread, Arcadia_Value_getInteger16Value(&x) * (Arcadia_Integer16Value)Arcadia_Value_getInteger8Value(&y));
  } else if (Arcadia_Value_isInteger16Value(&y)) {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_Value_getInteger16Value(&x) * Arcadia_Value_getInteger16Value(&y));
  } else if (Arcadia_Value_isInteger32Value(&y)) {
    Arcadia_ValueStack_pushInteger32Value(thread, (Arcadia_Integer32Value)Arcadia_Value_getInteger16Value(&x) * Arcadia_Value_getInteger32Value(&y));
  } else if (Arcadia_Value_isInteger64Value(&y)) {
    Arcadia_ValueStack_pushInteger64Value(thread, (Arcadia_Integer64Value)Arcadia_Value_getInteger16Value(&x) * Arcadia_Value_getInteger64Value(&y));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
negate
  (
    Arcadia_Thread* thread
  )
{
  UNARY_OPERATION();
  Arcadia_ValueStack_pushInteger16Value(thread, -Arcadia_Value_getInteger16Value(&x));
}

static void
isNotEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isInteger8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) != (Arcadia_Integer16Value)Arcadia_Value_getInteger8Value(&y));
  } else if (Arcadia_Value_isInteger16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getInteger16Value(&x) != Arcadia_Value_getInteger16Value(&y));
  } else if (Arcadia_Value_isInteger32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer32Value)Arcadia_Value_getInteger16Value(&x) != Arcadia_Value_getInteger32Value(&y));
  } else if (Arcadia_Value_isInteger64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Integer64Value)Arcadia_Value_getInteger16Value(&x) != Arcadia_Value_getInteger64Value(&y));
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_True);
  }
}

static void
subtract
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isInteger8Value(&y)) {
    Arcadia_ValueStack_pushInteger16Value(thread, Arcadia_Value_getInteger16Value(&x) - (Arcadia_Integer16Value)Arcadia_Value_getInteger8Value(&y));
  } else if (Arcadia_Value_isInteger16Value(&y)) {
    Arcadia_ValueStack_pushInteger16Value(thread, Arcadia_Value_getInteger16Value(&x) - Arcadia_Value_getInteger16Value(&y));
  } else if (Arcadia_Value_isInteger32Value(&y)) {
    Arcadia_ValueStack_pushInteger32Value(thread, (Arcadia_Integer32Value)Arcadia_Value_getInteger16Value(&x) - Arcadia_Value_getInteger32Value(&y));
  } else if (Arcadia_Value_isInteger64Value(&y)) {
    Arcadia_ValueStack_pushInteger64Value(thread, (Arcadia_Integer64Value)Arcadia_Value_getInteger16Value(&x) - Arcadia_Value_getInteger64Value(&y));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

Arcadia_defineScalarType(Arcadia_Integer16, u8"Arcadia.Integer16", &_typeOperations);
