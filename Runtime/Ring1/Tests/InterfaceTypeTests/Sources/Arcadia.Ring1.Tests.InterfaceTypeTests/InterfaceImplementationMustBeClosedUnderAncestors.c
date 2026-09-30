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

/// The registration of the object type is expected to fail, so the operations of the object type
/// are never invoked, but they have to be provided.
static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

/// Register the interface types "Arcadia.Tests.InterfaceTypeTests.Closed.Ancestor" and
/// "Arcadia.Tests.InterfaceTypeTests.Closed.Derived", where "Derived" extends "Ancestor". Then
/// attempt to register an object type which implements only "Derived". The dispatches of interface
/// types are independent: the dispatch of "Derived" never is a dispatch of "Ancestor", so an
/// implementation of "Derived" without an implementation of "Ancestor" could never dispatch to
/// "Ancestor" operations. The set of implemented interface types is therefore required to be
/// closed under the interface types they extend, and the registration must raise. The object type
/// operations are provided because they are required, but they are never invoked, since
/// registration fails before any object of the type can exist. The raise is caught with a jump
/// target, and the status is cleared afterwards so the test itself is not reported as failed.
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

  // The ancestor interface type is not implemented, so the registration must raise.
  Arcadia_ObjectType_InterfaceSpecification interfaces[] = {
    { .interfaceType = derived, .dispatchSize = sizeof(Arcadia_InterfaceDispatch), .initializeDispatch = NULL }
  };
  Arcadia_JumpTarget jumpTarget;
  Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_registerObjectTypeWithInterfaces
      (
        thread,
        Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Tests.InterfaceTypeTests.Closed.Object", sizeof(u8"Arcadia.Tests.InterfaceTypeTests.Closed.Object") - 1),
        0,
        _Arcadia_Object_getType(thread),
        sizeof(Arcadia_ObjectDispatch),
        &_typeOperations,
        interfaces,
        1,
        &Helper_typeDestructing
      );
  } else {
    raised = Arcadia_BooleanValue_True;
  }
  Arcadia_Thread_popJumpTarget(thread);

  // The status has to be cleared, or the test is reported as failed.
  Arcadia_Tests_assertTrue(thread, raised == Arcadia_BooleanValue_True);
  Arcadia_Thread_setStatus(thread, 0);
}
