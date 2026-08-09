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

#if !defined(ARCADIA_ARMS_ARMS_H_INCLUDED)
#define ARCADIA_ARMS_ARMS_H_INCLUDED

#include "Arcadia/ARMS/Configure.h"
#include "Arcadia/ARMS/SizeType.h"
#include "Arcadia/ARMS/Natural8Type.h"
#include "Arcadia/ARMS/StatusType.h"
#include "Arcadia/ARMS/MemoryManager.h"
#include "Arcadia/ARMS/TypeName.h"

#if defined(Arcadia_ARMS_Configuration_WithNotifyDestroy) && 1 ==  Arcadia_ARMS_Configuration_WithNotifyDestroy

#include "Arcadia/ARMS/NotifyDestroy.h"

#endif // Arcadia_ARMS_Configuration_WithNotifyDestroy

/// Alias for sizeof(expression).
#define Arcadia_ARMS_SizeOf(expression) sizeof(expression)

/// Alias for _Alignof(expression).
#define Arcadia_ARMS_AlignOf(expression) _Alignof(expression)

/// Alias for offsetof(expression).
#define Arcadia_ARMS_OffsetOf(expression) offsetof(expression)

typedef void (Arcadia_ARMS_TypeRemovedCallbackFunction)(void* context, Arcadia_ARMS_Natural8 const* name, Arcadia_ARMS_Size nameLength);

typedef void (Arcadia_ARMS_VisitCallbackFunction)(void* context, void* object);

typedef void (Arcadia_ARMS_FinalizeCallbackFunction)(void* context, void* object);

Arcadia_ARMS_Status
Arcadia_ARMS_startup
  (
  );

Arcadia_ARMS_Status
Arcadia_ARMS_shutdown
  (
  );

Arcadia_ARMS_Status
Arcadia_ARMS_addType
  (
    Arcadia_ARMS_Natural8 const* name,
    Arcadia_ARMS_Size nameLength,
    void* context,
    Arcadia_ARMS_TypeRemovedCallbackFunction* typeRemoved,
    Arcadia_ARMS_VisitCallbackFunction* visit,
    Arcadia_ARMS_FinalizeCallbackFunction* finalize
  );

Arcadia_ARMS_Status
Arcadia_ARMS_allocate
  (
    void** pObject,
    Arcadia_ARMS_Natural8 const* name,
    Arcadia_ARMS_Size nameLength,
    Arcadia_ARMS_Size size
  );

typedef struct Arcadia_ARMS_RunStatistics {
  /// The number of locked objects.
  Arcadia_ARMS_Size locked;
  /// The number of dead objects.
  Arcadia_ARMS_Size dead;
  /// The number of live objects.
  Arcadia_ARMS_Size live;
  /// The number of finalized objects.
  Arcadia_ARMS_Size finalized;
} Arcadia_ARMS_RunStatistics;

#define Arcadia_ARMS_RunStatistics_StaticInitializer() { .locked = 0, .live = 0, .dead = 0, .finalized = 0 };

Arcadia_ARMS_Status
Arcadia_ARMS_run
  (
    Arcadia_ARMS_RunStatistics* statistics
  );

void
Arcadia_ARMS_visit
  (
    void* object
  );

#if defined(Arcadia_ARMS_Configuration_WithBarriers) && 1 == Arcadia_ARMS_Configuration_WithBarriers

// Ensures the invariant no black object may refer to a white object is preserved.
// If source is black and target is white, then color target gray.
void
Arcadia_ARMS_forwardBarrier
  (
    void* source,
    void* target
  );

// Ensures the invariant no black object may refer to a white object is preserved.
// If source is black and target is white, then color source gray.
void
Arcadia_ARMS_backwardBarrier
  (
    void* source,
    void* target
  );

#endif // Arcadia_ARMS_Configuration_WithBarriers

#if defined(Arcadia_ARMS_Configuration_WithLocks) && 1 == Arcadia_ARMS_Configuration_WithLocks

Arcadia_ARMS_Status
Arcadia_ARMS_lock
  (
    void* object
  );

Arcadia_ARMS_Status
Arcadia_ARMS_unlock
  (
    void* object
  );

#endif // Arcadia_ARMS_Configuration_WithLocks

Arcadia_ARMS_MemoryManager*
Arcadia_ARMS_getDefaultMemoryManager
  (
  );

Arcadia_ARMS_MemoryManager*
Arcadia_ARMS_getSlabMemoryManager
  (
  );

#endif // ARCADIA_ARMS_ARMS_H_INCLUDED
