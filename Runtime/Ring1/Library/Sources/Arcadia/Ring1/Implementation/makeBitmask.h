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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_MAKEBITMASK_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_MAKEBITMASK_H_INCLUDED

#include "Arcadia/Ring1/Implementation/Natural16.h"
#include "Arcadia/Ring1/Implementation/Natural32.h"
#include "Arcadia/Ring1/Implementation/Natural64.h"
#include "Arcadia/Ring1/Implementation/Natural8.h"

#include "Arcadia/Ring1/Implementation/Size.h"

/// @brief Create a bitmask.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param i The index of the first 1 bit.
/// @param n The number of 1 bits.
/// @return A bitmask such that the bits [i, i + n) are 1 and all other bits are zero.
/// @error Arcadia_Status_ArgumentValueInvalid <code>i + n</code> is greater than 16.
Arcadia_Natural16Value
Arcadia_makeBitmaskN16
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue i,
    Arcadia_SizeValue n
  );

/// @brief Create a bitmask.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param i The index of the first 1 bit.
/// @param n The number of 1 bits.
/// @return A bitmask such that the bits [i, i + n) are 1 and all other bits are zero.
/// @error Arcadia_Status_ArgumentValueInvalid <code>i + n</code> is greater than 32.
Arcadia_Natural32Value
Arcadia_makeBitmaskN32
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue i,
    Arcadia_SizeValue n
  );

/// @brief Create a bitmask.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param i The index of the first 1 bit.
/// @param n The number of 1 bits.
/// @return A bitmask such that the bits [i, i + n) are 1 and all other bits are zero.
/// @error Arcadia_Status_ArgumentValueInvalid <code>i + n</code> is greater than 64.
Arcadia_Natural64Value
Arcadia_makeBitmaskN64
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue i,
    Arcadia_SizeValue n
  );

/// @brief Create a bitmask.
/// @param thread A pointer to the Arcadia_Thread object.
/// @param i The index of the first 1 bit.
/// @param n The number of 1 bits.
/// @return A bitmask such that the bits [i, i + n) are 1 and all other bits are zero.
/// @error Arcadia_Status_ArgumentValueInvalid <code>i + n</code> is greater than 8.
Arcadia_Natural8Value
Arcadia_makeBitmaskN8
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue i,
    Arcadia_SizeValue n
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_MAKEBITMASK_H_INCLUDED
