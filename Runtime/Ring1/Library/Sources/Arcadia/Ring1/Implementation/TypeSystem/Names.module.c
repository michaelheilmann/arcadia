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
#include "Arcadia/Ring1/Implementation/TypeSystem/Names.module.h"

#include "Arcadia/Ring1/Implementation/Thread.h"
#include "Arcadia/Ring1/Implementation/Diagnostics.h"
#include "Arcadia/Ring1/Implementation/Memory.h"
#include <assert.h>

#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  #include <time.h>
  #include <math.h> // round
  #include <string.h> // memcpy
#else
  #error("operating system not (yet) supported")
#endif

// 60 seconds
#define Arcadia_Name_Threshold 1000*60

static void
Arcadia_Name_typeRemovedCallback
  (
    Arcadia_Process* context,
    const uint8_t* name,
    size_t nameLength
  );

static void
Arcadia_Name_finalizeCallback
  (
    Arcadia_Process* process,
    Arcadia_Name* name
  );

void
Arcadia_Name_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Name* self
  );

static void
_Arcadia_Names_onFinalize
  (
    Arcadia_Thread* thread,
    size_t* finalized
  );

static void
_Arcadia_Names_onVisit
  (
    Arcadia_Thread* thread
  );

static uint64_t
getTickCount
  (
    Arcadia_Thread* thread
  );

static Arcadia_Names*
Arcadia_Names_create
  (
    Arcadia_Process* process
  );

static void
Arcadia_Names_destroy
  (
    Arcadia_Process* process,
    Arcadia_Names* names
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define Arcadia_Name_TypeName "Arcadia.Name"

struct Arcadia_Names {
  Arcadia_Name** buckets;
  size_t size;
  size_t capacity;
};

static Arcadia_Names* g_names = NULL;
static uint32_t g_referenceCount = 0;
static bool g_registered = false;

static void
Arcadia_Name_typeRemovedCallback
  (
    Arcadia_Process* context,
    const uint8_t* name,
    size_t nameLength
  )
{ g_registered = false; }

static void
Arcadia_Name_finalizeCallback
  (
    Arcadia_Process* process,
    Arcadia_Name* name
  )
{
    size_t index = name->hashValue % g_names->capacity;
    Arcadia_Name** previous = &g_names->buckets[index];
    Arcadia_Name* current = g_names->buckets[index];
    while (current) {
      if (name == current) {
        *previous = current->next;
        g_names->size--;
        return;
      } else {
        previous = &current->next;
        current = current->next;
      }
    }
}

void
Arcadia_Name_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Name* self
  )
{
  self->lastVisited = getTickCount(thread);
  Arcadia_Process_visitObject(Arcadia_Thread_getProcess(thread), self);
}

size_t
Arcadia_Name_getHash
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name
  )
{
  return name->hashValue;
}

size_t
Arcadia_Name_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name
  )
{
  return name->numberOfBytes;
}

const char*
Arcadia_Name_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name
  )
{ return name->bytes; }

static uint64_t
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Arcadia_Names*
Arcadia_Names_create
  (
    Arcadia_Process* process
  )
{
  Arcadia_Names* names = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(Arcadia_Process_getThread(process), &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    names = Arcadia_Memory_allocateUnmanaged(Arcadia_Process_getThread(process), sizeof(Arcadia_Names));
    names->buckets = NULL;
    names->size = 0;
    names->capacity = 8;
    names->buckets = Arcadia_Memory_allocateUnmanaged(Arcadia_Process_getThread(process), sizeof(Arcadia_Name*) * 8);
    for (size_t i = 0, n = 8; i < n; ++i) {
      names->buckets[i] = NULL;
    }
    Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
  } else {
    Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
    if (names) {
      if (names->buckets) {
        Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), names->buckets);
        names->buckets = NULL;
      }
      Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), names);
      names = NULL;;
    }
    Arcadia_Thread_jump(Arcadia_Process_getThread(process));
  }
  return names;
}

static void
Arcadia_Names_destroy
  (
    Arcadia_Process* process,
    Arcadia_Names* names
  )
{
  // Remove all names.
  for (size_t i = 0, n = names->capacity; i < n; ++i) {
    while (names->buckets[i]) {
      Arcadia_Name* current = names->buckets[i];
      names->buckets[i] = current->next;
      Arcadia_Process_unlockObject(process, current);
      names->size--;
    }
  }
  Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), names->buckets);
  Arcadia_Memory_deallocateUnmanaged(Arcadia_Process_getThread(process), names);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_DefineModule("Arcadia.Names", Arcadia_Names);

