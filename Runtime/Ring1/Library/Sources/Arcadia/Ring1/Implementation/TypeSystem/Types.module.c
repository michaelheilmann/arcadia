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
#include "Arcadia/Ring1/Implementation/TypeSystem/Types.module.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/Names.module.h"
#include "Arcadia/Ring1/Implementation/Atoms.module.h"

#include "Arcadia/Ring1/Implementation/TypeSystem/EnumerationTypeNode.module.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/InterfaceTypeNode.module.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/InternalTypeNode.module.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/ObjectTypeNode.module.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/ScalarTypeNode.module.h"

#include <assert.h>
#include <string.h>
#include <limits.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static uint32_t g_referenceCount = 0;
static TypeNodes* g_typeNodes = NULL;

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
        if (!Arcadia_Type_hasChildren(Arcadia_Process_getThread(process), current)) {
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

Arcadia_TypeValue
Arcadia_ObjectType_getParentObjectType
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType* self
  )
{ return ((ObjectTypeNode*)self)->parentObjectType; }

Arcadia_Object_VisitCallbackFunction*
Arcadia_Type_getVisitObjectCallbackFunction
  (
    Arcadia_TypeValue self
  )
{
  TypeNode const* typeNode = (TypeNode const*)self;
  if (!typeNode->typeOperations->objectTypeOperations) {
    return NULL;
  }
  return typeNode->typeOperations->objectTypeOperations->visit;
}

Arcadia_Object_DestructCallbackFunction*
Arcadia_Type_getDestructObjectCallbackFunction
  (
    Arcadia_TypeValue self
  )
{
  TypeNode const* typeNode = (TypeNode const*)self;
  if (!typeNode->typeOperations->objectTypeOperations) {
    return NULL;
  }
  return typeNode->typeOperations->objectTypeOperations->destruct;
}

Arcadia_BooleanValue
Arcadia_Type_hasChildren
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self
  )
{
  if (!Arcadia_Type_isObjectKind(thread, self)) {
    return Arcadia_BooleanValue_False;
  }
  for (size_t i = 0, n = g_typeNodes->capacity; i < n; ++i) {
    TypeNode* typeNode = g_typeNodes->buckets[i];
    while (typeNode) {
      if (Arcadia_Type_isObjectKind(thread, typeNode)) {
        ObjectTypeNode* objectTypeNode = (ObjectTypeNode*)typeNode;
        if (Arcadia_ObjectType_getParentObjectType(thread, objectTypeNode) == (TypeNode const*)self) {
          return Arcadia_BooleanValue_True;
        }
      }
      typeNode = typeNode->next;
    }
  }
  return Arcadia_BooleanValue_False;
}

Arcadia_TypeValue
Arcadia_registerEnumerationType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t valueSize,
    Arcadia_Type_Operations const* typeOperations,
    Arcadia_Type_TypeDestructingCallbackFunction* typeDestructing
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

  typeNode->next = g_typeNodes->buckets[index];
  g_typeNodes->buckets[index] = typeNode;
  g_typeNodes->size++;

  Arcadia_Process_lockObject(Arcadia_Thread_getProcess(thread), typeNode); /* @todo Can raise due to allocation failure. Can we built-in a guarantee that at least one lock is always available? */

  return typeNode;
}

Arcadia_TypeValue
Arcadia_registerInterfaceType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t dispatchSize,
    Arcadia_Type_TypeDestructingCallbackFunction* typeDestructing
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
  TypeNode* typeNode = (TypeNode*)InterfaceTypeNode_allocate(thread);
  typeNode->kind = Arcadia_TypeKind_Interface;
  typeNode->name = name;
  typeNode->typeOperations = NULL;
  typeNode->typeDestructing = typeDestructing;

  typeNode->next = g_typeNodes->buckets[index];
  g_typeNodes->buckets[index] = typeNode;
  g_typeNodes->size++;

  Arcadia_Process_lockObject(Arcadia_Thread_getProcess(thread), typeNode);

  return typeNode;
}

Arcadia_TypeValue
Arcadia_registerInternalType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    Arcadia_Type_Operations const* typeOperations,
    Arcadia_Type_TypeDestructingCallbackFunction* typeDestructing
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

  typeNode->next = g_typeNodes->buckets[index];
  g_typeNodes->buckets[index] = typeNode;
  g_typeNodes->size++;

  Arcadia_Process_lockObject(Arcadia_Thread_getProcess(thread), typeNode);

  return typeNode;
}

