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

#include "Arcadia/Network/ByteQueue.h"
#include <stdlib.h>
#include <string.h>

Arcadia_Network_Result
Arcadia_Network_ByteQueue_initialize
  (
    Arcadia_Network_ByteQueue* self,
    size_t capacity
  )
{
  if (!self || !capacity || self->bytes) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  uint8_t* memory = malloc(capacity);
  if (!memory) {
    return Arcadia_Network_Result_AllocationFailed;
  }
  self->bytes = memory;
  self->capacity = capacity;
  self->first = 0;
  self->count = 0;
  return Arcadia_Network_Result_Success;
}

void
Arcadia_Network_ByteQueue_uninitialize
  (
    Arcadia_Network_ByteQueue* self
  )
{
  if (self) {
    free(self->bytes);
    memset(self, 0, sizeof(*self));
  }
}

Arcadia_Network_Result
Arcadia_Network_ByteQueue_append
  (
    Arcadia_Network_ByteQueue* self,
    const uint8_t* input,
    size_t inputLength
  )
{
  if (!self || !self->bytes || (!input && inputLength)) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  if (inputLength > self->capacity - self->count) {
    return Arcadia_Network_Result_Full;
  }
  if (!inputLength) {
    return Arcadia_Network_Result_Success;
  }
  size_t index = self->first >= self->capacity - self->count
                 ? self->first - (self->capacity - self->count)
                 : self->first + self->count;
  size_t tailCapacity = self->capacity - index;
  size_t firstLength = inputLength < tailCapacity ? inputLength : tailCapacity;
  memcpy(self->bytes + index, input, firstLength);
  if (firstLength < inputLength) {
    memcpy(self->bytes, input + firstLength, inputLength - firstLength);
  }
  self->count += inputLength;
  return Arcadia_Network_Result_Success;
}

Arcadia_Network_Result
Arcadia_Network_ByteQueue_peek
  (
    const Arcadia_Network_ByteQueue* self,
    const uint8_t** bytes,
    size_t* count
  )
{
  if (!self || !self->bytes || !bytes || !count) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  *bytes = self->bytes + self->first;
  size_t available = self->capacity - self->first;
  *count = self->count < available ? self->count : available;
  return Arcadia_Network_Result_Success;
}

Arcadia_Network_Result
Arcadia_Network_ByteQueue_consume
  (
    Arcadia_Network_ByteQueue* self,
    size_t count
  )
{
  if (!self || !self->bytes || count > self->count) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  self->first = (self->first + count) % self->capacity;
  self->count -= count;
  return Arcadia_Network_Result_Success;
}

Arcadia_Network_Result
Arcadia_Network_ByteQueue_read
  (
    Arcadia_Network_ByteQueue* self,
    uint8_t* output,
    size_t outputCapacity,
    size_t* read
  )
{
  if (!read || !self || !self->bytes || (!output && outputCapacity)) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  *read = 0;
  size_t length = self->count < outputCapacity ? self->count : outputCapacity;
  if (!length) {
    return Arcadia_Network_Result_Success;
  }
  size_t firstLength = length < self->capacity - self->first ? length : self->capacity - self->first;
  memcpy(output, self->bytes + self->first, firstLength);
  if (firstLength < length) {
    memcpy(output + firstLength, self->bytes, length - firstLength);
  }
  Arcadia_Network_ByteQueue_consume(self, length);
  *read = length;
  return Arcadia_Network_Result_Success;
}