static void
_Arcadia_Names_onPreMark
  (
    Arcadia_Thread* thread,
    bool purgeCache
  )
{
    uint64_t now = getTickCount(thread);
    for (size_t i = 0, n = g_names->capacity; i < n; ++i) {
      Arcadia_Name* current = g_names->buckets[i];
      while (current) {
        if (now - current->lastVisited > Arcadia_Name_Threshold && !purgeCache) {
          Arcadia_ARMS_visit(current);
        }
        current = current->next;
      }
    }
}

static void
_Arcadia_Names_onFinalize
  (
    Arcadia_Thread* thread,
    size_t* finalized
  )
{
  if (finalized) {
    *finalized = 0;
  }
}

static void
_Arcadia_Names_onVisit
  (
    Arcadia_Thread* thread
  )
{/*Intentionally empty.*/}

static void
_Arcadia_Names_onStartUp
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
    if (!g_registered) {
      Arcadia_Process_registerType(process,
                                   u8"Arcadia.Name", sizeof(u8"Arcadia.Name") - 1,
                                   process,
                                   (Arcadia_Process_TypeRemovedCallback*)&Arcadia_Name_typeRemovedCallback,
                                   NULL,
                                   (Arcadia_Process_FinalizeCallback*)&Arcadia_Name_finalizeCallback);
      g_registered = Arcadia_BooleanValue_True;
    }
    g_names = Arcadia_Names_create(process);
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_Process_addArenaPreMarkCallback(process, &_Arcadia_Names_onPreMark);
      Arcadia_Process_addArenaVisitCallback(process, &_Arcadia_Names_onVisit);
      Arcadia_Process_addArenaFinalizeCallback(process, &_Arcadia_Names_onFinalize);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
      Arcadia_Process_removeArenaFinalizeCallback(process, &_Arcadia_Names_onFinalize);
      Arcadia_Process_removeArenaVisitCallback(process, &_Arcadia_Names_onVisit);
      Arcadia_Process_removeArenaPreMarkCallback(process, &_Arcadia_Names_onPreMark);
      Arcadia_Names_destroy(process, g_names);
      g_names = NULL;
      Arcadia_Status status = Arcadia_Process_runARMS(process, true);
      if (status) {
        /*Intentionally empty.*/
      }
      Arcadia_Thread_jump(thread);
    }
  }
  g_referenceCount++;
}

static void
_Arcadia_Names_onShutDown
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
    Arcadia_Names_destroy(process, g_names);
    g_names = NULL;
    Arcadia_Process_removeArenaFinalizeCallback(process, &_Arcadia_Names_onFinalize);
    Arcadia_Process_removeArenaVisitCallback(process, &_Arcadia_Names_onVisit);
    Arcadia_Process_removeArenaPreMarkCallback(process, &_Arcadia_Names_onPreMark);
    status = Arcadia_Process_runARMS(process, true);
    if (status) {
      /* Intentionally empty.*/
    }
  }
}

Arcadia_Name*
Arcadia_Names_getOrCreateName
  (
    Arcadia_Thread* thread,
    char const* bytes,
    size_t numberOfBytes
  )
{
  assert(NULL != thread);
  assert(NULL != bytes);
  size_t hashValue = numberOfBytes;
  for (size_t i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = (hashValue << 3) ^ (hashValue >> 7) | (size_t)bytes[i];
  }
  size_t hashIndex = hashValue % g_names->capacity;
  Arcadia_Name* name = NULL;
  for (name = g_names->buckets[hashIndex]; NULL != name; name = name->next) {
    if (name->numberOfBytes == numberOfBytes && name->hashValue == hashValue && !memcmp(name->bytes, bytes, numberOfBytes)) {
      return name;
    }
  }
  if (SIZE_MAX - sizeof(Arcadia_Name) < numberOfBytes || g_names->size == (SIZE_MAX / sizeof(Arcadia_Name*))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_Process_allocate(Arcadia_Thread_getProcess(thread), (void**)&name, u8"Arcadia.Name", sizeof(u8"Arcadia.Name") - 1, sizeof(Arcadia_Name) + numberOfBytes);
  memcpy(name->bytes, bytes, numberOfBytes);
  name->numberOfBytes = numberOfBytes;
  name->hashValue = hashValue;
  name->lastVisited = getTickCount(thread);

  name->next = g_names->buckets[hashIndex];
  g_names->buckets[hashIndex] = name;
  g_names->size++;

  return name;
}
