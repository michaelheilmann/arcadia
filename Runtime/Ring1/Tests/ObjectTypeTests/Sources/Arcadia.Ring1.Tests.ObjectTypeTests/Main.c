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
{
  // Not invoked by this registration-only test.
}

static void
registrationDestruct
  (
    void* context
  )
{
  // The test keeps no descriptor cache.
}

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

static Arcadia_SizeValue metadataPublicationAttempts = 0;

static void
publishMetadataAfterFailure
  (
    Arcadia_Thread* thread,
    Arcadia_TypeDescriptorValue owner
  )
{
  ++metadataPublicationAttempts;
  if (1 == metadataPublicationAttempts) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_TypeDescriptor_publishMembers(thread, owner, NULL, 0, NULL, 0);
}

typedef struct MetadataCycleObject {
  Arcadia_Object _parent;
  Arcadia_Object* other;
} MetadataCycleObject;

static void
testMetadataFailureAndCycles
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TypeDescriptorValue parent = _Arcadia_Object_getType(thread);
  Arcadia_Name* failureName = Arcadia_Names_getOrCreateName
    (
      thread, u8"Arcadia.Tests.MetadataFailure", sizeof(u8"Arcadia.Tests.MetadataFailure") - 1
    );
  Arcadia_TypeDescriptorValue failureType = Arcadia_registerObjectType
    (
      thread, failureName, sizeof(Arcadia_Object), parent,
      sizeof(Arcadia_ObjectDispatch), &workingRegistrationOperations,
      &registrationDestruct
    );
  Arcadia_BooleanValue publishing = Arcadia_BooleanValue_False;
  metadataPublicationAttempts = 0;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_TypeDescriptor_ensureMembers
      (
        thread, failureType, &publishMetadataAfterFailure, &publishing
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
  Arcadia_Tests_assertTrue(thread, publishing == Arcadia_BooleanValue_False);
  Arcadia_Tests_assertTrue
    (
      thread, !Arcadia_TypeDescriptor_hasPublishedMembers(thread, failureType)
    );
  Arcadia_TypeDescriptor_ensureMembers
    (
      thread, failureType, &publishMetadataAfterFailure, &publishing
    );
  Arcadia_Tests_assertTrue(thread, metadataPublicationAttempts == 2);
  Arcadia_Tests_assertTrue
    (
      thread, Arcadia_TypeDescriptor_hasPublishedMembers(thread, failureType)
    );

  // Identity registration precedes both publications, so the metadata
  // references form a cycle without recursive registration.
  Arcadia_Name* firstName = Arcadia_Names_getOrCreateName
    (
      thread, u8"Arcadia.Tests.MetadataCycleA", sizeof(u8"Arcadia.Tests.MetadataCycleA") - 1
    );
  Arcadia_Name* secondName = Arcadia_Names_getOrCreateName
    (
      thread, u8"Arcadia.Tests.MetadataCycleB", sizeof(u8"Arcadia.Tests.MetadataCycleB") - 1
    );
  Arcadia_TypeDescriptorValue first = Arcadia_registerObjectType
    (
      thread, firstName, sizeof(MetadataCycleObject), parent,
      sizeof(Arcadia_ObjectDispatch), &workingRegistrationOperations,
      &registrationDestruct
    );
  Arcadia_TypeDescriptorValue second = Arcadia_registerObjectType
    (
      thread, secondName, sizeof(MetadataCycleObject), parent,
      sizeof(Arcadia_ObjectDispatch), &workingRegistrationOperations,
      &registrationDestruct
    );
  Arcadia_Name* member = Arcadia_Names_getOrCreateName
    (
      thread, u8"other", sizeof(u8"other") - 1
    );
  Arcadia_FieldSpecification firstField = {
    .name = member,
    .valueType = second,
    .offset = offsetof(MetadataCycleObject, other),
    .storageSize = sizeof(Arcadia_Object*)
  };
  Arcadia_FieldSpecification secondField = firstField;
  secondField.valueType = first;
  Arcadia_TypeDescriptor_publishMembers(thread, first, &firstField, 1, NULL, 0);
  Arcadia_TypeDescriptor_publishMembers(thread, second, &secondField, 1, NULL, 0);
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_FieldDescriptor_getValueType
        (
          Arcadia_TypeDescriptor_getDeclaredField(thread, first, member)
        ) == second
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_FieldDescriptor_getValueType
        (
          Arcadia_TypeDescriptor_getDeclaredField(thread, second, member)
        ) == first
    );
}

