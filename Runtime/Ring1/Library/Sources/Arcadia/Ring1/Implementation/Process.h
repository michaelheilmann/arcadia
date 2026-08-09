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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_PROCESS_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_PROCESS_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Annotations/NoReturn.h"

#if Arcadia_Configuration_CompilerC_Gcc == Arcadia_Configuration_CompilerC
#include <stddef.h>
#endif

#include "Arcadia/Ring1/Implementation/Status.h"

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <setjmp.h>
typedef struct Arcadia_JumpTarget Arcadia_JumpTarget; // Forward declaration.
typedef struct Arcadia_Thread Arcadia_Thread; // Forward declaration.

/// the process object provides access to the current thread.
/// the current thread provides access to the thread's jump target stack and the thread's status variable.
typedef struct Arcadia_Process Arcadia_Process;

typedef uint32_t Arcadia_ProcessStatus;
#define Arcadia_ProcessStatus_Success (0)
#define Arcadia_ProcessStatus_ArgumentValueInvalid (1)
#define Arcadia_ProcessStatus_AllocationFailed (2)
#define Arcadia_ProcessStatus_OperationInvalid (3)
#define Arcadia_ProcessStatus_EnvironmentFailed (4)

/// @brief Get the thread of this process.
/// @param process A pointer to this Arcadia_Process object.
/// @return A pointer to the Arcadia_Thread object of this Arcadia_Process object.
Arcadia_Thread*
Arcadia_Process_getThread
  (
    Arcadia_Process* process
  );

/// @return #Arcadia_ProcessStatus_Success on success.
/// - #Arcadia_ProcessStatus_ArgumentValueInvalid @a process is a null pointer
/// - #Arcadia_ProcessStatus_OperationInvalid the reference counter would overflow or underflow from this call
Arcadia_ProcessStatus
Arcadia_Process_acquire
  (
    Arcadia_Process* process
  );

/// @return #Arcadia_ProcessStatus_Success on success.
/// - #Arcadia_ProcessStatus_ArgumentValueInvalid @a process is a null pointer
/// - #Arcadia_ProcessStatus_OperationInvalid the reference counter would overflow or underflow from this call
Arcadia_ProcessStatus
Arcadia_Process_relinquish
  (
    Arcadia_Process* process
  );

/// @brief Acquire an initial handle to the process object singleton.
/// @param [out] process A pointer to a <code>Arcadia_Process*</code> variable.
/// @post
/// If the call to this function was is successful:
/// - <code>*process</code> was assigned a pointer to the process object singleton.
/// - the caller acquired a reference to that object which he must relinquish by a call to Arcadia_Process_relinquish when no longer required.
/// @return #Arcadia_ProcessStatus_Success on success.
/// - #Arcadia_ProcessStatus_ArgumentValueInvalid @a process is a null pointer
/// - #Arcadia_ProcessStatus_OperationInvalid the reference counter would overflow or underflow from this call
/// - #Arcadia_ProcessStatus_EnvironmentFailed initialization of Arcadia ARMS failed
/// - #Arcadia_ProcessStatus_AllocationFailed an allocation failed
Arcadia_ProcessStatus
Arcadia_Process_get
  (
    Arcadia_Process** process
  );

/// @param process A pointer to the Arcadia_Process object
void
Arcadia_Process_visitObject
  (
    Arcadia_Process* process,
    void* object
  );

/// @param process A pointer to the Arcadia_Process object
Arcadia_Status
Arcadia_Process_lockObject
  (
    Arcadia_Process* process,
    void* object
  );

/// @param process A pointer to the Arcadia_Process object
Arcadia_Status
Arcadia_Process_unlockObject
  (
    Arcadia_Process* process,
    void* object
  );

Arcadia_Status
Arcadia_Process_stepARMS
  (
    Arcadia_Process* process
  );

Arcadia_Status
Arcadia_Process_runARMS
  (
    Arcadia_Process* process,
    bool purgeCaches
  );

