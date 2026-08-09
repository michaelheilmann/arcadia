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

#if !defined(ARCADIA_ARMS_INTERNAL_STATISTICS_H_INCLUDED)
#define ARCADIA_ARMS_INTERNAL_STATISTICS_H_INCLUDED

#include "Arcadia/ARMS/SizeType.h"
#include "Arcadia/ARMS/StatusType.h"

/// @brief
/// Statistics of the GC.
/// @remarks
/// The statistics keep track of the total number of objects and the total number of Bytes allocated.
typedef struct Arcadia_ARMS_AllocatorStatistics {

  Arcadia_ARMS_Size numberOfBlocks;    ///< The number of blocks currently allocated.
                                       ///< @a numberOfBytes is the sum of the sizes, in Bytes, of all these blocks.

  Arcadia_ARMS_Size numberOfBytes;     ///< The number of Bytes currently allocated.

} Arcadia_ARMS_AllocatorStatistics;

/// @brief Initialize a @a Arcadia_ARMS_AllocatorStatistics object.
/// @param self a pointer to an uninitialized a @a Arcadia_ARMS_AllocatorStatistics object.
/// @return #Arcadia_ARMS_Status_Success on success. A non-zero Arcadia_ARMS_Status value on failure.
Arcadia_ARMS_Status
Arcadia_ARMS_AllocatorStatistics_initialize
  (
    Arcadia_ARMS_AllocatorStatistics* statistics
  );

/// @brief Uninitialize a @a Arcadia_ARMS_AllocatorStatistics object.
/// @param statistics a pointer to an uninitialized @a Arcadia_ARMS_AllocatorStatistics object
/// @return #Arcadia_ARMS_Status_Success on success. A non-zero Arcadia_ARMS_Status value on failure.
Arcadia_ARMS_Status
Arcadia_ARMS_AllocatorStatistics_uninitialize
  (
    Arcadia_ARMS_AllocatorStatistics* statistics
  );

/// @brief Update memory statistics for an allocation.
/// @param statistics a pointer to an initialized @a Arcadia_ARMS_AllocatorStatistics object
/// @param numberOfBytes The number of Bytes that has been allocated.
/// @return #Arcadia_ARMS_Status_Success on success. A non-zero Arcadia_ARMS_Status value on failure.
Arcadia_ARMS_Status
Arcadia_ARMS_AllocatorStatistics_onBlockAllocated
  (
    Arcadia_ARMS_AllocatorStatistics* statistics,
    Arcadia_ARMS_Size numberOfBytes
  );

/// @brief Update memory statistics for a deallocation.
/// @param statistics a pointer to an initialized @a Arcadia_ARMS_AllocatorStatistics object
/// @param numberOfBytes The number of Bytes that has been deallocated.
/// @return #Arcadia_ARMS_Status_Success on success. A non-zero Arcadia_ARMS_Status value on failure.
Arcadia_ARMS_Status
Arcadia_ARMS_AllocatorStatistics_onBlockDeallocated
  (
    Arcadia_ARMS_AllocatorStatistics* statistics,
    Arcadia_ARMS_Size numberOfBytes
  );

#endif // ARCADIA_ARMS_INTERNAL_STATISTICS_H_INCLUDED
