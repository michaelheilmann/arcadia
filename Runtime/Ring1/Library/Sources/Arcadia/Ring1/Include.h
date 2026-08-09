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

#if !defined(ARCADIA_RING1_INCLUDE_H_INCLUDED)
#define ARCADIA_RING1_INCLUDE_H_INCLUDED

// If a file x belongs to a module a and ARCADIA_a_MODULE is not defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_RING1_MODULE")
#define ARCADIA_RING1_MODULE (1)

// If a file x of a module a is not an export file of that module and ARCADIA_a_EXPORT is defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_RING1_EXPORT")
#define ARCADIA_RING1_EXPORT (1)

#include "Arcadia/Ring1/Implementation/Arrays.h"

#include "Arcadia/Ring1/Implementation/Atoms.h"

#include "Arcadia/Ring1/Implementation/BigInteger/Include.h"

#include "Arcadia/Ring1/Implementation/Boolean.h"

#include "Arcadia/Ring1/Implementation/Diagnostics.h"

#include "Arcadia/Ring1/Implementation/Enumeration.h"

#include "Arcadia/Ring1/Implementation/ForeignProcedure.h"

#include "Arcadia/Ring1/Implementation/getTickCount.h"

#include "Arcadia/Ring1/Implementation/ImmutableByteArray.h"

#include "Arcadia/Ring1/Implementation/Unicode/UTF8/toUpperASCII.h"
#include "Arcadia/Ring1/Implementation/Unicode/UTF8/toLowerASCII.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8StringExtensions.h"

#include "Arcadia/Ring1/Implementation/ImmutableByteArray.h"

#include "Arcadia/Ring1/Implementation/Integer16.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
#include "Arcadia/Ring1/Implementation/Integer64.h"
#include "Arcadia/Ring1/Implementation/Integer8.h"

#include "Arcadia/Ring1/Implementation/Unicode.h"

#include "Arcadia/Ring1/Implementation/makeBitmask.h"

#include "Arcadia/Ring1/Implementation/Memory.h"

#include "Arcadia/Ring1/Implementation/Natural16.h"
#include "Arcadia/Ring1/Implementation/Natural32.h"
#include "Arcadia/Ring1/Implementation/Natural64.h"
#include "Arcadia/Ring1/Implementation/Natural8.h"

#include "Arcadia/Ring1/Implementation/Annotations/Likely.h"
#include "Arcadia/Ring1/Implementation/Annotations/NoReturn.h"
#include "Arcadia/Ring1/Implementation/Annotations/ThreadLocal.h"
#include "Arcadia/Ring1/Implementation/Annotations/Unlikely.h"

#include "Arcadia/Ring1/Implementation/NumberLiteral.h"

#include "Arcadia/Ring1/Implementation/Numerics/Include.h"

#include "Arcadia/Ring1/Implementation/Object.h"

#include "Arcadia/Ring1/Implementation/ObjectReference.h"

#include "Arcadia/Ring1/Implementation/Process.h"
#include "Arcadia/Ring1/Implementation/ProcessExtensions.h"

#include "Arcadia/Ring1/Implementation/Real32.h"
#include "Arcadia/Ring1/Implementation/Real64.h"
#include "Arcadia/Ring1/Implementation/Real32_getBits.h"
#include "Arcadia/Ring1/Implementation/Real64_getBits.h"
#include "Arcadia/Ring1/Implementation/Real32_isFinite.h"
#include "Arcadia/Ring1/Implementation/Real64_isFinite.h"

#include "Arcadia/Ring1/Implementation/safeAdd.h"
#include "Arcadia/Ring1/Implementation/safeMultiply.h"

#include "Arcadia/Ring1/Implementation/Size.h"

#include "Arcadia/Ring1/Implementation/StaticAssert.h"

#include "Arcadia/Ring1/Implementation/Status.h"

#include "Arcadia/Ring1/Implementation/Thread.h"
#include "Arcadia/Ring1/Implementation/ThreadExtensions.h"

#include "Arcadia/Ring1/Implementation/swap.h"

#include "Arcadia/Ring1/Implementation/Tests.h"

#include "Arcadia/Ring1/Implementation/RealToString/Include.h"

#include "Arcadia/Ring1/Implementation/Signals/Include.h"

#include "Arcadia/Ring1/Implementation/StringToInteger/Include.h"
#include "Arcadia/Ring1/Implementation/StringToNatural/Include.h"
#include "Arcadia/Ring1/Implementation/StringToReal/toReal32.h"
#include "Arcadia/Ring1/Implementation/StringToReal/toReal64.h"

#include "Arcadia/Ring1/Implementation/TypeSystem/Include.h"

#include "Arcadia/Ring1/Implementation/Value.h"

#include "Arcadia/Ring1/Implementation/Void.h"

#include "Arcadia/Ring1/Implementation/Objects/Include.h"

#undef ARCADIA_RING1_EXPORT
#pragma pop_macro("ARCADIA_RING1_EXPORT")

#undef ARCADIA_RING1_MODULE
#pragma pop_macro("ARCADIA_RING1_MODULE")

#endif // ARCADIA_RING1_INCLUDE_H_INCLUDED
