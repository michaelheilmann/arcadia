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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/TypeSystem/Types.module.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/TypeSystem/Names.module.h"
#include "Arcadia/Ring1/Atoms.module.h"

#include "Arcadia/Ring1/TypeSystem/EnumerationTypeNode.module.h"
#include "Arcadia/Ring1/TypeSystem/InterfaceTypeNode.module.h"
#include "Arcadia/Ring1/TypeSystem/InternalTypeNode.module.h"
#include "Arcadia/Ring1/TypeSystem/ObjectTypeNode.module.h"
#include "Arcadia/Ring1/TypeSystem/ScalarTypeNode.module.h"

#include <assert.h>
#include <string.h>
#include <limits.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static uint32_t g_referenceCount = 0;
static TypeNodes* g_typeNodes = NULL;

static Arcadia_BooleanValue
InterfaceTypeNode_isDescendantOf
  (
    InterfaceTypeNode* self,
    InterfaceTypeNode* other
  );
static Arcadia_BooleanValue
ObjectTypeNode_implementsInterface
  (
    ObjectTypeNode* self,
    InterfaceTypeNode* other
  );
static Arcadia_BooleanValue
InterfaceSpecifications_contains
  (
    Arcadia_ObjectType_InterfaceSpecification const* interfaceSpecifications,
    Arcadia_SizeValue numberOfInterfaceSpecifications,
    Arcadia_TypeDescriptorValue interfaceType
  );
static void
InterfaceSpecifications_append
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType_InterfaceSpecification** interfaceSpecifications,
    Arcadia_SizeValue* numberOfInterfaceSpecifications,
    Arcadia_SizeValue* capacityOfInterfaceSpecifications,
    Arcadia_TypeDescriptorValue interfaceType
  );
static void
InterfaceSpecifications_appendAncestors
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType_InterfaceSpecification** interfaceSpecifications,
    Arcadia_SizeValue* numberOfInterfaceSpecifications,
    Arcadia_SizeValue* capacityOfInterfaceSpecifications,
    InterfaceTypeNode* interfaceTypeNode
  );
