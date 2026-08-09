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
#include "Arcadia/Ring1/Implementation/Natural16.h"

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
notEqualTo
  (
    Arcadia_Thread* thread
  );

static void
subtract
  (
    Arcadia_Thread* thread
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
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
  .isNotEqualTo = &notEqualTo,
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
  if (Arcadia_Value_isNatural8Value(&y)) {
    Arcadia_ValueStack_pushNatural16Value(thread, Arcadia_Value_getNatural16Value(&x) + (Arcadia_Natural16Value)Arcadia_Value_getNatural8Value(&y));
  } else if (Arcadia_Value_isNatural16Value(&y)) {
    Arcadia_ValueStack_pushNatural16Value(thread, Arcadia_Value_getNatural16Value(&x) + Arcadia_Value_getNatural16Value(&y));
  } else if (Arcadia_Value_isNatural32Value(&y)) {
    Arcadia_ValueStack_pushNatural32Value(thread, (Arcadia_Natural32Value)Arcadia_Value_getNatural16Value(&x) + Arcadia_Value_getNatural32Value(&y));
  } else if (Arcadia_Value_isNatural64Value(&y)) {
    Arcadia_ValueStack_pushNatural64Value(thread, (Arcadia_Natural64Value)Arcadia_Value_getNatural16Value(&x) + Arcadia_Value_getNatural64Value(&y));
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
  if (Arcadia_Value_isNatural8Value(&y)) {
    if (!Arcadia_Value_getNatural8Value(&y)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_DivisionByZero);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ValueStack_pushNatural16Value(thread, Arcadia_Value_getNatural16Value(&x) / (Arcadia_Natural16Value)Arcadia_Value_getNatural8Value(&y));
  } else if (Arcadia_Value_isNatural16Value(&y)) {
    if (!Arcadia_Value_getNatural16Value(&y)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_DivisionByZero);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ValueStack_pushNatural16Value(thread, Arcadia_Value_getNatural16Value(&x) / Arcadia_Value_getNatural16Value(&y));
  } else if (Arcadia_Value_isNatural32Value(&y)) {
    if (!Arcadia_Value_getNatural32Value(&y)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_DivisionByZero);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ValueStack_pushNatural32Value(thread, (Arcadia_Natural32Value)Arcadia_Value_getNatural16Value(&x) / Arcadia_Value_getNatural32Value(&y));
  } else if (Arcadia_Value_isNatural64Value(&y)) {
    if (!Arcadia_Value_getNatural64Value(&y)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_DivisionByZero);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ValueStack_pushNatural64Value(thread, (Arcadia_Natural64Value)Arcadia_Value_getNatural16Value(&x) / Arcadia_Value_getNatural64Value(&y));
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
  if (Arcadia_Value_isNatural8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) == (Arcadia_Natural16Value)Arcadia_Value_getNatural8Value(&y));
  } else if (Arcadia_Value_isNatural16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) == Arcadia_Value_getNatural16Value(&y));
  } else if (Arcadia_Value_isNatural32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural32Value)Arcadia_Value_getNatural16Value(&x) == Arcadia_Value_getNatural32Value(&y));
  } else if (Arcadia_Value_isNatural64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural64Value)Arcadia_Value_getNatural16Value(&x) == Arcadia_Value_getNatural64Value(&y));
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
  if (Arcadia_Value_isNatural8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) > (Arcadia_Natural16Value)Arcadia_Value_getNatural8Value(&y));
  } else if (Arcadia_Value_isNatural16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) > Arcadia_Value_getNatural16Value(&y));
  } else if (Arcadia_Value_isNatural32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural32Value)Arcadia_Value_getNatural16Value(&x) > Arcadia_Value_getNatural32Value(&y));
  } else if (Arcadia_Value_isNatural64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural64Value)Arcadia_Value_getNatural16Value(&x) > Arcadia_Value_getNatural64Value(&y));
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
  if (Arcadia_Value_isNatural8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) >= (Arcadia_Natural16Value)Arcadia_Value_getNatural8Value(&y));
  } else if (Arcadia_Value_isNatural16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) >= Arcadia_Value_getNatural16Value(&y));
  } else if (Arcadia_Value_isNatural32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural32Value)Arcadia_Value_getNatural16Value(&x) >= Arcadia_Value_getNatural32Value(&y));
  } else if (Arcadia_Value_isNatural64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural64Value)Arcadia_Value_getNatural16Value(&x) >= Arcadia_Value_getNatural64Value(&y));
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
  Arcadia_ValueStack_pushSizeValue(thread, (Arcadia_SizeValue)Arcadia_Value_getNatural16Value(&x));
}