static int
runMetadataFailureAndCycles
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMetadataFailureAndCycles)
    ? EXIT_SUCCESS : EXIT_FAILURE;
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
    {
      .name = fieldName,
      .valueType = valueType,
      .offset = offsetof(Arcadia_MyObjectA, member),
      .storageSize = sizeof(Arcadia_SizeValue)
    },
    {
      .name = fieldName,
      .valueType = valueType,
      .offset = offsetof(Arcadia_MyObjectA, member),
      .storageSize = sizeof(Arcadia_SizeValue)
    }
  };
  Arcadia_TypeDescriptorValue parameters[] = { valueType };
  Arcadia_Name* referenceName = Arcadia_Names_getOrCreateName
    (
      thread, u8"reference", sizeof(u8"reference") - 1
    );
  Arcadia_FieldSpecification referenceSpecification = {
    .name = referenceName,
    .valueType = derived,
    .offset = offsetof(Arcadia_MyObjectA, reference),
    .storageSize = sizeof(Arcadia_Object*)
  };
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

  // Reject invalid byte spans before publication, including overflowing sizes.
  Arcadia_FieldSpecification malformed = fields[0];
  for (size_t i = 0; i < 4; ++i) {
    malformed = fields[0];
    switch (i) {
      case 0: {
        malformed.storageSize = 0;
      } break;
      case 1: {
        malformed.storageSize = sizeof(Arcadia_MyObjectA);
      } break;
      case 2: {
        malformed.offset = sizeof(Arcadia_MyObjectA);
      } break;
      case 3: {
        malformed.storageSize = (size_t)-1;
      } break;
    }
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_TypeDescriptor_publishMembers(thread, owner, &malformed, 1, methods, 1);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }

  // Publish scalar and object-reference descriptors as one immutable collection.
  Arcadia_FieldSpecification publishedFields[] = { fields[0], referenceSpecification };
  Arcadia_TypeDescriptor_publishMembers(thread, owner, publishedFields, 2, methods, 1);
  Arcadia_TypeDescriptor_publishMembers(thread, derived, NULL, 0, NULL, 0);

  Arcadia_FieldDescriptor const* field = Arcadia_TypeDescriptor_getDeclaredField(thread, owner, fieldName);
  Arcadia_MethodDescriptor const* method = Arcadia_TypeDescriptor_getDeclaredMethod(thread, owner, methodName);
  Arcadia_Tests_assertTrue(thread, field != NULL && method != NULL);

  // Distinct tagged values preserve the native, type-owned descriptor identity.
  Arcadia_Value fieldValue = Arcadia_Value_makeFieldDescriptorValue(field);
  Arcadia_Value methodValue = Arcadia_Value_makeMethodDescriptorValue(method);
  Arcadia_Value fieldCopy = Arcadia_Value_makeFieldDescriptorValue(field);
  Arcadia_Value methodCopy = Arcadia_Value_makeMethodDescriptorValue(method);
  Arcadia_Value typeValue = Arcadia_Value_makeTypeDescriptorValue(owner);

  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isFieldDescriptorValue(&fieldValue));
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isMethodDescriptorValue(&methodValue));
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getFieldDescriptorValue(&fieldValue) == field);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getMethodDescriptorValue(&methodValue) == method);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEqualTo(thread, &fieldValue, &fieldCopy));
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEqualTo(thread, &methodValue, &methodCopy));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isEqualTo(thread, &fieldValue, &methodValue));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isEqualTo(thread, &typeValue, &fieldValue));

  Arcadia_Tests_assertEqualSize
    (
      thread,
      Arcadia_Value_getHash(thread, &fieldValue),
      Arcadia_Value_getHash(thread, &fieldCopy)
    );
  Arcadia_Tests_assertEqualSize
    (
      thread,
      Arcadia_Value_getHash(thread, &methodValue),
      Arcadia_Value_getHash(thread, &methodCopy)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_Value_getTypeDescriptor(thread, &fieldValue) == _Arcadia_FieldDescriptor_getType(thread)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_Value_getTypeDescriptor(thread, &methodValue) == _Arcadia_MethodDescriptor_getType(thread)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_Value_isInstanceOf(thread, &fieldValue, _Arcadia_Descriptor_getType(thread))
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_Value_isInstanceOf(thread, &methodValue, _Arcadia_Descriptor_getType(thread))
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_Value_isInstanceOf(thread, &typeValue, _Arcadia_Descriptor_getType(thread))
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      !Arcadia_Value_isInstanceOf(thread, &fieldValue, _Arcadia_MethodDescriptor_getType(thread))
    );
  Arcadia_Value_visit(thread, &fieldValue);
  Arcadia_Value_visit(thread, &methodValue);
  Arcadia_TypeDescriptorValue descriptorType = _Arcadia_Descriptor_getType(thread);
  Arcadia_TypeDescriptorValue typeDescriptorType = _Arcadia_TypeDescriptor_getType(thread);
  Arcadia_TypeDescriptorValue fieldDescriptorType = _Arcadia_FieldDescriptor_getType(thread);
  Arcadia_TypeDescriptorValue methodDescriptorType = _Arcadia_MethodDescriptor_getType(thread);

  // Descriptor inheritance is independent of ordinary object inheritance.
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_TypeDescriptor_isDescendantType(thread, typeDescriptorType, descriptorType)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_TypeDescriptor_isDescendantType(thread, fieldDescriptorType, descriptorType)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_TypeDescriptor_isDescendantType(thread, methodDescriptorType, descriptorType)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_TypeDescriptor_isDescendantType(thread, descriptorType, descriptorType)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      !Arcadia_TypeDescriptor_isDescendantType(thread, descriptorType, fieldDescriptorType)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      !Arcadia_TypeDescriptor_isDescendantType(thread, fieldDescriptorType, methodDescriptorType)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      !Arcadia_TypeDescriptor_isDescendantType(thread, methodDescriptorType, typeDescriptorType)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_TypeDescriptor_isDescendantType(thread, derived, owner)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      !Arcadia_TypeDescriptor_isDescendantType(thread, owner, descriptorType)
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      !Arcadia_TypeDescriptor_isDescendantType(thread, fieldDescriptorType, owner)
    );

  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_Descriptor_getKind(thread, (Arcadia_Descriptor const*)owner) == Arcadia_DescriptorKind_Type
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_Descriptor_getKind(thread, (Arcadia_Descriptor const*)field) == Arcadia_DescriptorKind_Field
    );
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_Descriptor_getKind(thread, (Arcadia_Descriptor const*)method) == Arcadia_DescriptorKind_Method
    );
  Arcadia_Tests_assertTrue(thread, Arcadia_FieldDescriptor_getDeclaringType(field) == owner);
  Arcadia_Tests_assertTrue(thread, Arcadia_FieldDescriptor_getValueType(field) == valueType);
  Arcadia_Tests_assertTrue(thread, Arcadia_FieldDescriptor_getName(field) == fieldName);
  Arcadia_Tests_assertEqualSize
    (
      thread,
      offsetof(Arcadia_MyObjectA, member),
      Arcadia_FieldDescriptor_getOffset(field)
    );
  Arcadia_Tests_assertEqualSize
    (
      thread,
      sizeof(Arcadia_SizeValue),
      Arcadia_FieldDescriptor_getStorageSize(field)
    );
  // Reflective writes use the same C slot as direct native field access.
  Arcadia_MyObjectA* instance = Arcadia_MyObjectA_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)instance);
  Arcadia_FieldDescriptor_setSizeValue(thread, field, (Arcadia_Object*)instance, 17);
  Arcadia_Tests_assertEqualSize(thread, 17, instance->member);
  instance->member = 29;
  Arcadia_Tests_assertEqualSize
    (
      thread,
      29,
      Arcadia_FieldDescriptor_getSizeValue(thread, field, (Arcadia_Object*)instance)
    );
  Arcadia_Object_unlock(thread, (Arcadia_Object*)instance);
  // An inherited field is addressed using its declaring type's offset.
  Arcadia_MyObjectB* subtypeInstance = Arcadia_MyObjectB_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)subtypeInstance);
  Arcadia_FieldDescriptor_setSizeValue(thread, field, (Arcadia_Object*)subtypeInstance, 47);
  Arcadia_Tests_assertEqualSize(thread, 47, ((Arcadia_MyObjectA*)subtypeInstance)->member);
  Arcadia_Tests_assertEqualSize
    (
      thread,
      47,
      Arcadia_FieldDescriptor_getSizeValue(thread, field, (Arcadia_Object*)subtypeInstance)
    );
  Arcadia_Object_unlock(thread, (Arcadia_Object*)subtypeInstance);

  Arcadia_FieldDescriptor const* referenceField =
    Arcadia_TypeDescriptor_getDeclaredField(thread, owner, referenceName);
  Arcadia_Tests_assertTrue(thread, referenceField != NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_FieldDescriptor_getValueType(referenceField) == derived);
  Arcadia_Tests_assertEqualSize
    (
      thread, sizeof(Arcadia_Object*), Arcadia_FieldDescriptor_getStorageSize(referenceField)
    );

  // Keep both the container and target rooted throughout these assertions.
  Arcadia_Object_lock(thread, (Arcadia_Object*)instance);
  Arcadia_Object_lock(thread, (Arcadia_Object*)subtypeInstance);
  Arcadia_FieldDescriptor_setObjectReferenceValue
    (
      thread, referenceField, (Arcadia_Object*)instance, (Arcadia_Object*)subtypeInstance
    );
  Arcadia_Tests_assertTrue(thread, instance->reference == (Arcadia_Object*)subtypeInstance);
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_FieldDescriptor_getObjectReferenceValue(thread, referenceField, (Arcadia_Object*)instance)
      == (Arcadia_Object*)subtypeInstance
    );

  // A target of the wrong runtime type must leave the old pointer unchanged.
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_FieldDescriptor_setObjectReferenceValue
      (
        thread, referenceField, (Arcadia_Object*)instance, (Arcadia_Object*)instance
      );
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentTypeInvalid, status);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }
  Arcadia_Tests_assertTrue(thread, instance->reference == (Arcadia_Object*)subtypeInstance);

  // Scalar fields must not be reinterpreted as reference storage.
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_FieldDescriptor_getObjectReferenceValue(thread, field, (Arcadia_Object*)instance);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentTypeInvalid, status);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }

  Arcadia_FieldDescriptor_setObjectReferenceValue
    (
      thread, referenceField, (Arcadia_Object*)instance, NULL
    );
  Arcadia_Tests_assertTrue(thread, instance->reference == NULL);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)subtypeInstance);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)instance);

  // Reject invalid receivers instead of producing a raw C pointer.
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_FieldDescriptor_getSizeValue(thread, field, NULL);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }

  Arcadia_Tests_assertTrue(thread, Arcadia_MethodDescriptor_getDeclaringType(method) == owner);
  Arcadia_Tests_assertTrue(thread, Arcadia_MethodDescriptor_getName(method) == methodName);
  Arcadia_Tests_assertTrue(thread, Arcadia_MethodDescriptor_getReturnType(method) == valueType);
  Arcadia_Tests_assertEqualSize(thread, 1, Arcadia_MethodDescriptor_getNumberOfParameters(method));
  Arcadia_Tests_assertTrue(thread, Arcadia_MethodDescriptor_getParameterType(method, 0) == valueType);

  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getDeclaredField(thread, derived, fieldName) == NULL);
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_TypeDescriptor_getDeclaredMethod(thread, derived, methodName) == NULL
    );
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
  Arcadia_Tests_assertTrue
    (
      thread,
      Arcadia_TypeDescriptor_getDeclaredMethod(thread, owner, methodName) == method
    );
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
    .offset = offsetof(Arcadia_MyObjectA, member),
    .storageSize = sizeof(Arcadia_SizeValue)
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
    .parameterTypes = parameterTypes,
    .isVirtual = Arcadia_BooleanValue_True
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

