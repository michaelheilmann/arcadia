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

#if !defined(ARCADIA_NETWORK_BYTEQUEUE_H_INCLUDED)
#define ARCADIA_NETWORK_BYTEQUEUE_H_INCLUDED

#include "Arcadia/Network/Result.h"
#include <stddef.h>
#include <stdint.h>

typedef struct Arcadia_Network_ByteQueue Arcadia_Network_ByteQueue;

// Single-owner fixed-capacity byte ring: no internal synchronization.
// Intended for per-connection native buffers on the I/O worker side.
// Initialize a zero-initialized queue, then uninitialize before reuse.
struct Arcadia_Network_ByteQueue {
  uint8_t* bytes;
  size_t capacity;
  size_t first;
  size_t count;
};

Arcadia_Network_Result
Arcadia_Network_ByteQueue_initialize
  (
    Arcadia_Network_ByteQueue* self,
    size_t capacity
  );

void
Arcadia_Network_ByteQueue_uninitialize
  (
    Arcadia_Network_ByteQueue* self
  );

// Atomically append ALL bytes, or return Full without changing queue.
// NULL input is only accepted for inputLength == 0.
Arcadia_Network_Result
Arcadia_Network_ByteQueue_append
  (
    Arcadia_Network_ByteQueue* self,
    const uint8_t* input,
    size_t inputLength
  );

// Borrow the largest contiguous occupied region; no copy and no mutation.
Arcadia_Network_Result
Arcadia_Network_ByteQueue_peek
  (
    const Arcadia_Network_ByteQueue* self,
    const uint8_t** bytes,
    size_t* count
  );

// Release exactly count bytes from the head.
Arcadia_Network_Result
Arcadia_Network_ByteQueue_consume
  (
    Arcadia_Network_ByteQueue* self,
    size_t count
  );

// Copy <= outputCapacity bytes and consume them. *read is always set.
Arcadia_Network_Result
Arcadia_Network_ByteQueue_read
  (
    Arcadia_Network_ByteQueue* self,
    uint8_t* output,
    size_t outputCapacity,
    size_t* read
  );

#endif // ARCADIA_NETWORK_BYTEQUEUE_H_INCLUDED
