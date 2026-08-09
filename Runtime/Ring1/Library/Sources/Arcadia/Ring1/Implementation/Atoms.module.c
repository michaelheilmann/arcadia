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
#include "Arcadia/Ring1/Implementation/Atoms.module.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/Implementation/Module.h"
#include "Arcadia/Ring1/Implementation/Process.h"
#include "Arcadia/ARMS/Include.h"
#include "Arcadia/Ring1/Implementation/Diagnostics.h"

#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  #include <time.h>
  #include <math.h> // round
  #include <string.h>
#else
  #error("operating system not (yet) supported")
#endif

// 60 seconds
#define THRESHOLD 1000*60

typedef struct Singleton Singleton;

struct Arcadia_Atom {
  Arcadia_Atom* next;
  /// The last time this atom was visited.
  Arcadia_Natural64Value lastVisited;
  /// The hash value of this atom.
  Arcadia_SizeValue hash;
  /// The number of Bytes of this atom.
  Arcadia_SizeValue numberOfBytes;
  /// The Bytes of this key.
  char bytes[];
};

struct Singleton {
  Arcadia_SizeValue minimumCapacity;
  Arcadia_SizeValue maximumCapacity;
  Arcadia_Atom **buckets;
  Arcadia_SizeValue size;
  Arcadia_SizeValue capacity;
};

static uint32_t g_referenceCount = 0;
static Singleton* g_singleton = NULL;

static Arcadia_Natural64Value
getTickCount
  (
    Arcadia_Thread* thread
  );

static void
typeRemovedCallback
  (
    Arcadia_Process* context,
    const uint8_t* name,
    size_t nameLength
  );

static void
finalizeCallback
  (
    Arcadia_Process* context,
    Arcadia_AtomValue atom
  );

static void
visitCallback
  (
    Arcadia_Process* context,
    Arcadia_AtomValue atom
  );

static Arcadia_SizeValue
hashBytes
  (
    Arcadia_Thread* thread,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  );

static void
resize
  (
    Arcadia_Thread* thread
  );

static Arcadia_BooleanValue g_typeRegistered = Arcadia_BooleanValue_False;

static Arcadia_Natural64Value
getTickCount
  (
    Arcadia_Thread* thread
  )
{
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  return GetTickCount64();
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  // This is incorrect. It provides the time since some unspecified point in the past and not since the start of the process.
  struct timespec t;
  //t = (struct timespec *)malloc(sizeof(t));
  clock_gettime(CLOCK_MONOTONIC, &t);
  Arcadia_Natural64Value milliseconds = 0;
  // Nanoseconds to milliseconds.
  milliseconds = round(t.tv_nsec / 1.0e6);
  // Seconds to milliseconds.
  milliseconds += t.tv_sec * 1000;
  return milliseconds;
#else
  #error("environment not (yet) supported")
#endif
}

static void
typeRemovedCallback
  (
    Arcadia_Process* context,
    const uint8_t* name,
    size_t nameLength
  )
{
  g_typeRegistered = false;
}

