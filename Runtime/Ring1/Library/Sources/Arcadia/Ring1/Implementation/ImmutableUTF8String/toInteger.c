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
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/toInteger.h"

#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/NumeralParser.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String.h"
#include "Arcadia/Ring1/Implementation/NumberLiteral.h"
#include "Arcadia/Ring1/Implementation/Process.h"
#include "Arcadia/Ring1/Implementation/safeAdd.h"
#include "Arcadia/Ring1/Implementation/safeMultiply.h"
#include "Arcadia/Ring1/Implementation/Thread.h"

static Arcadia_Integer64Value
_toInteger64Internal
  (
    Arcadia_Thread* thread,
    _State* state
  )
{
  static const Arcadia_Integer64Value BASE = 10;
  // The maximum decimal value of an Arcadia_Integer64Value is -9,223,372,036,854,775,808. These are 19 decimal digits.
  // The minimum decimal value of an Arcadia_Integer64Value is +9,223,372,036,854,775,807. These are 19 decimal digits.
  // 18 = 19 - 1 is the number of decimal digits which always fit into an Arcadia_Integer64Value.
  static const Arcadia_SizeValue SAFEDIGITSBASE10 = 18;
  next(state);
  bool negative = false;
  if (isMinus(state)) {
    negative = true;
    next(state);
  } else if (isPlus(state)) {
    next(state);
  }
  Arcadia_Integer64Value v = 0;
  if (!isDigit(state)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  // Skip leading zeroes.
  while (isZero(state)) {
    next(state);
  }
  while (isDigit(state)) {
    Arcadia_Integer64Value w = 0;
    // We accumulate up to 18 decimal digits in w.
    Arcadia_SizeValue i = 0, n = SAFEDIGITSBASE10;
    for (; isDigit(state) && i < n; ++i) {
      Arcadia_Integer64Value digit = (Arcadia_Integer64Value)(state->codePoint - '0');
      w = w * BASE + digit;
      next(state);
    }
    // We need to multiply v by the number of digits in w.
    while (i > 0) {
      if (v < Arcadia_Integer64Value_Minimum / 10) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
        Arcadia_Thread_jump(thread);
      }
      v = v * 10;
      i--;
    }
    // If we cannot subtract w from v, then this number is not representable.
    if (v < Arcadia_Integer64Value_Minimum + w) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
      Arcadia_Thread_jump(thread);
    }
    v -= w;
  }
  if (!isEnd(state)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  if (negative) {
    return v;
  }
  if (v == Arcadia_Integer64Value_Minimum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  v = -v;
  return v;
}

Arcadia_Integer16Value
_toInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  )
{
  Arcadia_Integer64Value v = _toInteger64(thread, immutableUTF8StringValue);
  if (v < Arcadia_Integer16Value_Minimum || v > Arcadia_Integer16Value_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_Integer16Value)v;
}

Arcadia_Integer32Value
_toInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  )
{
  Arcadia_Integer64Value v = _toInteger64(thread, immutableUTF8StringValue);
  if (v < Arcadia_Integer32Value_Minimum || v > Arcadia_Integer32Value_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_Integer32Value)v;
}

Arcadia_Integer64Value
_toInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  )
{
  _State state;
  _State_init(&state, Arcadia_RuntimeUTF8String_getBytes(thread, immutableUTF8StringValue),
                      Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, immutableUTF8StringValue));
  Arcadia_Integer64Value value;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    value = _toInteger64Internal(thread, &state);
    _State_uninit(&state);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    _State_uninit(&state);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_jump(thread);
  }
  return value;
}

Arcadia_Integer8Value
_toInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  )
{
  Arcadia_Integer64Value v = _toInteger64(thread, immutableUTF8StringValue);
  if (v < Arcadia_Integer8Value_Minimum || v > Arcadia_Integer8Value_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_Integer8Value)v;
}
