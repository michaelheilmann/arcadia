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
#include "Arcadia/Ring1/Implementation/safeMultiply.h"

#include "Arcadia/Ring1/Implementation/Thread.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_BooleanValue
Arcadia_safeMultiplyNatural16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value multiplier,
    Arcadia_Natural16Value multiplicand,
    Arcadia_Natural16Value* productHigh,
    Arcadia_Natural16Value* productLow
  )
{
  if (!productHigh | !productLow) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Natural32Value a = multiplier;
  Arcadia_Natural32Value b = multiplicand;
  Arcadia_Natural32Value c = a * b;
  *productHigh = (c & 0xffff0000) >> 16;
  *productLow = (Arcadia_Natural16Value)(c & 0x0000ffff);
  return !(*productHigh);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_BooleanValue
Arcadia_safeMultiplyNatural32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value multiplier,
    Arcadia_Natural32Value multiplicand,
    Arcadia_Natural32Value* productHigh,
    Arcadia_Natural32Value* productLow
  )
{
  if (!productHigh | !productLow) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Natural64Value a = multiplier;
  Arcadia_Natural64Value b = multiplicand;
  Arcadia_Natural64Value c = a * b;
  *productHigh = (c & 0xffffffff00000000) >> 32;
  *productLow = (Arcadia_Natural32Value)(c & 0x00000000ffffffff);
  return !(*productHigh);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// @remarks
// Given two w Bit unsigned numbers
// Rewrite multiplier x and multiplicand y in terms of their upper and lower bits:
// x = a * 2^32 + b
// y = c * 2^32 + d
// Consequently, their product can be rewritten as
//   multiplier * multiplicand
// =   [a * 2^32 * c * 2^32]
//   + [a * 2^32 * d]
//   + [c * 2^32 * b]
//   + b * d
// =   [a * c * 2^64]
//   + [a * d + b * c] * 2^32
//   + b * d
// @remarks
// From "Computer Systems: A Programmer's Perspective; Bryant and Halaron; Prentice-Hall; 2003; pp 42"
// A binary string x of length w can be interpreted as unsigned integer B2U(x) = \sum_{i=0}^{w-1} x_i * 2^i.
// A binary string x of length w can be interpreted as two's complement integer B2T(x) = -x_{w-1} * 2^{w-1} + \sum_{i=0}^{w-2} x_i * 2^i.
// A binary string x of length w can be interpreted as sign magnitude integer B2S(x) = (-1)^(x_{w-1}) * (\sum_{i=0}^{w-2} x_i 2^i).
// @remarks
// On two's complement systems, casting from signed to unsigned and vice whilst retaining the width retains the bit pattern.
// @remarks
// Adapted from xmrig (https://github.com/xmrig/xmrig/blob/master/src/base/tools/cryptonote/umul128.h).
Arcadia_BooleanValue
Arcadia_safeMultiplyNatural64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value multiplier,
    Arcadia_Natural64Value multiplicand,
    Arcadia_Natural64Value* productHigh,
    Arcadia_Natural64Value* productLow
  )
{
  Arcadia_Natural64Value a = multiplier >> 32;
  Arcadia_Natural64Value b = multiplier & 0xffffffff;
  Arcadia_Natural64Value c = multiplicand >> 32;
  Arcadia_Natural64Value d = multiplicand & 0xffffffff;

  // Compute the partial products.
  // The products cannot overflow:
  // a, b, c, d are all smaller than or equal to 2^32-1 and hence a product of two of these has the maximal value of (2^32-1)^2 = 2^64 - 2^33 + 1 which is strictly smaller than 2^64 - 1.
  Arcadia_Natural64Value ac = a * c;
  Arcadia_Natural64Value ad = a * d;
  Arcadia_Natural64Value bc = b * c;
  Arcadia_Natural64Value bd = b * d;

  // Compute the middle part.
  Arcadia_Natural64Value mi = ad + bc; // CAN overflow. However, it's bitwise value is proper.
  Arcadia_Natural64Value miCarry = mi < ad ? 1 : 0;

  // Compute the low part.
  Arcadia_Natural64Value lo = bd + (mi << 32); // CAN overflow. However, it's bitwise value is proper.
  Arcadia_Natural64Value loCarry = lo < bd ? 1 : 0;

  // Compute the high part.
  Arcadia_Natural64Value hi = ac + (mi >> 32) + (miCarry << 32) + loCarry;

  if (productHigh) {
    *productHigh = hi;
  }
  if (productLow) {
    *productLow = lo;
  }

  return !hi;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_BooleanValue
Arcadia_safeMultiplyNatural8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value multiplier,
    Arcadia_Natural8Value multiplicand,
    Arcadia_Natural8Value* productHigh,
    Arcadia_Natural8Value* productLow
  )
{
  if (!productHigh | !productLow) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Natural16Value a = multiplier;
  Arcadia_Natural16Value b = multiplicand;
  Arcadia_Natural16Value c = a * b;
  Arcadia_Natural8Value hi = (c & 0xff00) >> 8;
  Arcadia_Natural8Value lo = (Arcadia_Natural8Value)(c & 0x00ff);
  if (productHigh) {
    *productHigh = hi;
  }
  if (productLow) {
    *productLow = lo;
  }
  return !hi;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_BooleanValue
Arcadia_safeMultiplySizeValue
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue multiplier,
    Arcadia_SizeValue multiplicand,
    Arcadia_SizeValue* productHigh,
    Arcadia_SizeValue* productLow
  )
{
#if Arcadia_Configuration_CompilerC_Msvc == Arcadia_Configuration_CompilerC
  #if Arcadia_Configuration_InstructionSetArchitecture_X64 == Arcadia_Configuration_InstructionSetArchitecture
    Arcadia_StaticAssert(Arcadia_SizeValue_Maximum == Arcadia_Natural64Value_Maximum && Arcadia_SizeValue_NumberOfBits == Arcadia_Natural64Value_NumberOfBits, "environment not (yet) supported");
    return Arcadia_safeMultiplyNatural64Value(thread, multiplier, multiplicand, (Arcadia_Natural64Value*)productHigh, (Arcadia_Natural64Value*)productLow);
  #elif Arcadia_Configuration_InstructionSetArchitecture_X86 == Arcadia_Configuration_InstructionSetArchitecture
    Arcadia_StaticAssert(Arcadia_SizeValue_Maximum == Arcadia_Natural32Value_Maximum && Arcadia_SizeValue_NumberOfBits == Arcadia_Natural32Value_NumberOfBits, "environment not (yet) supported");
    return Arcadia_safeMultiplyNatural32Value(thread, multiplier, multiplicand, (Arcadia_Natural32Value*)productHigh, (Arcadia_Natural32Value*)productLow);
  #else
    #error ("environemnt not (yet) supported");
  #endif
#elif Arcadia_Configuration_CompilerC_Gcc == Arcadia_Configuration_CompilerC
  #if Arcadia_Configuration_InstructionSetArchitecture_X64 == Arcadia_Configuration_InstructionSetArchitecture
    Arcadia_StaticAssert(Arcadia_SizeValue_Maximum == Arcadia_Natural64Value_Maximum && Arcadia_SizeValue_NumberOfBits == Arcadia_Natural64Value_NumberOfBits, "environment not (yet) supported");
    return Arcadia_safeMultiplyNatural64Value(thread, multiplier, multiplicand, (Arcadia_Natural64Value*)productHigh, (Arcadia_Natural64Value*)productLow);
  #elif Arcadia_Configuration_InstructionSetArchitecture_X86 == Arcadia_Configuration_InstructionSetArchitecture
    Arcadia_StaticAssert(Arcadia_SizeValue_Maximum == Arcadia_Natural32Value_Maximum && Arcadia_SizeValue_NumberOfBits == Arcadia_Natural32Value_NumberOfBits, "environment not (yet) supported");
    return Arcadia_safeMultiplyNatural32Value(thread, multiplier, multiplicand, (Arcadia_Natural32Value*)productHigh, (Arcadia_Natural32Value*)productLow);
  #else
    #error("environemnt not (yet) supported");
  #endif
#else
  #error ("environemnt not (yet) supported");
#endif
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_BooleanValue
Arcadia_safeMultiplyInteger16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value multiplier,
    Arcadia_Integer16Value multiplicand,
    Arcadia_Integer16Value* upperProduct,
    Arcadia_Integer16Value* lowerProduct
  )
{
  Arcadia_Integer16Value product = ((Arcadia_Integer16Value)multiplier) * ((Arcadia_Integer16Value)multiplicand);
  Arcadia_Integer16Value hi = (Arcadia_Integer16Value)((product & 0xffff0000) >> 16);
  Arcadia_Integer16Value lo = (Arcadia_Integer16Value)((product & 0x0000ffff) >> 0);
  if (upperProduct) {
    *upperProduct = hi;
  }
  if (lowerProduct) {
    *lowerProduct = lo;
  }
  if ((lo < 0 && hi != -1) || (lo >= 0 && hi != 0)) {
    return Arcadia_BooleanValue_True;
  } else {
    return Arcadia_BooleanValue_False;
  }
}

