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

#include "Arcadia/ARMS/Internal/Statistics.h"

Arcadia_ARMS_Status
Arcadia_ARMS_AllocatorStatistics_initialize
  (
    Arcadia_ARMS_AllocatorStatistics* statistics
  )
{
  statistics->numberOfBlocks = 0;
  statistics->numberOfBytes = 0;
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_AllocatorStatistics_uninitialize
  (
    Arcadia_ARMS_AllocatorStatistics* statistics
  )
{
  statistics->numberOfBlocks = 0;
  statistics->numberOfBytes = 0;
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_AllocatorStatistics_onBlockAllocated
  (
    Arcadia_ARMS_AllocatorStatistics* statistics,
    Arcadia_ARMS_Size numberOfBytes
  )
{
  if (!statistics) {
    return Arcadia_ARMS_Status_ArgumentValueInvalid;
  }
  // It is possible to have n > 0 blocks with m = 0 Bytes as zero-sized allocations are allowed.
  if (Arcadia_ARMS_Size_Maximum - statistics->numberOfBytes < numberOfBytes || Arcadia_ARMS_Size_Maximum - statistics->numberOfBlocks < 1) {
    return Arcadia_ARMS_Status_ArgumentValueInvalid;
  }
  statistics->numberOfBytes += numberOfBytes;
  statistics->numberOfBlocks += 1;
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_AllocatorStatistics_onBlockDeallocated
  (
    Arcadia_ARMS_AllocatorStatistics* statistics,
    Arcadia_ARMS_Size numberOfBytes
  )
{
  if (!statistics) {
    return Arcadia_ARMS_Status_ArgumentValueInvalid;
  }
  if (statistics->numberOfBytes < numberOfBytes || statistics->numberOfBlocks < 1) {
    return Arcadia_ARMS_Status_ArgumentValueInvalid;
  }
  statistics->numberOfBytes -= numberOfBytes;
  statistics->numberOfBlocks -= 1;
  return Arcadia_ARMS_Status_Success;
}
