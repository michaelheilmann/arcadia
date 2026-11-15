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

#include "Arcadia/Network/FrameDecoder.h"
#include "Arcadia/Network/Frame.h"
#include <stdlib.h>
#include <string.h>

#define ARCADIA_NETWORK_FRAME_DECODER_LIMIT (16u * 1024u * 1024u)

Arcadia_Network_Result
Arcadia_Network_FrameDecoder_initialize
  (
    Arcadia_Network_FrameDecoder* self,
    size_t maximumPayloadLength
  )
{
  if (!self || self->initialized || !maximumPayloadLength ||
      maximumPayloadLength > ARCADIA_NETWORK_FRAME_DECODER_LIMIT) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  uint8_t* data = malloc(maximumPayloadLength);
  if (!data) {
    return Arcadia_Network_Result_AllocationFailed;
  }
  memset(self, 0, sizeof(*self));
  self->data = data;
  self->capacity = maximumPayloadLength;
  self->initialized = true;
  return Arcadia_Network_Result_Success;
}

void
Arcadia_Network_FrameDecoder_uninitialize
  (
    Arcadia_Network_FrameDecoder* self
  )
{
  if (self && self->initialized) {
    free(self->data);
    memset(self, 0, sizeof(*self));
  }
}

void
Arcadia_Network_FrameDecoder_reset
  (
    Arcadia_Network_FrameDecoder* self
  )
{
  if (self && self->initialized) {
    self->count = 0;
    self->length = 0;
    self->headerCount = 0;
    self->ready = false;
    self->failed = false;
  }
}

Arcadia_Network_Result
Arcadia_Network_FrameDecoder_feed
  (
    Arcadia_Network_FrameDecoder* self,
    const uint8_t* input,
    size_t inputLength,
    size_t* consumed
  )
{
  if (!self || !consumed || !self->initialized || (!input && inputLength)) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  *consumed = 0;
  if (self->ready || self->failed) {
    return Arcadia_Network_Result_InvalidState;
  }
  while (*consumed < inputLength) {
    if (self->headerCount < Arcadia_Network_Frame_HeaderSize) {
      self->header[self->headerCount++] = input[(*consumed)++];
      if (self->headerCount == Arcadia_Network_Frame_HeaderSize) {
        self->length = ((uint32_t)self->header[0] << 24) |
                       ((uint32_t)self->header[1] << 16) |
                       ((uint32_t)self->header[2] << 8) |
                       (uint32_t)self->header[3];
        if (self->length > self->capacity) {
          self->failed = true;
          return Arcadia_Network_Result_Malformed;
        }
        if (!self->length) {
          self->ready = true;
          return Arcadia_Network_Result_Ready;
        }
      }
    } else {
      size_t remaining = (size_t)self->length - self->count;
      size_t available = inputLength - *consumed;
      size_t take = remaining < available ? remaining : available;
      memcpy(self->data + self->count, input + *consumed, take);
      self->count += take;
      *consumed += take;
      if (self->count == self->length) {
        self->ready = true;
        return Arcadia_Network_Result_Ready;
      }
    }
  }
  return Arcadia_Network_Result_Success;
}

Arcadia_Network_Result
Arcadia_Network_FrameDecoder_peek
  (
    const Arcadia_Network_FrameDecoder* self,
    const uint8_t** data,
    size_t* length
  )
{
  if (!self || !self->initialized || !data || !length) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  if (!self->ready) {
    return Arcadia_Network_Result_InvalidState;
  }
  *data = self->length ? self->data : NULL;
  *length = self->length;
  return Arcadia_Network_Result_Success;
}

Arcadia_Network_Result
Arcadia_Network_FrameDecoder_consume
  (
    Arcadia_Network_FrameDecoder* self
  )
{
  if (!self || !self->initialized) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  if (!self->ready) {
    return Arcadia_Network_Result_InvalidState;
  }
  Arcadia_Network_FrameDecoder_reset(self);
  return Arcadia_Network_Result_Success;
}
