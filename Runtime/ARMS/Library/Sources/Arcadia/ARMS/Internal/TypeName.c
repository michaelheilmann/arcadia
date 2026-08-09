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

#include "Arcadia/ARMS/Internal/TypeName.h"

#include "Arcadia/ARMS/Internal/ReferenceCounter.h"
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct Node Node;
typedef struct Table Table;

// Maximal capacity of the table as power of two
//  HI0 = floor(SIZE_MAX / POINTER_SIZE) is the maximum number of buckets.
//  The program fails if POINTER_SIZE >= SIZE_MAX, that is, if HI0 is 0.
//  Consequently, HI0 is at least 1 and there exists a greatest power of two HI less than or equal to HI0.
//  Let HI1 = log2(HI0). HI0 is in the range 0, 1, 2, ... .
//  The program fails if HI1 is less than 2.
//  HI = HI1 - 1 is the maximal capacity of the table. As HI1 is at least 3, HI is at least 2.
//
// Minimal capacity of the table as power of two
//  The initial size of the buckets array is 1 << L0.
//  The maximal size of the buckets array is 1 << HI.
// Growing the table.
//  As the current capacit is within the bounds [LO,HI],
//  we can compute a new capacity by multiplying the
//  current capacity by two. If the new capacity is outside
//  the bounds [LO,HI], then we fail to grow the table.
//  As HI is a power of two LESS THAN the maximal power of two representablw by size_t, doubling the capacity itself cannot overflow.

struct Node {
  Node* next;
  Arcadia_ARMS_ReferenceCounter referenceCount;
  size_t hashValue;
  size_t numberOfBytes;
  uint8_t bytes[];
};

struct Table {
  Node** buckets;
  size_t size, capacity;
};

// Get the greatest power of two less than or equalt to if it exists.
// Returns
// - Arcadia_ARMS_Status_Success on success
// - Arcadia_ARMS_Status_ArgumentValueInvalid if result is 0
// - Arcadia_ARMS_Status_ArgumentValueInvalid if value is 0
static inline Arcadia_ARMS_Status
Arcadia_ARMS_greatestPowerOfTwoLessThanOrEqualTo
  (
    size_t* result,
    size_t value
  );

static Arcadia_ARMS_Status
initialize
  (
  );

static Arcadia_ARMS_Status
doubleCapacity
  (
  );

static Arcadia_ARMS_Status
onPostInsert
  (
  );

static inline Arcadia_ARMS_Status
Arcadia_ARMS_greatestPowerOfTwoLessThanOrEqualTo
  (
    size_t* result,
    size_t value
  )
{
  if (!result) {
    return Arcadia_ARMS_Status_ArgumentValueInvalid;
  }
  size_t SIZE_BITS = sizeof(size_t) * 8;
  for (size_t i = SIZE_BITS; i > 0; --i) {
    size_t mask = (size_t)1 << (i - 1);
    if (value & mask) {
      *result = i;
      return Arcadia_ARMS_Status_Success;
    }
  }
  return Arcadia_ARMS_Status_ArgumentValueInvalid;
}

static size_t g_minimalCapacityPowerOfTwo = 0,
              g_maximalCapacityPowerOfTwo = 0;
static Table* g_table = NULL;

static Arcadia_ARMS_Status
initialize
  (
  )
{
  // (1) compute maximal capacity
  size_t n = SIZE_MAX / sizeof(Node*);
  if (n == 0) {
    // Actually impossible case which would require sizeof(Node*) to be greater than SIZE_MAX.
    return Arcadia_ARMS_Status_EnvironmentFailed;
  }
  if (Arcadia_ARMS_greatestPowerOfTwoLessThanOrEqualTo(&n, n)) {
    Arcadia_ARMS_Status_EnvironmentFailed;
  }
  if (n < 2) {
    return Arcadia_ARMS_Status_EnvironmentFailed;
  }
  g_maximalCapacityPowerOfTwo = n - 1;

  // (2) compute mimimal capacity
  g_minimalCapacityPowerOfTwo = 3;
  if (g_minimalCapacityPowerOfTwo > g_maximalCapacityPowerOfTwo) {
    g_minimalCapacityPowerOfTwo = g_maximalCapacityPowerOfTwo;
  }

  return Arcadia_ARMS_Status_Success;
}

static Arcadia_ARMS_Status
doubleCapacity
  (
  )
{
  size_t max = (size_t)1 << g_maximalCapacityPowerOfTwo;
  size_t old = g_table->capacity;
  size_t new = old << 1;
  if (new > max) {
    return Arcadia_ARMS_Status_AllocationFailed;
  }
  Node** oldBuckets = g_table->buckets;
  Node** newBuckets = malloc(sizeof(Node*) * new);
  if (!newBuckets) {
    return Arcadia_ARMS_Status_AllocationFailed;
  }
  for (size_t i = 0, n = new; i < n; ++i) {
    newBuckets[i] = NULL;
  }
  for (size_t i = 0, n = old; i < n; ++i) {
    while (oldBuckets[i]) {
      Node* node = oldBuckets[i];
      oldBuckets[i] = oldBuckets[i]->next;

      size_t j = node->hashValue & (new - 1);
      node->next = newBuckets[j];
      newBuckets[j]= node;
    }
  }
  free(oldBuckets);
  g_table->buckets = newBuckets;
  g_table->capacity = new;

  return Arcadia_ARMS_Status_Success;
}