Arcadia_BooleanValue
Arcadia_safeMultiplyInteger32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value multiplier,
    Arcadia_Integer32Value multiplicand,
    Arcadia_Integer32Value* upperProduct,
    Arcadia_Integer32Value* lowerProduct
  )
{
  Arcadia_Integer64Value product = ((Arcadia_Integer64Value)multiplier) * ((Arcadia_Integer64Value)multiplicand);
  Arcadia_Integer32Value hi = (Arcadia_Integer32Value)((product & 0xffffffff00000000) >> 32);
  Arcadia_Integer32Value lo = (Arcadia_Integer32Value)((product & 0x00000000ffffffff) >> 0);
  if (upperProduct) {
    *upperProduct = hi;
  }
  if (lowerProduct) {
    *lowerProduct = lo;
  }
  if ((lo < 0 && hi != -1) || (lo >= 0 && hi != 0)) {
    return Arcadia_BooleanValue_True;
  } else {
    return Arcadia_BooleanValue_False;
  }
}

#if Arcadia_Configuration_CompilerC_Msvc == Arcadia_Configuration_CompilerC && \
    Arcadia_Configuration_InstructionSetArchitecture_X64 == Arcadia_Configuration_InstructionSetArchitecture

#include <intrin.h>

// product of 64 bit two's complement integers
static inline Arcadia_Integer64Value
__imul128
  (
    Arcadia_Integer64Value multiplier,
    Arcadia_Integer64Value multiplicand,
    Arcadia_Integer64Value* upperProduct
  )
{ return _mul128(multiplier, multiplicand, upperProduct); }

