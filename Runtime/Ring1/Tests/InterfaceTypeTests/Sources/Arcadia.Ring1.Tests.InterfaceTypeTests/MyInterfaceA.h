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

#if !defined(ARCADIA_RING1_TESTS_OBJECTTYPETESTS_MYINTERFACEA_H_INCLUDED)
#define ARCADIA_RING1_TESTS_OBJECTTYPETESTS_MYINTERFACEA_H_INCLUDED

#include "Arcadia/Ring1/Include.h"

Arcadia_declareObjectType(u8"Arcadia.MyInterfaceA", Arcadia_MyInterfaceA,
                          u8"Arcadia.Interface");

struct Arcadia_MyInterfaceADispatch {
  Arcadia_InterfaceDispatch parent;
};

#if 0
struct Arcadia_MyObjectA {
  Arcadia_Object parent;
};

Arcadia_MyObjectA*
Arcadia_MyObjectA_create
  (
    Arcadia_Thread* thread
  );
#endif

#endif // ARCADIA_RING1_TESTS_OBJECTTYPETESTS_MYINTERFACEA_H_INCLUDED
