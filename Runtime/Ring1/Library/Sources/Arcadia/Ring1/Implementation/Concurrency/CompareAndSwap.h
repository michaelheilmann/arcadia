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

#if !defined(ARCADIA_RING1_CONCURRENCY_COMPAREANDSWAP_H_INCLUDED)
#define ARCADIA_RING1_CONCURRENCY_COMPAREANDSWAP_H_INCLUDED

#include "Arcadia/Ring1/Implementation/Configure.h"
#include <stddef.h>

/// @brief Compare the old value of a pointer to a value. If the comparison yields true, then replace the old value by a specified value.
/// @param comperand The value to compare the old value of *destination to.
/// @param exchange The new value of *destination if the comparison yields true.
/// @return The old value of *destination.
/// @warning The parameters must be aligned to 64 bit boundaries.
/// @warning This function does not check for the alignment of parameters to 64 bit boundaries.
void*
Arcadia_Memory_compareAndSwap
  (
    void* volatile *destination,
    void* comperand,
    void* exchange
  );

#endif // ARCADIA_RING1_CONCURRENCY_COMPAREANDSWAP_H_INCLUDED