static Arcadia_BooleanValue
InterfaceTypeNode_isDescendantOf
  (
    InterfaceTypeNode* self,
    InterfaceTypeNode* other
  )
{
  if (self == other) {
    return Arcadia_BooleanValue_True;
  }
  for (Arcadia_SizeValue i = 0, n = self->extends.size; i < n; ++i) {
    if (InterfaceTypeNode_isDescendantOf((InterfaceTypeNode*)self->extends.elements[i], other)) {
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
ObjectTypeNode_implementsInterface
  (
    ObjectTypeNode* self,
    InterfaceTypeNode* other
  )
{
  for (ObjectTypeNode* x = self; NULL != x; x = (ObjectTypeNode*)x->parentObjectType) {
    for (Arcadia_SizeValue i = 0, n = x->interfaceImplementations.size; i < n; ++i) {
      if (InterfaceTypeNode_isDescendantOf
            (
              (InterfaceTypeNode*)x->interfaceImplementations.elements[i].interfaceType,
              other
            )
          )
      {
        return Arcadia_BooleanValue_True;
      }
    }
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
InterfaceSpecifications_contains
  (
    Arcadia_ObjectType_InterfaceSpecification const* interfaceSpecifications,
    Arcadia_SizeValue numberOfInterfaceSpecifications,
    Arcadia_TypeDescriptorValue interfaceType
  )
{
  for (Arcadia_SizeValue i = 0; i < numberOfInterfaceSpecifications; ++i) {
    if (interfaceSpecifications[i].interfaceType == interfaceType) {
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

static void
InterfaceSpecifications_append
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType_InterfaceSpecification** interfaceSpecifications,
    Arcadia_SizeValue* numberOfInterfaceSpecifications,
    Arcadia_SizeValue* capacityOfInterfaceSpecifications,
    Arcadia_TypeDescriptorValue interfaceType
  )
{
  if (InterfaceSpecifications_contains(*interfaceSpecifications, *numberOfInterfaceSpecifications, interfaceType)) {
    return;
  }
  if (*numberOfInterfaceSpecifications == *capacityOfInterfaceSpecifications) {
    if (*capacityOfInterfaceSpecifications > SIZE_MAX / 2) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_SizeValue newCapacity = *capacityOfInterfaceSpecifications ? (*capacityOfInterfaceSpecifications * 2) : 1;
    if (newCapacity > SIZE_MAX / sizeof(Arcadia_ObjectType_InterfaceSpecification)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ObjectType_InterfaceSpecification* newInterfaceSpecifications =
      realloc(*interfaceSpecifications, sizeof(Arcadia_ObjectType_InterfaceSpecification) * newCapacity);
    if (!newInterfaceSpecifications) {
      // The caller owns the old array and releases it during transaction rollback.
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
      Arcadia_Thread_jump(thread);
    }
    *interfaceSpecifications = newInterfaceSpecifications;
    *capacityOfInterfaceSpecifications = newCapacity;
  }
  (*interfaceSpecifications)[*numberOfInterfaceSpecifications].interfaceType = interfaceType;
  (*interfaceSpecifications)[*numberOfInterfaceSpecifications].dispatchSize = ((InterfaceTypeNode*)interfaceType)->dispatchSize;
  (*interfaceSpecifications)[*numberOfInterfaceSpecifications].initializeDispatch = NULL;
  ++*numberOfInterfaceSpecifications;
}

static void
InterfaceSpecifications_appendAncestors
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType_InterfaceSpecification** interfaceSpecifications,
    Arcadia_SizeValue* numberOfInterfaceSpecifications,
    Arcadia_SizeValue* capacityOfInterfaceSpecifications,
    InterfaceTypeNode* interfaceTypeNode
  )
{
  for (Arcadia_SizeValue i = 0, n = interfaceTypeNode->extends.size; i < n; ++i) {
    InterfaceTypeNode* ancestorInterfaceTypeNode = (InterfaceTypeNode*)interfaceTypeNode->extends.elements[i];
    InterfaceSpecifications_append
      (
        thread,
        interfaceSpecifications,
        numberOfInterfaceSpecifications,
        capacityOfInterfaceSpecifications,
        (Arcadia_TypeDescriptorValue)ancestorInterfaceTypeNode
      );
    InterfaceSpecifications_appendAncestors
      (
        thread,
        interfaceSpecifications,
        numberOfInterfaceSpecifications,
        capacityOfInterfaceSpecifications,
        ancestorInterfaceTypeNode
      );
  }
}

void*
Arcadia_ObjectType_getInterfaceDispatch
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType* self,
    Arcadia_InterfaceType* interfaceType
  )
{
  if (!Arcadia_TypeDescriptor_isInterfaceKind(thread, interfaceType)) {
    return NULL;
  }
  InterfaceTypeNode* requested = (InterfaceTypeNode*)interfaceType;
  // The object type chain is walked from the most derived to the least derived object type,
  // so an implementation of a subtype overrides an inherited implementation of a supertype.
  for (ObjectTypeNode* x = (ObjectTypeNode*)self; NULL != x; x = (ObjectTypeNode*)x->parentObjectType) {
    // Only an implementation of the requested interface type itself is used. The dispatch of a
    // derived interface type is an independent dispatch which never is a dispatch of one of its
    // ancestors, so an object type which implements a derived interface type implements its
    // ancestors as well, with a dispatch of their own.
    for (Arcadia_SizeValue i = 0, n = x->interfaceImplementations.size; i < n; ++i) {
      if (x->interfaceImplementations.elements[i].interfaceType == (TypeNode*)requested) {
        return x->interfaceImplementations.elements[i].dispatch;
      }
    }
  }
  return NULL;
}

static TypeNodes*
TypeNodes_create
  (
    Arcadia_Process* process
  )
{
  TypeNodes* typeNodes = NULL;
  typeNodes = Arcadia_Memory_allocateUnmanaged(Arcadia_Process_getThread(process), sizeof(TypeNodes));

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(Arcadia_Process_getThread(process), &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    typeNodes->buckets = Arcadia_Memory_allocateUnmanaged(Arcadia_Process_getThread(process), sizeof(TypeNode*) * 8);
    for (size_t i = 0, n = 8; i < n; ++i) {
      typeNodes->buckets[i] = NULL;
    }
    typeNodes->size = 0;
    typeNodes->capacity = 8;
    Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
  } else {
    Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
    Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), typeNodes);
    Arcadia_Thread_jump(Arcadia_Process_getThread(process));
  }
  return typeNodes;
}

static void
TypeNodes_destroy
  (
    Arcadia_Process* process,
    TypeNodes* typeNodes
  )
{
  while (typeNodes->size) {
    for (size_t i = 0, n = typeNodes->capacity; i < n; ++i) {
      // At each iteration, remove the leaves of the type tree.
      TypeNode** previous = &typeNodes->buckets[i];
      TypeNode* current = typeNodes->buckets[i];
      while (current) {
        if (!Arcadia_TypeDescriptor_hasChildren(Arcadia_Process_getThread(process), current)) {
          TypeNode* node = current;
          *previous = current->next;
          current = current->next;
          node->typeDestructing(NULL);
          Arcadia_Process_unlockObject(process, node);
          typeNodes->size--;
        } else {
          previous = &current->next;
          current = current->next;
        }
      }
    }
  }
  // Entries are managed by ARMS; the bucket array and container are unmanaged.
  Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), typeNodes->buckets);
  Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), typeNodes);
}

