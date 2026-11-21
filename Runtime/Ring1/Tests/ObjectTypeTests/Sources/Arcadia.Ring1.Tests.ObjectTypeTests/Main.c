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

#include <stddef.h>
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

static void
testDescriptorMetadata
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TypeDescriptorValue valueType = _Arcadia_SizeValue_getType(thread);
  Arcadia_TypeDescriptorValue owner = _Arcadia_MyObjectA_getType(thread);
  Arcadia_TypeDescriptorValue derived = _Arcadia_MyObjectB_getType(thread);
  Arcadia_Name* fieldName = Arcadia_Names_getOrCreateName
    (
      thread, u8"member", sizeof(u8"member") - 1
    );
  Arcadia_Name* methodName = Arcadia_Names_getOrCreateName
    (
      thread, u8"measure", sizeof(u8"measure") - 1
    );
  Arcadia_FieldSpecification fields[] = {
    { .name = fieldName, .valueType = valueType, .offset = offsetof(Arcadia_MyObjectA, member) },
    { .name = fieldName, .valueType = valueType, .offset = offsetof(Arcadia_MyObjectA, member) }
  };
  Arcadia_TypeDescriptorValue parameters[] = { valueType };
  Arcadia_MethodSpecification methods[] = {
    { .name = methodName, .returnType = valueType, .numberOfParameters = 1, .parameterTypes = parameters }
  };

  // A failed publication must not consume the one permitted publication.
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_TypeDescriptor_publishMembers(thread, owner, fields, 2, methods, 1);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }

  Arcadia_TypeDescriptor_publishMembers(thread, owner, fields, 1, methods, 1);
  Arcadia_TypeDescriptor_publishMembers(thread, derived, NULL, 0, NULL, 0);

  Arcadia_FieldDescriptor const* field = Arcadia_TypeDescriptor_getDeclaredField(thread, owner, fieldName);
  Arcadia_MethodDescriptor const* method = Arcadia_TypeDescriptor_getDeclaredMethod(thread, owner, methodName);
  Arcadia_Tests_assertTrue(thread, field != NULL && method != NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_Descriptor_getKind(thread, (Arcadia_Descriptor const*)owner) == Arcadia_DescriptorKind_Type);
  Arcadia_Tests_assertTrue(thread, Arcadia_Descriptor_getKind(thread, (Arcadia_Descriptor const*)field) == Arcadia_DescriptorKind_Field);
  Arcadia_Tests_assertTrue(thread, Arcadia_Descriptor_getKind(thread, (Arcadia_Descriptor const*)method) == Arcadia_DescriptorKind_Method);
  Arcadia_Tests_assertTrue(thread, Arcadia_FieldDescriptor_getDeclaringType(field) == owner);
  Arcadia_Tests_assertTrue(thread, Arcadia_FieldDescriptor_getValueType(field) == valueType);
  Arcadia_Tests_assertTrue(thread, Arcadia_FieldDescriptor_getName(field) == fieldName);
  Arcadia_Tests_assertEqualSize(thread, offsetof(Arcadia_MyObjectA, member), Arcadia_FieldDescriptor_getOffset(field));
  Arcadia_Tests_assertTrue(thread, Arcadia_MethodDescriptor_getDeclaringType(method) == owner);
  Arcadia_Tests_assertTrue(thread, Arcadia_MethodDescriptor_getName(method) == methodName);
  Arcadia_Tests_assertTrue(thread, Arcadia_MethodDescriptor_getReturnType(method) == valueType);
  Arcadia_Tests_assertEqualSize(thread, 1, Arcadia_MethodDescriptor_getNumberOfParameters(method));
  Arcadia_Tests_assertTrue(thread, Arcadia_MethodDescriptor_getParameterType(method, 0) == valueType);

  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getDeclaredField(thread, derived, fieldName) == NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getDeclaredMethod(thread, derived, methodName) == NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getField(thread, derived, fieldName) == field);
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getMethod(thread, derived, methodName) == method);

  // Published member addresses must remain unchanged when republishing is rejected.
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_TypeDescriptor_publishMembers(thread, owner, fields, 1, methods, 1);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_Exists, status);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getDeclaredField(thread, owner, fieldName) == field);
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getDeclaredMethod(thread, owner, methodName) == method);
}

static void
testLateParentShadowing
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TypeDescriptorValue valueType = _Arcadia_SizeValue_getType(thread);
  Arcadia_TypeDescriptorValue parent = _Arcadia_MyObjectA_getType(thread);
  Arcadia_TypeDescriptorValue child = _Arcadia_MyObjectB_getType(thread);
  Arcadia_Name* name = Arcadia_Names_getOrCreateName
    (
      thread, u8"conflictingField", sizeof(u8"conflictingField") - 1
    );
  Arcadia_FieldSpecification spec = {
    .name = name,
    .valueType = valueType,
    .offset = offsetof(Arcadia_MyObjectA, member)
  };

  Arcadia_TypeDescriptor_publishMembers(thread, child, &spec, 1, NULL, 0);
  Arcadia_FieldDescriptor const* childField = Arcadia_TypeDescriptor_getDeclaredField(thread, child, name);

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_TypeDescriptor_publishMembers(thread, parent, &spec, 1, NULL, 0);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }

  Arcadia_TypeDescriptor_publishMembers(thread, parent, NULL, 0, NULL, 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getDeclaredField(thread, parent, name) == NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getField(thread, child, name) == childField);
}

