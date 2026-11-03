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

#if !defined(ARCADIA_NETWORK_FRAME_H_INCLUDED)
#define ARCADIA_NETWORK_FRAME_H_INCLUDED

#include "Arcadia/Network/Result.h"
#include <stddef.h>
#include <stdint.h>

#define Arcadia_Network_Frame_HeaderSize (4u)

// Optional, protocol-neutral framing on top of a raw TCP byte stream:
// four-byte unsigned network-order (big endian) payload length, followed by
// exactly that many bytes. Empty frames are supported. It is NOT a TLS format.
Arcadia_Network_Result
Arcadia_Network_Frame_writeHeader
  (
    uint8_t* header,
    size_t payloadLength
  );

// A frame is copied into the provided output buffer in one operation.
// input may be null only when payloadLength is zero. The input and output
// memory ranges must not overlap.
Arcadia_Network_Result
Arcadia_Network_Frame_encode
  (
    const uint8_t* input,
    size_t payloadLength,
    uint8_t* output,
    size_t outputCapacity,
    size_t* written
  );

#endif // ARCADIA_NETWORK_FRAME_H_INCLUDED