Arcadia_SizeValue
Arcadia_EnumerationType_getValueSize
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType* self
  )
{ return ((EnumerationTypeNode*)self)->valueSize; }

Arcadia_SizeValue
Arcadia_ObjectType_getValueSize
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType* self
  )
{ return ((ObjectTypeNode*)self)->valueSize; }

Arcadia_TypeDescriptorValue
Arcadia_ObjectType_getParentObjectType
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType* self
  )
{ return ((ObjectTypeNode*)self)->parentObjectType; }

Arcadia_Object_VisitCallbackFunction*
Arcadia_TypeDescriptor_getVisitObjectCallbackFunction
  (
    Arcadia_TypeDescriptorValue self
  )
{
  TypeNode const* typeNode = (TypeNode const*)self;
  if (!typeNode->typeOperations->objectTypeOperations) {
    return NULL;
  }
  return typeNode->typeOperations->objectTypeOperations->visit;
}

Arcadia_Object_DestructCallbackFunction*
Arcadia_TypeDescriptor_getDestructObjectCallbackFunction
  (
    Arcadia_TypeDescriptorValue self
  )
{
  TypeNode const* typeNode = (TypeNode const*)self;
  if (!typeNode->typeOperations->objectTypeOperations) {
    return NULL;
  }
  return typeNode->typeOperations->objectTypeOperations->destruct;
}

Arcadia_BooleanValue
Arcadia_TypeDescriptor_hasChildren
  (
    Arcadia_Thread* thread,
    Arcadia_TypeDescriptorValue self
  )
{
  for (size_t i = 0, n = g_typeNodes->capacity; i < n; ++i) {
    TypeNode* typeNode = g_typeNodes->buckets[i];
    while (typeNode) {
      if (Arcadia_TypeDescriptor_isObjectKind(thread, self)) {
        if (Arcadia_TypeDescriptor_isObjectKind(thread, typeNode)) {
          ObjectTypeNode* objectTypeNode = (ObjectTypeNode*)typeNode;
          if (Arcadia_ObjectType_getParentObjectType(thread, objectTypeNode) == (TypeNode const*)self) {
            return Arcadia_BooleanValue_True;
          }
        }
      } else if (Arcadia_TypeDescriptor_isInterfaceKind(thread, self)) {
        if (Arcadia_TypeDescriptor_isInterfaceKind(thread, typeNode)) {
          InterfaceTypeNode* interfaceTypeNode = (InterfaceTypeNode*)typeNode;
          for (Arcadia_SizeValue j = 0, m = interfaceTypeNode->extends.size; j < m; ++j) {
            if (interfaceTypeNode->extends.elements[j] == self) {
              return Arcadia_BooleanValue_True;
            }
          }
        } else if (Arcadia_TypeDescriptor_isObjectKind(thread, typeNode)) {
          // An object type which implements this interface type holds a lock on it.
          // It must be destructed before this interface type is destructed.
          if (ObjectTypeNode_implementsInterface((ObjectTypeNode*)typeNode, (InterfaceTypeNode*)self)) {
            return Arcadia_BooleanValue_True;
          }
        }
      }
      typeNode = typeNode->next;
    }
  }
  return Arcadia_BooleanValue_False;
}

Arcadia_TypeDescriptorValue
Arcadia_registerEnumerationType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t valueSize,
    Arcadia_TypeDescriptor_Operations const* typeOperations,
    Arcadia_TypeDescriptor_DestructingCallbackFunction* typeDestructing
  )
{
  size_t hash = Arcadia_Name_getHash(thread, name);
  size_t index = hash % g_typeNodes->capacity;
  for (TypeNode* typeNode = g_typeNodes->buckets[index]; NULL != typeNode; typeNode = typeNode->next) {
    if (typeNode->name == name) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TypeExists);
      Arcadia_Thread_jump(thread);
    }
  }
  TypeNode* typeNode = (TypeNode*)EnumerationTypeNode_allocate(thread);
  typeNode->kind = Arcadia_TypeKind_Enumeration;
  typeNode->name = name;
  typeNode->typeOperations = typeOperations;
  ((EnumerationTypeNode*)typeNode)->valueSize = valueSize;
  typeNode->typeDestructing = typeDestructing;

  assert(NULL != typeNode->typeOperations);

  // Establish registry ownership before publishing the identity.
  if (Arcadia_Process_lockObject(Arcadia_Thread_getProcess(thread), typeNode)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }

  typeNode->next = g_typeNodes->buckets[index];
  g_typeNodes->buckets[index] = typeNode;
  g_typeNodes->size++;

  return typeNode;
}