static Arcadia_ARMS_Status
onPostInsert
  (
  )
{
  // This function keeps doubling the capacity until the ratio sz / cp is smaller than 0.75.
  // In most cases, this loop will be executed exactly once.
  // To illustrate that:
  // Assume an already unlikely ratio 2^k / 2^k = 1 which is attained by sz = cp,
  // then doubling the capacity once will immediatly bring down the ratio to 2^k / 2^(k+1) = 0.5.
  while (true) {
    double bestf = ceil(g_table->size / 0.75);
    if (bestf < ((size_t)0) || bestf > SIZE_MAX || isnan(bestf) || isinf(bestf)) {
      return Arcadia_ARMS_Status_AllocationFailed;
    }
    size_t current = g_table->capacity;
    size_t best = (size_t)bestf;
    if (best > current) {
      Arcadia_ARMS_Status status = doubleCapacity();
      if (status) {
        return status;
      }
    } else {
      return Arcadia_ARMS_Status_Success;
    }
  }
}

Arcadia_ARMS_Status
Arcadia_ARMS_TypeNameModule_startup
  (
  )
{
  if (!g_table) {
    if (initialize()) {
      return Arcadia_ARMS_Status_EnvironmentFailed;
    }
  }
  g_table = malloc(sizeof(Table));
  if (!g_table) {
    return Arcadia_ARMS_Status_AllocationFailed;
  }
  g_table->size = 0;
  g_table->capacity = (size_t)1 << g_minimalCapacityPowerOfTwo;
  g_table->buckets = malloc(sizeof(Node*) * g_table->capacity);
  if (!g_table->buckets) {
    free(g_table);
    g_table = NULL;
    return Arcadia_ARMS_Status_AllocationFailed;
  }
  for (size_t i = 0, n = g_table->capacity; i < n; ++i) {
    g_table->buckets[i] = NULL;
  }
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_TypeNameModule_shutdown
  (
  )
{
  free(g_table->buckets);
  g_table->buckets = NULL;
  free(g_table);
  g_table = NULL;
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_TypeName_getOrCreate
  (
    Arcadia_ARMS_TypeName** result,
    const Arcadia_ARMS_Natural8* bytes,
    Arcadia_ARMS_Size numberOfBytes
  )
{
  if (!result || !bytes || SIZE_MAX - sizeof(Node) < numberOfBytes) {
    return Arcadia_ARMS_Status_ArgumentValueInvalid;
  }
  Arcadia_ARMS_Size hashValue = numberOfBytes;
  for (Arcadia_ARMS_Size i = 0; i < numberOfBytes; ++i) {
    hashValue = 37 * hashValue + (Arcadia_ARMS_Size)bytes[i];
  }
  Arcadia_ARMS_Size hashIndex = hashValue & (g_table->capacity - 1);
  Node* node;
  for (node = g_table->buckets[hashIndex]; NULL != node; node = node->next) {
    if (node->numberOfBytes == numberOfBytes && node->hashValue == hashValue) {
      if (!memcmp(node->bytes, bytes, numberOfBytes)) {
        *result = (Arcadia_ARMS_TypeName*)node;
        return Arcadia_ARMS_Status_Success;
      }
    }
  }
  if (g_table->size == SIZE_MAX) {
    return Arcadia_ARMS_Status_AllocationFailed;
  }
  node = malloc(sizeof(Node) + numberOfBytes);
  if (!node) {
    return Arcadia_ARMS_Status_AllocationFailed;
  }
  node->hashValue = hashValue;
  node->numberOfBytes = numberOfBytes;
  memcpy(node->bytes, bytes, numberOfBytes);
  node->next = g_table->buckets[hashIndex];
  g_table->buckets[hashIndex] = node;
  g_table->size++;

  *result = (Arcadia_ARMS_TypeName*)node;

  onPostInsert();

  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_TypeName_getData
  (
    Arcadia_ARMS_TypeName* typeName,
    const Arcadia_ARMS_Natural8** bytes,
    Arcadia_ARMS_Size* numberOfBytes
  )
{
  if (!typeName || !bytes || !numberOfBytes) {
    return Arcadia_ARMS_Status_ArgumentValueInvalid;
  }
  *bytes = ((Node*)typeName)->bytes;
  *numberOfBytes = ((Node*)typeName)->numberOfBytes;
  return Arcadia_ARMS_Status_Success;
}
