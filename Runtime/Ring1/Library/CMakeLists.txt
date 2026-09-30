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

  SetFolder(${this} "Runtime/Ring1")

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Ring1/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Ring1/Configure.h.i)

  OnSourceFile(${this} Arcadia/Ring1/Random.c)
  OnHeaderFile(${this} Arcadia/Ring1/Random.h)

  OnSourceFile(${this} Arcadia/Ring1/BigInteger/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/add.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/add.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/and.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/and.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/BigInteger.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/BigInteger.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/divide.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/divide.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/compareByMagnitudeTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/compareByMagnitudeTo.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/compareTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/compareTo.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/countSignificandBits.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/countSignificandBits.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/equalTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/equalTo.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/fromDecimalString.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/fromDecimalString.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/fromTwosComplement.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/fromTwosComplement.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/getBitLength.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/getBitLength.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/greaterThan.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/greaterThan.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/greaterThanOrEqualTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/greaterThanOrEqualTo.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/getHigh64.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/getHigh64.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/lowerThan.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/lowerThan.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/lowerThanOrEqualTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/lowerThanOrEqualTo.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/multiply.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/multiply.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/notEqualTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/notEqualTo.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/or.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/or.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/setNatural.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/setNatural.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/setInteger.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/setInteger.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/setPowerOfFive.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/setPowerOfFive.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/setPowerOfTen.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/setPowerOfTen.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/setPowerOfTwo.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/setPowerOfTwo.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/shiftLeft.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/shiftLeft.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/shiftRight.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/shiftRight.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/subtract.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/subtract.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/toDecimalString.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/toDecimalString.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/toNatural.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/toNatural.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/toInteger.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/toInteger.h)
  OnSourceFile(${this} Arcadia/Ring1/BigInteger/toTwosComplement.c)
  OnHeaderFile(${this} Arcadia/Ring1/BigInteger/toTwosComplement.h)

    # >> Signals

  OnSourceFile(${this} Arcadia/Ring1/Signals/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Signals/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/Signals/Signal.c)
  OnHeaderFile(${this} Arcadia/Ring1/Signals/Signal.h)
  OnSourceFile(${this} Arcadia/Ring1/Signals/Slot.c)
  OnHeaderFile(${this} Arcadia/Ring1/Signals/Slot.h)
  OnSourceFile(${this} Arcadia/Ring1/Signals/Internal.c)
  OnHeaderFile(${this} Arcadia/Ring1/Signals/Internal.h)

  # << Signals

  # >> String to number

  OnSourceFile(${this} Arcadia/Ring1/StringToInteger/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/StringToInteger/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/StringToNatural/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/StringToNatural/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/StringToReal/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/StringToReal/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/StringToReal/toReal64.c)
  OnHeaderFile(${this} Arcadia/Ring1/StringToReal/toReal64.h)
  OnSourceFile(${this} Arcadia/Ring1/StringToReal/toReal32.c)
  OnHeaderFile(${this} Arcadia/Ring1/StringToReal/toReal32.h)
  OnSourceFile(${this} Arcadia/Ring1/StringToReal/Lemire.c)
  OnHeaderFile(${this} Arcadia/Ring1/StringToReal/Lemire.h)
  OnSourceFile(${this} Arcadia/Ring1/StringToReal/Gay.c)
  OnHeaderFile(${this} Arcadia/Ring1/StringToReal/Gay.h)
  OnSourceFile(${this} Arcadia/Ring1/StringToReal/Clinger.c)
  OnHeaderFile(${this} Arcadia/Ring1/StringToReal/Clinger.h)
  OnSourceFile(${this} Arcadia/Ring1/StringToReal/Result.c)
  OnHeaderFile(${this} Arcadia/Ring1/StringToReal/Result.h)
  OnSourceFile(${this} Arcadia/Ring1/RealToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/RealToString/Include.h)

  ## << String to number

  ## >> Number to string

  OnSourceFile(${this} Arcadia/Ring1/RealToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/RealToString/Include.h)
  OnInlayFile(${this} Arcadia/Ring1/RealToString/Common.i)
  OnSourceFile(${this} Arcadia/Ring1/RealToString/Real32ToString.c)
  OnHeaderFile(${this} Arcadia/Ring1/RealToString/Real32ToString.h)
  OnSourceFile(${this} Arcadia/Ring1/RealToString/Real64ToString.c)
  OnHeaderFile(${this} Arcadia/Ring1/RealToString/Real64ToString.h)
  OnInlayFile(${this} Arcadia/Ring1/RealToString/BigTablesReal64.i)
  OnInlayFile(${this} Arcadia/Ring1/RealToString/SmallTablesReal64.i)
  OnInlayFile(${this} Arcadia/Ring1/RealToString/TablesReal32.i)

  OnSourceFile(${this} Arcadia/Ring1/BooleanToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/BooleanToString/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/IntegerToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/IntegerToString/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/NaturalToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/NaturalToString/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/SizeToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/SizeToString/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/VoidToString/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/VoidToString/Include.h)

  ## << Number to string

  OnSourceFile(${this} Arcadia/Ring1/Arrays.c)
  OnHeaderFile(${this} Arcadia/Ring1/Arrays.h)

  OnSourceFile(${this} Arcadia/Ring1/Arrays.c)
  OnHeaderFile(${this} Arcadia/Ring1/Arrays.h)

  OnSourceFile(${this} Arcadia/Ring1/safeAdd.c)
  OnHeaderFile(${this} Arcadia/Ring1/safeAdd.h)
  OnSourceFile(${this} Arcadia/Ring1/safeMultiply.c)
  OnHeaderFile(${this} Arcadia/Ring1/safeMultiply.h)

  OnSourceFile(${this} Arcadia/Ring1/Atoms.c)
  OnHeaderFile(${this} Arcadia/Ring1/Atoms.h)
  OnSourceFile(${this} Arcadia/Ring1/Atoms.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Atoms.module.h)

  OnSourceFile(${this} Arcadia/Ring1/makeBitmask.c)
  OnHeaderFile(${this} Arcadia/Ring1/makeBitmask.h)

  OnSourceFile(${this} Arcadia/Ring1/Boolean.c)
  OnHeaderFile(${this} Arcadia/Ring1/Boolean.h)

  # Numerics >>

  OnSourceFile(${this} Arcadia/Ring1/Numerics/ceil.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/ceil.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/clamp.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/clamp.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/cos.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/cos.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/countLeadingZeroes.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/countLeadingZeroes.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/countSignificandBits.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/countSignificandBits.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/countTrailingZeroes.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/countTrailingZeroes.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/floor.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/floor.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/hash.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/hash.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/Include.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/isPowerOfTwo.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/isPowerOfTwo.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/maximum.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/maximum.h)
  OnSourceFile(${this} Arcadia/Ring1/Numerics/minimum.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/minimum.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/nextPowerOfTwoGreaterThan.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/nextPowerOfTwoGreaterThan.h)
  OnSourceFile(${this} Arcadia/Ring1/Numerics/nextPowerOfTwoGreaterThanOrEqualTo.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/nextPowerOfTwoGreaterThanOrEqualTo.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/quotientRemainder.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/quotientRemainder.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/sin.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/sin.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/sqrt.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/sqrt.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/tan.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/tan.h)

  OnSourceFile(${this} Arcadia/Ring1/Numerics/isDivisibleByPowerOfFive.c)
  OnHeaderFile(${this} Arcadia/Ring1/Numerics/isDivisibleByPowerOfFive.h)

  # << Numerics

  OnHeaderFile(${this} Arcadia/Ring1/_declareScalarType.h)
  OnHeaderFile(${this} Arcadia/Ring1/_defineScalarType.h)

  OnSourceFile(${this} Arcadia/Ring1/Diagnostics.c)
  OnHeaderFile(${this} Arcadia/Ring1/Diagnostics.h)

  OnSourceFile(${this} Arcadia/Ring1/Enumeration.c)
  OnHeaderFile(${this} Arcadia/Ring1/Enumeration.h)

  OnSourceFile(${this} Arcadia/Ring1/ForeignProcedure.c)
  OnHeaderFile(${this} Arcadia/Ring1/ForeignProcedure.h)

  OnSourceFile(${this} Arcadia/Ring1/getNumberOfCores.c)
  OnHeaderFile(${this} Arcadia/Ring1/getNumberOfCores.h)
  OnSourceFile(${this} Arcadia/Ring1/getPageSize.c)
  OnHeaderFile(${this} Arcadia/Ring1/getPageSize.h)

  OnSourceFile(${this} Arcadia/Ring1/getTickCount.c)
  OnHeaderFile(${this} Arcadia/Ring1/getTickCount.h)

  OnSourceFile(${this} Arcadia/Ring1/ImmutableByteArray.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableByteArray.h)

  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8String.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8String.h)
  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8StringExtensions.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8StringExtensions.h)
  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8String/fromBytes.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8String/fromBytes.h)
  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8String/hash.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8String/hash.h)
  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8String/NumeralParser.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8String/NumeralParser.h)
  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8String/type.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8String/type.h)

  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8String/toBoolean.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8String/toBoolean.h)
  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8String/toInteger.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8String/toInteger.h)
  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8String/toNatural.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8String/toNatural.h)
  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8String/toReal.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8String/toReal.h)
  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8String/toSize.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8String/toSize.h)
  OnSourceFile(${this} Arcadia/Ring1/ImmutableUTF8String/toVoid.c)
  OnHeaderFile(${this} Arcadia/Ring1/ImmutableUTF8String/toVoid.h)

  OnSourceFile(${this} Arcadia/Ring1/Integer16.c)
  OnHeaderFile(${this} Arcadia/Ring1/Integer16.h)
  OnSourceFile(${this} Arcadia/Ring1/Integer32.c)
  OnHeaderFile(${this} Arcadia/Ring1/Integer32.h)
  OnSourceFile(${this} Arcadia/Ring1/Integer64.c)
  OnHeaderFile(${this} Arcadia/Ring1/Integer64.h)
  OnSourceFile(${this} Arcadia/Ring1/Integer8.c)
  OnHeaderFile(${this} Arcadia/Ring1/Integer8.h)

  OnSourceFile(${this} Arcadia/Ring1/Natural16.c)
  OnHeaderFile(${this} Arcadia/Ring1/Natural16.h)
  OnSourceFile(${this} Arcadia/Ring1/Natural32.c)
  OnHeaderFile(${this} Arcadia/Ring1/Natural32.h)
  OnSourceFile(${this} Arcadia/Ring1/Natural64.c)
  OnHeaderFile(${this} Arcadia/Ring1/Natural64.h)
  OnSourceFile(${this} Arcadia/Ring1/Natural8.c)
  OnHeaderFile(${this} Arcadia/Ring1/Natural8.h)

  OnSourceFile(${this} Arcadia/Ring1/Annotations/NoReturn.c)
  OnHeaderFile(${this} Arcadia/Ring1/Annotations/NoReturn.h)
  OnSourceFile(${this} Arcadia/Ring1/Annotations/Likely.c)
  OnHeaderFile(${this} Arcadia/Ring1/Annotations/Likely.h)
  OnSourceFile(${this} Arcadia/Ring1/Annotations/ThreadLocal.c)
  OnHeaderFile(${this} Arcadia/Ring1/Annotations/ThreadLocal.h)
  OnSourceFile(${this} Arcadia/Ring1/Annotations/Unlikely.c)
  OnHeaderFile(${this} Arcadia/Ring1/Annotations/Unlikely.h)

  OnSourceFile(${this} Arcadia/Ring1/Object.c)
  OnHeaderFile(${this} Arcadia/Ring1/Object.h)
  OnHeaderFile(${this} Arcadia/Ring1/Object.internal.h)
  OnInlayFile(${this} Arcadia/Ring1/Object/TypeFunctions.i)

  OnSourceFile(${this} Arcadia/Ring1/ObjectReference.c)
  OnHeaderFile(${this} Arcadia/Ring1/ObjectReference.h)

  OnSourceFile(${this} Arcadia/Ring1/Process.c)
  OnHeaderFile(${this} Arcadia/Ring1/Process.h)
  OnSourceFile(${this} Arcadia/Ring1/ProcessExtensions.c)
  OnHeaderFile(${this} Arcadia/Ring1/ProcessExtensions.h)

  OnSourceFile(${this} Arcadia/Ring1/Real32.c)
  OnHeaderFile(${this} Arcadia/Ring1/Real32.h)
  OnSourceFile(${this} Arcadia/Ring1/Real32_getBits.c)
  OnHeaderFile(${this} Arcadia/Ring1/Real32_getBits.h)
  OnSourceFile(${this} Arcadia/Ring1/Real32_isFinite.c)
  OnHeaderFile(${this} Arcadia/Ring1/Real32_isFinite.h)
  OnSourceFile(${this} Arcadia/Ring1/Real64.c)
  OnHeaderFile(${this} Arcadia/Ring1/Real64.h)
  OnSourceFile(${this} Arcadia/Ring1/Real64_getBits.c)
  OnHeaderFile(${this} Arcadia/Ring1/Real64_getBits.h)
  OnSourceFile(${this} Arcadia/Ring1/Real64_isFinite.c)
  OnHeaderFile(${this} Arcadia/Ring1/Real64_isFinite.h)

  OnSourceFile(${this} Arcadia/Ring1/Size.c)
  OnHeaderFile(${this} Arcadia/Ring1/Size.h)

  OnSourceFile(${this} Arcadia/Ring1/StaticAssert.c)
  OnHeaderFile(${this} Arcadia/Ring1/StaticAssert.h)

  OnSourceFile(${this} Arcadia/Ring1/Status.c)
  OnHeaderFile(${this} Arcadia/Ring1/Status.h)

  OnHeaderFile(${this} Arcadia/Ring1/swap.c)
  OnHeaderFile(${this} Arcadia/Ring1/swap.h)

  OnSourceFile(${this} Arcadia/Ring1/Tests.c)
  OnHeaderFile(${this} Arcadia/Ring1/Tests.h)

  OnSourceFile(${this} Arcadia/Ring1/Thread.c)
  OnHeaderFile(${this} Arcadia/Ring1/Thread.h)
  OnSourceFile(${this} Arcadia/Ring1/Thread.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/Thread.module.h)
  OnSourceFile(${this} Arcadia/Ring1/ThreadExtensions.c)
  OnHeaderFile(${this} Arcadia/Ring1/ThreadExtensions.h)

  OnSourceFile(${this} Arcadia/Ring1/Unicode.c)
  OnHeaderFile(${this} Arcadia/Ring1/Unicode.h)
  OnHeaderFile(${this} Arcadia/Ring1/Unicode/Unicode.h)
  OnHeaderFile(${this} Arcadia/Ring1/Unicode/UTF8.h)

  OnSourceFile(${this} Arcadia/Ring1/Unicode/isUTF8.c)
  OnHeaderFile(${this} Arcadia/Ring1/Unicode/isUTF8.h)
  OnSourceFile(${this} Arcadia/Ring1/Unicode/UTF8ArrayIterator.c)
  OnHeaderFile(${this} Arcadia/Ring1/Unicode/UTF8ArrayIterator.h)
  OnSourceFile(${this} Arcadia/Ring1/Unicode/UTF8/toLowerASCII.c)
  OnHeaderFile(${this} Arcadia/Ring1/Unicode/UTF8/toLowerASCII.h)
  OnSourceFile(${this} Arcadia/Ring1/Unicode/UTF8/toUpperASCII.c)
  OnHeaderFile(${this} Arcadia/Ring1/Unicode/UTF8/toUpperASCII.h)
  OnSourceFile(${this} Arcadia/Ring1/Unicode/UTF8/mapASCIIToASCII.c)
  OnHeaderFile(${this} Arcadia/Ring1/Unicode/UTF8/mapASCIIToASCII.h)


  OnSourceFile(${this} Arcadia/Ring1/NumberLiteral.c)
  OnHeaderFile(${this} Arcadia/Ring1/NumberLiteral.h)

  OnSourceFile(${this} Arcadia/Ring1/Void.c)
  OnHeaderFile(${this} Arcadia/Ring1/Void.h)

  OnSourceFile(${this} Arcadia/Ring1/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Include.h)

  OnSourceFile(${this} Arcadia/Ring1/Value.c)
  OnHeaderFile(${this} Arcadia/Ring1/Value.h)

  # Objects >>

  OnSourceFile(${this} Arcadia/Ring1/Objects/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/Include.h)

  OnSourceFile(${this} Arcadia/Ring1/Objects/ByteArray.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/ByteArray.h)
  OnSourceFile(${this} Arcadia/Ring1/Objects/ByteArrayDefaultImpl.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/ByteArrayDefaultImpl.h)
  OnSourceFile(${this} Arcadia/Ring1/Objects/ByteArraySliceImpl.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/ByteArraySliceImpl.h)

  OnSourceFile(${this} Arcadia/Ring1/Objects/ByteArray.ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/ByteArray.ByteReader.h)
  OnSourceFile(${this} Arcadia/Ring1/Objects/ByteArrayBuilder.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/ByteArrayBuilder.h)
  OnSourceFile(${this} Arcadia/Ring1/Objects/ByteArrayBuilder.ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/ByteArrayBuilder.ByteReader.h)
  OnSourceFile(${this} Arcadia/Ring1/Objects/ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/ByteReader.h)
  OnSourceFile(${this} Arcadia/Ring1/Objects/ByteReader.UnicodeCodePointReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/ByteReader.UnicodeCodePointReader.h)

  OnSourceFile(${this} Arcadia/Ring1/Objects/UnicodeCodePointReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/UnicodeCodePointReader.h)
  OnSourceFile(${this} Arcadia/Ring1/Objects/UnicodeCodePointReader.State.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/UnicodeCodePointReader.State.h)

  OnSourceFile(${this} Arcadia/Ring1/Objects/ByteReader.UnicodeCodePointReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/ByteReader.UnicodeCodePointReader.h)

  OnSourceFile(${this} Arcadia/Ring1/Objects/RuntimeByteArray.ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/RuntimeByteArray.ByteReader.h)

  OnSourceFile(${this} Arcadia/Ring1/Objects/String.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/String.h)
  OnSourceFile(${this} Arcadia/Ring1/Objects/String.ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/String.ByteReader.h)
  OnSourceFile(${this} Arcadia/Ring1/Objects/StringBuilder.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/StringBuilder.h)
  OnSourceFile(${this} Arcadia/Ring1/Objects/StringBuilder.ByteReader.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/StringBuilder.ByteReader.h)
  
  OnSourceFile(${this} Arcadia/Ring1/Objects/WeakReference.c)
  OnHeaderFile(${this} Arcadia/Ring1/Objects/WeakReference.h)

  # << Objects

  OnSourceFile(${this} Arcadia/Ring1/Memory.c)
  OnHeaderFile(${this} Arcadia/Ring1/Memory.h)

  # Type System >>

  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/Include.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/Include.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/Names.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/Names.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/Types.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/Types.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/TypeKind.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/TypeKind.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/Type.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/Type.h)

  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/TypeNameParser.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/TypeNameParser.module.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/Types.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/Types.module.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/Names.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/Names.module.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/TypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/TypeNode.module.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/EnumerationTypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/EnumerationTypeNode.module.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/InterfaceTypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/InterfaceTypeNode.module.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/InternalTypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/InternalTypeNode.module.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/ObjectTypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/ObjectTypeNode.module.h)
  OnSourceFile(${this} Arcadia/Ring1/TypeSystem/ScalarTypeNode.module.c)
  OnHeaderFile(${this} Arcadia/Ring1/TypeSystem/ScalarTypeNode.module.h)

  # << Type System

  OnHeaderFile(${this} Arcadia/Ring1/Module.h)

  OnSourceFile(${this} Arcadia/Ring1/Concurrency/Mutex.c)
  OnHeaderFile(${this} Arcadia/Ring1/Concurrency/Mutex.h)
  OnSourceFile(${this} Arcadia/Ring1/Concurrency/Condition.c)
  OnHeaderFile(${this} Arcadia/Ring1/Concurrency/Condition.h)
  OnSourceFile(${this} Arcadia/Ring1/Concurrency/CompareAndSwap.c)
  OnHeaderFile(${this} Arcadia/Ring1/Concurrency/CompareAndSwap.h)

  OnSourceFile(${this} Arcadia/Ring1/Network/AddressFamily.c)
  OnHeaderFile(${this} Arcadia/Ring1/Network/AddressFamily.h)
  OnSourceFile(${this} Arcadia/Ring1/Network/Protocol.c)
  OnHeaderFile(${this} Arcadia/Ring1/Network/Protocol.h)
  OnSourceFile(${this} Arcadia/Ring1/Network/SocketType.c)
  OnHeaderFile(${this} Arcadia/Ring1/Network/SocketType.h)

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

endif()