#elif (Arcadia_Configuration_CompilerC_Msvc == Arcadia_Configuration_CompilerC && \
       Arcadia_Configuration_InstructionSetArchitecture_X86 == Arcadia_Configuration_InstructionSetArchitecture)

static inline Arcadia_Integer64Value
__imul128
  (
    Arcadia_Integer64Value multiplier,
    Arcadia_Integer64Value multiplicand,
    Arcadia_Integer64Value* upperProduct
  )
{
#define N64(e) ((Arcadia_Natural64Value)(e))
#define I64(e) ((Arcadia_Integer64Value)(e))

  static const Arcadia_Natural64Value mask = 0xffffffff;
  Arcadia_Integer64Value u = multiplier, v = multiplicand;
  Arcadia_Integer64Value u_hi = u >> 32,
                         v_hi = v >> 32;
  Arcadia_Natural64Value u_lo = N64(u) & mask,
                         v_lo = N64(v) & mask;

  Arcadia_Natural64Value const t1 = u_lo * v_lo;
  Arcadia_Natural64Value const w3 = t1 & mask;
  Arcadia_Natural64Value const k1 = t1 >> 32;

  Arcadia_Natural64Value const t2 = N64(u_hi) * v_lo + k1;
  Arcadia_Natural64Value const w2 = t2 & mask;
  Arcadia_Natural64Value const w1 = N64(I64(t2) >> 32);

  Arcadia_Natural64Value const t3 = u_lo * N64(v_hi) + w2;
  Arcadia_Natural64Value const k2 = N64(I64(t3) >> 32);

  Arcadia_Integer64Value const hi = I64(N64(u_hi * v_hi) + w1 + k2);
  Arcadia_Integer64Value const lo = I64((t3 << 32) + w3);
  *upperProduct = hi;

  return lo;
#undef I64
#undef N64
}

#elif Arcadia_Configuration_CompilerC_Gcc == Arcadia_Configuration_CompilerC || \
      Arcadia_Configuration_CompilerC_Clang == Arcadia_Configuration_CompilerC

static inline Arcadia_Integer64Value
__imul128
  (
    Arcadia_Integer64Value multiplier,
    Arcadia_Integer64Value multiplicand,
    Arcadia_Integer64Value* upperProduct
  )
{
  __int128 product = ((__int128)multiplier) * ((__int128)multiplicand);
  *upperProduct = (Arcadia_Integer64Value)(product >> 64);
  return (Arcadia_Integer64Value)product;
}

#else
  #error("environment not (yet) supported")
#endif

Arcadia_BooleanValue
Arcadia_safeMultiplyInteger64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value multiplier,
    Arcadia_Integer64Value multiplicand,
    Arcadia_Integer64Value* upperProduct,
    Arcadia_Integer64Value* lowerProduct
  )
{
  Arcadia_Integer64Value hi, lo;
  lo = __imul128(multiplier, multiplicand, &hi);
  if (upperProduct) {
    *upperProduct = hi;
  }
  if (lowerProduct) {
    *lowerProduct = lo;
  }
  if ((lo < 0 && hi != -1) || (lo >= 0 && hi != 0)) {
    return Arcadia_BooleanValue_True;
  } else {
    return Arcadia_BooleanValue_False;
  }
}

Arcadia_BooleanValue
Arcadia_safeMultiplyInteger8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value multiplier,
    Arcadia_Integer8Value multiplicand,
    Arcadia_Integer8Value* upperProduct,
    Arcadia_Integer8Value* lowerProduct
  )
{
  Arcadia_Integer16Value product = ((Arcadia_Integer16Value)multiplier) * ((Arcadia_Integer16Value)multiplicand);
  Arcadia_Integer8Value hi = (Arcadia_Integer8Value)((product & 0xff00) >> 8);
  Arcadia_Integer8Value lo = (Arcadia_Integer8Value)((product & 0x00ff) >> 0);
  if (upperProduct) {
    *upperProduct = hi;
  }
  if (lowerProduct) {
    *lowerProduct = lo;
  }
  if ((lo < 0 && hi != -1) || (lo >= 0 && hi != 0)) {
    return Arcadia_BooleanValue_True;
  } else {
    return Arcadia_BooleanValue_False;
  }
}