static Arcadia_SizeValue
invokeBaseMeasurement
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{
  return 11;
}

static Arcadia_SizeValue
invokeDerivedMeasurement
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{
  return 37;
}

static Arcadia_SizeValue
invokeOneSizeValue
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_SizeValue argument
  )
{
  return argument + 11;
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
    .parameterTypes = parameters,
    .isVirtual = Arcadia_BooleanValue_True
  };
  Arcadia_Name* invocationName = Arcadia_Names_getOrCreateName
    (
      thread,
      u8"measureWithoutArguments",
      sizeof(u8"measureWithoutArguments") - 1
    );
  Arcadia_MethodSpecification baseInvocation = {
    .name = invocationName,
    .returnType = valueType,
    .numberOfParameters = 0,
    .parameterTypes = NULL,
    .invokeSizeValue0 = &invokeBaseMeasurement,
    .isVirtual = Arcadia_BooleanValue_True
  };
  // A callable adapter is valid only for an exact zero-argument SizeValue signature.
  Arcadia_MethodSpecification invalidAdapter = method;
  invalidAdapter.invokeSizeValue0 = &invokeBaseMeasurement;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_TypeDescriptor_publishMembers(thread, parent, NULL, 0, &invalidAdapter, 1);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }

  Arcadia_Name* nonvirtualName = Arcadia_Names_getOrCreateName
    (
      thread, u8"measureNonvirtual", sizeof(u8"measureNonvirtual") - 1
    );
  Arcadia_MethodSpecification nonvirtualInvocation = baseInvocation;
  nonvirtualInvocation.name = nonvirtualName;
  nonvirtualInvocation.isVirtual = Arcadia_BooleanValue_False;
  Arcadia_Name* oneArgumentName = Arcadia_Names_getOrCreateName
    (
      thread, u8"measureWithArgument", sizeof(u8"measureWithArgument") - 1
    );
  Arcadia_MethodSpecification oneArgumentInvocation = {
    .name = oneArgumentName,
    .returnType = valueType,
    .numberOfParameters = 1,
    .parameterTypes = parameters,
    .invokeSizeValue1 = &invokeOneSizeValue,
    .isVirtual = Arcadia_BooleanValue_False
  };
  Arcadia_MethodSpecification parentMethods[] = {
    method, baseInvocation, nonvirtualInvocation, oneArgumentInvocation
  };
  Arcadia_TypeDescriptor_publishMembers(thread, parent, NULL, 0, parentMethods, 4);
  Arcadia_MethodDescriptor const* inherited = Arcadia_TypeDescriptor_getMethod(thread, parent, name);
  Arcadia_Tests_assertTrue(thread, inherited != NULL);

  Arcadia_MethodSpecification incompatible = {
    .name = name,
    .returnType = valueType,
    .numberOfParameters = 0,
    .parameterTypes = NULL
  };
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

  // A nonvirtual member cannot be overridden by publishing the same name.
  Arcadia_MethodSpecification forbiddenOverride = nonvirtualInvocation;
  forbiddenOverride.isVirtual = Arcadia_BooleanValue_True;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_TypeDescriptor_publishMembers(thread, child, NULL, 0, &forbiddenOverride, 1);
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
  Arcadia_MethodSpecification childInvocation = baseInvocation;
  childInvocation.invokeSizeValue0 = &invokeDerivedMeasurement;
  Arcadia_MethodSpecification childMethods[] = { method, childInvocation };
  Arcadia_TypeDescriptor_publishMembers(thread, child, NULL, 0, childMethods, 2);
  Arcadia_MethodDescriptor const* overridden = Arcadia_TypeDescriptor_getMethod(thread, child, name);
  Arcadia_Tests_assertTrue(thread, overridden != NULL && overridden != inherited);
  Arcadia_Tests_assertTrue(thread, Arcadia_MethodDescriptor_getDeclaringType(overridden) == child);
  Arcadia_Tests_assertTrue(thread, Arcadia_TypeDescriptor_getDeclaredMethod(thread, parent, name) == inherited);
  Arcadia_MethodDescriptor const* baseMethod =
    Arcadia_TypeDescriptor_getDeclaredMethod(thread, parent, invocationName);
  Arcadia_MethodDescriptor const* childMethod =
    Arcadia_TypeDescriptor_getDeclaredMethod(thread, child, invocationName);
  Arcadia_Tests_assertTrue(thread, baseMethod != NULL && childMethod != NULL);
  Arcadia_MethodDescriptor const* nonvirtualMethod =
    Arcadia_TypeDescriptor_getDeclaredMethod(thread, parent, nonvirtualName);
  Arcadia_Tests_assertTrue(thread, nonvirtualMethod != NULL);

  Arcadia_MyObjectA* baseObject = Arcadia_MyObjectA_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)baseObject);
  Arcadia_MyObjectB* derivedObject = Arcadia_MyObjectB_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)derivedObject);

  Arcadia_Tests_assertEqualSize
    (
      thread,
      11,
      Arcadia_MethodDescriptor_invokeSizeValue0(thread, baseMethod, (Arcadia_Object*)baseObject)
    );
  // A nonvirtual member binds to its declaring implementation even when
  // the receiver's runtime type is derived.
  Arcadia_Tests_assertEqualSize
    (
      thread,
      11,
      Arcadia_MethodDescriptor_invokeSizeValue0
        (
          thread, nonvirtualMethod, (Arcadia_Object*)derivedObject
        )
    );
  Arcadia_MethodDescriptor const* oneArgumentMethod =
    Arcadia_TypeDescriptor_getDeclaredMethod(thread, parent, oneArgumentName);
  Arcadia_Tests_assertTrue(thread, oneArgumentMethod != NULL);
  Arcadia_Tests_assertEqualSize
    (
      thread, 16,
      Arcadia_MethodDescriptor_invokeSizeValue1
        (
          thread, oneArgumentMethod, (Arcadia_Object*)derivedObject, 5
        )
    );
  // Calling through a base declaration must use the derived adapter.
  Arcadia_Tests_assertEqualSize
    (
      thread,
      37,
      Arcadia_MethodDescriptor_invokeSizeValue0(thread, baseMethod, (Arcadia_Object*)derivedObject)
    );
  Arcadia_Tests_assertEqualSize
    (
      thread,
      37,
      Arcadia_MethodDescriptor_invokeSizeValue0(thread, childMethod, (Arcadia_Object*)derivedObject)
    );

  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_MethodDescriptor_invokeSizeValue0
      (
        thread, childMethod, (Arcadia_Object*)baseObject
      );
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentTypeInvalid, status);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }

  Arcadia_Object_unlock(thread, (Arcadia_Object*)derivedObject);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)baseObject);
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
    { "MetadataFailureAndCycles", &runMetadataFailureAndCycles },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
