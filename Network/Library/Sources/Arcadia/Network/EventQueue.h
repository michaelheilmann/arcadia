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

#if !defined(ARCADIA_NETWORK_EVENTQUEUE_H_INCLUDED)
#define ARCADIA_NETWORK_EVENTQUEUE_H_INCLUDED

#include "Arcadia/Network/Event.h"
#include "Arcadia/Network/Result.h"
#include <stddef.h>
#include <stdint.h>

typedef struct Arcadia_Network_EventQueue Arcadia_Network_EventQueue;

// Bounded multi-producer/single-consumer native queue.
// create, close and destroy are lifecycle operations; the caller must join
// all producer threads before destroy. push/pop may run concurrently.
// post makes a defensive copy; pop transfers ownership to the caller.
// No worker calls the Arcadia runtime or application callbacks.
Arcadia_Network_Result
Arcadia_Network_EventQueue_create
  (
    Arcadia_Network_EventQueue** result,
    size_t maximumEvents,
    size_t maximumPayloadBytes
  );

// Closed queues reject pushes; already queued events remain available to pop.
void
Arcadia_Network_EventQueue_close
  (
    Arcadia_Network_EventQueue* queue
  );

void
Arcadia_Network_EventQueue_destroy
  (
    Arcadia_Network_EventQueue* queue
  );

// *data may be NULL only for zero length. The copy remains owned by queue.
// Full indicates either event-count or total-payload-byte limit reached.
Arcadia_Network_Result
Arcadia_Network_EventQueue_post
  (
    Arcadia_Network_EventQueue* queue,
    uint64_t connectionId,
    Arcadia_Network_EventType type,
    int32_t errorCode,
    const uint8_t* data,
    size_t length
  );

// Returns Empty for open empty queue, Closed for drained closed queue.
Arcadia_Network_Result
Arcadia_Network_EventQueue_pop
  (
    Arcadia_Network_EventQueue* queue,
    Arcadia_Network_Event* event
  );

Arcadia_Network_Result
Arcadia_Network_EventQueue_getCounts
  (
    Arcadia_Network_EventQueue* queue,
    size_t* eventCount,
    size_t* payloadBytes
  );

#endif // ARCADIA_NETWORK_EVENTQUEUE_H_INCLUDED
