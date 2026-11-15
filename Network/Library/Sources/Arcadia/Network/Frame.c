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

#include "Arcadia/Network/Frame.h"
#include <limits.h>
#include <string.h>

Arcadia_Network_Result
Arcadia_Network_Frame_writeHeader
  (
    uint8_t* header,
    size_t payloadLength
  )
{
  if (!header || payloadLength > UINT32_MAX) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  header[0] = (uint8_t)(payloadLength >> 24);
  header[1] = (uint8_t)(payloadLength >> 16);
  header[2] = (uint8_t)(payloadLength >> 8);
  header[3] = (uint8_t)payloadLength;
  return Arcadia_Network_Result_Success;
}

Arcadia_Network_Result
Arcadia_Network_Frame_encode
  (
    const uint8_t* input,
    size_t payloadLength,
    uint8_t* output,
    size_t outputCapacity,
    size_t* written
  )
{
  if (!written) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  *written = 0;
  if (!output || (!input && payloadLength) || payloadLength > UINT32_MAX ||
      outputCapacity < Arcadia_Network_Frame_HeaderSize ||
      payloadLength > outputCapacity - Arcadia_Network_Frame_HeaderSize) {
    return Arcadia_Network_Result_InvalidArgument;
  }
  // Caller-provided input and output must not overlap.
  if (input && payloadLength) {
    // Copy payload first so no prefix is exposed on invalid input.
    // The caller supplies separate non-overlapping input/output regions.
    memcpy(output + Arcadia_Network_Frame_HeaderSize, input, payloadLength);
  }
  Arcadia_Network_Frame_writeHeader(output, payloadLength);
  *written = Arcadia_Network_Frame_HeaderSize + payloadLength;
  return Arcadia_Network_Result_Success;
}
