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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_FOREIGNPROCEDURE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_FOREIGNPROCEDURE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/_declareScalarType.h"
#include "Arcadia/Ring1/Implementation/Size.h"
typedef struct Arcadia_Value Arcadia_Value;

/// The foreign procedure has the following calling convention:
/// It can expect the stack to be non-empty.
/// The element on top of the stack is a Natural8 value indicating the number of arguments.
/// The foreign procedure MUST remove this Natural8 value AND n values where n is the value of the Natural8Value from the stack.
/// If the fp raises an error, then it may leave any number of values on the stack. These will be removed by the caller.
/// If the fp raises no error, it shall leave a single value on the stack which is the return value.
typedef void (Arcadia_ForeignProcedure)(Arcadia_Thread* thread);

typedef Arcadia_ForeignProcedure* Arcadia_ForeignProcedureValue;

#define Arcadia_ForeignProcedureValue_Null (NULL)

#define Arcadia_ForeignProcedureValue_NumberOfBytes (sizeof(void*))

#if Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X64
  Arcadia_StaticAssert(Arcadia_ForeignProcedureValue_NumberOfBytes == 8, "Arcadia.ForeignProcedureValue.NumberOfBytes must be 8");
#elif Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X86
  Arcadia_StaticAssert(Arcadia_ForeignProcedureValue_NumberOfBytes == 4, "Arcadia.ForeignProcedureValue.NumberOfBytes must be 4");
#else
  #error("invalid/unknown value for Arcadia_Configuration_InstructionSetArchitecture")
#endif

#define Arcadia_ForeignProcedureValue_NumberOfBits (Arcadia_ForeignProcedureValue_NumberOfBytes * 8)

Arcadia_declareScalarType(Arcadia_ForeignProcedure);

#endif // ARCADIA_RING1_IMPLEMENTATION_FOREIGNPROCEDURE_H_INCLUDED
