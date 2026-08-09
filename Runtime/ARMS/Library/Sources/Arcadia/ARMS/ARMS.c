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

#include "Arcadia/ARMS/ARMS.h"

#include "Arcadia/ARMS/Internal/Common.h"
#include "Arcadia/ARMS/Internal/DefaultMemoryManager.h"
#include "Arcadia/ARMS/Internal/MemoryManager.private.h"
#include "Arcadia/ARMS/Internal/ReferenceCounter.h"
#include "Arcadia/ARMS/Internal/SlabMemoryManager.h"
#include "Arcadia/ARMS/Internal/Statistics.h"

#include "Arcadia/ARMS/Internal/TypeName.h" // The "type name" module.
#include "Arcadia/ARMS/Internal/NotifyDestroy.h" // The "notify destroy" module.

#include "Arcadia/ARMS/Internal/Tag.h"

// malloc, free, realloc
#include <malloc.h>
// memcmp, memcpy
#include <string.h>
// uint8_t
#include <stdint.h>
// bool, true, false
#include <stdbool.h>
// assert()
#include <assert.h>

#if defined(Arcadia_ARMS_Configuration_WithLocks) && 1 == Arcadia_ARMS_Configuration_WithLocks

// INT_MAX
#include <limits.h>

#endif

typedef struct ARMS_Type ARMS_Type;


#if defined(Arcadia_ARMS_Configuration_WithLocks) && 1 == Arcadia_ARMS_Configuration_WithLocks

typedef struct ARMS_Lock ARMS_Lock;

#endif

struct ARMS_Type {
  ARMS_Type* next;
  Arcadia_ARMS_TypeName* typeName;
  void* context;
  Arcadia_ARMS_TypeRemovedCallbackFunction* typeRemoved;
  Arcadia_ARMS_VisitCallbackFunction* visit;
  Arcadia_ARMS_FinalizeCallbackFunction* finalize;
};

#if defined(Arcadia_ARMS_Configuration_WithLocks) && 1 == Arcadia_ARMS_Configuration_WithLocks

struct ARMS_Lock {
  ARMS_Lock* next;
  int count;
  void* object;
};

#endif

static Arcadia_ARMS_ReferenceCounter g_referenceCount = 0;

static ARMS_Type* g_types = NULL;
static Arcadia_ARMS_Tag* g_allObjects = NULL;
static Arcadia_ARMS_Tag* g_grayObjects = NULL;

#if defined(Arcadia_ARMS_Configuration_WithLocks) && 1 == Arcadia_ARMS_Configuration_WithLocks

static ARMS_Lock* g_locks = NULL;

#endif

static Arcadia_ARMS_DefaultMemoryManager* g_defaultMemoryManager = NULL;
static Arcadia_ARMS_SlabMemoryManager* g_slabMemoryManager = NULL;