// Release only dependencies whose locks were successfully acquired.
// Clearing the record also makes the later ARMS finalizer harmless.
static void
InterfaceTypeNode_rollbackRegistration
  (
    InterfaceTypeNode* node
  )
{
  for (Arcadia_SizeValue i = 0; i < node->extends.size; ++i) {
    Arcadia_ARMS_unlock(node->extends.elements[i]);
  }
  free(node->extends.elements);
  node->extends.elements = NULL;
  node->extends.size = 0;
}

Arcadia_TypeDescriptorValue
Arcadia_registerInterfaceType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t dispatchSize,
    size_t numberOfExtendedInterfaces,
    Arcadia_TypeDescriptorValue const* extendedInterfaces,
    Arcadia_TypeDescriptor_DestructingCallbackFunction* typeDestructing
  )
{
  size_t hash = Arcadia_Name_getHash(thread, name);
  assert(NULL != g_typeNodes);
  size_t index = hash % g_typeNodes->capacity;
  for (TypeNode* existing = g_typeNodes->buckets[index]; NULL != existing; existing = existing->next) {
    if (existing->name == name) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TypeExists);
      Arcadia_Thread_jump(thread);
    }
  }
  if (dispatchSize < sizeof(Arcadia_InterfaceDispatch) ||
      (numberOfExtendedInterfaces && !extendedInterfaces) ||
      numberOfExtendedInterfaces > SIZE_MAX / sizeof(TypeNode*)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  TypeNode* typeNode = (TypeNode*)InterfaceTypeNode_allocate(thread);
  InterfaceTypeNode* node = (InterfaceTypeNode*)typeNode;
  typeNode->kind = Arcadia_TypeKind_Interface;
  typeNode->name = name;
  typeNode->typeOperations = NULL;
  typeNode->typeDestructing = typeDestructing;
  node->dispatchSize = dispatchSize;

  Arcadia_Process* process = Arcadia_Thread_getProcess(thread);
  if (Arcadia_Process_lockObject(process, typeNode)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    if (numberOfExtendedInterfaces) {
      node->extends.elements = malloc(sizeof(TypeNode*) * numberOfExtendedInterfaces);
      if (!node->extends.elements) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
        Arcadia_Thread_jump(thread);
      }
      for (Arcadia_SizeValue i = 0; i < numberOfExtendedInterfaces; ++i) {
        Arcadia_TypeDescriptorValue extended = extendedInterfaces[i];
        if (!extended || !Arcadia_TypeDescriptor_isInterfaceKind(thread, extended)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
          Arcadia_Thread_jump(thread);
        }
        if (Arcadia_Process_lockObject(process, extended)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
          Arcadia_Thread_jump(thread);
        }
        node->extends.elements[i] = (TypeNode*)extended;
        ++node->extends.size;
      }
    }

    // All operations that can fail have completed; publish exactly once.
    typeNode->next = g_typeNodes->buckets[index];
    g_typeNodes->buckets[index] = typeNode;
    ++g_typeNodes->size;
    Arcadia_Thread_popJumpTarget(thread);
    return typeNode;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    InterfaceTypeNode_rollbackRegistration(node);
    Arcadia_Process_unlockObject(process, typeNode);
    Arcadia_Thread_jump(thread);
  }
}

Arcadia_TypeDescriptorValue
Arcadia_registerInternalType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    Arcadia_TypeDescriptor_Operations const* typeOperations,
    Arcadia_TypeDescriptor_DestructingCallbackFunction* typeDestructing
  )
{
  size_t hash = Arcadia_Name_getHash(thread, name);
  assert(NULL != g_typeNodes);
  size_t index = hash % g_typeNodes->capacity;
  for (TypeNode* typeNode = g_typeNodes->buckets[index]; NULL != typeNode; typeNode = typeNode->next) {
    if (typeNode->name == name) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TypeExists);
      Arcadia_Thread_jump(thread);
    }
  }
  TypeNode* typeNode = (TypeNode*)InternalTypeNode_allocate(thread);
  typeNode->kind = Arcadia_TypeKind_Internal;
  typeNode->name = name;
  typeNode->typeOperations = typeOperations;
  ((InternalTypeNode*)typeNode)->valueSize = 0;
  typeNode->typeDestructing = typeDestructing;

  //assert(NULL != typeNode->typeOperations);
  //assert(NULL == typeNode->typeOperations->objectTypeOperations);

  // Establish registry ownership before publishing the identity.
  if (Arcadia_Process_lockObject(Arcadia_Thread_getProcess(thread), typeNode)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }

  typeNode->next = g_typeNodes->buckets[index];
  g_typeNodes->buckets[index] = typeNode;
  g_typeNodes->size++;

  return typeNode;
}

