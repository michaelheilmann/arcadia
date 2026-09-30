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

#include "Arcadia/Ring1/Arrays.h"

#include "Arcadia/Ring1/Atoms.h"

#include "Arcadia/Ring1/BigInteger/Include.h"

#include "Arcadia/Ring1/Boolean.h"

#include "Arcadia/Ring1/Diagnostics.h"

#include "Arcadia/Ring1/Enumeration.h"

#include "Arcadia/Ring1/ForeignProcedure.h"

#include "Arcadia/Ring1/getTickCount.h"

#include "Arcadia/Ring1/ImmutableByteArray.h"

#include "Arcadia/Ring1/Unicode/UTF8/toUpperASCII.h"
#include "Arcadia/Ring1/Unicode/UTF8/toLowerASCII.h"
#include "Arcadia/Ring1/ImmutableUTF8String.h"
#include "Arcadia/Ring1/ImmutableUTF8StringExtensions.h"

#include "Arcadia/Ring1/ImmutableByteArray.h"

#include "Arcadia/Ring1/Integer16.h"
#include "Arcadia/Ring1/Integer32.h"
#include "Arcadia/Ring1/Integer64.h"
#include "Arcadia/Ring1/Integer8.h"

#include "Arcadia/Ring1/Unicode.h"

#include "Arcadia/Ring1/makeBitmask.h"

#include "Arcadia/Ring1/Memory.h"

#include "Arcadia/Ring1/Natural16.h"
#include "Arcadia/Ring1/Natural32.h"
#include "Arcadia/Ring1/Natural64.h"
#include "Arcadia/Ring1/Natural8.h"

#include "Arcadia/Ring1/Annotations/Likely.h"
#include "Arcadia/Ring1/Annotations/NoReturn.h"
#include "Arcadia/Ring1/Annotations/ThreadLocal.h"
#include "Arcadia/Ring1/Annotations/Unlikely.h"

#include "Arcadia/Ring1/NumberLiteral.h"

#include "Arcadia/Ring1/Numerics/Include.h"

#include "Arcadia/Ring1/Object.h"

#include "Arcadia/Ring1/ObjectReference.h"

#include "Arcadia/Ring1/Process.h"
#include "Arcadia/Ring1/ProcessExtensions.h"

#include "Arcadia/Ring1/Real32.h"
#include "Arcadia/Ring1/Real64.h"
#include "Arcadia/Ring1/Real32_getBits.h"
#include "Arcadia/Ring1/Real64_getBits.h"
#include "Arcadia/Ring1/Real32_isFinite.h"
#include "Arcadia/Ring1/Real64_isFinite.h"

#include "Arcadia/Ring1/safeAdd.h"
#include "Arcadia/Ring1/safeMultiply.h"

#include "Arcadia/Ring1/Size.h"

#include "Arcadia/Ring1/StaticAssert.h"

#include "Arcadia/Ring1/Status.h"

#include "Arcadia/Ring1/Thread.h"
#include "Arcadia/Ring1/ThreadExtensions.h"

#include "Arcadia/Ring1/swap.h"

#include "Arcadia/Ring1/Tests.h"

#include "Arcadia/Ring1/RealToString/Include.h"

#include "Arcadia/Ring1/Signals/Include.h"

#include "Arcadia/Ring1/StringToInteger/Include.h"
#include "Arcadia/Ring1/StringToNatural/Include.h"
#include "Arcadia/Ring1/StringToReal/toReal32.h"
#include "Arcadia/Ring1/StringToReal/toReal64.h"

#include "Arcadia/Ring1/TypeSystem/Include.h"

#include "Arcadia/Ring1/Value.h"

#include "Arcadia/Ring1/Void.h"

#include "Arcadia/Ring1/Objects/Include.h"

#undef ARCADIA_RING1_EXPORT
#pragma pop_macro("ARCADIA_RING1_EXPORT")

#undef ARCADIA_RING1_MODULE
#pragma pop_macro("ARCADIA_RING1_MODULE")

#endif // ARCADIA_RING1_INCLUDE_H_INCLUDED
