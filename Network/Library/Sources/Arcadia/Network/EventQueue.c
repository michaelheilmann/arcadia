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

#include "Arcadia/Network/EventQueue.h"
#include "Arcadia/Network/Internal/Mutex.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct Arcadia_Network_EventQueue {
  Arcadia_Network_Internal_Mutex mutex;
  Arcadia_Network_Event* events;
  size_t maximumEvents;
  size_t maximumPayloadBytes;
  size_t first;
  size_t count;
  size_t payloadBytes;
  int closed;
};

Arcadia_Network_Result
Arcadia_Network_EventQueue_create
  (
    Arcadia_Network_EventQueue** result,
    size_t maximumEvents,
    size_t maximumPayloadBytes
  )
{
  if (!result || !maximumEvents || maximumEvents > SIZE_MAX / sizeof(Arcadia_Network_Event)) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  *result = NULL;
  Arcadia_Network_EventQueue* q = calloc(1, sizeof(*q));
  if (!q) {
    return Arcadia_Network_Result_AllocationFailed;
  }
  q->events = calloc(maximumEvents, sizeof(*q->events));
  if (!q->events) {
    free(q);
    return Arcadia_Network_Result_AllocationFailed;
  }
  if (!Arcadia_Network_Internal_Mutex_initialize(&q->mutex)) {
    free(q->events);
    free(q);
    return Arcadia_Network_Result_EnvironmentFailed;
  }
  q->maximumEvents = maximumEvents;
  q->maximumPayloadBytes = maximumPayloadBytes;
  *result = q;
  return Arcadia_Network_Result_Success;
}

void
Arcadia_Network_EventQueue_close
  (
    Arcadia_Network_EventQueue* queue
  )
{
  if (queue && Arcadia_Network_Internal_Mutex_lock(&queue->mutex)) {
    queue->closed = 1;
    Arcadia_Network_Internal_Mutex_unlock(&queue->mutex);
  }
}

void
Arcadia_Network_EventQueue_destroy
  (
    Arcadia_Network_EventQueue* queue
  )
{
  if (!queue) {
    return;
  }
  // Only call after all producers and consumer have been joined/stopped.
  for (size_t i = 0; i < queue->count; i++) {
    size_t index = queue->first >= queue->maximumEvents - i
                   ? queue->first - (queue->maximumEvents - i)
                   : queue->first + i;
    Arcadia_Network_Event_dispose(&queue->events[index]);
  }
  Arcadia_Network_Internal_Mutex_uninitialize(&queue->mutex);
  free(queue->events);
  free(queue);
}

Arcadia_Network_Result
Arcadia_Network_EventQueue_post
  (
    Arcadia_Network_EventQueue* queue,
    uint64_t connectionId,
    Arcadia_Network_EventType type,
    int32_t errorCode,
    const uint8_t* data,
    size_t length
  )
{
  if (!queue || (!data && length) ||
      type < Arcadia_Network_EventType_Connected ||
      type > Arcadia_Network_EventType_Accepted) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  if (length > queue->maximumPayloadBytes) {
    return Arcadia_Network_Result_Full;
  }
  if (!Arcadia_Network_Internal_Mutex_lock(&queue->mutex)) {
    return Arcadia_Network_Result_EnvironmentFailed;
  }
  Arcadia_Network_Result status = Arcadia_Network_Result_Success;
  if (queue->closed) {
    status = Arcadia_Network_Result_Closed;
  } else if (queue->count == queue->maximumEvents ||
             length > queue->maximumPayloadBytes - queue->payloadBytes) {
    status = Arcadia_Network_Result_Full;
  } else {
    // Allocate under the lock after checking capacity; no unbounded burst of
    // temporary copies can be allocated by simultaneous producers.
    uint8_t* copy = NULL;
    if (length) {
      copy = malloc(length);
      if (!copy) {
        status = Arcadia_Network_Result_AllocationFailed;
      } else {
        memcpy(copy, data, length);
      }
    }
    if (status == Arcadia_Network_Result_Success) {
      // Index arithmetic avoids overflow of first + count.
      size_t index = queue->first >= queue->maximumEvents - queue->count
                     ? queue->first - (queue->maximumEvents - queue->count)
                     : queue->first + queue->count;
      Arcadia_Network_Event* event = &queue->events[index];
      event->connectionId = connectionId;
      event->type = type;
      event->errorCode = errorCode;
      event->data = copy;
      event->dataLength = length;
      queue->count++;
      queue->payloadBytes += length;
    }
  }
  Arcadia_Network_Internal_Mutex_unlock(&queue->mutex);
  return status;
}

Arcadia_Network_Result
Arcadia_Network_EventQueue_pop
  (
    Arcadia_Network_EventQueue* queue,
    Arcadia_Network_Event* event
  )
{
  if (!queue || !event) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  if (!Arcadia_Network_Internal_Mutex_lock(&queue->mutex)) {
    return Arcadia_Network_Result_EnvironmentFailed;
  }
  Arcadia_Network_Result status;
  if (queue->count) {
    *event = queue->events[queue->first];
    memset(&queue->events[queue->first], 0, sizeof(*event));
    queue->payloadBytes -= event->dataLength;
    queue->first++;
    if (queue->first == queue->maximumEvents) {
      queue->first = 0;
    }
    queue->count--;
    status = Arcadia_Network_Result_Success;
  } else {
    status = queue->closed ? Arcadia_Network_Result_Closed : Arcadia_Network_Result_Empty;
  }
  Arcadia_Network_Internal_Mutex_unlock(&queue->mutex);
  return status;
}

Arcadia_Network_Result
Arcadia_Network_EventQueue_getCounts
  (
    Arcadia_Network_EventQueue* queue,
    size_t* eventCount,
    size_t* payloadBytes
  )
{
  if (!queue || !eventCount || !payloadBytes) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  if (!Arcadia_Network_Internal_Mutex_lock(&queue->mutex)) {
    return Arcadia_Network_Result_EnvironmentFailed;
  }
  *eventCount = queue->count;
  *payloadBytes = queue->payloadBytes;
  Arcadia_Network_Internal_Mutex_unlock(&queue->mutex);
  return Arcadia_Network_Result_Success;
}