Arcadia_TypeDescriptorValue
Arcadia_registerObjectType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t valueSize,
    Arcadia_TypeDescriptorValue parentObjectType,
    size_t dispatchSize,
    Arcadia_TypeDescriptor_Operations const* typeOperations,
    Arcadia_TypeDescriptor_DestructingCallbackFunction* typeDestructing
  )
{
  return Arcadia_registerObjectTypeWithInterfaces
    (
      thread,
      name,
      valueSize,
      parentObjectType,
      dispatchSize,
      typeOperations,
      NULL,
      0,
      typeDestructing
    );
}

// An unpublished object-type node can be finalized by ARMS later. Reset every
// owned member after rollback so its finalizer does not release anything twice.
static void
ObjectTypeNode_rollbackRegistration
  (
    ObjectTypeNode* node
  )
{
  if (node->parentObjectType) {
    Arcadia_ARMS_unlock(node->parentObjectType);
    node->parentObjectType = NULL;
  }
  for (Arcadia_SizeValue i = 0; i < node->interfaceImplementations.size; ++i) {
    ObjectTypeNode_InterfaceImplementation* implementation = &node->interfaceImplementations.elements[i];
    if (implementation->interfaceType) {
      Arcadia_ARMS_unlock(implementation->interfaceType);
      implementation->interfaceType = NULL;
    }
    free(implementation->dispatch);
    implementation->dispatch = NULL;
  }
  free(node->interfaceImplementations.elements);
  node->interfaceImplementations.elements = NULL;
  node->interfaceImplementations.size = 0;
  free(node->dispatch);
  node->dispatch = NULL;
}

// Keep mutable transaction bookkeeping outside the setjmp stack frame.
typedef struct ObjectTypeRegistrationTransaction {
  ObjectTypeNode* node;
  Arcadia_ObjectType_InterfaceSpecification* expandedInterfaces;
  Arcadia_BooleanValue nodeLocked;
} ObjectTypeRegistrationTransaction;