typedef void (Arcadia_Process_ArenaPreMarkCallback)(Arcadia_Thread* thread, bool purgeCaches);
typedef void (Arcadia_Process_ArenaVisitCallback)(Arcadia_Thread* thread);
typedef void (Arcadia_Process_ArenaFinalizeCallback)(Arcadia_Thread* thread, size_t* destroyed);

/// @brief Add a premark callback to the list of premark callbacks.
/// @param process A pointer to the process.
/// @param callback A pointer to the premark callback.
void
Arcadia_Process_addArenaPreMarkCallback
  (
    Arcadia_Process* process,
    Arcadia_Process_ArenaPreMarkCallback* callback
  );

/// @brief Remove an arena premark callback from the list of premark callbacks.
/// @param process A pointer to the process.
/// @param callback A pointer to the arena premark callback.
/// @remark One occurrence of the callback in the list of arena premark callbacks was removed.
void
Arcadia_Process_removeArenaPreMarkCallback
  (
    Arcadia_Process* process,
    Arcadia_Process_ArenaPreMarkCallback* callback
  );

/// @brief Add an arena visit callback to the list of visit callbacks.
/// @param process A pointer to the process.
/// @param callback A pointer to the arena visit callback.
void
Arcadia_Process_addArenaVisitCallback
  (
    Arcadia_Process* process,
    Arcadia_Process_ArenaVisitCallback* callback
  );

/// @brief Remove an arena visit callback from the list of visit callbacks.
/// @param process A pointer to the process.
/// @param callback A pointer to the arena visit callback.
/// @remark One occurrence of the callback in the list of arena visit callbacks was removed.
void
Arcadia_Process_removeArenaVisitCallback
  (
    Arcadia_Process* process,
    Arcadia_Process_ArenaVisitCallback* callback
  );

/// @brief Add an arena finalize callback to the list of finalize callbacks.
/// @param process A pointer to the process.
/// @param callback A pointer to the arena finalize callback.
void
Arcadia_Process_addArenaFinalizeCallback
  (
    Arcadia_Process* process,
    Arcadia_Process_ArenaFinalizeCallback* callback
  );

/// @brief Remove an arena finalize callback from the list of finalize callbacks.
/// @param process A pointer to the process.
/// @param callback A pointer to the finalize callback.
/// @remark One occurrence of the callback in the list of arena finalize callbacks was removed.
void
Arcadia_Process_removeArenaFinalizeCallback
  (
    Arcadia_Process* process,
    Arcadia_Process_ArenaFinalizeCallback* callback
  );

typedef void (Arcadia_Process_TypeRemovedCallback)(void*, uint8_t const*, size_t);
typedef void (Arcadia_Process_VisitCallback)(void*, void*);
typedef void (Arcadia_Process_FinalizeCallback)(void*, void*);

void
Arcadia_Process_registerType
  (
    Arcadia_Process* process,
    const char* name,
    size_t nameLength,
    void* context,
    Arcadia_Process_TypeRemovedCallback* typeRemoved,
    Arcadia_Process_VisitCallback* visit,
    Arcadia_Process_FinalizeCallback* finalize
  );

/// @brief Allocate managed memory.
/// @param process A pointer to the process.
/// @param p A pointer to a <code>void*</code> variable.
/// @param name A pointer to an array of Bytes.
/// @param nameLength The number of Bytes in the array pointed to by @a name.
/// @param size The size, in Bytes, of the memory to allocate. @a 0 is a valid size.
/// @return @a true on success. @a false on failure.
/// On failure, this function sets the status variable of the process.
/// - Arcadia_Status_AllocationFailed if an allocation failed.
/// - Arcadia_Status_TypeNotExists if the type does not exist.
/// - Arcadia_Status_ArgumentInvalid if @a p or @a name is a null pointer.
void
Arcadia_Process_allocate
  (
    Arcadia_Process* process,
    void** p,
    const char* name,
    size_t nameLength,
    size_t size
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_PROCESS_H_INCLUDED
