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

#include "Arcadia/Testing/Include.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Arcadia.Ring1.Tests.ObjectTypeTests/MyObjectA.h"
#include "Arcadia.Ring1.Tests.ObjectTypeTests/MyObjectB.h"

static void
test
  (
    Arcadia_Thread* thread
  )
{
  // Register the descriptor's own type before creating the object.
  Arcadia_TypeDescriptorValue descriptorType = _Arcadia_TypeDescriptor_getType(thread);
  Arcadia_MyObjectB* o = Arcadia_MyObjectB_create(thread);
  Arcadia_TypeDescriptorValue t = Arcadia_Object_getType(thread, (Arcadia_Object*)o);
  if (t != _Arcadia_MyObjectB_getType(thread) ||
      Arcadia_TypeDescriptor_getKind(thread, t) != Arcadia_TypeKind_Object) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    return;
  }

  Arcadia_Value objectValue = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)o);
  if (Arcadia_Value_getTypeDescriptor(thread, &objectValue) != t) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    return;
  }

  // The numeric tag is part of the stable first-class value representation.
  if (Arcadia_ValueTag_TypeDescriptor != 19) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    return;
  }

  Arcadia_Value selfDescriptorValue = Arcadia_Value_makeTypeDescriptorValue(descriptorType);
  if (Arcadia_Value_getTypeDescriptor(thread, &selfDescriptorValue) != descriptorType ||
      Arcadia_Value_getTypeDescriptorValue(&selfDescriptorValue) != descriptorType) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    return;
  }

  Arcadia_Value descriptorValue = Arcadia_Value_makeTypeDescriptorValue(t);
  if (Arcadia_Value_getTag(&descriptorValue) != Arcadia_ValueTag_TypeDescriptor ||
      !Arcadia_Value_isTypeDescriptorValue(&descriptorValue) ||
      Arcadia_Value_getTypeDescriptorValue(&descriptorValue) != t ||
      Arcadia_Value_getTypeDescriptor(thread, &descriptorValue) != descriptorType) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    return;
  }

  // Boxing preserves the identity-based equality and hash contract.
  Arcadia_Value sameDescriptorValue = Arcadia_Value_makeTypeDescriptorValue(t);
  Arcadia_Value differentDescriptorValue = Arcadia_Value_makeTypeDescriptorValue(descriptorType);
  if (!Arcadia_Value_isEqualTo(thread, &descriptorValue, &sameDescriptorValue) ||
      Arcadia_Value_isEqualTo(thread, &descriptorValue, &differentDescriptorValue) ||
      Arcadia_Value_getHash(thread, &descriptorValue) != Arcadia_Value_getHash(thread, &sameDescriptorValue) ||
      !Arcadia_Value_isInstanceOf(thread, &descriptorValue, descriptorType)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    return;
  }
}

static void
registrationConstruct
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{/* Not invoked by this registration-only test. */}

static void
registrationDestruct
  (
    void* context
  )
{/* The test keeps no descriptor cache. */}

static void
registrationFailDispatch
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectDispatch* dispatch
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
  Arcadia_Thread_jump(thread);
}

static const Arcadia_ObjectType_Operations failingObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = &registrationConstruct,
  .initializeDispatch = &registrationFailDispatch,
};

static const Arcadia_ObjectType_Operations workingObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = &registrationConstruct,
};

static const Arcadia_TypeDescriptor_Operations failingRegistrationOperations = {
  Arcadia_TypeDescriptor_Operations_Initializer,
  .objectTypeOperations = &failingObjectOperations,
};

static const Arcadia_TypeDescriptor_Operations workingRegistrationOperations = {
  Arcadia_TypeDescriptor_Operations_Initializer,
  .objectTypeOperations = &workingObjectOperations,
};

static void
testRegistrationRollback
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Name* name = Arcadia_Names_getOrCreateName
    (
      thread,
      u8"Arcadia.Tests.ObjectTypeTests.Rollback",
      sizeof(u8"Arcadia.Tests.ObjectTypeTests.Rollback") - 1
    );
  Arcadia_TypeDescriptorValue parent = _Arcadia_Object_getType(thread);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_registerObjectType
      (
        thread, name, sizeof(Arcadia_Object), parent,
        sizeof(Arcadia_ObjectDispatch), &failingRegistrationOperations,
        &registrationDestruct
      );
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_OperationFailed, status);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }

  // A failed initializer must not publish the name or retain a dangling entry.
  Arcadia_TypeDescriptorValue type = Arcadia_registerObjectType
    (
      thread, name, sizeof(Arcadia_Object), parent,
      sizeof(Arcadia_ObjectDispatch), &workingRegistrationOperations,
      &registrationDestruct
    );
  Arcadia_Tests_assertTrue(thread, type == Arcadia_getTypeDescriptorByName(thread, name));
}

static int
runTests
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(&test)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testRegistrationRollback)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "ObjectTypeTests", &runTests },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