Arcadia_TypeDescriptorValue
Arcadia_registerObjectTypeWithInterfaces
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t valueSize,
    Arcadia_TypeDescriptorValue parentObjectType,
    size_t dispatchSize,
    Arcadia_TypeDescriptor_Operations const* typeOperations,
    Arcadia_ObjectType_InterfaceSpecification const* interfaceSpecifications,
    size_t numberOfInterfaceSpecifications,
    Arcadia_TypeDescriptor_DestructingCallbackFunction* typeDestructing
  )
{
  size_t hash = Arcadia_Name_getHash(thread, name);
  size_t index = hash % g_typeNodes->capacity;
  for (TypeNode* existing = g_typeNodes->buckets[index]; NULL != existing; existing = existing->next) {
    if (existing->name == name) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TypeExists);
      Arcadia_Thread_jump(thread);
    }
  }
  if (!typeOperations || !typeOperations->objectTypeOperations ||
      (parentObjectType && !Arcadia_TypeDescriptor_isObjectKind(thread, parentObjectType)) ||
      (numberOfInterfaceSpecifications && !interfaceSpecifications) ||
      numberOfInterfaceSpecifications > SIZE_MAX / sizeof(Arcadia_ObjectType_InterfaceSpecification)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  ObjectTypeNode* parent = (ObjectTypeNode*)parentObjectType;
  if (parent && dispatchSize < parent->dispatchSize) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  ObjectTypeRegistrationTransaction* transaction = calloc(1, sizeof(ObjectTypeRegistrationTransaction));
  if (!transaction) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Process* process = Arcadia_Thread_getProcess(thread);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ObjectType_InterfaceSpecification const* effectiveInterfaces = interfaceSpecifications;
    Arcadia_SizeValue count = numberOfInterfaceSpecifications;
    Arcadia_SizeValue capacity = count;

    if (count) {
      transaction->expandedInterfaces = malloc(sizeof(*transaction->expandedInterfaces) * count);
      if (!transaction->expandedInterfaces) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
        Arcadia_Thread_jump(thread);
      }
      memcpy(transaction->expandedInterfaces, interfaceSpecifications, sizeof(*transaction->expandedInterfaces) * count);
      for (Arcadia_SizeValue i = 0; i < count; ++i) {
        Arcadia_TypeDescriptorValue interfaceType = transaction->expandedInterfaces[i].interfaceType;
        if (!interfaceType || !Arcadia_TypeDescriptor_isInterfaceKind(thread, interfaceType) ||
            transaction->expandedInterfaces[i].dispatchSize < ((InterfaceTypeNode*)interfaceType)->dispatchSize) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
          Arcadia_Thread_jump(thread);
        }
      }
      // This also adds independent ancestor-interface dispatch specifications.
      for (Arcadia_SizeValue i = 0; i < count; ++i) {
        InterfaceSpecifications_appendAncestors
          (
            thread,
            &transaction->expandedInterfaces,
            &count,
            &capacity,
            (InterfaceTypeNode*)transaction->expandedInterfaces[i].interfaceType
          );
      }
      effectiveInterfaces = transaction->expandedInterfaces;
    }

    transaction->node = ObjectTypeNode_allocate(thread);
    ObjectTypeNode* node = transaction->node;
    TypeNode* typeNode = (TypeNode*)node;
    typeNode->kind = Arcadia_TypeKind_Object;
    typeNode->name = name;
    typeNode->typeOperations = typeOperations;
    typeNode->typeDestructing = typeDestructing;
    node->valueSize = valueSize;
    node->dispatchSize = dispatchSize;

    // Root the unfinished node across all allocations and user dispatch callbacks.
    if (Arcadia_Process_lockObject(process, typeNode)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
      Arcadia_Thread_jump(thread);
    }
    transaction->nodeLocked = Arcadia_BooleanValue_True;

    if (parent) {
      if (Arcadia_Process_lockObject(process, parent)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
        Arcadia_Thread_jump(thread);
      }
      node->parentObjectType = (TypeNode*)parent;
    }

    if (count) {
      if (count > SIZE_MAX / sizeof(ObjectTypeNode_InterfaceImplementation)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      node->interfaceImplementations.elements = calloc(count, sizeof(ObjectTypeNode_InterfaceImplementation));
      if (!node->interfaceImplementations.elements) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
        Arcadia_Thread_jump(thread);
      }
      for (Arcadia_SizeValue i = 0; i < count; ++i) {
        Arcadia_TypeDescriptorValue interfaceType = effectiveInterfaces[i].interfaceType;
        InterfaceTypeNode* interfaceNode = (InterfaceTypeNode*)interfaceType;
        ObjectTypeNode_InterfaceImplementation* implementation = &node->interfaceImplementations.elements[i];
        size_t interfaceDispatchSize = effectiveInterfaces[i].dispatchSize;
        implementation->dispatchSize = interfaceDispatchSize;
        implementation->dispatch = malloc(interfaceDispatchSize ? interfaceDispatchSize : 1);
        if (!implementation->dispatch) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
          Arcadia_Thread_jump(thread);
        }
        node->interfaceImplementations.size = i + 1;
        memset(implementation->dispatch, 0, interfaceDispatchSize);
        if (parent) {
          void* inherited = Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)parent, (Arcadia_InterfaceType*)interfaceType);
          if (inherited) {
            memcpy(implementation->dispatch, inherited, interfaceNode->dispatchSize);
          }
        }
        if (effectiveInterfaces[i].initializeDispatch) {
          (*effectiveInterfaces[i].initializeDispatch)(thread, (Arcadia_InterfaceDispatch*)implementation->dispatch);
        }
        ((Arcadia_InterfaceDispatch*)implementation->dispatch)->objectType = typeNode;
        ((Arcadia_InterfaceDispatch*)implementation->dispatch)->interfaceType = interfaceType;
        if (Arcadia_Process_lockObject(process, interfaceType)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
          Arcadia_Thread_jump(thread);
        }
        implementation->interfaceType = (TypeNode*)interfaceType;
      }
    }

    node->dispatch = malloc(dispatchSize ? dispatchSize : 1);
    if (!node->dispatch) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
      Arcadia_Thread_jump(thread);
    }
    memset(node->dispatch, 0, dispatchSize);
    if (parent && parent->dispatch) {
      memcpy(node->dispatch, parent->dispatch, parent->dispatchSize);
    }
    if (typeOperations->objectTypeOperations->initializeDispatch && dispatchSize >= sizeof(Arcadia_ObjectDispatch)) {
      (*typeOperations->objectTypeOperations->initializeDispatch)(thread, node->dispatch);
    }
    if (dispatchSize >= sizeof(Arcadia_ObjectDispatch)) {
      ((Arcadia_ObjectDispatch*)node->dispatch)->type = typeNode;
    }

    assert(NULL != typeNode->typeOperations->objectTypeOperations->construct);
    // User dispatch callbacks may have registered a type with this name.
    for (TypeNode* existing = g_typeNodes->buckets[index]; NULL != existing; existing = existing->next) {
      if (existing->name == name) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_TypeExists);
        Arcadia_Thread_jump(thread);
      }
    }
    // No failure-capable work follows publication.
    typeNode->next = g_typeNodes->buckets[index];
    g_typeNodes->buckets[index] = typeNode;
    ++g_typeNodes->size;
    Arcadia_Thread_popJumpTarget(thread);
    free(transaction->expandedInterfaces);
    free(transaction);
    return typeNode;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (transaction->node) {
      ObjectTypeNode_rollbackRegistration(transaction->node);
      if (transaction->nodeLocked) {
        Arcadia_Process_unlockObject(process, transaction->node);
      }
    }
    free(transaction->expandedInterfaces);
    free(transaction);
    Arcadia_Thread_jump(thread);
  }
}

