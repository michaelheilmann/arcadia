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

#if !defined(NATIVERESOURCES_EXTERNAL_H_INCLUDED)
#define NATIVERESOURCES_EXTERNAL_H_INCLUDED

#include "Arcadia/Ring1/Include.h"

typedef enum Test_External_Kind {
  Test_External_Kind_Minimum = INT32_MIN,
  Test_External_Kind_Negative = -3,
  Test_External_Kind_First = 0,
  Test_External_Kind_Second = 7,
  Test_External_Kind_Maximum = INT32_MAX,
} Test_External_Kind;

Arcadia_declareEnumerationType(u8"Test.External.Kind", Test_External_Kind);

// A native unsigned enum ABI represented by a scalar alias and macros. This
// exercises range checking consistently, including MSVC's signed C enums.
typedef Arcadia_Natural32Value Test_External_UnsignedKind;
#define Test_External_UnsignedKind_First (Arcadia_Natural32Value_Literal(0))
#define Test_External_UnsignedKind_OutsideValueRange (Arcadia_Natural32Value_Maximum)

Arcadia_TypeValue
_Test_External_UnsignedKind_getType
  (
    Arcadia_Thread* thread
  );

Arcadia_declareObjectType(u8"Test.External", Test_External,
                          u8"Arcadia.Object");

struct Test_ExternalDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Test_External {
  Arcadia_Object _parent;
};

Test_External*
Test_External_create
  (
    Arcadia_Thread* thread
  );

Arcadia_Natural64Value
Test_External_getExternalValue
  (
    Arcadia_Thread* thread,
    Test_External* self
  );

#endif // NATIVERESOURCES_EXTERNAL_H_INCLUDED