Arcadia_ARMS_Status
Arcadia_ARMS_startup
  (
  )
{
  if (g_referenceCount == Arcadia_ARMS_ReferenceCounter_Maximum) {
    // Cannot increment further.
    return Arcadia_ARMS_Status_OperationInvalid;
  }
  if (!g_referenceCount) {
    switch (Arcadia_ARMS_DefaultMemoryManager_create(&g_defaultMemoryManager)) {
      case Arcadia_ARMS_MemoryManagerStartupShutdown_Status_AllocationFailed: {
        return Arcadia_ARMS_Status_AllocationFailed;
      } break;
      case Arcadia_ARMS_MemoryManagerStartupShutdown_Status_ArgumentValueInvalid: {
        return Arcadia_ARMS_Status_ArgumentValueInvalid;
      } break;
      case Arcadia_ARMS_MemoryManagerStartupShutdown_Status_Success: {
        /* Intentionally empty.*/
      } break;
      default: {
        return Arcadia_ARMS_Status_EnvironmentFailed;
      } break;
    };
    switch (Arcadia_ARMS_SlabMemoryManager_create(&g_slabMemoryManager)) {
      case Arcadia_ARMS_MemoryManagerStartupShutdown_Status_AllocationFailed: {
        Arcadia_ARMS_MemoryManager_destroy((Arcadia_ARMS_MemoryManager*)g_defaultMemoryManager);
        g_defaultMemoryManager = NULL;
        return Arcadia_ARMS_Status_AllocationFailed;
      } break;
      case Arcadia_ARMS_MemoryManagerStartupShutdown_Status_ArgumentValueInvalid: {
        Arcadia_ARMS_MemoryManager_destroy((Arcadia_ARMS_MemoryManager*)g_defaultMemoryManager);
        g_defaultMemoryManager = NULL;
        return Arcadia_ARMS_Status_ArgumentValueInvalid;
      } break;
      case Arcadia_ARMS_MemoryManagerStartupShutdown_Status_Success: {
        /* Intentionally empty.*/
      } break;
      default: {
        Arcadia_ARMS_MemoryManager_destroy((Arcadia_ARMS_MemoryManager*)g_defaultMemoryManager);
        g_defaultMemoryManager = NULL;
        return Arcadia_ARMS_Status_EnvironmentFailed;
      } break;
    };

    if (Arcadia_ARMS_TypeNameModule_startup()) {
      Arcadia_ARMS_MemoryManager_destroy((Arcadia_ARMS_MemoryManager*)g_slabMemoryManager);
      g_slabMemoryManager = NULL;
      Arcadia_ARMS_MemoryManager_destroy((Arcadia_ARMS_MemoryManager*)g_defaultMemoryManager);
      g_defaultMemoryManager = NULL;
      return Arcadia_ARMS_Status_EnvironmentFailed;
    }

    g_types = NULL;

    g_allObjects = NULL;
    g_grayObjects = NULL;

  #if defined(Arcadia_ARMS_Configuration_WithLocks) && 1 == Arcadia_ARMS_Configuration_WithLocks
    g_locks = NULL;
  #endif

  #if defined(Arcadia_ARMS_Configuration_WithNotifyDestroy) && 1 == Arcadia_ARMS_Configuration_WithNotifyDestroy
    if (Arcadia_ARMS_NotifyDestroyModule_startup()) {
      Arcadia_ARMS_TypeNameModule_shutdown();
      Arcadia_ARMS_MemoryManager_destroy((Arcadia_ARMS_MemoryManager*)g_slabMemoryManager);
      g_slabMemoryManager = NULL;
      Arcadia_ARMS_MemoryManager_destroy((Arcadia_ARMS_MemoryManager*)g_defaultMemoryManager);
      g_defaultMemoryManager = NULL;
      return Arcadia_ARMS_Status_EnvironmentFailed;
    }
  #endif
  }
  g_referenceCount++;
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_shutdown
  (
  )
{
  if (Arcadia_ARMS_ReferenceCounter_Minimum == g_referenceCount) {
    // Cannot decrement further.
    return Arcadia_ARMS_Status_OperationInvalid;
  }
  int32_t referenceCount = g_referenceCount - 1;
  if (!referenceCount) {
    if (g_grayObjects || g_allObjects) {
      Cxx_fatalError();
    }
  #if defined(Arcadia_ARMS_Configuration_WithLocks) && 1 == Arcadia_ARMS_Configuration_WithLocks
    if (g_locks) {
      Cxx_fatalError();
    }
  #endif
  #if defined(Arcadia_ARMS_Configuration_WithNotifyDestroy) && 1 == Arcadia_ARMS_Configuration_WithNotifyDestroy
    Arcadia_ARMS_NotifyDestroyModule_shutdown();
  #endif
    while (g_types) {
      ARMS_Type* type = g_types;
      g_types = type->next;
      if (type->typeRemoved) {
        const Arcadia_ARMS_Natural8* bytes; Arcadia_ARMS_Size numberOfBytes;
        Arcadia_ARMS_TypeName_getData(type->typeName, &bytes, &numberOfBytes);
        type->typeRemoved(type->context, bytes, numberOfBytes);
      }
      free(type);
      type = NULL;
    }
    Arcadia_ARMS_TypeNameModule_shutdown();
    Arcadia_ARMS_MemoryManager_destroy((Arcadia_ARMS_MemoryManager*)g_slabMemoryManager);
    g_slabMemoryManager = NULL;
    Arcadia_ARMS_MemoryManager_destroy((Arcadia_ARMS_MemoryManager*)g_defaultMemoryManager);
    g_defaultMemoryManager = NULL;
  }
  g_referenceCount = referenceCount;
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_addType
  (
    Arcadia_ARMS_Natural8 const* name,
    Arcadia_ARMS_Size nameLength,
    void* context,
    Arcadia_ARMS_TypeRemovedCallbackFunction* typeRemoved,
    Arcadia_ARMS_VisitCallbackFunction* visit,
    Arcadia_ARMS_FinalizeCallbackFunction* finalize
  )
{
  if (!name) {
    return Arcadia_ARMS_Status_ArgumentValueInvalid;
  }

  Arcadia_ARMS_TypeName* typeName = NULL;
  Arcadia_ARMS_Status status = Arcadia_ARMS_TypeName_getOrCreate(&typeName, name, nameLength);
  if (status) {
    return status;
  }

  for (ARMS_Type* type = g_types; NULL != type; type = type->next) {
    if (type->typeName == typeName){
      return Arcadia_ARMS_Status_TypeExists;
    }
  }

  ARMS_Type* type = malloc(sizeof(ARMS_Type));
  if (!type) {
    return Arcadia_ARMS_Status_AllocationFailed;
  }

  type->typeName = typeName;
  type->context = context;
  type->typeRemoved = typeRemoved;
  type->visit = visit;
  type->finalize = finalize;

  type->next = g_types;
  g_types = type;

  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_allocate
  (
    void** pObject,
    Arcadia_ARMS_Natural8 const* name,
    Arcadia_ARMS_Size nameLength,
    Arcadia_ARMS_Size size
  )
{
  if (!pObject || !name) {
    return Arcadia_ARMS_Status_ArgumentValueInvalid;
  }

  Arcadia_ARMS_TypeName* typeName = NULL;
  Arcadia_ARMS_Status status = Arcadia_ARMS_TypeName_getOrCreate(&typeName, name, nameLength);
  if (status) {
    return status;
  }

  ARMS_Type* type;
  for (type = g_types; NULL != type; type = type->next) {
    if (type->typeName == typeName) {
      break;
    }
  }
  if (!type) {
    return Arcadia_ARMS_Status_TypeNotExists;
  }
  if (SIZE_MAX - sizeof(Arcadia_ARMS_Tag) < size) {
    return Arcadia_ARMS_Status_ArgumentValueInvalid;
  }
  Arcadia_ARMS_Tag* object = malloc(sizeof(Arcadia_ARMS_Tag) + size);
  if (!object) {
    return Arcadia_ARMS_Status_AllocationFailed;
  }
  object->flags = Arcadia_ARMS_TagFlags_White;
  object->type = type;
  object->allNext = g_allObjects;
  g_allObjects = object;
  *pObject = object + 1;
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_run
  (
    Arcadia_ARMS_RunStatistics* statistics
  )
{
  Arcadia_ARMS_RunStatistics statistics0 = Arcadia_ARMS_RunStatistics_StaticInitializer();
  // Premark phase:
  // Add all locked objects to the gray list.
  // Also remove locks with a lock count of 0.
  ARMS_Lock** previousLock = &g_locks;
  ARMS_Lock* currentLock = g_locks;
  while (currentLock) {
    assert(NULL != currentLock->object);
    if (currentLock->count) {
      Arcadia_ARMS_visit(currentLock->object);
      statistics0.locked++;
      previousLock = &currentLock->next;
      currentLock = currentLock->next;
    } else {
      ARMS_Lock* lock = currentLock;
      *previousLock = currentLock->next;
      currentLock = currentLock->next;
      free(lock);
    }
  }
  // Mark phase:
  // As long as the gray list is not empty: Remove an object from the gray list, color it black, visit it.
  while (g_grayObjects) {
    Arcadia_ARMS_Tag* object = g_grayObjects;
    g_grayObjects = object->grayNext;
    Arcadia_ARMS_Tag_setBlack(object);
    if (object->type->visit) {
      object->type->visit(object->type->context, object + 1);
    }
  }
  // Sweep phase:
  // Iterate over all objects in the universe.
  // If an object is black: color it white.
  // If an object is white: Remove, finalize, and deallocate the object.
  Arcadia_ARMS_Tag** previousObject = &g_allObjects;
  Arcadia_ARMS_Tag* currentObject = g_allObjects;
  while (currentObject) {
    if (Arcadia_ARMS_Tag_isWhite(currentObject)) {
      Arcadia_ARMS_Tag* deadObject = currentObject;
      *previousObject = currentObject->allNext;
      currentObject = currentObject->allNext;
    #if defined(Arcadia_ARMS_Configuration_WithNotifyDestroy) && 1 == Arcadia_ARMS_Configuration_WithNotifyDestroy
      Arcadia_ARMS_NotifyDestroyModule_notifyDestroy(deadObject + 1);
    #endif
      if (deadObject->type->finalize) {
        deadObject->type->finalize(deadObject->type->context, deadObject + 1);
        statistics0.finalized++;
      }
      statistics0.dead++;
      free(deadObject);
    } else {
      Arcadia_ARMS_Tag_setWhite(currentObject);
      statistics0.live++;
      previousObject = &currentObject->allNext;
      currentObject = currentObject->allNext;
    }
  }
  *statistics = statistics0;
  return Arcadia_ARMS_Status_Success;
}

void
Arcadia_ARMS_visit
  (
    void* object
  )
{
  Arcadia_ARMS_Tag* tag = ((Arcadia_ARMS_Tag*)object) - 1;
  if (Arcadia_ARMS_Tag_isWhite(tag)) {
    if (tag->type->visit) {
      Arcadia_ARMS_Tag_setGray(tag);
      tag->grayNext = g_grayObjects;
      g_grayObjects = tag;
    } else {
      Arcadia_ARMS_Tag_setBlack(tag);
    }
  }
}

#if defined(Arcadia_ARMS_Configuration_WithLocks) && 1 == Arcadia_ARMS_Configuration_WithLocks

Arcadia_ARMS_Status
Arcadia_ARMS_lock
  (
    void* object
  )
{
  for (ARMS_Lock* lock = g_locks; NULL != lock; lock = lock->next) {
    if (lock->object == object) {
      if (lock->count == INT_MAX) {
         return Arcadia_ARMS_Status_OperationInvalid;
      } else {
         lock->count++;
         return Arcadia_ARMS_Status_Success;
      }
    }
  }
  ARMS_Lock *lock = malloc(sizeof(ARMS_Lock));
  if (!lock) {
    return Arcadia_ARMS_Status_AllocationFailed;
  }
  lock->next = g_locks;
  g_locks = lock;
  lock->object = object;
  lock->count = 1;
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_unlock
  (
    void* object
  )
{
  for (ARMS_Lock* lock = g_locks; NULL != lock; lock = lock->next) {
    if (lock->object == object) {
      if (lock->count == 0) {
         return Arcadia_ARMS_Status_OperationInvalid;
      } else {
         lock->count--;
         return Arcadia_ARMS_Status_Success;
      }
    }
  }
  return Arcadia_ARMS_Status_OperationInvalid;
}

#endif

#if defined(Arcadia_ARMS_Configuration_WithBarriers) && 1 == Arcadia_ARMS_Configuration_WithBarriers

void
Arcadia_ARMS_forwardBarrier
  (
    void* source,
    void* target
  )
{
  assert(NULL != source && NULL != target);
  Arcadia_ARMS_Tag* sourceTag = ((Arcadia_ARMS_Tag*)source) - 1;
  Arcadia_ARMS_Tag* targetTag = ((Arcadia_ARMS_Tag*)target) - 1;
  if (Arcadia_ARMS_Tag_isBlack(sourceTag) && Arcadia_ARMS_Tag_isWhite(targetTag)) {
    Arcadia_ARMS_Tag_setGray(targetTag); // the GC is not running; we color the object gray.
                                         // if the incremental GC steps, then we have three kinds of objects, black, gray, and white in the primary GC list.
                                         // we iterate once over this list and ice out gray objects.
    targetTag->grayNext = g_grayObjects;
    g_grayObjects = targetTag;
  }
}

void
Arcadia_ARMS_backwardBarrier
  (
    void* source,
    void* target
  )
{
  assert(NULL != source && NULL != target);
  Arcadia_ARMS_Tag* sourceTag = ((Arcadia_ARMS_Tag*)source) - 1;
  Arcadia_ARMS_Tag* targetTag = ((Arcadia_ARMS_Tag*)target) - 1;
  if (Arcadia_ARMS_Tag_isBlack(sourceTag) && Arcadia_ARMS_Tag_isWhite(targetTag)) {
    Arcadia_ARMS_Tag_setGray(sourceTag);
    sourceTag->grayNext = g_grayObjects;
    g_grayObjects = sourceTag;
  }
}

#endif // Arcadia_ARMS_Configuration_WithBarriers

Arcadia_ARMS_MemoryManager*
Arcadia_ARMS_getDefaultMemoryManager
  (
  )
{ return (Arcadia_ARMS_MemoryManager*)g_defaultMemoryManager; }

Arcadia_ARMS_MemoryManager*
Arcadia_ARMS_getSlabMemoryManager
  (
  )
{ return (Arcadia_ARMS_MemoryManager*)g_slabMemoryManager; }