Arcadia_TypeDescriptorValue
Arcadia_registerScalarType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    Arcadia_TypeDescriptor_Operations const* typeOperations,
    Arcadia_TypeDescriptor_DestructingCallbackFunction* typeDestructing
  )
{
  size_t hash = Arcadia_Name_getHash(thread, name);
  size_t index = hash % g_typeNodes->capacity;
  for (TypeNode* typeNode = g_typeNodes->buckets[index]; NULL != typeNode; typeNode = typeNode->next) {
    if (typeNode->name == name) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TypeExists);
      Arcadia_Thread_jump(thread);
    }
  }
  TypeNode* typeNode = (TypeNode*)ScalarTypeNode_allocate(thread);
  typeNode->kind = Arcadia_TypeKind_Scalar;
  typeNode->name = name;
  typeNode->typeOperations = typeOperations;
  ((ScalarTypeNode*)typeNode)->valueSize = 0;
  typeNode->typeDestructing = typeDestructing;

  assert(NULL != typeNode->typeOperations);
  assert(NULL == typeNode->typeOperations->objectTypeOperations);

  // Establish registry ownership before publishing the identity.
  if (Arcadia_Process_lockObject(Arcadia_Thread_getProcess(thread), typeNode)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }

  typeNode->next = g_typeNodes->buckets[index];
  g_typeNodes->buckets[index] = typeNode;
  g_typeNodes->size++;

  return typeNode;
}

Arcadia_BooleanValue
Arcadia_TypeDescriptor_isDescendantType
  (
    Arcadia_Thread* thread,
    Arcadia_TypeDescriptorValue self,
    Arcadia_TypeDescriptorValue other
  )
{
  TypeNode* a = (TypeNode*)self;
  TypeNode* b = (TypeNode*)other;
  if (a->kind == b->kind) {
    if (a->kind == Arcadia_TypeKind_Object) {
      ObjectTypeNode* x = (ObjectTypeNode*)a,
                    * y = (ObjectTypeNode*)b;
      do {
        if (x == y) {
          return Arcadia_BooleanValue_True;
        }
        x = (ObjectTypeNode*)x->parentObjectType;
      } while (NULL != x);
      return Arcadia_BooleanValue_False;
    } else if (a->kind == Arcadia_TypeKind_Interface) {
      return InterfaceTypeNode_isDescendantOf((InterfaceTypeNode*)a, (InterfaceTypeNode*)b);
    } else {
      return a == b;
    }
  } else if (a->kind == Arcadia_TypeKind_Object && b->kind == Arcadia_TypeKind_Interface) {
    return ObjectTypeNode_implementsInterface((ObjectTypeNode*)a, (InterfaceTypeNode*)b);
  } else {
    return Arcadia_BooleanValue_False;
  }
}

Arcadia_TypeDescriptorValue
Arcadia_getTypeDescriptorByName
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name
  )
{
  size_t hash = Arcadia_Name_getHash(thread, name);
  size_t index = hash % g_typeNodes->capacity;
  for (TypeNode* typeNode = g_typeNodes->buckets[index]; NULL != typeNode; typeNode = typeNode->next) {
    if (typeNode->name == name) {
      return typeNode;
    }
  }
  Arcadia_logf(Arcadia_LogFlags_Error, "type `%.*s` not found\n", Arcadia_Name_getNumberOfBytes(thread, name) > INT_MAX ? INT_MAX : Arcadia_Name_getNumberOfBytes(thread, name), name);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_TypeNotExists);
  Arcadia_Thread_jump(thread);
}

Arcadia_ObjectDispatch*
Arcadia_ObjectType_getDispatch
  (
    Arcadia_ObjectType* type
  )
{
  assert(NULL != type);
  ObjectTypeNode* typeNode = (ObjectTypeNode*)type;
  assert(NULL != typeNode->dispatch);
  return typeNode->dispatch;
}

