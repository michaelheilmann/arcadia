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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_ARRAYS_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_ARRAYS_H_INCLUDED

#include "Arcadia/Ring1/Implementation/Size.h"
#include "Arcadia/Ring1/Implementation/Process.h"
#include "Arcadia/ARMS/Include.h"

/// @brief The strategy for a call to <code>Arcadia_Arrays_resizeBy*</code> functions.
typedef enum Arcadia_Arrays_ResizeStrategy {

  /// Symbolic constant indicating a growth strategy.
  ///
  /// Let capacity denote the capacity of the array, size denote the size of the array.
  /// Let maximalCapacity denote the maximal capacity.
  /// Let additionalCapacity be the amount to increase the capacity by.
  ///
  /// The growth strategy computes:
  /// - capacity@new = capacity@old + additionalCapacity if maximalCapacity - capacity@old >= additionalCapacity
  /// - fail otherwise
  Arcadia_Arrays_ResizeStrategy_Type1 = 1,

  /// Symbolic constant indicating a growth strategy.
  ///
  /// Let capacity denote the capacity of the array, size denote the size of the array.
  /// Let maximalCapacity denote the maximal capacity.
  /// Let additionalCapacity be the amount to increase the capacity by.
  ///
  /// This growth strategy computes:
  /// - capacity@new = 2^n * 1 if capacity@old = 0
  /// - capacity@new = 2^n * capacity@old
  /// such that
  /// n is minimal whilst capacity@new >= capacity@old + additionalCapacity
  /// If no such n exists, fail.
  Arcadia_Arrays_ResizeStrategy_Type2 = 2,

  /// Symbolic constant indicating a growth strategy.
  ///
  /// Let capacity denote the capacity of the array, size denote the size of the array.
  /// Let maximalCapacity denote the maximal capacity.
  /// Let additionalCapacity be the amount to increase the capacity by.
  ///
  /// This growth strategy computes:
  /// - capacity@new = maximalCapacity if maximalCapacity - size >= additionalCapacity
  /// - fail otherwise
  Arcadia_Arrays_ResizeStrategy_Type3 = 3,

  /// Attempt Arcadia_Arrays_ResizeStrategy_Type2 first.
  /// If that growth strategy fails:
  /// Attempt Arcadia_Arrays_ResizeStrategy_Type3 second.
  /// If that growth strategy fails:
  /// Fail.
  Arcadia_Arrays_ResizeStrategy_Type4 = 4,

} Arcadia_Arrays_ResizeStrategy;

/// @brief Resize an array taking into account a required additional capacity.
/// @param elements A pointer to the element array of the array.
/// @param elementSize The size of an element.
/// @param size The size of the array.
/// @param capacity A pointer to the capacity of the array.
/// @param requiredAdditionalCapacity The required additional capacity.
/// @param strategy The resize strategy.
/// @error #Arcadia_Status_ArgumentValueInvalid @a memoryManager, @a elements, or @a capacity are null
/// @error #Arcadia_Status_ArgumentValueInvalid @a elementSize is @a 0
/// @error #Arcadia_Status_ArgumentValueInvalid @a size is greater than @a *capacity
/// @error #Arcadia_Status_NotExists the new capacity cannot be represented by the Arcadia_SizeValue type
/// @error #Arcadia_Status_AllocationFailed an allocation failed
void
Arcadia_Arrays_resizeByAdditionalCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_ARMS_MemoryManager* memoryManager,
    void** elements,
    Arcadia_SizeValue elementSize,
    Arcadia_SizeValue size,
    Arcadia_SizeValue* capacity,
    Arcadia_SizeValue requiredAdditionalCapacity,
    Arcadia_Arrays_ResizeStrategy strategy
  );

/// @brief Resize an array taking into account a required free capacity.
/// @param elements A pointer to the element array of the array.
/// @param elementSize The size of an element.
/// @param size The size of the array.
/// @param capacity A pointer to the capacity of the array.
/// @param requiredFreeCapacity The required free capacity.
/// @param strategy The resize strategy.
/// @error #Arcadia_Status_ArgumentValueInvalid @a memoryManager, @a elements, or @a capacity are null
/// @error #Arcadia_Status_ArgumentValueInvalid @a elementSize is @a 0
/// @error #Arcadia_Status_ArgumentValueInvalid @a size is greater than @a *capacity
/// @error #Arcadia_Status_NotExists the new capacity cannot be represented by the Arcadia_SizeValue type
/// @error #Arcadia_Status_AllocationFailed an allocation failed
void
Arcadia_Arrays_resizeByFreeCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_ARMS_MemoryManager* memoryManager,
    void** elements,
    Arcadia_SizeValue elementSize,
    Arcadia_SizeValue size,
    Arcadia_SizeValue* capacity,
    Arcadia_SizeValue requiredFreeCapacity,
    Arcadia_Arrays_ResizeStrategy strategy
  );

#endif // ARCADIA_RING1_ARRAYS_H_INCLUDED
