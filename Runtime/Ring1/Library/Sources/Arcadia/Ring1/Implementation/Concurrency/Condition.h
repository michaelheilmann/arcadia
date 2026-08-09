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

#if !defined(ARCADIA_RING1_CONCURRENCY_CONDITION_H_INCLUDED)
#define ARCADIA_RING1_CONCURRENCY_CONDITION_H_INCLUDED

#include "Arcadia/Ring1/Implementation/Configure.h"
#include <stddef.h>

/// @brief The type of return values of Arcadia_Concurrency_Condition functions.
typedef enum Arcadia_Concurrency_Condition_Result Arcadia_Concurrency_Condition_Result;

/// @brief A "condition" object to be embedded into structs or to be allocated on the stack or the heap.
typedef struct Arcadia_Concurrency_Condition Arcadia_Concurrency_Condition;

enum Arcadia_Concurrency_Condition_Result {
  Arcadia_Concurrency_Condition_Result_Success,
  Arcadia_Concurrency_Condition_Result_ArgumentInvalid,
  Arcadia_Concurrency_Condition_Result_AllocationFailed,
  Arcadia_Concurrency_Condition_Result_EnvironmentFailed,
  Arcadia_Concurrency_Condition_Result_Initialized,
};

struct Arcadia_Concurrency_Condition {
  void* pimpl;
}; // struct Arcadia_Concurrency_Condition

Arcadia_Concurrency_Condition_Result
Arcadia_Concurrency_Condition_initialize
  (
    Arcadia_Concurrency_Condition* condition
  );

Arcadia_Concurrency_Condition_Result
Arcadia_Concurrency_Condition_uninitialize
  (
    Arcadia_Concurrency_Condition* condition
  );

Arcadia_Concurrency_Condition_Result
Arcadia_Concurrency_Condition_wait
  (
    Arcadia_Concurrency_Condition* condition
  );

Arcadia_Concurrency_Condition_Result
Arcadia_Concurrency_Condition_signalOne
  (
    Arcadia_Concurrency_Condition* condition
  );

Arcadia_Concurrency_Condition_Result
Arcadia_Concurrency_Condition_signalAll
  (
    Arcadia_Concurrency_Condition* condition
  );

#endif // ARCADIA_RING1_CONCURRENCY_CONDITION_H_INCLUDED
