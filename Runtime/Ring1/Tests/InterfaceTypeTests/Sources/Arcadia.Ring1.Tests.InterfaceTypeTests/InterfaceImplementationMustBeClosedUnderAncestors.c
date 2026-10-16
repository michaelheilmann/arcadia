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

#include "Arcadia/Ring1/Include.h"
#include "Arcadia.Ring1.Tests.InterfaceTypeTests/Helper.h"
#include "Arcadia.Ring1.Tests.InterfaceTypeTests/InterfaceImplementationMustBeClosedUnderAncestors.h"

Helper_defineObjectTypeOperations(closedObject)

#undef Helper_defineObjectTypeOperations

/// Register the interface types "Arcadia.Tests.InterfaceTypeTests.Closed.Ancestor" and
/// "Arcadia.Tests.InterfaceTypeTests.Closed.Derived", where "Derived" extends "Ancestor". Then
/// register an object type which explicitly implements only "Derived". The runtime automatically
/// adds an "Ancestor" implementation record, because implementing a derived interface makes the
/// object type a descendant of its ancestor interfaces as well. The synthesized ancestor dispatch is
/// distinct from the derived dispatch and starts with the standard "Arcadia.InterfaceDispatch".
void
testInterfaceImplementationMustBeClosedUnderAncestors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TypeValue ancestor =
    Arcadia_registerInterfaceType
      (
        thread,
        Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Tests.InterfaceTypeTests.Closed.Ancestor", sizeof(u8"Arcadia.Tests.InterfaceTypeTests.Closed.Ancestor") - 1),
        sizeof(Arcadia_InterfaceDispatch),
        0,
        NULL,
        &Helper_typeDestructing
      );
  Arcadia_TypeValue derivedExtends[] = { ancestor };
  Arcadia_TypeValue derived =
    Arcadia_registerInterfaceType
      (
        thread,
        Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Tests.InterfaceTypeTests.Closed.Derived", sizeof(u8"Arcadia.Tests.InterfaceTypeTests.Closed.Derived") - 1),
        sizeof(Arcadia_InterfaceDispatch),
        1,
        derivedExtends,
        &Helper_typeDestructing
      );

  // The ancestor interface type is not specified explicitly. Registration should synthesize it.
  Arcadia_ObjectType_InterfaceSpecification interfaces[] = {
    { .interfaceType = derived, .dispatchSize = sizeof(Arcadia_InterfaceDispatch), .initializeDispatch = NULL }
  };
  Arcadia_TypeValue object =
    Arcadia_registerObjectTypeWithInterfaces
      (
        thread,
        Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Tests.InterfaceTypeTests.Closed.Object", sizeof(u8"Arcadia.Tests.InterfaceTypeTests.Closed.Object") - 1),
        0,
        _Arcadia_Object_getType(thread),
        sizeof(Arcadia_ObjectDispatch),
        &_closedObject_typeOperations,
        interfaces,
        1,
        &Helper_typeDestructing
      );
  g_closedObject_type = object;

  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, object, derived));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, object, ancestor));

  Arcadia_InterfaceDispatch* derivedDispatch =
    (Arcadia_InterfaceDispatch*)Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)object, (Arcadia_InterfaceType*)derived);
  Arcadia_InterfaceDispatch* ancestorDispatch =
    (Arcadia_InterfaceDispatch*)Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)object, (Arcadia_InterfaceType*)ancestor);
  Arcadia_Tests_assertTrue(thread, NULL != derivedDispatch);
  Arcadia_Tests_assertTrue(thread, NULL != ancestorDispatch);
  if (derivedDispatch && ancestorDispatch) {
    Arcadia_Tests_assertTrue(thread, derivedDispatch != ancestorDispatch);
    Arcadia_Tests_assertTrue(thread, derivedDispatch->objectType == object);
    Arcadia_Tests_assertTrue(thread, derivedDispatch->interfaceType == derived);
    Arcadia_Tests_assertTrue(thread, ancestorDispatch->objectType == object);
    Arcadia_Tests_assertTrue(thread, ancestorDispatch->interfaceType == ancestor);
  }
}