static void
finalizeCallback
  (
    Arcadia_Process* context,
    Arcadia_AtomValue atom
  )
{
  Arcadia_SizeValue index = atom->hash % g_singleton->capacity;
  Arcadia_Atom** previous = &g_singleton->buckets[index];
  Arcadia_Atom* current = g_singleton->buckets[index];
  while (current) {
    if (atom == current) {
      *previous = current->next;
      g_singleton->size--;
      return;
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
}

static void
visitCallback
  (
    Arcadia_Process* context,
    Arcadia_AtomValue atom
  )
{/*Intentionally empty.*/}

static Arcadia_SizeValue
hashBytes
  (
    Arcadia_Thread* thread,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  uint8_t const* p = (uint8_t const*)bytes;
  Arcadia_SizeValue hashValue = numberOfBytes;
  for (Arcadia_SizeValue i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = hashValue * 37 + p[i];
  }
  return hashValue;
}

static void
resize
  (
    Arcadia_Thread* thread
  )
{
  if (g_singleton->size == g_singleton->capacity) {
    Arcadia_SizeValue oldCapacity = g_singleton->capacity;
    Arcadia_SizeValue newCapacity = oldCapacity;
    if (oldCapacity > g_singleton->maximumCapacity / 2) {
      // If oldCapacity > maximumCapacity / 2 holds then oldCapacity * 2 > maximumCapacity holds.
      // Consequently, we cannot double the capacity. Try to saturate the capacity.
      if (oldCapacity == g_singleton->maximumCapacity) {
        // Do nothing.
        return;
      } else {
        newCapacity = g_singleton->maximumCapacity;
      }
    } else {
      newCapacity = oldCapacity * 2;
    }
    Arcadia_Atom** oldBuckets = g_singleton->buckets;
    Arcadia_Atom** newBuckets = Arcadia_Memory_allocateUnmanaged(thread, newCapacity * sizeof(Arcadia_Atom*));
    for (Arcadia_SizeValue i = 0, n = newCapacity; i < n; ++i) {
      newBuckets[i] = NULL;
    }
    for (Arcadia_SizeValue i = 0, n = oldCapacity; i < n; ++i) {
      while (oldBuckets[i]) {
        Arcadia_Atom*node = oldBuckets[i];
        oldBuckets[i] = oldBuckets[i]->next;
        Arcadia_SizeValue j = node->hash % newCapacity;
        node->next = newBuckets[j];
        newBuckets[j] = node;
      }
    }
    Arcadia_Memory_deallocateUnmanaged(thread, oldBuckets);
    g_singleton->buckets = newBuckets;
    g_singleton->capacity = newCapacity;
  }
}

#define Arcadia_Ring1_Configuration_Atoms_InitialCapacity (8)
#define Arcadia_Ring1_Configuration_Atoms_MinimalCapacity (8)
#define Arcadia_Ring1_Configuration_Atoms_MaximalCapacity (Arcadia_SizeValue_Maximum/sizeof(Arcadia_Atom*))

Arcadia_StaticAssert(Arcadia_Ring1_Configuration_Atoms_MinimalCapacity <= Arcadia_Ring1_Configuration_Atoms_MaximalCapacity, "configuration invalid");
Arcadia_StaticAssert(Arcadia_Ring1_Configuration_Atoms_MinimalCapacity <= Arcadia_Ring1_Configuration_Atoms_InitialCapacity, "configuration invalid");
Arcadia_StaticAssert(Arcadia_Ring1_Configuration_Atoms_InitialCapacity <= Arcadia_Ring1_Configuration_Atoms_MaximalCapacity, "configuration invalid");

Arcadia_AtomValue
Arcadia_Atoms_getOrCreateAtom
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value flags,
    const void* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (!bytes || (SIZE_MAX - sizeof(Arcadia_Atom)) < numberOfBytes) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  if (Arcadia_Unicode_isUTF8(thread, bytes, numberOfBytes, NULL)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_SizeValue hash = hashBytes(thread, bytes, numberOfBytes);
  Arcadia_SizeValue index = hash % g_singleton->capacity;
  for (Arcadia_Atom* atom = g_singleton->buckets[index]; NULL != atom; atom = atom->next) {
    if (atom->numberOfBytes == numberOfBytes && atom->hash == hash) {
      if (!memcmp(atom->bytes, bytes, numberOfBytes)) {
        return atom;
      }
    }
  }
  Arcadia_Atom* atom = NULL;
  Arcadia_Process_allocate(Arcadia_Thread_getProcess(thread), (void**)&atom, u8"Arcadia.Atom", sizeof(u8"Arcadia.Atom") - 1, sizeof(Arcadia_Atom) + numberOfBytes);
  Arcadia_Memory_copy(thread, atom->bytes, bytes, numberOfBytes);
  atom->numberOfBytes = numberOfBytes;
  atom->hash = hash;
  atom->lastVisited = getTickCount(thread);
  atom->next = g_singleton->buckets[index];
  g_singleton->buckets[index] = atom;
  g_singleton->size++;
  resize(thread);
  return atom;
}

void
Arcadia_Atom_visit
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self
  )
{
  self->lastVisited = getTickCount(thread);
  Arcadia_Process_visitObject(Arcadia_Thread_getProcess(thread), self);
}

#if defined(Arcadia_ARMS_Configuration_WithBarriers) && 1 == Arcadia_ARMS_Configuration_WithBarriers

void
Arcadia_Atom_ensureGray
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self
  )
{
  self->lastVisited = getTickCount(thread);
  /*Arcadia_Process_ensureGray(Arcadia_Thread_getProcess(thread), self);*/
}

#endif

void const*
Arcadia_Atom_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self
  )
{ return self->bytes; }

Arcadia_SizeValue
Arcadia_Atom_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self
  )
{ return self->numberOfBytes; }

Arcadia_SizeValue
Arcadia_Atom_getHash
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self
  )
{ return self->hash; }

Arcadia_BooleanValue
Arcadia_Atom_isEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self,
    Arcadia_AtomValue other
  )
{ return self == other; }

Arcadia_DefineModule("Arcadia.Atoms", Arcadia_Atoms);

static void
_Arcadia_Atoms_onVisit
  (
    Arcadia_Thread* thread
  )
{/*Intentionally empty.*/}

static void
_Arcadia_Atoms_onFinalize
  (
    Arcadia_Thread* thread,
    size_t* destroyed
  )
{
  *destroyed = 0;
}