Arcadia_TypeDescriptor_Operations const*
Arcadia_TypeDescriptor_getOperations
  (
    Arcadia_TypeDescriptorValue type
  )
{
  TypeNode* typeNode = (TypeNode*)type;
  return typeNode->typeOperations;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static TypeNode* g_memoryType = NULL;

static void
memoryTypeDestructing
  (
    void* context
  )
{ g_memoryType = NULL; }

Arcadia_TypeDescriptorValue
_Arcadia_Memory_getType
  (
    Arcadia_Thread* thread
  )
{
  if (!g_memoryType) {
    g_memoryType = Arcadia_registerInternalType(thread, Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Memory", sizeof(u8"Arcadia.Memory") - 1), NULL, &memoryTypeDestructing);
  }
  return g_memoryType;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static TypeNode* g_typeType = NULL;

static void
typeTypeDestructing
  (
    void* context
  )
{
  g_typeType = NULL;
}

Arcadia_TypeDescriptorValue
_Arcadia_TypeDescriptor_getType
  (
    Arcadia_Thread* thread
  )
{
  if (!g_typeType) {
    g_typeType = Arcadia_registerInternalType(thread, Arcadia_Names_getOrCreateName(thread, u8"Arcadia.TypeDescriptor", sizeof(u8"Arcadia.TypeDescriptor") - 1), NULL, &typeTypeDestructing);
  }
  return g_typeType;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static TypeNode* g_atomType = NULL;

static void
atomTypeDestructing
  (
    void* context
  )
{
  g_atomType = NULL;
}

Arcadia_TypeDescriptorValue
_Arcadia_AtomValue_getType
  (
    Arcadia_Thread* thread
  )
{
  if (!g_atomType) {
    g_atomType = Arcadia_registerInternalType(thread, Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Atom", sizeof(u8"Arcadia.Atom") - 1), NULL, &atomTypeDestructing);
  }
  return g_atomType;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_DefineModule("Arcadia.Types", Arcadia_Types);

static void
_Arcadia_Types_onPreMark
  (
    Arcadia_Thread* thread,
    bool purgeCache
  )
{/*Intentionally empty.*/}

static void
_Arcadia_Types_onFinalize
  (
    Arcadia_Thread* thread,
    size_t* destroyed
  )
{
  *destroyed = 0;
}

static void
startupDependencies
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Process* process = Arcadia_Thread_getProcess(thread);
  Arcadia_Names_getModule(process)->onStartUp(thread);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Atoms_getModule(process)->onStartUp(thread);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Names_getModule(process)->onShutDown(thread);
    Arcadia_Thread_jump(thread);
  }
}

static void
shutdownDependencies
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Process* process = Arcadia_Thread_getProcess(thread);
  Arcadia_Atoms_getModule(process)->onShutDown(thread);
  Arcadia_Names_getModule(process)->onShutDown(thread);
}

static void
_Arcadia_Types_onStartUp
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Process* process = Arcadia_Thread_getProcess(thread);
  if (g_referenceCount == UINT32_MAX) {
    Arcadia_logf(Arcadia_LogFlags_Error, "corrupted reference counter\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (g_referenceCount == 0) {
    startupDependencies(thread);
    g_typeNodes = TypeNodes_create(process);
    if (!g_typeNodes) {
      shutdownDependencies(thread);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_Process_addArenaPreMarkCallback(process, &_Arcadia_Types_onPreMark);
      Arcadia_Process_addArenaVisitCallback(process, &_Arcadia_Types_onVisit);
      Arcadia_Process_addArenaFinalizeCallback(process, &_Arcadia_Types_onFinalize);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
      Arcadia_Process_removeArenaFinalizeCallback(process, &_Arcadia_Types_onFinalize);
      Arcadia_Process_removeArenaVisitCallback(process, &_Arcadia_Types_onVisit);
      Arcadia_Process_removeArenaPreMarkCallback(process, &_Arcadia_Types_onPreMark);
      TypeNodes_destroy(process, g_typeNodes);
      g_typeNodes = NULL;
      Arcadia_Status status = Arcadia_Process_runARMS(process, true);
      if (status) {
        /*Intentionally empty.*/
      }
      shutdownDependencies(thread);
      Arcadia_Thread_jump(thread);
    }
  }
  g_referenceCount++;
}

static void
_Arcadia_Types_onShutDown
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Process* process = Arcadia_Thread_getProcess(thread);
  if (g_referenceCount == 0) {
    Arcadia_logf(Arcadia_LogFlags_Error, "corrupted reference counter\n");
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  g_referenceCount--;
  if (0 == g_referenceCount) {
    Arcadia_Status status;
    status = Arcadia_Process_runARMS(process, true);
    if (status) {
      /* Intentionally empty.*/
    }
    status = Arcadia_Process_runARMS(process, true);
    if (status) {
      /* Intentionally empty.*/
    }
    TypeNodes_destroy(process, g_typeNodes);
    g_typeNodes = NULL;
    Arcadia_Process_removeArenaFinalizeCallback(process, &_Arcadia_Types_onFinalize);
    Arcadia_Process_removeArenaVisitCallback(process, &_Arcadia_Types_onVisit);
    Arcadia_Process_removeArenaPreMarkCallback(process, &_Arcadia_Types_onPreMark);
    shutdownDependencies(thread);
  }
}

static void
_Arcadia_Types_onVisit
  (
    Arcadia_Thread* thread
  )
{/*Intentionally empty.*/}