static int
runLateParentShadowing
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testLateParentShadowing) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testMethodPublicationOrder
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TypeDescriptorValue parent = _Arcadia_MyObjectA_getType(thread);
  Arcadia_TypeDescriptorValue child = _Arcadia_MyObjectB_getType(thread);
  Arcadia_TypeDescriptorValue valueType = _Arcadia_SizeValue_getType(thread);
  Arcadia_Name* name = Arcadia_Names_getOrCreateName
    (
      thread, u8"overrideMethod", sizeof(u8"overrideMethod") - 1
    );
  Arcadia_TypeDescriptorValue parameterTypes[] = { valueType };
  Arcadia_MethodSpecification specification = {
    .name = name,
    .returnType = valueType,
    .numberOfParameters = 1,
    .parameterTypes = parameterTypes
  };

  // Publishing a child does not require retrofitting every legacy ancestor.
  Arcadia_TypeDescriptor_publishMembers(thread, child, NULL, 0, &specification, 1);
  Arcadia_MethodDescriptor const* childMethod = Arcadia_TypeDescriptor_getDeclaredMethod(thread, child, name);
  Arcadia_Tests_assertTrue(thread, childMethod != NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getMethod(thread, child, name) == childMethod);

  Arcadia_JumpTarget jumpTarget;
  // A late parent with a different signature must be rejected.
  Arcadia_MethodSpecification incompatible = {
    .name = name,
    .returnType = valueType,
    .numberOfParameters = 0,
    .parameterTypes = NULL
  };
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_TypeDescriptor_publishMembers(thread, parent, NULL, 0, &incompatible, 1);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }

  Arcadia_TypeDescriptor_publishMembers(thread, parent, NULL, 0, &specification, 1);
  Arcadia_MethodDescriptor const* parentMethod = Arcadia_TypeDescriptor_getDeclaredMethod(thread, parent, name);
  Arcadia_Tests_assertTrue(thread, parentMethod != NULL && childMethod != NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getMethod(thread, child, name) == childMethod);
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getMethod(thread, parent, name) == parentMethod);
}

static void
testMethodParentFirst
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TypeDescriptorValue parent = _Arcadia_MyObjectA_getType(thread);
  Arcadia_TypeDescriptorValue child = _Arcadia_MyObjectB_getType(thread);
  Arcadia_TypeDescriptorValue valueType = _Arcadia_SizeValue_getType(thread);
  Arcadia_Name* name = Arcadia_Names_getOrCreateName
    (
      thread, u8"parentFirstMethod", sizeof(u8"parentFirstMethod") - 1
    );
  Arcadia_TypeDescriptorValue parameters[] = { valueType };
  Arcadia_MethodSpecification method = {
    .name = name,
    .returnType = valueType,
    .numberOfParameters = 1,
    .parameterTypes = parameters
  };
  Arcadia_TypeDescriptor_publishMembers(thread, parent, NULL, 0, &method, 1);
  Arcadia_MethodDescriptor const* inherited = Arcadia_TypeDescriptor_getMethod(thread, parent, name);
  Arcadia_Tests_assertTrue(thread, inherited != NULL);

  Arcadia_MethodSpecification incompatible = {
    .name = name,
    .returnType = valueType,
    .numberOfParameters = 0,
    .parameterTypes = NULL
  };
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_TypeDescriptor_publishMembers(thread, child, NULL, 0, &incompatible, 1);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }

  // A failed override must not consume the child's publication opportunity.
  Arcadia_TypeDescriptor_publishMembers(thread, child, NULL, 0, &method, 1);
  Arcadia_MethodDescriptor const* overridden = Arcadia_TypeDescriptor_getMethod(thread, child, name);
  Arcadia_Tests_assertTrue(thread, overridden != NULL && overridden != inherited);
  Arcadia_Tests_assertTrue(thread, Arcadia_MethodDescriptor_getDeclaringType(overridden) == child);
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getDeclaredMethod(thread, parent, name) == inherited);
}

static int
runMethodParentFirst
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMethodParentFirst) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runMethodPublicationOrder
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMethodPublicationOrder) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runDescriptorMetadata
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testDescriptorMetadata) ? EXIT_SUCCESS : EXIT_FAILURE;
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
    { "DescriptorMetadata", &runDescriptorMetadata },
    { "LateParentShadowing", &runLateParentShadowing },
    { "MethodPublicationOrder", &runMethodPublicationOrder },
    { "MethodParentFirst", &runMethodParentFirst },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
