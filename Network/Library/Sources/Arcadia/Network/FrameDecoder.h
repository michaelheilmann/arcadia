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

#if !defined(ARCADIA_NETWORK_FRAMEDECODER_H_INCLUDED)
#define ARCADIA_NETWORK_FRAMEDECODER_H_INCLUDED

#include "Arcadia/Network/Result.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Arcadia_Network_FrameDecoder Arcadia_Network_FrameDecoder;

// Caller owns this decoder; not safe for concurrent use. Initialize once,
// uninitialize once, and consume a ready frame before feeding more bytes.
struct Arcadia_Network_FrameDecoder {
  uint8_t* data;
  size_t capacity;
  size_t count;
  uint32_t length;
  uint8_t header[4];
  uint8_t headerCount;
  bool initialized;
  bool ready;
  bool failed;
};

// capacity in [1, 16 MiB], also bounds the size of an accepted frame.
// The caller must pass a zero-initialized decoder.
Arcadia_Network_Result
Arcadia_Network_FrameDecoder_initialize
  (
    Arcadia_Network_FrameDecoder* self,
    size_t maximumPayloadLength
  );

void
Arcadia_Network_FrameDecoder_uninitialize
  (
    Arcadia_Network_FrameDecoder* self
  );

// Consume at most one frame; *consumed tells the caller where to resume.
// Returns Ready when a full frame is available, Success if more input is
// needed, Malformed on excessive length (decoder becomes failed).
// NULL input is valid only with inputLength == 0.
Arcadia_Network_Result
Arcadia_Network_FrameDecoder_feed
  (
    Arcadia_Network_FrameDecoder* self,
    const uint8_t* input,
    size_t inputLength,
    size_t* consumed
  );

// The payload is borrowed until consume/uninitialize. It may be null when
// length is zero; no bytes are transferred into Arcadia-owned memory.
Arcadia_Network_Result
Arcadia_Network_FrameDecoder_peek
  (
    const Arcadia_Network_FrameDecoder* self,
    const uint8_t** data,
    size_t* length
  );

Arcadia_Network_Result
Arcadia_Network_FrameDecoder_consume
  (
    Arcadia_Network_FrameDecoder* self
  );

// Reset also recovers from malformed frames, after the caller decides
// how to handle the corrupted stream (normally close the connection).
void
Arcadia_Network_FrameDecoder_reset
  (
    Arcadia_Network_FrameDecoder* self
  );

#endif // ARCADIA_NETWORK_FRAMEDECODER_H_INCLUDED