Arcadia_TypeValue
Arcadia_registerObjectType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t valueSize,
    Arcadia_TypeValue parentObjectType,
    size_t dispatchSize,
    Arcadia_Type_Operations const* typeOperations,
    Arcadia_Type_TypeDestructingCallbackFunction* typeDestructing
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
  if (!typeOperations->objectTypeOperations) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); /*@tood Add Arcadia_Status_NoObjectType.*/
    Arcadia_Thread_jump(thread);
  }
  if (parentObjectType && !Arcadia_Type_isObjectKind(thread, parentObjectType)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); /*@tood Add Arcadia_Status_NoObjectType.*/
    Arcadia_Thread_jump(thread);
  }
  ObjectTypeNode* parentObjectTypeNode = (ObjectTypeNode*)parentObjectType;
  TypeNode* typeNode = (TypeNode*)ObjectTypeNode_allocate(thread);
  typeNode->kind = Arcadia_TypeKind_Object;
  typeNode->name = name;
  typeNode->typeOperations = typeOperations;
  ((ObjectTypeNode*)typeNode)->parentObjectType = parentObjectType;
  ((ObjectTypeNode*)typeNode)->valueSize = valueSize;
  typeNode->typeDestructing = typeDestructing;

  ((ObjectTypeNode*)typeNode)->dispatch = NULL;
  ((ObjectTypeNode*)typeNode)->dispatchSize = dispatchSize;
  ((ObjectTypeNode*)typeNode)->initializeDispatch = NULL;

  // Validate relation with parent object type if any.
  if (parentObjectTypeNode) {
    if (Arcadia_Process_lockObject(Arcadia_Thread_getProcess(thread), parentObjectTypeNode)) {
      // Can fail due to allocation failure or lock count overflow.
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: <error>\n", __FILE__, __LINE__);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
      Arcadia_Thread_jump(thread);
    }
    if (dispatchSize < parentObjectTypeNode->dispatchSize) {
      // Can fail due to invalid inputs.
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
      Arcadia_Thread_jump(thread);
    }
  }

  // Initialize dispatch.
  ((ObjectTypeNode*)typeNode)->dispatch = malloc(dispatchSize);
  if (!((ObjectTypeNode*)typeNode)->dispatch) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  memset(((ObjectTypeNode*)typeNode)->dispatch, 0, dispatchSize);
  if (parentObjectTypeNode) {
    if (parentObjectTypeNode->dispatch) {
      memcpy(((ObjectTypeNode*)typeNode)->dispatch, parentObjectTypeNode->dispatch, parentObjectTypeNode->dispatchSize);
    }
  }
  if (typeOperations->objectTypeOperations->initializeDispatch) {
    (*typeOperations->objectTypeOperations->initializeDispatch)(thread, ((ObjectTypeNode*)typeNode)->dispatch);
    ((Arcadia_ObjectDispatch*)((ObjectTypeNode*)typeNode)->dispatch)->type = typeNode;
  }

  assert(NULL != typeNode->typeOperations);
  assert(NULL != typeNode->typeOperations->objectTypeOperations);
  assert(NULL != typeNode->typeOperations->objectTypeOperations->construct);

  typeNode->next = g_typeNodes->buckets[index];
  g_typeNodes->buckets[index] = typeNode;
  g_typeNodes->size++;

  Arcadia_Process_lockObject(Arcadia_Thread_getProcess(thread), typeNode); /* @todo Can raise due to allocation failure. Can we built-in a guarantee that at least one lock is always available? */

  return typeNode;
}

Arcadia_TypeValue
Arcadia_registerScalarType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    Arcadia_Type_Operations const* typeOperations,
    Arcadia_Type_TypeDestructingCallbackFunction* typeDestructing
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

  typeNode->next = g_typeNodes->buckets[index];
  g_typeNodes->buckets[index] = typeNode;
  g_typeNodes->size++;

  Arcadia_Process_lockObject(Arcadia_Thread_getProcess(thread), typeNode);

  return typeNode;
}

Arcadia_BooleanValue
Arcadia_Type_isDescendantType
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self,
    Arcadia_TypeValue other
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
    } else {
      return a == b;
    }
  } else {
    return Arcadia_BooleanValue_False;
  }
}

Arcadia_TypeValue
Arcadia_getType
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

Arcadia_Type_Operations const*
Arcadia_Type_getOperations
  (
    Arcadia_TypeValue type
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

Arcadia_TypeValue
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

Arcadia_TypeValue
_Arcadia_Type_getType
  (
    Arcadia_Thread* thread
  )
{
  if (!g_typeType) {
    g_typeType = Arcadia_registerInternalType(thread, Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Type", sizeof(u8"Arcadia.Type") - 1), NULL, &typeTypeDestructing);
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

Arcadia_TypeValue
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
    TypeNodes_destroy(process, g_typeNodes);
    g_typeNodes = NULL;
    Arcadia_Process_removeArenaFinalizeCallback(process, &_Arcadia_Types_onFinalize);
    Arcadia_Process_removeArenaVisitCallback(process, &_Arcadia_Types_onVisit);
    Arcadia_Process_removeArenaPreMarkCallback(process, &_Arcadia_Types_onPreMark);
    status = Arcadia_Process_runARMS(process, true);
    if (status) {
      /* Intentionally empty.*/
    }
    shutdownDependencies(thread);
  }
}

static void
_Arcadia_Types_onVisit
  (
    Arcadia_Thread* thread
  )
{/*Intentionally empty.*/}