static void
_Arcadia_Atoms_onStartUp
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
  if (0 == g_referenceCount) {
    Arcadia_JumpTarget jumpTarget;
    g_singleton = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Singleton));
    g_singleton->buckets = NULL;
    g_singleton->size = 0;
    g_singleton->capacity = 0;
    g_singleton->minimumCapacity = Arcadia_Ring1_Configuration_Atoms_MinimalCapacity;
    g_singleton->maximumCapacity = SIZE_MAX / sizeof(Arcadia_Atom*);
    if (g_singleton->maximumCapacity > Arcadia_Integer32Value_Maximum) {
      g_singleton->maximumCapacity = Arcadia_Integer32Value_Maximum;
    }
    if (g_singleton->minimumCapacity > g_singleton->maximumCapacity) {
      Arcadia_Memory_deallocateUnmanaged(thread, g_singleton);
      g_singleton = NULL;
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    // (1) create buckets
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      g_singleton->buckets = Arcadia_Memory_allocateUnmanaged(thread, Arcadia_Ring1_Configuration_Atoms_InitialCapacity * sizeof(Arcadia_Atom*));
      for (Arcadia_SizeValue i = 0, n = Arcadia_Ring1_Configuration_Atoms_InitialCapacity; i < n; ++i) {
        g_singleton->buckets[i] = NULL;
      }
      g_singleton->size = 0;
      g_singleton->capacity = Arcadia_Ring1_Configuration_Atoms_InitialCapacity;
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Memory_deallocateUnmanaged(thread, g_singleton);
      g_singleton = NULL;
      Arcadia_Thread_jump(thread);
    }

    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_Process_addArenaPreMarkCallback(process, &_Arcadia_Atoms_onPreMark);
      Arcadia_Process_addArenaVisitCallback(process, &_Arcadia_Atoms_onVisit);
      Arcadia_Process_addArenaFinalizeCallback(process, &_Arcadia_Atoms_onFinalize);
      Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
    } else {
      Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
      Arcadia_Process_removeArenaFinalizeCallback(process, &_Arcadia_Atoms_onFinalize);
      Arcadia_Process_removeArenaVisitCallback(process, &_Arcadia_Atoms_onVisit);
      Arcadia_Process_removeArenaPreMarkCallback(process, &_Arcadia_Atoms_onPreMark);
      Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), g_singleton->buckets);
      g_singleton->buckets = NULL;
      Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), g_singleton);
      g_singleton = NULL;
      Arcadia_Thread_jump(thread);
    }

    if (!g_typeRegistered) {
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        Arcadia_Process_registerType(process,
                                     u8"Arcadia.Atom", sizeof(u8"Arcadia.Atom") - 1,
                                     process,
                                     (Arcadia_Process_TypeRemovedCallback*)&typeRemovedCallback,
                                     (Arcadia_Process_VisitCallback*)&visitCallback,
                                     (Arcadia_Process_FinalizeCallback*)&finalizeCallback);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Process_removeArenaFinalizeCallback(process, &_Arcadia_Atoms_onFinalize);
        Arcadia_Process_removeArenaVisitCallback(process, &_Arcadia_Atoms_onVisit);
        Arcadia_Process_removeArenaPreMarkCallback(process, &_Arcadia_Atoms_onPreMark);
        Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), g_singleton->buckets);
        g_singleton->buckets = NULL;
        Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), g_singleton);
        g_singleton = NULL;
        Arcadia_Thread_jump(thread);
      }
    }
    g_typeRegistered = Arcadia_BooleanValue_True;
  }
  g_referenceCount++;
}

static void
_Arcadia_Atoms_onShutDown
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
    Arcadia_Process_removeArenaFinalizeCallback(process, &_Arcadia_Atoms_onFinalize);
    Arcadia_Process_removeArenaVisitCallback(process, &_Arcadia_Atoms_onVisit);
    Arcadia_Process_removeArenaPreMarkCallback(process, &_Arcadia_Atoms_onPreMark);
    Arcadia_Memory_deallocateUnmanaged(thread, g_singleton->buckets);
    g_singleton->buckets = NULL;
    Arcadia_Memory_deallocateUnmanaged(thread, g_singleton);
    g_singleton = NULL;
  }
}

static void
_Arcadia_Atoms_onPreMark
  (
    Arcadia_Thread* thread,
    bool purgeCache
  )
{
  Arcadia_Natural64Value now = getTickCount(thread);
  for (Arcadia_SizeValue i = 0, n = g_singleton->capacity; i < n; ++i) {
    Arcadia_Atom* current = g_singleton->buckets[i];
    while (current) {
      if (now - current->lastVisited > THRESHOLD && !purgeCache) {
        Arcadia_ARMS_visit(current);
      }
      current = current->next;
    }
  }
}
