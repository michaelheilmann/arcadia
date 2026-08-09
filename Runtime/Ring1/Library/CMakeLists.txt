# Arcadia
# Copyright (C) 2024-2026 Michael Heilmann
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU Affero General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
# details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.29)

set(this ${MyProjectName}.Ring1)

# Create library.
if (NOT TARGET ${this})
  BeginProduct(${this} library)

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Ring1/Implementation/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Ring1/Implementation/Configure.h.i)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Random.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Random.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/add.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/add.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/and.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/and.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/BigInteger.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/BigInteger.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/divide.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/divide.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/compareByMagnitudeTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/compareByMagnitudeTo.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/compareTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/compareTo.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/countSignificandBits.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/countSignificandBits.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/equalTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/equalTo.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/fromDecimalString.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/fromDecimalString.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/fromTwosComplement.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/fromTwosComplement.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/getBitLength.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/getBitLength.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/greaterThan.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/greaterThan.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/greaterThanOrEqualTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/greaterThanOrEqualTo.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/getHigh64.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/getHigh64.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/lowerThan.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/lowerThan.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/lowerThanOrEqualTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/lowerThanOrEqualTo.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/multiply.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/multiply.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/notEqualTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/notEqualTo.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/or.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/or.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/setNatural.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/setNatural.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/setInteger.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/setInteger.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/setPowerOfFive.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/setPowerOfFive.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/setPowerOfTen.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/setPowerOfTen.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/setPowerOfTwo.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/setPowerOfTwo.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/shiftLeft.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/shiftLeft.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/shiftRight.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/shiftRight.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/subtract.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/subtract.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/toDecimalString.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/toDecimalString.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/toNatural.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/toNatural.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/toInteger.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/toInteger.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/BigInteger/toTwosComplement.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BigInteger/toTwosComplement.h)

    # >> Signals

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Signals/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Signals/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Signals/Signal.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Signals/Signal.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Signals/Slot.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Signals/Slot.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Signals/Internal.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Signals/Internal.h)

  # << Signals

  # >> String to number

  OnSourceFile(${this} Arcadia/Ring1/Implementation/StringToInteger/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/StringToInteger/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/StringToNatural/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/StringToNatural/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/StringToReal/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/StringToReal/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/StringToReal/toReal64.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/StringToReal/toReal64.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/StringToReal/toReal32.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/StringToReal/toReal32.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/StringToReal/Lemire.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/StringToReal/Lemire.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/StringToReal/Gay.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/StringToReal/Gay.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/StringToReal/Clinger.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/StringToReal/Clinger.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/StringToReal/Result.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/StringToReal/Result.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/RealToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/RealToString/Include.h)

  ## << String to number

  ## >> Number to string

  OnSourceFile(${this} Arcadia/Ring1/Implementation/RealToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/RealToString/Include.h)
  OnInlayFile(${this} Arcadia/Ring1/Implementation/RealToString/Common.i)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/RealToString/Real32ToString.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/RealToString/Real32ToString.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/RealToString/Real64ToString.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/RealToString/Real64ToString.h)
  OnInlayFile(${this} Arcadia/Ring1/Implementation/RealToString/BigTablesReal64.i)
  OnInlayFile(${this} Arcadia/Ring1/Implementation/RealToString/SmallTablesReal64.i)
  OnInlayFile(${this} Arcadia/Ring1/Implementation/RealToString/TablesReal32.i)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/BooleanToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/BooleanToString/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/IntegerToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/IntegerToString/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/NaturalToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/NaturalToString/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/SizeToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/SizeToString/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/VoidToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/VoidToString/Include.h)

  ## << Number to string

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Arrays.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Arrays.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Arrays.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Arrays.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/safeAdd.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/safeAdd.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/safeMultiply.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/safeMultiply.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Atoms.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Atoms.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Atoms.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Atoms.module.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/makeBitmask.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/makeBitmask.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Boolean.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Boolean.h)

  # Numerics >>

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/ceil.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/ceil.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/clamp.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/clamp.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/cos.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/cos.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/countLeadingZeroes.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/countLeadingZeroes.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/countSignificandBits.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/countSignificandBits.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/countTrailingZeroes.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/countTrailingZeroes.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/floor.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/floor.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/hash.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/hash.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/Include.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/isPowerOfTwo.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/isPowerOfTwo.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/maximum.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/maximum.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/minimum.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/minimum.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/nextPowerOfTwoGreaterThan.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/nextPowerOfTwoGreaterThan.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/nextPowerOfTwoGreaterThanOrEqualTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/nextPowerOfTwoGreaterThanOrEqualTo.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/quotientRemainder.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/quotientRemainder.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/sin.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/sin.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/sqrt.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/sqrt.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/tan.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/tan.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Numerics/isDivisibleByPowerOfFive.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Numerics/isDivisibleByPowerOfFive.h)

  # << Numerics

  OnHeaderFile(${this} Arcadia/Ring1/Implementation/_declareScalarType.h)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/_defineScalarType.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Diagnostics.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Diagnostics.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Enumeration.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Enumeration.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/ForeignProcedure.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ForeignProcedure.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/getNumberOfCores.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/getNumberOfCores.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/getPageSize.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/getPageSize.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/getTickCount.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/getTickCount.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableByteArray.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableByteArray.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8StringExtensions.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8StringExtensions.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/fromBytes.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/fromBytes.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/hash.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/hash.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/NumeralParser.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/NumeralParser.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/type.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/type.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toBoolean.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toBoolean.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toInteger.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toInteger.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toNatural.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toNatural.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toReal.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toReal.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toSize.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toSize.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toVoid.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ImmutableUTF8String/toVoid.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Integer16.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Integer16.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Integer32.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Integer32.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Integer64.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Integer64.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Integer8.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Integer8.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Natural16.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Natural16.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Natural32.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Natural32.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Natural64.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Natural64.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Natural8.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Natural8.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Annotations/NoReturn.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Annotations/NoReturn.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Annotations/Likely.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Annotations/Likely.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Annotations/ThreadLocal.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Annotations/ThreadLocal.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Annotations/Unlikely.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Annotations/Unlikely.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Object.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Object.h)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Object.internal.h)
  OnInlayFile(${this} Arcadia/Ring1/Implementation/Object/TypeFunctions.i)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/ObjectReference.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ObjectReference.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Process.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Process.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ProcessExtensions.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ProcessExtensions.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Real32.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Real32.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Real32_getBits.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Real32_getBits.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Real32_isFinite.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Real32_isFinite.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Real64.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Real64.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Real64_getBits.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Real64_getBits.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Real64_isFinite.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Real64_isFinite.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Size.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Size.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/StaticAssert.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/StaticAssert.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Status.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Status.h)

  OnHeaderFile(${this} Arcadia/Ring1/Implementation/swap.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/swap.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Tests.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Tests.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Thread.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Thread.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Thread.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Thread.module.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/ThreadExtensions.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/ThreadExtensions.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Unicode.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Unicode.h)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Unicode/Unicode.h)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Unicode/UTF8.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Unicode/isUTF8.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Unicode/isUTF8.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Unicode/UTF8ArrayIterator.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Unicode/UTF8ArrayIterator.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Unicode/UTF8/toLowerASCII.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Unicode/UTF8/toLowerASCII.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Unicode/UTF8/toUpperASCII.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Unicode/UTF8/toUpperASCII.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Unicode/UTF8/mapASCIIToASCII.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Unicode/UTF8/mapASCIIToASCII.h)


  OnSourceFile(${this} Arcadia/Ring1/Implementation/NumberLiteral.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/NumberLiteral.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Void.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Void.h)

  OnSourceFile(${this} Arcadia/Ring1/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Include.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Value.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Value.h)

  # Objects >>

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/Include.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArray.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArray.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArrayDefaultImpl.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArrayDefaultImpl.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArraySliceImpl.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArraySliceImpl.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArray.ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArray.ByteReader.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArrayBuilder.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArrayBuilder.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArrayBuilder.ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/ByteArrayBuilder.ByteReader.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/ByteReader.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/ByteReader.UnicodeCodePointReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/ByteReader.UnicodeCodePointReader.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/UnicodeCodePointReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/UnicodeCodePointReader.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/ByteReader.UnicodeCodePointReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/ByteReader.UnicodeCodePointReader.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/RuntimeByteArray.ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/RuntimeByteArray.ByteReader.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/String.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/String.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/String.ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/String.ByteReader.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/StringBuilder.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/StringBuilder.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/StringBuilder.ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/StringBuilder.ByteReader.h)
  
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Objects/WeakReference.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Objects/WeakReference.h)

  # << Objects

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Memory.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Memory.h)

  # Type System >>

  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Names.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Names.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Types.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Types.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/TypeKind.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/TypeKind.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Type.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Type.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/TypeNameParser.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/TypeNameParser.module.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Types.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Types.module.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Names.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/Names.module.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/TypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/TypeNode.module.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/EnumerationTypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/EnumerationTypeNode.module.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/InterfaceTypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/InterfaceTypeNode.module.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/InternalTypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/InternalTypeNode.module.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/ObjectTypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/ObjectTypeNode.module.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/TypeSystem/ScalarTypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/TypeSystem/ScalarTypeNode.module.h)

  # << Type System

  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Module.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Concurrency/Mutex.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Concurrency/Mutex.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Concurrency/Condition.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Concurrency/Condition.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Concurrency/CompareAndSwap.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Concurrency/CompareAndSwap.h)

  OnSourceFile(${this} Arcadia/Ring1/Implementation/Network/AddressFamily.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Network/AddressFamily.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Network/Protocol.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Network/Protocol.h)
  OnSourceFile(${this} Arcadia/Ring1/Implementation/Network/SocketType.c)
  OnHeaderFile(${this} Arcadia/Ring1/Implementation/Network/SocketType.h)

  OnModuleDependency(${this} ${MyProjectName}.ARMS)

  if (${${this}_Compiler_ASM} STREQUAL ${${this}_Compiler_ASM_MASM})
	  if (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X64})
      elseif (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X86})
	  endif()
	  if (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X64})
      elseif (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X86})
	  endif()
  endif()

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})

    # link "math" library
	list(APPEND ${this}.Libraries m)

	# link "pthread" library
	set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
	set(THREADS_PREFER_PTHREAD_FLAG TRUE)
	find_package(Threads REQUIRED)
	list(APPEND ${this}.Libraries Threads::Threads)

  endif()

  set(${this}.Install TRUE)

  EndProduct(${this})

  set_target_properties(${this} PROPERTIES FOLDER "Runtime/Ring1")

endif()
