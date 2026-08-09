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

add_subdirectory(BigIntegerTests)
add_subdirectory(ClampTests)
add_subdirectory(CountLeadingZeroesTests)
add_subdirectory(CountTrailingZeroesTests)
add_subdirectory(MaximumTests)
add_subdirectory(MinimumTests)
add_subdirectory(RealTypesTests)

add_subdirectory(IntegerToStringTests)
add_subdirectory(NaturalToStringTests)
add_subdirectory(RealToStringTests)

add_subdirectory(SafeAddTests)
add_subdirectory(SafeMultiplyTests)
add_subdirectory(StringToIntegerTests)
add_subdirectory(StringToNaturalTests)
add_subdirectory(StringToRealTests)
add_subdirectory(NextPowerOfTwoTests)

add_subdirectory(ObjectTypeTests)
add_subdirectory(WeakReferenceTests)

add_subdirectory(UTF8ArrayIteratorTests)
add_subdirectory(SubStringTests)

add_subdirectory(UnicodeCodePointReaderTests)