static void
isIdenticalTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isNatural16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) == Arcadia_Value_getNatural16Value(&y));
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
  if (Arcadia_Value_isNatural8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) < (Arcadia_Natural16Value)Arcadia_Value_getNatural8Value(&y));
  } else if (Arcadia_Value_isNatural16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) < Arcadia_Value_getNatural16Value(&y));
  } else if (Arcadia_Value_isNatural32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural32Value)Arcadia_Value_getNatural16Value(&x) < Arcadia_Value_getNatural32Value(&y));
  } else if (Arcadia_Value_isNatural64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural64Value)Arcadia_Value_getNatural16Value(&x) < Arcadia_Value_getNatural64Value(&y));
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
  if (Arcadia_Value_isNatural8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) <= (Arcadia_Natural16Value)Arcadia_Value_getNatural8Value(&y));
  } else if (Arcadia_Value_isNatural16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) <= Arcadia_Value_getNatural16Value(&y));
  } else if (Arcadia_Value_isNatural32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural32Value)Arcadia_Value_getNatural16Value(&x) <= Arcadia_Value_getNatural32Value(&y));
  } else if (Arcadia_Value_isNatural64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural64Value)Arcadia_Value_getNatural16Value(&x) <= Arcadia_Value_getNatural64Value(&y));
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
  if (Arcadia_Value_isNatural8Value(&y)) {
    Arcadia_ValueStack_pushNatural16Value(thread, Arcadia_Value_getNatural16Value(&x) * (Arcadia_Natural16Value)Arcadia_Value_getNatural8Value(&y));
  } else if (Arcadia_Value_isNatural16Value(&y)) {
    Arcadia_ValueStack_pushNatural16Value(thread, Arcadia_Value_getNatural16Value(&x) * Arcadia_Value_getNatural16Value(&y));
  } else if (Arcadia_Value_isNatural32Value(&y)) {
    Arcadia_ValueStack_pushNatural32Value(thread, (Arcadia_Natural32Value)Arcadia_Value_getNatural16Value(&x) * Arcadia_Value_getNatural32Value(&y));
  } else if (Arcadia_Value_isNatural64Value(&y)) {
    Arcadia_ValueStack_pushNatural64Value(thread, (Arcadia_Natural64Value)Arcadia_Value_getNatural16Value(&x) * Arcadia_Value_getNatural64Value(&y));
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
  Arcadia_ValueStack_pushNatural32Value(thread, -Arcadia_Value_getNatural32Value(&x));
}

static void
notEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isNatural8Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) != (Arcadia_Natural16Value)Arcadia_Value_getNatural8Value(&y));
  } else if (Arcadia_Value_isNatural16Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getNatural16Value(&x) != Arcadia_Value_getNatural16Value(&y));
  } else if (Arcadia_Value_isNatural32Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural32Value)Arcadia_Value_getNatural16Value(&x) != Arcadia_Value_getNatural32Value(&y));
  } else if (Arcadia_Value_isNatural64Value(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, (Arcadia_Natural64Value)Arcadia_Value_getNatural16Value(&x) != Arcadia_Value_getNatural64Value(&y));
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
  if (Arcadia_Value_isNatural8Value(&y)) {
    Arcadia_ValueStack_pushNatural16Value(thread, Arcadia_Value_getNatural16Value(&x) - (Arcadia_Natural16Value)Arcadia_Value_getNatural8Value(&y));
  } else if (Arcadia_Value_isNatural16Value(&y)) {
    Arcadia_ValueStack_pushNatural16Value(thread, Arcadia_Value_getNatural16Value(&x) - Arcadia_Value_getNatural16Value(&y));
  } else if (Arcadia_Value_isNatural32Value(&y)) {
    Arcadia_ValueStack_pushNatural32Value(thread, (Arcadia_Natural32Value)Arcadia_Value_getNatural16Value(&x) - Arcadia_Value_getNatural32Value(&y));
  } else if (Arcadia_Value_isNatural64Value(&y)) {
    Arcadia_ValueStack_pushNatural64Value(thread, (Arcadia_Natural64Value)Arcadia_Value_getNatural16Value(&x) - Arcadia_Value_getNatural64Value(&y));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

Arcadia_defineScalarType(Arcadia_Natural16, u8"Arcadia.Natural16", &_typeOperations);
